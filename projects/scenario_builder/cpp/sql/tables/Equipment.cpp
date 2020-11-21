#include "Equipment.h"
#include "Citation.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>

QString ParameterTypeEnumToString(ParameterTypeEnum value)
{
  if (value == ParameterTypeEnum::STRING) {
    return "STRING";
  } else if (value == ParameterTypeEnum::BOOLEAN) {
    return "BOOLEAN";
  } else if (value == ParameterTypeEnum::INTEGRAL) {
    return "INTEGRAL";
  } else if (value == ParameterTypeEnum::RANGE) {
    return "RANGE";
  } else if (value == ParameterTypeEnum::SCALAR) {
    return "SCALAR";
  } else if (value == ParameterTypeEnum::ENUM) {
    return "ENUM";
  } else if (value == ParameterTypeEnum::eOPTION) {
    return "OPTION";
  } else {
    return "UNKOWN";
  }
}
//--------------------------------------------------------------------------------------------
ParameterTypeEnum ParameterTypeEnumFromString(QString value)
{
  if (value == "STRING") {
    return ParameterTypeEnum::STRING;
  } else if (value == "BOOLEAN") {
    return ParameterTypeEnum::BOOLEAN;
  } else if (value == "INTEGRAL" || value == "INTEGER") {
    return ParameterTypeEnum::INTEGRAL;
  } else if (value == "RANGE") {
    return ParameterTypeEnum::RANGE;
  } else if (value == "SCALAR") {
    return ParameterTypeEnum::SCALAR;
  } else if (value == "ENUM") {
    return ParameterTypeEnum::ENUM;
  } else if (value == "OPTION") {
    return ParameterTypeEnum::eOPTION;
  }

  QRegularExpression enumRegxp { R"(\s*ENUM\s*{\s*(.*)\s*}\s*)" };
  QRegularExpressionMatch enumMatches;
  enumMatches = enumRegxp.match(value);
  if (enumMatches.hasMatch()) {
    return ParameterTypeEnum::ENUM;
  } else {
    return ParameterTypeEnum::UNKOWN;
  }
}
//--------------------------------------------------------------------------------------------
ParameterTypeEnum ParameterTypeEnumFromString(std::string value)
{
  return ParameterTypeEnumFromString(QString(value.c_str()));
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
ParameterField::ParameterField(QString n, ParameterTypeEnum t, QObject* parent)
  : QObject(parent)
  , name(n)
  , eType(t)
{
}
//--------------------------------------------------------------------------------------------
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
ParameterField* ParameterField::make(QString name, ParameterTypeEnum type, QObject* parent)
{
  return new ParameterField(name, type, parent);
}
//--------------------------------------------------------------------------------------------
QString ParameterField::toString()
{
  return QString("%1:%2").arg(name).arg(ParameterTypeEnumToString(eType));
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
  eType = ParameterTypeEnum::SCALAR;
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
  , eType(ParameterTypeEnum::UNKOWN)
{

  //This constructor needs to properly report errors loading text files
  //means we could run in to any number of exceptions.

  //Error 1: General Parse Error
  //Error 2: First Name:Type does not resovle to a known eType
  //Error 3: Any Param that doesn't properly parse

  for (auto param : parameter_string.split(',')) {
    auto parts = param.split(':');
    if (eType == ParameterTypeEnum::UNKOWN) {
      name = parts[0];
      eType = ParameterTypeEnumFromString(parts[1].trimmed());
    } else if (eType == ParameterTypeEnum::ENUM) {
      enumOptions.push_back(param.trimmed());
    } else {
      fields.push_back(ParameterField::make(parts[0].trimmed(), ParameterTypeEnumFromString(parts[1].trimmed()), this));
    }
  }
}
//--------------------------------------------------------------------------------------------
EquipmentParameter::EquipmentParameter(QString n, ParameterTypeEnum t, QList<QString> eo, QObject* parent)
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
EquipmentParameter* EquipmentParameter::make(QString name, ParameterTypeEnum type, QList<QString> enumOptions, QObject* make)
{
  return new EquipmentParameter(name, type, std::move(enumOptions));
}
//--------------------------------------------------------------------------------------------
void EquipmentParameter::appendField(QString name, ParameterTypeEnum type)
{
  fields.append(ParameterField::make(name, type));
}
//--------------------------------------------------------------------------------------------
void EquipmentParameter::appendField(std::string name, ParameterTypeEnum type)
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
  eType = ParameterTypeEnum::UNKOWN;
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
QString EquipmentParameter::toString()
{
  QString value = QString("%1:%2").arg(name).arg(ParameterTypeEnumToString(eType));
  for (auto options : enumOptions) {
    value += QString(",%1").arg(options);
  }
  for (auto field : fields) {
    value += QString(",%1").arg(field->toString());
  }
  return value;
}
//--------------------------------------------------------------------------------------------
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
void Equipment::appendParameter(QString name, ParameterTypeEnum type, QList<QString> enumOptions)
{
  parameters.push_back(EquipmentParameter::make(name, type, std::move(enumOptions)));
}
//--------------------------------------------------------------------------------------------
void Equipment::appendParameter(std::string name, ParameterTypeEnum type, QList<QString> enumOptions)
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
  qDeleteAll(citations);
  citations.clear();
  image.clear();
  qDeleteAll(parameters);
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
