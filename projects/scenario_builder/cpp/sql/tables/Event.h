#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_EVENT_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_EVENT_H

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

struct Role;
struct Equipment;

struct Event : public QObject {
  Q_OBJECT
  Q_PROPERTY(int event_id MEMBER id)
  Q_PROPERTY(QString uuid MEMBER uuid)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(QString description MEMBER description)
  Q_PROPERTY(QString category MEMBER category)
  Q_PROPERTY(QString fidelity MEMBER fidelity)

  Q_PROPERTY(Role* actor_1 MEMBER fk_actor_1)
  Q_PROPERTY(Role* actor_2 MEMBER fk_actor_2)
  Q_PROPERTY(Equipment* fk_equipmet MEMBER fk_equipment)

  Q_PROPERTY(QString details MEMBER details)
public:
  int32_t id = -1;
  QString uuid = "";
  QString name = "";
  QString description = "";
  QString category = "";
  QString fidelity = "";
  Role* fk_actor_1;
  Role* fk_actor_2;
  Equipment* fk_equipment;
  QString details = "";

  Event(QObject* parent = nullptr);
  Event(Role* source, Role* target, Equipment* equipment, QObject* parent = nullptr);
  Event(const Event&) = delete;
  Event(Event&&) = delete;
  Event& operator=(const Event&) = delete;
  Event& operator=(Event&&) = delete;
  virtual ~Event() = default;

  bool operator==(const Event& rhs) const;
  bool operator!=(const Event& rhs) const;

  static Q_INVOKABLE Event* make();

  Q_INVOKABLE void assign(Event* rhs);
  void assign(const Event& rhs);

  void clear();
};

#endif