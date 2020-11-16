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
  Q_PROPERTY(int event_id MEMBER id NOTIFY idChanged)
  Q_PROPERTY(QString uuid MEMBER uuid NOTIFY uuidChanged)
  Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)
  Q_PROPERTY(QString description MEMBER description NOTIFY descriptionChanged)
  Q_PROPERTY(QString category MEMBER category NOTIFY categoryChanged)
  Q_PROPERTY(QString fidelity MEMBER fidelity NOTIFY fidelityChanged)

  Q_PROPERTY(Role* actorOne MEMBER fk_actor_1 NOTIFY actorOneChanged)
  Q_PROPERTY(Role* actorTwo MEMBER fk_actor_2 NOTIFY actorTwoChanged)
  Q_PROPERTY(Equipment* fk_equipment MEMBER fk_equipment NOTIFY equipmentChanged)

  Q_PROPERTY(QString details MEMBER details NOTIFY detailsChanged)
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

signals:
  void idChanged();
  void uuidChanged();
  void nameChanged();
  void descriptionChanged();
  void categoryChanged();
  void fidelityChanged();
  void actorOneChanged();
  void actorTwoChanged();
  void equipmentChanged();
  void detailsChanged();

public:
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

  Q_INVOKABLE void clear();
  Q_INVOKABLE void clear(int index);
};

#endif