#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_PROPERTY_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_PROPERTY_H

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

struct Property : public QObject {
  Q_OBJECT
  Q_PROPERTY(int property_id MEMBER id)
  Q_PROPERTY(QString uuid MEMBER uuid)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(QString value MEMBER value)
public:
  int32_t id = -1;
  QString uuid = "";
  QString name = "";
  QString value = "";

  Property(QObject* parent = nullptr);
  Property(const Property&) = delete;
  Property(Property&&) = delete;
  Property& operator=(const Property&) = delete;
  Property& operator=(Property&&) = delete;
  virtual ~Property() = default;

  bool operator==(const Property& rhs) const;
  bool operator!=(const Property& rhs) const;

  static Q_INVOKABLE Property* make();

  Q_INVOKABLE void assign(Property* rhs);
  void assign(const Property& rhs);

  void clear();
};

#endif