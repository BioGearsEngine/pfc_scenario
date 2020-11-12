#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_ROLE_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_ROLE_H

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

struct TraumaProfile;

struct Role : public QObject {
  Q_OBJECT
  Q_PROPERTY(int role_id MEMBER id)
  Q_PROPERTY(QString uuid MEMBER uuid)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(QString description MEMBER description)
  Q_PROPERTY(QString short_name MEMBER description)
  Q_PROPERTY(TraumaProfile* trauma_profile MEMBER trauma_profile)
public:
  int32_t id = -1;
  QString uuid = "";
  QString name = "";
  QString short_name = "";
  QString description = "";
  TraumaProfile* trauma_profile = nullptr;

  Role(QObject* parent = nullptr);
  Role(TraumaProfile* profile, QObject* parent = nullptr);
  Role(const Role&) = delete;
  Role(Role&&) = delete;
  Role& operator=(const Role&) = delete;
  Role& operator=(Role&&) = delete;
  virtual ~Role() = default;

  bool operator==(const Role& rhs) const;
  bool operator!=(const Role& rhs) const;

  static Q_INVOKABLE Role* make();

  Q_INVOKABLE void assign(Role* rhs);
  void assign(const Role& rhs);

  void clear();
};

#endif