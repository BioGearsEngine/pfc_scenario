#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_SCENE_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_SCENE_H

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

struct Event;
struct Role;
struct EquipmentMap;

struct Scene : public QObject {
  Q_OBJECT
  Q_PROPERTY(int scene_id MEMBER id NOTIFY idChanged)
  Q_PROPERTY(QString uuid MEMBER uuid NOTIFY uuidChanged)
  Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)
  Q_PROPERTY(QString description MEMBER description NOTIFY descriptionChanged)
  Q_PROPERTY(int  timeOfDay MEMBER time_of_day NOTIFY timeOfDayChanged)
  Q_PROPERTY(int timeInSimulation MEMBER time_in_simulation NOTIFY timeInSimulationChanged)
  Q_PROPERTY(QString weather MEMBER weather NOTIFY weatherChanged)
  Q_PROPERTY(QQmlListProperty<Event> events READ getEvents  NOTIFY eventsChanged)
  Q_PROPERTY(QQmlListProperty<Role> roles READ getRoles  NOTIFY rolesChanged)

public:
  int32_t id = -1;
  QString uuid = "";
  QString name = "";
  QString description = "";
  int32_t time_of_day = 0;
  int32_t time_in_simulation = 0;
  QString weather = "";
  QList<Event*> events;
  QList<Role*> roles;
;
signals:
  void idChanged();
  void uuidChanged();
  void nameChanged();
  void descriptionChanged();
  void timeOfDayChanged();
  void timeInSimulationChanged();
  void weatherChanged();
  void eventsChanged();
  void rolesChanged();


public:
  Scene(QObject* parent = nullptr);
  Scene(const Scene&) = delete;
  Scene(Scene&&) = delete;
  Scene& operator=(const Scene&) = delete;
  Scene& operator=(Scene&&) = delete;
  virtual ~Scene() = default;

  bool operator==(const Scene& rhs) const;
  bool operator!=(const Scene& rhs) const;

  static Q_INVOKABLE Scene* make();

  Q_INVOKABLE void assign(Scene* rhs);
  void assign(const Scene& rhs);

  Q_INVOKABLE void clear();
  Q_INVOKABLE void clear(int index);

  Q_INVOKABLE void RemoveEvent(int index);
  Q_INVOKABLE void ReplaceEvent(int index, Event* value);
  Q_INVOKABLE void RemoveRole(int index);
  Q_INVOKABLE void ReplaceRole(int index, Role* value);

private:
  //! Helper functions for Events
  QQmlListProperty<Event> getEvents();
  static void AppendEvent(QQmlListProperty<Event>* list, Event* value);
  static auto GetEvent(QQmlListProperty<Event>* list, int index) -> Event*;
  static void ClearEvents(QQmlListProperty<Event>* list);
  static auto CountEvents(QQmlListProperty<Event>* list) -> int;

  //! Helper functions for Roles
  QQmlListProperty<Role> getRoles();
  static void AppendRole(QQmlListProperty<Role>* list, Role* value);
  static auto GetRole(QQmlListProperty<Role>* list, int index) -> Role*;
  static void ClearRoles(QQmlListProperty<Role>* list);
  static auto CountRoles(QQmlListProperty<Role>* list) -> int;
};

#endif