
#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_EQUIPMENTMAP_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_EQUIPMENTMAP_H

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
#include <QString>

struct Scene;
struct Equipment;

struct EquipmentMap : public QObject {

  Q_OBJECT
  Q_PROPERTY(int equipmentMap_id MEMBER id NOTIFY idChanged)
  Q_PROPERTY(Scene* scene MEMBER scene NOTIFY sceneChanged)
  Q_PROPERTY(Equipment* equipment MEMBER equipment NOTIFY equipmentChanged)
  Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)
  Q_PROPERTY(QString values MEMBER property_values NOTIFY valuesChanged)
  Q_PROPERTY(QString notes MEMBER notes NOTIFY notesChanged)

signals:
  void idChanged(int);
  void sceneChanged(Scene*);
  void equipmentChanged(Equipment*);
  void nameChanged(QString);
  void valuesChanged(QString);
  void notesChanged(QString);

public:
  int32_t id = -1;
  Scene* scene = nullptr;
  Equipment* equipment = nullptr;
  QString name = "";
  QString property_values = "";
  QString notes;
  ;
  EquipmentMap(QObject* parent = nullptr);
  EquipmentMap(Scene* s, Equipment* e, QObject* parent = nullptr);

  EquipmentMap(const EquipmentMap&) = delete;
  EquipmentMap(EquipmentMap&&) = delete;
  EquipmentMap& operator=(const EquipmentMap&) = delete;
  EquipmentMap& operator=(EquipmentMap&&) = delete;
  virtual ~EquipmentMap() = default;

  bool operator==(const EquipmentMap& rhs) const;
  bool operator!=(const EquipmentMap& rhs) const;

  static Q_INVOKABLE EquipmentMap* make();

  Q_INVOKABLE void assign(EquipmentMap* rhs);
  void assign(const EquipmentMap& rhs);

  Q_INVOKABLE void clear();
};

#endif