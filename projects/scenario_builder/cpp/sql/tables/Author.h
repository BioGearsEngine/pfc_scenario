#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_AUTHOR_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_AUTHOR_H

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

struct Author : public QObject {
  Q_OBJECT
  Q_PROPERTY(int author_id MEMBER id NOTIFY idChanged)
  Q_PROPERTY(QString uuid MEMBER uuid NOTIFY uuidChanged)
  Q_PROPERTY(QString first MEMBER first NOTIFY firstChanged)
  Q_PROPERTY(QString middle MEMBER middle NOTIFY middleChanged)
  Q_PROPERTY(QString last MEMBER last NOTIFY lastChanged)
  Q_PROPERTY(QString email MEMBER email NOTIFY emailChanged)
  Q_PROPERTY(QString zip MEMBER zip NOTIFY zipChanged)
  Q_PROPERTY(QString plus_4 MEMBER plus_4 NOTIFY plus4Changed)
  Q_PROPERTY(QString state MEMBER state NOTIFY stateChanged)
  Q_PROPERTY(QString country MEMBER country NOTIFY countryChanged)
  Q_PROPERTY(QString phone MEMBER phone NOTIFY phoneChanged)
  Q_PROPERTY(QString organization MEMBER organization NOTIFY organizationChanged)

signals:
  void idChanged();
  void uuidChanged();
  void firstChanged();
  void middleChanged();
  void lastChanged();
  void emailChanged();
  void zipChanged();
  void plus4Changed();
  void stateChanged();
  void countryChanged();
  void phoneChanged();
  void organizationChanged();
  void refreshed();
public:
  int32_t id = -1;
  QString uuid = "";
  QString first = "";
  QString middle = "";
  QString last = "";
  QString email = "";
  QString zip = "";
  QString plus_4 = "";
  QString state = "";
  QString country = "";
  QString phone = "";
  QString organization = "";

  Author(QObject* parent = nullptr);
  Author(const Author&) = delete;
  Author(Author&&) = delete;
  Author& operator=(const Author&) = delete;
  Author& operator=(Author&&) = delete;
  virtual ~Author() = default;

  bool operator==(const Author& rhs) const;
  bool operator!=(const Author& rhs) const;

  static Q_INVOKABLE Author* make();

  Q_INVOKABLE void assign(Author* rhs);
  void assign(const Author& rhs);

  Q_INVOKABLE void clear();
  Q_INVOKABLE void clear(int index);
};

#endif