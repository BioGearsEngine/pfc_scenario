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

enum EquipmentTypeEnum {
  
};
Q_ENUMS(EquipmentTypeEnum)

// struct EquipmentParameter : public QObject {
//   Q_OBJECT
//   Q_PROPERTY(EquipmentParameter type MEMBER description NOTIFY descriptionChanged)
//   Q_PROPERTY(QString location MEMBER location NOTIFY locationChanged)
//   Q_PROPERTY(QString severity MEMBER severity NOTIFY severityChanged)
//
// signals:
//   void idChanged();
//   void traumaChanged();
//   void descriptionChanged();
//   void locationChanged();
//   void severityChanged();
//
// public:
//
//   QString description = "";
//   QString location = "";
//   QString severity = "";
//
//   EquipmentParameter(QObject* parent = nullptr);
//     EquipmentParameter(const EquipmentParameter&) = delete;
//   EquipmentParameter(EquipmentParameter&&) = delete;
//   EquipmentParameter& operator=(const EquipmentParameter&) = delete;
//   EquipmentParameter& operator=(EquipmentParameter&&) = delete;
//   virtual ~EquipmentParameter() = default;
//
//   bool operator==(const EquipmentParameter& rhs) const;
//   bool operator!=(const EquipmentParameter& rhs) const;
//   static Q_INVOKABLE EquipmentParameter* make();
//   Q_INVOKABLE void assign(EquipmentParameter* rhs);
//   void assign(const EquipmentParameter& rhs);
//   void clear();
// };

//----End Event
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
  Q_PROPERTY(QString properties MEMBER properties NOTIFY propertiesChanged)
public:
  int32_t id = -1;
  QString uuid = "";
  int32_t type = 1;
  QString name = "";
  QString summary = "";
  QString description = "";
  QList<Citation*> citations;
  QString image = "";
  QString properties = "";

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
  virtual ~Equipment() = default;

  bool operator==(const Equipment& rhs) const;
  bool operator!=(const Equipment& rhs) const;

  static Q_INVOKABLE Equipment* make();

  Q_INVOKABLE void assign(Equipment* rhs);
  void assign(const Equipment& rhs);

  Q_INVOKABLE void clear();
  Q_INVOKABLE void clear(int index);

  Q_INVOKABLE void replaceCitation(int index, Citation* value);
  Q_INVOKABLE void removeCitation(int index);

private:
  //! Helper functions for Citations
  QQmlListProperty<Citation> getCitations();
  static void AppendCitation(QQmlListProperty<Citation>* list, Citation* value);
  static auto CountCitations(QQmlListProperty<Citation>* list) -> int;
  static auto GetCitation(QQmlListProperty<Citation>* list, int index) -> Citation*;
  static void ClearCitations(QQmlListProperty<Citation>* list);

  //   //! Helper functions for Traumas
  // QQmlListProperty<Parameter> getParameters();
  // static void AppendParameter(QQmlListProperty<Parameter>* list, Parameter* value);
  // static auto CountParameters(QQmlListProperty<Parameter>* list) -> int;
  // static auto GetParameter(QQmlListProperty<Parameter>* list, int index) -> Parameter*;
  // static void ClearParameters(QQmlListProperty<Parameter>* list);
};

#endif