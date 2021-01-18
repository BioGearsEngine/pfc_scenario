#include "Equipment.h"
#include "Citation.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>

Sustain::Sustain(QObject* parent)
  : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------
QString TypeToString(Sustain::Type value)
{
  if (value == Sustain::STRING) {
    return "STRING";
  } else if (value == Sustain::BOOLEAN) {
    return "BOOLEAN";
  } else if (value == Sustain::INTEGRAL) {
    return "INTEGRAL";
  } else if (value == Sustain::REAL) {
    return "REAL";
  } else if (value == Sustain::RANGE) {
    return "RANGE";
  } else if (value == Sustain::SCALAR) {
    return "SCALAR";
  } else if (value == Sustain::ENUM) {
    return "ENUM";
  } else if (value == Sustain::eOPTION) {
    return "OPTION";
  } else if (value == Sustain::CONST) {
    return "CONST";
  } else {
    return "UNKNOWN";
  }
}
//--------------------------------------------------------------------------------------------
Sustain::Type TypeFromString(QString value)
{
  if (value == "STRING") {
    return Sustain::STRING;
  } else if (value == "BOOLEAN") {
    return Sustain::BOOLEAN;
  } else if (value == "INTEGRAL" || value == "INTEGER") {
    return Sustain::INTEGRAL;
  } else if (value == "REAL" || value == "FLOAT") {
    return Sustain::REAL;
  } else if (value == "RANGE") {
    return Sustain::RANGE;
  } else if (value == "SCALAR") {
    return Sustain::SCALAR;
  } else if (value == "ENUM") {
    return Sustain::ENUM;
  } else if (value == "OPTION") {
    return Sustain::eOPTION;
  } else if (value == "CONST") {
    return Sustain::CONST;
  }

  QRegularExpression enumRegxp { R"(\s*ENUM\s*{\s*(.*)\s*}\s*)" };
  QRegularExpressionMatch enumMatches;
  enumMatches = enumRegxp.match(value);
  if (enumMatches.hasMatch()) {
    return Sustain::ENUM;
  } else {
    return Sustain::UNKNOWN;
  }
}
//--------------------------------------------------------------------------------------------
Sustain::Type TypeFromString(std::string value)
{
  return TypeFromString(QString(value.c_str()));
}
//--------------------------------------------------------------------------------------------
//!
//!  Parameter Field Members
//!
//--------------------------------------------------------------------------------------------
ParameterField::ParameterField(QObject* parent)
  : QObject(parent)
{
}
ParameterField::ParameterField(QString n, Sustain::Type t, QVariant v, QObject* parent)
  : QObject(parent)
  , name(n)
  , _value(v)
  , eType(t)
{
}
//-------------------------------------------------------------------------------------------
Sustain::Type ParameterField::type() const
{
  return eType;
}
//-------------------------------------------------------------------------------------------
void ParameterField::type(Sustain::Type type)
{
  eType = type;
  typeChanged();
}
//-------------------------------------------------------------------------------------------
bool ParameterField::operator==(const ParameterField& rhs) const
{
  return name == rhs.name
    && eType == rhs.eType;
}
//--------------------------------------------------------------------------------------------
bool ParameterField::operator!=(const ParameterField& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
ParameterField* ParameterField::make(QObject* parent)
{
  return new ParameterField(parent);
}
//--------------------------------------------------------------------------------------------
ParameterField* ParameterField::make(QString name, Sustain::Type type, QObject* parent)
{
  return new ParameterField(name, type, QVariant(""), parent);
}
//--------------------------------------------------------------------------------------------
ParameterField* ParameterField::make(QString name, Sustain::Type type, QVariant value, QObject* parent)
{
  return new ParameterField(name, type, value, parent);
}
//--------------------------------------------------------------------------------------------
QVariant ParameterField::value() const
{
  return _value;
}
//--------------------------------------------------------------------------------------------
void ParameterField::value(QVariant value)
{
  _value = value;
}

//--------------------------------------------------------------------------------------------
QString ParameterField::toString() const
{
  if (_value.isValid()) {
    if (_value.type() == QMetaType::Int) {
      return QString("%1:%2:%3").arg(name).arg(TypeToString(eType)).arg(_value.toInt());
    }
    if (_value.type() == QMetaType::Double) {
      return QString("%1:%2:%3").arg(name).arg(TypeToString(eType)).arg(_value.toDouble());
    }
    if (_value.type() == QMetaType::QString) {
      return QString("%1:%2:%3").arg(name).arg(TypeToString(eType)).arg(_value.toString());
    }
  }
  return QString("%1:%2").arg(name).arg(TypeToString(eType));
}
//--------------------------------------------------------------------------------------------
QString ParameterField::typeString() const
{
  return TypeToString(eType);
}
//--------------------------------------------------------------------------------------------
void ParameterField::assign(ParameterField* rhs)
{
  assign(*rhs);
}
//--------------------------------------------------------------------------------------------
void ParameterField::assign(const ParameterField& rhs)
{
  name = rhs.name;
  eType = rhs.eType;
}
//--------------------------------------------------------------------------------------------
void ParameterField::clear()
{
  name = "Unknown";
  eType = Sustain::SCALAR;
}

//!
//!  Equipment Parameter Members
//!
EquipmentParameter::EquipmentParameter(QObject* parent)
  : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------
EquipmentParameter::EquipmentParameter(QString parameter_string, QObject* parent)
  : QObject(parent)
  , name("")
  , eType(Sustain::UNKNOWN)
{

  //This constructor needs to properly report errors loading text files
  //means we could run in to any number of exceptions.

  //Error 1: General Parse Error
  //Error 2: First Name:Type does not resovle to a known eType
  //Error 3: Any Param that doesn't properly parse

  //Examples
  //NAME:STRING
  //SEVERITY:RANGE,MIN:INTEGRAL:6,MAX:INTEGRAL:6
  //TYPE:ENUM,STRING,INTEGRAL,ENUM
  //VOLUME:SCALAR,UNIT:ENUM,ml:eOption,l:eOption,gal:eoption - VOLUME:SCALAR,UNIT:ENUM:ml|l|gal (Second form needs patching to be supported)

  for (auto param : parameter_string.split(',')) {
    auto parts = param.split(':');
    if (eType == Sustain::UNKNOWN) {
      name = parts[0];
      eType = TypeFromString(parts[1].trimmed());

    } else if (eType == Sustain::ENUM) {
      enumOptions.push_back(param.trimmed());
    } else {
      //Tokenization is currently broken.
      fields.push_back(ParameterField::make(parts[0].trimmed(), TypeFromString(parts[1].trimmed()), (parts.size() > 2) ? parts[2] : 0, this));
    }
  }
}
//--------------------------------------------------------------------------------------------
EquipmentParameter::EquipmentParameter(QString n, Sustain::Type t, QList<QString> eo, QObject* parent)
  : QObject(parent)
  , name(n)
  , eType(t)
  , enumOptions(eo)
{
}
EquipmentParameter::~EquipmentParameter()
{
  clear();
}
//--------------------------------------------------------------------------------------------
bool EquipmentParameter::operator==(const EquipmentParameter& rhs) const
{
  bool equality = true;
  equality &= name == rhs.name;
  equality &= eType == rhs.eType;
  equality &= enumOptions == rhs.enumOptions;
  equality &= fields.count() == rhs.fields.count();
  if (equality) {
    for (auto ii = 0; ii > fields.count(); ++ii) {
      equality &= fields[ii] == rhs.fields[ii];
    }
  }
  return equality;
}
//--------------------------------------------------------------------------------------------
bool EquipmentParameter::operator!=(const EquipmentParameter& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
EquipmentParameter* EquipmentParameter::make(QObject* parent)
{
  return new EquipmentParameter(parent);
}
//--------------------------------------------------------------------------------------------
EquipmentParameter* EquipmentParameter::make(QString name, Sustain::Type type, QList<QString> enumOptions, QObject* make)
{
  return new EquipmentParameter(name, type, std::move(enumOptions));
}
//--------------------------------------------------------------------------------------------
Sustain::Type EquipmentParameter::Type() const
{
  return eType;
}
//--------------------------------------------------------------------------------------------
void EquipmentParameter::Type(Sustain::Type type)
{
  eType = type;
  fields.clear();
  enumOptions.clear();

  switch (type) {
  case Sustain::Type::CONST: //{ANY VLALUE CAN NOT BE CHANGED}
    fields.append(ParameterField::make("Value", Sustain::Type::UNKNOWN, QVariant(),this));
    //NOTE: It is a point of debate as to if the value of a type should have a type
    //NOTE: On one hand you could use he ParameterField:Type for validation, but its also redundent
    //NOTE: I thought about having an additional enum value ANY, but UNKNOWN seems to be fine.
    break;
    break;
  case Sustain::Type::RANGE:
    fields.append(ParameterField::make("Min", Sustain::Type::REAL, 0., this));
    fields.append(ParameterField::make("Max", Sustain::Type::REAL, 1., this));
    break;
  case Sustain::Type::SCALAR:
    fields.append(ParameterField::make("Type", Sustain::Type::STRING, QVariant(),this));
    fields.append(ParameterField::make("Unit", Sustain::Type::INTEGRAL,QVariant(), this));
    //NOTE: All additional fields are of type eOption are represent valid unit demensions
    //      Abence of any eOption field means the scalar is without a demension
    break;
  case Sustain::Type::BOOLEAN: //{0,1}
  case Sustain::Type::INTEGRAL: //{int64_t}
  case Sustain::Type::STRING: //{char const*}
  case Sustain::Type::ENUM:
  case Sustain::Type::UNKNOWN:
  default:
    break;
  }

  typeChanged();
}
//--------------------------------------------------------------------------------------------
void EquipmentParameter::appendField(QString name, Sustain::Type type)
{
  fields.append(ParameterField::make(name, type));
}
//--------------------------------------------------------------------------------------------
void EquipmentParameter::appendField(std::string name, Sustain::Type type)
{
  fields.append(ParameterField::make(name.c_str(), type));
}
//--------------------------------------------------------------------------------------------
void EquipmentParameter::assign(EquipmentParameter* rhs)
{
  assign(*rhs);
}
//--------------------------------------------------------------------------------------------
void EquipmentParameter::assign(const EquipmentParameter& rhs)
{
  eType = rhs.eType;
  name = rhs.name;
  enumOptions = rhs.enumOptions;
  fields.clear();
  for (auto field : rhs.fields) {
    fields.push_back(ParameterField::make());
    fields.back()->assign(field);
  }
}
//--------------------------------------------------------------------------------------------
void EquipmentParameter::clear()
{
  name.clear();
  eType = Sustain::UNKNOWN;
  qDeleteAll(fields);
  fields.clear();
  enumOptions.clear();
}
//-------------------------------------------------------------------------------
//! Equipment Parameter helper functions for Parameters
QQmlListProperty<ParameterField> EquipmentParameter::getParameterFields()
{
  return QQmlListProperty<ParameterField>(this, this,
                                          &EquipmentParameter::AppendParameterField,
                                          &EquipmentParameter::CountParameterFields,
                                          &EquipmentParameter::GetParameterField,
                                          &EquipmentParameter::ClearParameterFields);
}
//--------------------------------------------------------------------------------------------
void EquipmentParameter::AppendParameterField(QQmlListProperty<ParameterField>* list, ParameterField* value)
{
  EquipmentParameter* parameter = qobject_cast<EquipmentParameter*>(list->object);
  if (parameter) {
    parameter->fields.append(value);
  }
}
//--------------------------------------------------------------------------------------------
auto EquipmentParameter::CountParameterFields(QQmlListProperty<ParameterField>* list) -> int
{
  EquipmentParameter* parameter = qobject_cast<EquipmentParameter*>(list->object);
  if (parameter) {
    return parameter->fields.count();
  }
  return 0;
}
//--------------------------------------------------------------------------------------------
auto EquipmentParameter::GetParameterField(QQmlListProperty<ParameterField>* list, int index) -> ParameterField*
{
  EquipmentParameter* parameter = qobject_cast<EquipmentParameter*>(list->object);
  if (parameter) {
    return parameter->fields[index];
  }
  return nullptr;
}
//--------------------------------------------------------------------------------------------
void EquipmentParameter::ClearParameterFields(QQmlListProperty<ParameterField>* list)
{
  EquipmentParameter* parameter = qobject_cast<EquipmentParameter*>(list->object);
  if (parameter) {
    parameter->fields.clear();
  }
}
//--------------------------------------------------------------------------------------------
void EquipmentParameter::replaceField(int index, ParameterField* value)
{
  return fields.replace(index, value);
}
//--------------------------------------------------------------------------------------------
void EquipmentParameter::removeField(int index)
{
  fields.removeAt(index);
}
//--------------------------------------------------------------------------------------------
QString EquipmentParameter::toString() const
{
  QString value = QString("%1:%2").arg(name).arg(TypeToString(eType));
  for (auto options : enumOptions) {
    value += QString(",%1").arg(options);
  }
  for (auto field : fields) {
    value += QString(",%1").arg(field->toString());
  }
  return value;
}
//--------------------------------------------------------------------------------------------
QString EquipmentParameter::typeString() const
{
  return TypeToString(eType);
}
EquipmentParameter* EquipmentParameter::fromString(QString parameter_string, QObject* parent)
{
  return new EquipmentParameter(parameter_string, parent);
}
//--------------------------------------------------------------------------------------------

//!
//! Equipment Members
//!
Equipment::Equipment(QObject* parent)
  : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------
Equipment::~Equipment()
{
  clear();
}
//--------------------------------------------------------------------------------------------
bool Equipment::operator==(const Equipment& rhs) const
{
  return name == rhs.name
    && type == rhs.type
    && summary == rhs.summary
    && description == rhs.description
    && citations == rhs.citations
    && image == rhs.image
    && parameters == rhs.parameters; //TODO: We should iterate over each one and do a comparison.
}
//--------------------------------------------------------------------------------------------
bool Equipment::operator!=(const Equipment& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
Equipment* Equipment::make(QObject* parent)
{
  return new Equipment(parent);
}
//--------------------------------------------------------------------------------------------
void Equipment::appendParameter(QString name, Sustain::Type type, QList<QString> enumOptions)
{
  parameters.push_back(EquipmentParameter::make(name, type, std::move(enumOptions)));
}
//--------------------------------------------------------------------------------------------
void Equipment::appendParameter(std::string name, Sustain::Type type, QList<QString> enumOptions)
{
  parameters.push_back(EquipmentParameter::make(name.c_str(), type, std::move(enumOptions)));
}
//--------------------------------------------------------------------------------------------
void Equipment::assign(Equipment* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//--------------------------------------------------------------------------------------------
void Equipment::assign(const Equipment& rhs)
{
  id = rhs.id;
  uuid = rhs.uuid;
  name = rhs.name;
  type = rhs.type;
  summary = rhs.summary;
  description = rhs.description;
  citations = rhs.citations;
  image = rhs.image;
  parameters = rhs.parameters;
}
//--------------------------------------------------------------------------------------------
void Equipment::clear()
{
  id = -1;
  uuid = "";
  name.clear();
  type = -1;
  summary.clear();
  description.clear();
  // qDeleteAll(citations);
  citations.clear();
  image.clear();
  // qDeleteAll(parameters);
  parameters.clear();
}
//--------------------------------------------------------------------------------------------
void Equipment::clear(int index)
{
  id = -1;
  uuid = "";
  name = QString(" New Equipment:%1").arg(index);
  description = QString("Undefined equipment %1.").arg(index);
  type = -1;
  summary = QString("summary for equipment %1.").arg(index);
  citations.clear();
  image.clear();
  parameters.clear();
}
//--------------------------------------------------------------------------------------------
QQmlListProperty<Citation> Equipment::getCitations()
{
  return QQmlListProperty<Citation>(this, this,
                                    &Equipment::AppendCitation,
                                    &Equipment::CountCitations,
                                    &Equipment::GetCitation,
                                    &Equipment::ClearCitations);
}

//-------------------------------------------------------------------------------
//! Equipment helper functions for Equipments
void Equipment::AppendCitation(QQmlListProperty<Citation>* list, Citation* value)
{
  Equipment* EquipmentOccurance = qobject_cast<Equipment*>(list->object);
  if (EquipmentOccurance) {
    EquipmentOccurance->citations.append(value);
  }
}
//-------------------------------------------------------------------------------
Citation* Equipment::GetCitation(QQmlListProperty<Citation>* list, int index)
{
  Equipment* EquipmentOccurance = qobject_cast<Equipment*>(list->object);
  if (EquipmentOccurance) {
    return EquipmentOccurance->citations[index];
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
void Equipment::ClearCitations(QQmlListProperty<Citation>* list)
{
  Equipment* EquipmentOccurance = qobject_cast<Equipment*>(list->object);
  if (EquipmentOccurance) {
    return EquipmentOccurance->citations.clear();
  }
}
//-------------------------------------------------------------------------------
int Equipment::CountCitations(QQmlListProperty<Citation>* list)
{
  Equipment* EquipmentOccurance = qobject_cast<Equipment*>(list->object);
  if (EquipmentOccurance) {
    return EquipmentOccurance->citations.count();
  }
  return 0;
}
//-------------------------------------------------------------------------------
void Equipment::removeCitation(int index)
{
  citations.removeAt(index);
}
//-------------------------------------------------------------------------------
void Equipment::replaceCitation(int index, Citation* value)
{
  citations.replace(index, value);
}

//-------------------------------------------------------------------------------
//! Equipment helper functions for Traumas
QQmlListProperty<EquipmentParameter> Equipment::getParameters()
{
  return QQmlListProperty<EquipmentParameter>(this, this,
                                              &Equipment::AppendParameter,
                                              &Equipment::CountParameters,
                                              &Equipment::GetParameter,
                                              &Equipment::ClearParameters);
}
//-------------------------------------------------------------------------------
void Equipment::AppendParameter(QQmlListProperty<EquipmentParameter>* list, EquipmentParameter* value)
{
  Equipment* EquipmentOccurance = qobject_cast<Equipment*>(list->object);
  if (EquipmentOccurance) {
    EquipmentOccurance->parameters.append(value);
  }
}
//-------------------------------------------------------------------------------
auto Equipment::CountParameters(QQmlListProperty<EquipmentParameter>* list) -> int
{
  Equipment* EquipmentOccurance = qobject_cast<Equipment*>(list->object);
  if (EquipmentOccurance) {
    return EquipmentOccurance->parameters.count();
  }
  return 0;
}
//-------------------------------------------------------------------------------
auto Equipment::GetParameter(QQmlListProperty<EquipmentParameter>* list, int index) -> EquipmentParameter*
{
  Equipment* EquipmentOccurance = qobject_cast<Equipment*>(list->object);
  if (EquipmentOccurance) {
    return EquipmentOccurance->parameters[index];
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
void Equipment::ClearParameters(QQmlListProperty<EquipmentParameter>* list)
{
  Equipment* EquipmentOccurance = qobject_cast<Equipment*>(list->object);
  if (EquipmentOccurance) {
    return EquipmentOccurance->parameters.clear();
  }
}
//-------------------------------------------------------------------------------
void Equipment::replaceParameter(int index, EquipmentParameter* value)
{
  parameters.replace(index, value);
}
//-------------------------------------------------------------------------------
void Equipment::removeParameter(int index)
{
  parameters.removeAt(index);
}
//-------------------------------------------------------------------------------
QString Equipment::parametersToString()
{
  QString result;
  for (auto parm : parameters) {
    result += QString("%1;").arg(parm->toString());
  }
  if (!result.isEmpty()) {
    result.remove(result.size() - 1, 1);
  }
  return result;
}
//-------------------------------------------------------------------------------
void Equipment::parametersFromString(QString parameter_string)
{
  for (auto parameter : parameter_string.split(';')) {
    if (!parameter.isEmpty()) {
      parameters.push_back(EquipmentParameter::fromString(parameter, this));
    }
  }
}
