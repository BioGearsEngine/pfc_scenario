#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_ROLEMAP_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_ROLEMAP_H

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
struct Scene;

struct RoleMap : public QObject {
  Q_OBJECT
  Q_PROPERTY(int map_id MEMBER id)
  Q_PROPERTY(Scene* fk_scene MEMBER fk_scene)
  Q_PROPERTY(Role* fk_role MEMBER fk_role)

public:
  int32_t id = -1;
  Scene* fk_scene = nullptr;
  Role* fk_role = nullptr;

  RoleMap(QObject* parent = nullptr);
  RoleMap(const RoleMap&) = delete;
  RoleMap(RoleMap&&) = delete;
  RoleMap& operator=(const RoleMap&) = delete;
  RoleMap& operator=(RoleMap&&) = delete;
  virtual ~RoleMap() = default;

  bool operator==(const RoleMap& rhs) const;
  bool operator!=(const RoleMap& rhs) const;
  static Q_INVOKABLE RoleMap* make();
  Q_INVOKABLE void assign(RoleMap* rhs);
  void assign(const RoleMap& rhs);
};

#endif

