#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_EQUIPMENT_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_EQUIPMENT_H

/**************************************************************************************
Copyright 2019 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#include <QList>
#include <QObject>
#include <QQmlListProperty>
#include <QString>

struct Citation;


class Sustain : public QObject {
Q_OBJECT
public:
Sustain(QObject* parent = nullptr);

  enum Type {
  UNKNOWN = 0,
  STRING,
  BOOLEAN,
  INTEGRAL,
  REAL,
  RANGE,
  SCALAR,
  ENUM,
  eOPTION,  
  CONST
  };
  Q_ENUM(Type)

};

                                                                                                        
QString TypeToString(Sustain::Type value);
Sustain::Type TypeFromString(QString value);
Sustain::Type TypeFromString(std::string value);
                                                                                                                                                                                                                                      

struct ParameterField : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)
  Q_PROPERTY(Sustain::Type type READ type WRITE type NOTIFY typeChanged)
  Q_PROPERTY(QString typeString READ typeString NOTIFY typeChanged)
  Q_PROPERTY(QVariant value READ value WRITE value NOTIFY variantValueChanged)
signals:
  void nameChanged();
  void typeChanged();

public:
  QString name;
  Sustain::Type eType;
  QVariant _value;

  ParameterField(QObject* parent = nullptr);
  ParameterField(QString n, Sustain::Type t, QVariant value = QVariant(), QObject* parent = nullptr);
  ParameterField(const ParameterField&) = delete;
  ParameterField(ParameterField&&) = delete;
  ParameterField& operator=(const ParameterField&) = delete;
  ParameterField& operator=(ParameterField&&) = delete;
  virtual ~ParameterField() = default;

  Sustain::Type type() const;
  void type(Sustain::Type type);

  bool operator==(const ParameterField& rhs) const;
  bool operator!=(const ParameterField& rhs) const;

  static Q_INVOKABLE ParameterField* make(QObject* parent = nullptr);
  static Q_INVOKABLE ParameterField* make(QString name, Sustain::Type type, QObject* parent = nullptr);
  static Q_INVOKABLE ParameterField* make(QString name, Sustain::Type type, QVariant value, QObject* parent = nullptr);

  QVariant value() const;
  void value(QVariant);

  Q_INVOKABLE QString toString() const;
  Q_INVOKABLE QString typeString() const;

  Q_INVOKABLE void assign(ParameterField* rhs);
  void assign(const ParameterField& rhs);

  void clear();

signals:
    void variantValueChanged(QVariant value);
};


//!
//!   Equipment Parmaters are fields for storing values that can be changed when an equipment is instanced
//!
//!   While EquipmentParameter is designed to be generic allowing custom types with arbitary subfields
//!   Currently the underlying type is limited to the following 
//!    UNKNOWN,
//!    STRING,  0 Subfields  1 Implict Field Value  [Any Arbitary Char String]
//!    BOOLEAN, 0 Subfields  1 Implict Field Value  [0=False 1=True]
//!    INTEGRAL 0 Subfields  1 Implict Field Value  [64bit integer]
//!    RANGE,   0 Subfields [MIN,MAX] 1 Implicit Field Value   [ITEEE double 734]
//!    SCALAR,
//!    ENUM     N subfields (stored internally as enumOptions)  Values Enum can be turned in to
//!    eOPTION  Enum Value used only programatically for subfields of an enum
//!    REAL     Subtype for ParameterFields for the numerical component of a SCALAR. For demensional SCALARS  use SCALAR and set the enum component to None
//!    
//!    Currently setting the type using the constructor or Type(Sustain::Type) functions will erase the curent fields and enum options
//!    To create a properly formed paramater of that type.  Additiona/Fields can be created and removed once this is done for the creation of 
//!    CUSTOM parameters but it is best left to leave the SUSTAIN::TYPE to UNKNOWN when this occurs and the UI currently will not reflect these p
//!    parameters correctly. Or Range from -Inf , +Inf

struct EquipmentParameter : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)
  Q_PROPERTY(Sustain::Type type READ Type WRITE Type NOTIFY typeChanged)
  Q_PROPERTY(QQmlListProperty<ParameterField> fields READ getParameterFields NOTIFY fieldsChanged)
  Q_PROPERTY(QList<QString> enumOptions MEMBER enumOptions NOTIFY enumOptionsChanged)
  Q_PROPERTY(QString typeString READ typeString NOTIFY typeChanged)

signals:
  void nameChanged();
  void typeChanged();
  void fieldsChanged();
  void enumOptionsChanged();

public:
  QString name = "";
  Sustain::Type eType = Sustain::UNKNOWN;
  QList<ParameterField*> fields;
  QList<QString> enumOptions;

  EquipmentParameter(QObject* parent = nullptr);
  EquipmentParameter(QString parameter_string, QObject* parent);
  EquipmentParameter(QString n, Sustain::Type t, QList<QString> enumOptions = {}, QObject* parent = nullptr);
  EquipmentParameter(const EquipmentParameter&) = delete;
  EquipmentParameter(EquipmentParameter&&) = delete;
  EquipmentParameter& operator=(const EquipmentParameter&) = delete;
  EquipmentParameter& operator=(EquipmentParameter&&) = delete;
  virtual ~EquipmentParameter();

  bool operator==(const EquipmentParameter& rhs) const;
  bool operator!=(const EquipmentParameter& rhs) const;

  static Q_INVOKABLE EquipmentParameter* make(QObject* make = nullptr);
  static Q_INVOKABLE EquipmentParameter* make(QString name, Sustain::Type type, QList<QString> enumOptions = {}, QObject* make = nullptr);

  Sustain::Type Type() const;
  void Type(Sustain::Type type);

  Q_INVOKABLE void appendField(QString name, Sustain::Type type);
  Q_INVOKABLE void appendField(std::string name, Sustain::Type type);

  Q_INVOKABLE void assign(EquipmentParameter* rhs);
  void assign(const EquipmentParameter& rhs);

  Q_INVOKABLE void clear();


  Q_INVOKABLE void replaceField(int index, ParameterField* value);
  Q_INVOKABLE void removeField(int index);

  Q_INVOKABLE QString toString() const;
  Q_INVOKABLE QString typeString() const;

  static EquipmentParameter* fromString(QString paramater_string, QObject* parent = nullptr);

private:
  QQmlListProperty<ParameterField> getParameterFields();
  static void AppendParameterField(QQmlListProperty<ParameterField>* list, ParameterField* value);
  static auto CountParameterFields(QQmlListProperty<ParameterField>* list) -> int;
  static auto GetParameterField(QQmlListProperty<ParameterField>* list, int index) -> ParameterField*;
  static void ClearParameterFields(QQmlListProperty<ParameterField>* list);
};

//!
//!  Equipment is any item that cna be used in a scenario 
//!
//!
//! 
//! Type is defined as the following categories
    //0: OTHER
    //1: Equipment (IV Pole, Monitor, Oxygen Tank)
    //2: Attachment (Tempus Pro, Splint)
    //3:Substance (Drugs)
    //4:Consumable Equipment (IV Bags, Syringes)
struct Equipment : public QObject {
  Q_OBJECT
  Q_PROPERTY(int equipment_id MEMBER id NOTIFY idChanged)
  Q_PROPERTY(QString uuid MEMBER uuid NOTIFY uuidChanged)
  Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)
  Q_PROPERTY(int type MEMBER type NOTIFY typeChanged)
  Q_PROPERTY(QString summary MEMBER summary NOTIFY summaryChanged)
  Q_PROPERTY(QString description MEMBER description NOTIFY descriptionChanged)
  Q_PROPERTY(QQmlListProperty<Citation> citations READ getCitations NOTIFY citationsChanged)
  Q_PROPERTY(QString image MEMBER image NOTIFY imageChanged)
  Q_PROPERTY(QQmlListProperty<EquipmentParameter> parameters READ getParameters NOTIFY propertiesChanged)

public:
  int32_t id = -1;
  QString uuid = "";
  int32_t type = 1;
  QString name = "";
  QString summary = "";
  QString description = "";
  QList<Citation*> citations;
  QString image = "";
  QList<EquipmentParameter*> parameters;

signals:
  void idChanged(int);
  void uuidChanged(QString);
  void nameChanged(QString);
  void typeChanged(int);
  void summaryChanged(QString);
  void descriptionChanged(QString);
  void citationsChanged();
  void imageChanged(QString);
  void propertiesChanged();
  void refreshed();

public:
  Equipment(QObject* parent = nullptr);
  Equipment(const Equipment&) = delete;
  Equipment(Equipment&&) = delete;
  Equipment& operator=(const Equipment&) = delete;
  Equipment& operator=(Equipment&&) = delete;
  virtual ~Equipment();

  bool operator==(const Equipment& rhs) const;
  bool operator!=(const Equipment& rhs) const;

  static Q_INVOKABLE Equipment* make(QObject* parent = nullptr);

  Q_INVOKABLE void appendParameter(QString name, Sustain::Type type, QList<QString> enumOptions = {});
  Q_INVOKABLE void appendParameter(std::string name, Sustain::Type type, QList<QString> enumOptions = {});

  Q_INVOKABLE void assign(Equipment* rhs);
  void assign(const Equipment& rhs);

  Q_INVOKABLE void clear();
  Q_INVOKABLE void clear(int index);

  Q_INVOKABLE void replaceCitation(int index, Citation* value);
  Q_INVOKABLE void removeCitation(int index);
  Q_INVOKABLE void replaceParameter(int index, EquipmentParameter* value);
  Q_INVOKABLE void removeParameter(int index);

  Q_INVOKABLE QString parametersToString();
  Q_INVOKABLE void parametersFromString(QString parameter_string);

private:
  //! Helper functions for Citations
  QQmlListProperty<Citation> getCitations();
  static void AppendCitation(QQmlListProperty<Citation>* list, Citation* value);
  static auto CountCitations(QQmlListProperty<Citation>* list) -> int;
  static auto GetCitation(QQmlListProperty<Citation>* list, int index) -> Citation*;
  static void ClearCitations(QQmlListProperty<Citation>* list);

  //! Helper functions for Traumas
  QQmlListProperty<EquipmentParameter> getParameters();
  static void AppendParameter(QQmlListProperty<EquipmentParameter>* list, EquipmentParameter* value);
  static auto CountParameters(QQmlListProperty<EquipmentParameter>* list) -> int;
  static auto GetParameter(QQmlListProperty<EquipmentParameter>* list, int index) -> EquipmentParameter*;
  static void ClearParameters(QQmlListProperty<EquipmentParameter>* list);
};

#endif