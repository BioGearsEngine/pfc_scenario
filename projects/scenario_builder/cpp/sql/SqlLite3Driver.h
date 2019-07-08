#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_DRVER_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_DRVER_H

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

#include "sqlite3.h"

#include <QList>
#include <QObject>
#include <QString>
#include <qobjectdefs.h>

struct sqlite3;

namespace pfc {

enum Sqlite3Table {
  AUTHORS,
  RESTRICTIONS,
  PROPERTIES,
};

struct Author {
  int32_t id;
  QString frist;
  QString last;
  QString email;
  QString zip;
  QString plus_4;
  QString state;
  QString country;
  QString phone;
  QString organization;
};

struct Property {
  int32_t id;
  QString name;
  QString value;
};

struct Restriction {
  int32_t id;
  QString name;
  QString value;
};

class SQLite3Driver : public QObject {
public:
  Q_OBJECT

  Q_PROPERTY(QString name READ Name WRITE Name NOTIFY nameChanged)
  Q_PROPERTY(QString path READ Path WRITE Path NOTIFY pathChanged)

public:
  explicit SQLite3Driver();
  SQLite3Driver(const std::string& dbName, const std::string& path = "./");
  SQLite3Driver(const SQLite3Driver&) = default;
  SQLite3Driver(SQLite3Driver&&) = default;
  SQLite3Driver& operator=(const SQLite3Driver&) = default;
  SQLite3Driver& operator=(SQLite3Driver&&) = default;
  ~SQLite3Driver() = default;

  Q_INVOKABLE bool open(QString db_name);
  Q_INVOKABLE bool close();

  Q_INVOKABLE bool initialize_db();

  Q_INVOKABLE bool clear_db();
  Q_INVOKABLE bool clear_table(enum Sqlite3Table);

  Q_INVOKABLE QList<Author> authors() const;
  Q_INVOKABLE QList<Property> properties() const;
  Q_INVOKABLE QList<Restriction> restrictions() const;

  Q_INVOKABLE bool select(Author&) const;
  Q_INVOKABLE bool select(Property&) const;
  Q_INVOKABLE bool select(Restriction&) const;

  Q_INVOKABLE bool insert(Author&);
  Q_INVOKABLE bool insert(Property&);
  Q_INVOKABLE bool insert(Restriction&);

  Q_INVOKABLE bool remove(Author&);
  Q_INVOKABLE bool remove(Property&);
  Q_INVOKABLE bool remove(Restriction&);

  Q_INVOKABLE int raw_error() const { return _error_code; };
  Q_INVOKABLE QString error_message() const { return QString(_error_msg); }
  
  Q_INVOKABLE bool success() const { return 0 == _error_code; };
  Q_INVOKABLE bool open() const { return nullptr == _db; };

  QString Path() const { return _db_path; };
  QString Name() const { return _db_name; };

  void Path(const QString p) { _db_path = p; };
  void Name(const QString n) { _db_name = n; };

signals:
  void nameChanged();
  void pathChanged();

private:
  bool open();
private:
  QString _db_name = "";
  QString _db_path = "./";
  ::sqlite3* _db = nullptr;
  mutable int _error_code = 0;
  char* _error_msg = 0;
  char _error_msg_buffer[512];
};
}
#endif //PFC_SCENARIO_BUILDER_SQL_SQLLITE3_DRVER_H