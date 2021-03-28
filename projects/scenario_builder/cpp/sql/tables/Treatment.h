#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_TREATMENT_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_TREATMENT_H

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

#include "Image.h"

struct Equipment;
struct Citation;

struct Treatment : public QObject {
  Q_OBJECT
  Q_PROPERTY(int treatment_id MEMBER id NOTIFY idChanged)
  Q_PROPERTY(QString uuid MEMBER uuid NOTIFY uuidChanged)
  Q_PROPERTY(QString medicalName MEMBER medical_name NOTIFY medicalNameChanged)
  Q_PROPERTY(QString commonName MEMBER common_name NOTIFY commonNameChanged)
  Q_PROPERTY(QString description MEMBER description NOTIFY descriptionChanged)
  Q_PROPERTY(Image* image MEMBER fk_image NOTIFY imageChanged)
  Q_PROPERTY(QQmlListProperty<Equipment> equipment READ getEquipment NOTIFY equipmentChanged)
  Q_PROPERTY(QQmlListProperty<Citation> citations READ getCitations NOTIFY citationsChanged)
  Q_PROPERTY(QQmlListProperty<Citation> cpgs READ getCPGS NOTIFY cpgsChanged)

signals:
  void idChanged();
  void uuidChanged();
  void medicalNameChanged();
  void commonNameChanged();
  void descriptionChanged();
  void equipmentChanged();
  void citationsChanged();
  void cpgsChanged();
  void imageChanged();

public:
  int32_t id = -1;
  QString uuid = "";
  QString medical_name = "";
  QString common_name = "";
  QString description = "";
  QList<Equipment*> equipment;
  QList<Citation*> citations;
  QList<Citation*> cpgs;
  Image* fk_image;

  Treatment(QObject* parent = nullptr);
  Treatment(const Treatment&) = delete;
  Treatment(Treatment&&) = delete;
  Treatment& operator=(const Treatment&) = delete;
  Treatment& operator=(Treatment&&) = delete;
  virtual ~Treatment() = default;

  bool operator==(const Treatment& rhs) const;
  bool operator!=(const Treatment& rhs) const;
  static Q_INVOKABLE Treatment* make();
  Q_INVOKABLE void assign(Treatment* rhs);
  void assign(const Treatment& rhs);

  Q_INVOKABLE void clear();
  Q_INVOKABLE void clear(int index);

  Q_INVOKABLE void RemoveCitation(int index);
  Q_INVOKABLE void ReplaceCitation(int index, Citation* value);
  Q_INVOKABLE void RemoveCPG(int index);
  Q_INVOKABLE void ReplaceCPG(int index, Citation* value);
  Q_INVOKABLE void RemoveEquipment(int index);
  Q_INVOKABLE void ReplaceEquipment(int index, Equipment* value);

private:
  //! Helper functions for Citations
  QQmlListProperty<Citation> getCitations();
  static void AppendCitation(QQmlListProperty<Citation>* list, Citation* value);
  static auto GetCitation(QQmlListProperty<Citation>* list, int index) -> Citation*;
  static void ClearCitations(QQmlListProperty<Citation>* list);
  static auto CountCitations(QQmlListProperty<Citation>* list) -> int;

  //! Helper functions for CPGs
  QQmlListProperty<Citation> getCPGS();
  static void AppendCPG(QQmlListProperty<Citation>* list, Citation* value);
  static auto GetCPG(QQmlListProperty<Citation>* list, int index) -> Citation*;
  static void ClearCPGs(QQmlListProperty<Citation>* list);
  static auto CountCPGs(QQmlListProperty<Citation>* list) -> int;

  //! Helper functions for Equipments
  QQmlListProperty<Equipment> getEquipment();
  static void AppendEquipment(QQmlListProperty<Equipment>* list, Equipment* value);
  static auto GetEquipment(QQmlListProperty<Equipment>* list, int index) -> Equipment*;
  static void ClearEquipments(QQmlListProperty<Equipment>* list);
  static auto CountEquipments(QQmlListProperty<Equipment>* list) -> int;
};

#endif