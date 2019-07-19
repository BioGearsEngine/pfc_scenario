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
#include "SqlLite3Driver.h"

#include <QDebug>
#include <QMessageLogger>
#include <QRegularExpression>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtGlobal>

#include "SqlLite3_Statments.h"
#include "sqlite3ext.h"

namespace pfc {
inline void assign_author(const QSqlRecord& record, Author& author)
{
  author.id = record.value(AUTHOR_ID).toInt();
  author.first = record.value(AUTHOR_FIRST_NAME).toString();
  author.last = record.value(AUTHOR_LAST_NAME).toString();
  author.email = record.value(AUTHOR_EMAIL).toString();

  auto zip_qstr = record.value(AUTHOR_ZIPCODE).toString();

  QRegularExpression zip(R"((\d{5}))");
  QRegularExpression zip_plus4(R"((\d{5})-(\d{4}))"
                               );
  QRegularExpressionMatch match = zip_plus4.match(zip_qstr);

  if (match.hasMatch()) {
    author.zip = match.captured(1);
    author.plus_4 = match.captured(2);
  } else {
    match = zip.match(zip_qstr);
    if (match.hasMatch()) {
      author.zip = match.captured(1);
    }
  }
  author.state = record.value(AUTHOR_STATE).toString();
  author.country = record.value(AUTHOR_COUNTRY).toString();
  author.phone = record.value(AUTHOR_PHONE).toString();
  author.organization = record.value(AUTHOR_ORGANIZATION).toString();
}

inline void assign_property(const QSqlRecord& record, Property& property)
{
  property.id = record.value(PROPERTY_ID).toInt();
  property.name = record.value(PROPERTY_NAME).toString();
  property.value = record.value(PROPERTY_VALUE).toString();
}
inline void assign_restriction(const QSqlRecord& record, Restriction& restriction)
{
  restriction.id = record.value(RESTRICTION_ID).toInt();
  restriction.name = record.value(RESTRICTION_NAME).toString();
  restriction.value = record.value(RESTRICTION_VALUE).toString();
}
//------------------------------------------------------------------------------
SQLite3Driver::SQLite3Driver(QObject* parent)
  : QObject(parent)
  , _db(QSqlDatabase::database())
{

}
//------------------------------------------------------------------------------
SQLite3Driver::SQLite3Driver(const std::string& dbName, const std::string& path, QObject* parent)
  : QObject(parent)
  , _db_name(dbName.c_str())
  , _db_path(path.c_str())
  , _db(QSqlDatabase::database())
{
  if (!open()) {
    throw std::runtime_error("Database Creation Failure");
  }
  //TODO: Add Logic for detecting if this is required
  initialize_db();
}
//------------------------------------------------------------------------------
SQLite3Driver::~SQLite3Driver()
{
  _db.close();
}
//------------------------------------------------------------------------------
bool SQLite3Driver::open(QString name)
{
  _db.close();
  _db_name = name;
  return open();
}
//------------------------------------------------------------------------------
bool SQLite3Driver::open()
{
  if (!_db.isValid()) {
    _db = QSqlDatabase::addDatabase("QSQLITE");
  }
  _db.setDatabaseName(_db_path + "/" + _db_name);

  if (!_db.open()) {
    qDebug() << _db.lastError();
    return false;
  }
  return true;
}
//------------------------------------------------------------------------------
void SQLite3Driver::close()
{
  _db.close();
}
//------------------------------------------------------------------------------
bool SQLite3Driver::initialize_db()
{
  QSqlQuery query(_db);
  if (_db.isOpen()) {

    query.prepare(sqlite3::create_properties_table);
    if (!query.exec()) {
      qCritical() << query.lastError();
      return false;
    }

    query.prepare(sqlite3::create_authors_table);
    if (!query.exec()) {
      qCritical() << query.lastError();
      return false;
    }

    query.prepare(sqlite3::create_restrictions_table);
    if (!query.exec()) {
      qCritical() << query.lastError();
      return false;
    }
    return true;
  }
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::clear_db()
{
  QSqlQuery query(_db);
  if (_db.isOpen()) {
    query.prepare(sqlite3::drop_table);
    query.bindValue(":table", tables[AUTHORS]);
    bool iResult = query.exec();
    query.bindValue(":table", tables[PROPERTIES]);
    iResult &= query.exec();
    query.bindValue(":table", tables[RESTRICTIONS]);
    return query.exec() && iResult;
  }
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::clear_table(enum SQLite3Driver::Sqlite3Table t)
{

  QSqlQuery query{ _db };
  query.prepare(sqlite3::drop_table);
  if (_db.isOpen()) {
    switch (t) {
    case AUTHORS:
      query.bindValue(":table", tables[AUTHORS]);
      break;
    case PROPERTIES:
      query.bindValue(":table", tables[PROPERTIES]);
      break;
    case RESTRICTIONS:
      query.bindValue(":table", tables[RESTRICTIONS]);
      break;
    default:
      return false;
    }
    return query.exec();
  }
  return false;
}
//------------------------------------------------------------------------------
auto SQLite3Driver::authors() -> QList<pfc::Author*>
{
  qDeleteAll(_authors);
  _authors.clear();
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_authors);
    query.exec();
    while (query.next()) {
      auto author = std::make_unique<Author>();
      auto record = query.record();
      assert(record.count() == 9);
      assign_author(record, *author);
      _authors.push_back(author.release());
    }
  }
  emit authorsChanged();

  return _authors;
}
//------------------------------------------------------------------------------
auto SQLite3Driver::properties() -> QList<pfc::Property*>
{
  qDeleteAll(_properties);
  _properties.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_properties);
    query.exec();
    while (query.next()) {
      auto property = std::make_unique<pfc::Property>();
      auto record = query.record();
      assert(record.count() == 3);
      assign_property(record, *property);
      _properties.push_back(property.release());
    }
  }
  return _properties;
}
//------------------------------------------------------------------------------
auto SQLite3Driver::restrictions() -> QList<pfc::Restriction*>
{
  qDeleteAll(_restirctions);
  _restirctions.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_restrictions);
    query.exec();
    while (query.next()) {
      auto restriction = std::unique_ptr<pfc::Restriction>();
      auto record = query.record();
      assert(record.count() == 3);
      assign_restriction(record, *restriction);
      _restirctions.push_back(restriction.release());
    }
  }
  return _restirctions;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::select_author(Author* author) const
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (author->id != -1) {
      query.prepare(sqlite3::select_author_by_id);
      query.bindValue(":id", author->id);
    } else if (!author->email.isEmpty()) {
      query.prepare(sqlite3::select_author_by_email);
      query.bindValue(":email", author->email);
    } else {
      qWarning() << "Provided Author has no id or email one is required";
      return false;
    }
    query.exec();
    while (query.next()) {
      record = query.record();
      assign_author(record, *author);
    }
    return true;
  }
  qWarning() << "No Databsae connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::select_property(Property* property) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (property->id != -1) {
      query.prepare(sqlite3::select_property_by_id);
      query.bindValue(":id", property->id);
    } else if (!property->name.isEmpty()) {
      query.prepare(sqlite3::select_property_by_name);
      query.bindValue(":name", property->name);
    } else {
      qWarning() << "Provided Propertiy has no id or name one is required";
      return false;
    }
    query.exec();
    while (query.next()) {
      record = query.record();
      assign_property(record, *property);
    }
    return true;
  }
  qWarning() << "No Databsae connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::select_restriction(Restriction* restriction) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (restriction->id != -1) {
      query.prepare(sqlite3::select_restriction_by_id);
      query.bindValue(":id", restriction->id);
    } else if (!restriction->name.isEmpty()) {
      query.prepare(sqlite3::select_restriction_by_name);
      query.bindValue(":name", restriction->name);
    } else {
      qWarning() << "Provided Propertiy has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_restriction(record, *restriction);
      }
      return true;
    }
    qWarning() << query.lastError();
  }
  qWarning() << "No Databsae connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::update_author(Author* author)
{

  if (_db.isOpen()) {
    if (!author->email.isEmpty()) {
      QSqlQuery query{ _db };
      query.prepare(sqlite3::insert_or_update_authors);
      query.bindValue(":name_first", author->first);
      query.bindValue(":name_last", author->last);
      query.bindValue(":email", author->email);

      const auto zipcode = (author->plus_4.isEmpty()) ? QString("%1").arg(author->zip) : QString("%1-%2").arg(author->zip).arg(author->plus_4);
      query.bindValue(":zipcode", zipcode);

      query.bindValue(":state", author->state);
      query.bindValue(":country", author->country);
      query.bindValue(":phone", author->phone);
      query.bindValue(":organization", author->organization);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      return true;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::update_first_author(Author* author)
{

  if (_db.isOpen()) {
    if (!author->email.isEmpty()) {
      QSqlQuery query{ _db };
      query.prepare(sqlite3::insert_or_update_first_author);
      query.bindValue(":name_first", author->first);
      query.bindValue(":name_last", author->last);
      query.bindValue(":email", author->email);

      const auto zipcode = (author->plus_4.isEmpty()) ? QString("%1").arg(author->zip) : QString("%1-%2").arg(author->zip).arg(author->plus_4);
      query.bindValue(":zipcode", zipcode);

      query.bindValue(":state", author->state);
      query.bindValue(":country", author->country);
      query.bindValue(":phone", author->phone);
      query.bindValue(":organization", author->organization);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      return true;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::update_property(Property* property)
{

  if (_db.isOpen()) {
    if (!property->name.isEmpty()) {
      QSqlQuery query{ _db };
      query.prepare(sqlite3::insert_or_update_properties);
      query.bindValue(":name", property->name);
      query.bindValue(":value", property->value);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }

      return true;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::update_restriction(Restriction* restriction)
{

  if (_db.isOpen()) {
    if (!restriction->name.isEmpty()) {
      QSqlQuery query{ _db };
      query.prepare(sqlite3::insert_or_update_restrictions);
      query.bindValue(":name", restriction->name);
      query.bindValue(":value", restriction->value);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }

      return true;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::remove_author(Author* author)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (author->id != -1) {
      query.prepare(sqlite3::delete_author_by_id);
      query.bindValue(":id", author->id);
    } else if (!author->email.isEmpty()) {
      query.prepare(sqlite3::delete_author_by_email);
      query.bindValue(":email", author->email);
    } else {
      qWarning() << "Provided Author has no id or email one is required";
      return false;
    }
    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::remove_property(Property* property)
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (property->id != -1) {
      query.prepare(sqlite3::delete_property_by_id);
      query.bindValue(":id", property->id);
    } else if (!property->name.isEmpty()) {
      query.prepare(sqlite3::delete_property_by_name);
      query.bindValue(":name", property->name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::remove_restriction(Restriction* restriction)
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (restriction->id != -1) {
      query.prepare(sqlite3::delete_restriction_by_id);
      query.bindValue(":id", restriction->id);
    } else if (!restriction->name.isEmpty()) {
      query.prepare(sqlite3::delete_restriction_by_name);
      query.bindValue(":name", restriction->name);
    } else {
      qWarning() << "Provided Restriction has no id or name one is required";
      return false;
    }
    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
}
