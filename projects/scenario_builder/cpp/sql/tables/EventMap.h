#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_EVENTMAP_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_EVENTMAP_H

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
struct Event;

struct EventMap : public QObject {
  Q_OBJECT
  Q_PROPERTY(int map_id MEMBER id NOTIFY idChanged)
  Q_PROPERTY(Scene* scene MEMBER fk_scene NOTIFY sceneChanged)
  Q_PROPERTY(Event* event MEMBER fk_event NOTIFY eventChanged)

public:
  int32_t id = -1;
  Scene* fk_scene = nullptr;
  Event* fk_event = nullptr;

signals:
  void idChanged();
  void sceneChanged();
  void eventChanged();

public:
  EventMap(QObject* parent = nullptr);
  EventMap(Scene* scene, Event* event, QObject* parent = nullptr);

  EventMap(const EventMap&) = delete;
  EventMap(EventMap&&) = delete;
  EventMap& operator=(const EventMap&) = delete;
  EventMap& operator=(EventMap&&) = delete;
  virtual ~EventMap() = default;

  bool operator==(const EventMap& rhs) const;
  bool operator!=(const EventMap& rhs) const;

  static Q_INVOKABLE EventMap* make();

  Q_INVOKABLE void assign(EventMap* rhs);
  void assign(const EventMap& rhs);

  Q_INVOKABLE void clear();
};

#endif