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

enum class ParameterTypeEnum {
  UNKOWN,
  STRING,
  BOOLEAN,
  INTEGRAL,
  RANGE,
  SCALAR,
  ENUM,
  eOPTION
};

QString ParameterTypeEnumToString(ParameterTypeEnum value);
ParameterTypeEnum ParameterTypeEnumFromString(QString value);
ParameterTypeEnum ParameterTypeEnumFromString(std::string value);

Q_ENUMS(ParameterTypeEnum)

struct ParameterField : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)
  Q_PROPERTY(ParameterTypeEnum type MEMBER eType NOTIFY typeChanged)

signals:
  void nameChanged();
  void typeChanged();

public:
  QString name;
  ParameterTypeEnum eType;

  ParameterField(QObject* parent = nullptr);
  ParameterField(QString n, ParameterTypeEnum t, QObject* parent = nullptr);
  ParameterField(const ParameterField&) = delete;
  ParameterField(ParameterField&&) = delete;
  ParameterField& operator=(const ParameterField&) = delete;
  ParameterField& operator=(ParameterField&&) = delete;
  virtual ~ParameterField() = default;

  bool operator==(const ParameterField& rhs) const;
  bool operator!=(const ParameterField& rhs) const;

  static Q_INVOKABLE ParameterField* make(QObject* parent = nullptr);
  static Q_INVOKABLE ParameterField* make(QString name, ParameterTypeEnum type, QObject* parent = nullptr);

  Q_INVOKABLE QString toString();

  Q_INVOKABLE void assign(ParameterField* rhs);
  void assign(const ParameterField& rhs);

  void clear();
};

struct EquipmentParameter : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)
  Q_PROPERTY(ParameterTypeEnum type MEMBER eType NOTIFY typeChanged)
  Q_PROPERTY(QQmlListProperty<ParameterField> field READ getParameterFields NOTIFY fieldsChanged)
  Q_PROPERTY(QList<QString> enumOptions MEMBER enumOptions NOTIFY enumOptionsChanged)

signals:
  void nameChanged();
  void typeChanged();
  void fieldsChanged();
  void enumOptionsChanged();

public:
  QString name = "";
  ParameterTypeEnum eType = ParameterTypeEnum::UNKOWN;
  QList<ParameterField*> fields;
  QList<QString> enumOptions;

  EquipmentParameter(QObject* parent = nullptr);
  EquipmentParameter(QString parameter_string, QObject* parent);
  EquipmentParameter(QString n, ParameterTypeEnum t, QList<QString> enumOptions = {}, QObject* parent = nullptr);
  EquipmentParameter(const EquipmentParameter&) = delete;
  EquipmentParameter(EquipmentParameter&&) = delete;
  EquipmentParameter& operator=(const EquipmentParameter&) = delete;
  EquipmentParameter& operator=(EquipmentParameter&&) = delete;
  virtual ~EquipmentParameter();

  bool operator==(const EquipmentParameter& rhs) const;
  bool operator!=(const EquipmentParameter& rhs) const;

  static Q_INVOKABLE EquipmentParameter* make(QObject* make = nullptr);
  static Q_INVOKABLE EquipmentParameter* make(QString name, ParameterTypeEnum type, QList<QString> enumOptions = {}, QObject* make = nullptr);

  Q_INVOKABLE void appendField(QString name, ParameterTypeEnum type);
  Q_INVOKABLE void appendField(std::string name, ParameterTypeEnum type);

  Q_INVOKABLE void assign(EquipmentParameter* rhs);
  void assign(const EquipmentParameter& rhs);

  Q_INVOKABLE void clear();

  Q_INVOKABLE void replaceField(int index, ParameterField* value);
  Q_INVOKABLE void removeField(int index);

  Q_INVOKABLE QString toString();

  static EquipmentParameter* fromString(QString paramater_string, QObject* parent = nullptr);

private:
  QQmlListProperty<ParameterField> getParameterFields();
  static void AppendParameterField(QQmlListProperty<ParameterField>* list, ParameterField* value);
  static auto CountParameterFields(QQmlListProperty<ParameterField>* list) -> int;
  static auto GetParameterField(QQmlListProperty<ParameterField>* list, int index) -> ParameterField*;
  static void ClearParameterFields(QQmlListProperty<ParameterField>* list);
};

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
  void citationsChanged(QString);
  void imageChanged(QString);
  void propertiesChanged(QString);

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

  Q_INVOKABLE void appendParameter(QString name, ParameterTypeEnum type, QList<QString> enumOptions = {});
  Q_INVOKABLE void appendParameter(std::string name, ParameterTypeEnum type, QList<QString> enumOptions = {});

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