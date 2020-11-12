#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_LOCATIONMAP_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_LOCATIONMAP_H

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
struct Location;

struct LocationMap : public QObject {
  Q_OBJECT
  Q_PROPERTY(Scene* fk_scene MEMBER fk_scene)
  Q_PROPERTY(Location* fk_location MEMBER fk_location)

public:
  int32_t id = -1;
  Scene* fk_scene = nullptr;
  Location* fk_location = nullptr;

  LocationMap(QObject* parent = nullptr);
  LocationMap(Scene* scene, Location* location, QObject* parent = nullptr);
  LocationMap(const LocationMap&) = delete;
  LocationMap(LocationMap&&) = delete;
  LocationMap& operator=(const LocationMap&) = delete;
  LocationMap& operator=(LocationMap&&) = delete;
  virtual ~LocationMap() = default;

  bool operator==(const LocationMap& rhs) const;
  bool operator!=(const LocationMap& rhs) const;

  static Q_INVOKABLE LocationMap* make();

  Q_INVOKABLE void assign(LocationMap* rhs);
  void assign(const LocationMap& rhs);

  void clear();
};

#endif