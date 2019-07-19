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

#include <QList>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QString>

namespace pfc {

struct Author : public QObject {
  Q_OBJECT
public:
  int32_t id = -1;
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
  Q_PROPERTY(int author_id MEMBER id)
  Q_PROPERTY(QString first MEMBER first)
  Q_PROPERTY(QString middle MEMBER middle)
  Q_PROPERTY(QString last MEMBER last)
  Q_PROPERTY(QString email MEMBER email)
  Q_PROPERTY(QString zip MEMBER zip)
  Q_PROPERTY(QString plus_4 MEMBER plus_4)
  Q_PROPERTY(QString state MEMBER state)
  Q_PROPERTY(QString country MEMBER country)
  Q_PROPERTY(QString phone MEMBER phone)
  Q_PROPERTY(QString organization MEMBER organization)

  Author(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  Author(const Author&) = delete;
  Author(Author&&) = delete;
  Author& operator=(const Author&) = delete;
  Author& operator=(Author&&) = delete;
  virtual ~Author() = default;

  bool operator==(const Author& rhs) const
  {
    return id == rhs.id
      && first == rhs.first
      && middle == rhs.middle
      && last == rhs.last
      && email == rhs.email
      && zip == rhs.zip
      && plus_4 == rhs.plus_4
      && state == rhs.state
      && country == rhs.country
      && phone == rhs.phone
      && organization == rhs.organization;
  }
  bool operator!=(const Author& rhs) const
  {
    return !(*this == rhs);
  }
};

struct Property : public QObject {
  Q_OBJECT
  Q_PROPERTY(int property_id MEMBER id)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(QString value MEMBER value)
public:
  int32_t id = -1;
  QString name = "";
  QString value = "";
    
  Property(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  Property(const Property&) = delete;
  Property(Property&&) = delete;
  Property& operator=(const Property&) = delete;
  Property& operator=(Property&&) = delete;
  virtual ~Property() = default;

  bool operator==(const Property& rhs) const
  {
    return id == rhs.id
      && name == rhs.name
      && value == rhs.value;
  } 
  bool operator!=(const Property& rhs) const
  {
    return !(*this == rhs);
  }
};

struct Restriction : public QObject {
  Q_OBJECT
  Q_PROPERTY(int restriction_id MEMBER id)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(QString value MEMBER value)
public:
  int32_t id = -1;
  QString name = "";
  QString value = "";

  Restriction(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  Restriction(const Restriction&) = delete;
  Restriction(Restriction&&) = delete;
  Restriction& operator=(const Restriction&) = delete;
  Restriction& operator=(Restriction&&) = delete;
  virtual ~Restriction() = default;

  bool operator==(const Restriction& rhs) const
  {
    return id == rhs.id
      && name == rhs.name
      && value == rhs.value;
  }
  bool operator!=(const Restriction& rhs) const
  {
    return !(*this == rhs);
  }
};

class SQLite3Driver : public QObject {
public:
  Q_OBJECT

  Q_PROPERTY(QString name READ Name WRITE Name NOTIFY nameChanged)
  Q_PROPERTY(QString path READ Path WRITE Path NOTIFY pathChanged)

public:
  enum Sqlite3Table {
    AUTHORS,
    RESTRICTIONS,
    PROPERTIES,
  };

  Q_ENUM(Sqlite3Table)

  explicit SQLite3Driver(QObject* parent = nullptr);
  SQLite3Driver(const std::string& dbName, const std::string& path = "./", QObject* parent = nullptr);
  SQLite3Driver(const SQLite3Driver&) = delete;
  SQLite3Driver(SQLite3Driver&&) = delete;
  SQLite3Driver& operator=(const SQLite3Driver&) = delete;
  SQLite3Driver& operator=(SQLite3Driver&&) = delete;
  ~SQLite3Driver();

  Q_INVOKABLE bool open(QString db_name);
  Q_INVOKABLE void close();

  Q_INVOKABLE bool initialize_db();

  Q_INVOKABLE bool clear_db();
  Q_INVOKABLE bool clear_table(enum SQLite3Driver::Sqlite3Table);

  Q_INVOKABLE QList<Author*> authors();
  Q_INVOKABLE QList<Property*> properties();
  Q_INVOKABLE QList<Restriction*> restrictions();

  Q_INVOKABLE bool select_author(Author*) const;
  Q_INVOKABLE bool select_property(Property*) const;
  Q_INVOKABLE bool select_restriction(Restriction*) const;

  Q_INVOKABLE bool update_author(Author*);
  Q_INVOKABLE bool update_first_author(Author*);
  Q_INVOKABLE bool update_property(Property*);
  Q_INVOKABLE bool update_restriction(Restriction*);

  Q_INVOKABLE bool remove_author(Author*);
  Q_INVOKABLE bool remove_property(Property*);
  Q_INVOKABLE bool remove_restriction(Restriction*);

  Q_INVOKABLE int raw_error() const { return _db.lastError().type(); };
  Q_INVOKABLE QString error_message() const { return _db.lastError().text(); }
  Q_INVOKABLE bool success() const { return !error(); }
  Q_INVOKABLE bool error() const { return _db.lastError().isValid(); };
  Q_INVOKABLE bool open() const { return _db.isOpen(); };

  QString Path() const { return _db_path; };
  QString Name() const { return _db_name; };

  void Path(const QString p) { _db_path = p; };
  void Name(const QString n) { _db_name = n; };

signals:
  void nameChanged();
  void pathChanged();
  
  void authorsChanged();
  void propertiesChanged();
  void restictionsChanged();

private:
  bool open();

private:
  QString _db_name = "";
  QString _db_path = "./";
  QSqlDatabase _db;
  mutable int _error_code = 0;

  QList<Author*> _authors;
  QList<Property*> _properties;
  QList<Restriction*> _restirctions;
};
}
#endif //PFC_SCENARIO_BUILDER_SQL_SQLLITE3_DRVER_H