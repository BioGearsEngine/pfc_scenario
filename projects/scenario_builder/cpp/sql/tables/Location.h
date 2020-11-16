#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_LOCATION_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_LOCATION_H

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

struct Location : public QObject {
  Q_OBJECT
  Q_PROPERTY(int location_id MEMBER id NOTIFY idChanged)
  Q_PROPERTY(QString uuid MEMBER uuid NOTIFY uuidChanged)
  Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)
  Q_PROPERTY(QString description MEMBER description NOTIFY descriptionChanged)
  Q_PROPERTY(QString environment MEMBER environment NOTIFY environmentChanged)

public:
  int32_t id = -1;
  QString uuid = "";
  QString name = "";
  QString description = "";
  QString environment = "";

signals:
  void idChanged();
  void uuidChanged();
  void nameChanged();
  void descriptionChanged();
  void environmentChanged();

public:
  Location(QObject* parent = nullptr);
  Location(const Location&) = delete;
  Location(Location&&) = delete;
  Location& operator=(const Location&) = delete;
  Location& operator=(Location&&) = delete;
  virtual ~Location() = default;

  bool operator==(const Location& rhs) const;
  bool operator!=(const Location& rhs) const;

  static Q_INVOKABLE Location* make();

  Q_INVOKABLE void assign(Location* rhs);
  void assign(const Location& rhs);

  void clear();
  void clear(int index);
};

#endif