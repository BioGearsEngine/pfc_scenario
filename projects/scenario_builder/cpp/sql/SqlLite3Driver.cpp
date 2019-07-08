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

#include <QRegularExpression>

#include "SqlLite3_Statments.h"
#include "sqlite3.h"

namespace pfc {
inline void assign_author(sqlite3_stmt* stmt, Author& author)
{
  author.id = sqlite3_column_int(stmt, AUTHOR_ID);
  author.frist = QString(reinterpret_cast<char const*>(sqlite3_column_text(stmt, AUTHOR_FIRST_NAME)));
  author.last = QString(reinterpret_cast<char const*>(sqlite3_column_text(stmt, AUTHOR_LAST_NAME)));
  author.email = QString(reinterpret_cast<char const*>(sqlite3_column_text(stmt, AUTHOR_EMAIL)));

  auto zip_qstr = QString(reinterpret_cast<char const*>(sqlite3_column_text(stmt, AUTHOR_ZIPCODE)));
  QRegularExpression zip(R"((\d{5})-(\d{4}))");
  QRegularExpression zip_plus4(R"((\d{5}))");

  QRegularExpressionMatch match = zip_plus4.match(zip_qstr);
  if (match.hasMatch()) {
    author.zip = match.captured(0);
    author.plus_4 = match.captured(1);
  } else {
    match = zip.match(zip_qstr);
    if (match.hasMatch()) {
      author.zip = match.captured(0);
    }
  }

  author.state = QString(reinterpret_cast<char const*>(sqlite3_column_text(stmt, AUTHOR_STATE)));
  author.country = QString(reinterpret_cast<char const*>(sqlite3_column_text(stmt, AUTHOR_COUNTRY)));
  author.phone = QString(reinterpret_cast<char const*>(sqlite3_column_text(stmt, AUTHOR_PHONE)));
  author.organization = QString(reinterpret_cast<char const*>(sqlite3_column_text(stmt, AUTHOR_ORGANIZATION)));
}
inline void assign_property(sqlite3_stmt* stmt, Property& property)
{
  property.id = sqlite3_column_int(stmt, PROPERTY_ID);
  property.name = QString(reinterpret_cast<char const*>(sqlite3_column_text(stmt, PROPERTY_NAME)));
  property.value = QString(reinterpret_cast<char const*>(sqlite3_column_text(stmt, PROPERTY_VALUE)));
}
inline void assign_restriction(sqlite3_stmt* stmt, Restriction& restriction)
{
  restriction.id = sqlite3_column_int(stmt, RESTRICTION_ID);
  restriction.name = QString(reinterpret_cast<char const*>(sqlite3_column_text(stmt, RESTRICTION_NAME)));
  restriction.value = QString(reinterpret_cast<char const*>(sqlite3_column_text(stmt, RESTRICTION_VALUE)));
}
//------------------------------------------------------------------------------
SQLite3Driver::SQLite3Driver()
{
  
}
  //------------------------------------------------------------------------------
SQLite3Driver::SQLite3Driver(const std::string& dbName, const std::string& path)
  : _db_name(dbName.c_str())
  , _db_path(path.c_str())
{
  if (!open()) {
    throw std::runtime_error("Database Creation Failure");
  }
}
//------------------------------------------------------------------------------
bool SQLite3Driver::open(QString name)
{
  _db_path = name;
  return open();
}
//------------------------------------------------------------------------------
bool SQLite3Driver::open()
{
  _error_code = sqlite3_open_v2(QString(_db_path+"/"+_db_name).toStdString().c_str(), &_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);

  if (_error_code) {
    sprintf(_error_msg_buffer, "Can't open database: %s\n", sqlite3_errmsg(_db));
    _error_msg = _error_msg_buffer;
    return (0);
  } 
  return SQLITE_OK == _error_code;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::close()
{
  _error_code = sqlite3_close_v2(_db);
  return SQLITE_OK == _error_code;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::initialize_db()
{
  if (_db) {

    if (SQLITE_OK != (_error_code = sqlite3_exec(_db, /* An open database */
                                                 sqlite3::create_properties_table, /* SQL to be evaluated */
                                                 nullptr, /* Callback function */
                                                 nullptr, /* 1st argument to callback */
                                                 &_error_msg /* Error msg written here */
                                                 ))) {
      return false;
    }
    if (SQLITE_OK != (_error_code = sqlite3_exec(_db, /* An open database */
                                                 sqlite3::create_authors_table, /* SQL to be evaluated */
                                                 nullptr, /* Callback function */
                                                 nullptr, /* 1st argument to callback */
                                                 &_error_msg /* Error msg written here */
                                                 ))) {
      return false;
    }
    if (SQLITE_OK != (_error_code = sqlite3_exec(_db, /* An open database */
                                                 sqlite3::create_restrictions_table, /* SQL to be evaluated */
                                                 nullptr, /* Callback function */
                                                 nullptr, /* 1st argument to callback */
                                                 &_error_msg /* Error msg written here */
                                                 ))) {
      return false;
    }
  }
  return SQLITE_OK == _error_code;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::clear_db()
{
  sqlite3_stmt* stmt = 0;
  if (_db) {
    _error_code = sqlite3_prepare_v2(_db, sqlite3::drop_table, -1, &stmt, 0);
    _error_code = sqlite3_bind_text(stmt, 1, tables[AUTHORS], sizeof(tables[AUTHORS]), nullptr);
    _error_code = sqlite3_step(stmt);
    _error_code = sqlite3_bind_text(stmt, 1, tables[PROPERTIES], sizeof(tables[PROPERTIES]), nullptr);
    _error_code = sqlite3_step(stmt);
    _error_code = sqlite3_bind_text(stmt, 1, tables[RESTRICTIONS], sizeof(tables[RESTRICTIONS]), nullptr);
    _error_code = sqlite3_step(stmt);
    _error_code = sqlite3_finalize(stmt);
  }
  return SQLITE_OK == _error_code;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::clear_table(enum Sqlite3Table t)
{
  sqlite3_stmt* stmt = 0;
  if (_db) {
    switch (t) {
    case AUTHORS:
      _error_code = sqlite3_prepare_v2(_db, sqlite3::drop_table, -1, &stmt, 0);
      _error_code = sqlite3_bind_text(stmt, 1, tables[AUTHORS], sizeof(tables[AUTHORS]), nullptr);
      break;
    case PROPERTIES:
      _error_code = sqlite3_prepare_v2(_db, sqlite3::drop_table, -1, &stmt, 0);
      _error_code = sqlite3_bind_text(stmt, 1, tables[PROPERTIES], sizeof(tables[PROPERTIES]), nullptr);
      break;
    case RESTRICTIONS:
      _error_code = sqlite3_prepare_v2(_db, sqlite3::drop_table, -1, &stmt, 0);
      _error_code = sqlite3_bind_text(stmt, 1, tables[RESTRICTIONS], sizeof(tables[RESTRICTIONS]), nullptr);
      break;
    default:
      return false;
    }

    _error_code = sqlite3_step(stmt);
    _error_code = sqlite3_finalize(stmt);
  }
  return SQLITE_OK == _error_code;
}
//------------------------------------------------------------------------------
auto SQLite3Driver::authors() const -> QList<Author>
{
  QList<Author> results;
  sqlite3_stmt* stmt = 0;
  if (_db) {

    _error_code = sqlite3_prepare(_db, sqlite3::select_all_authors, sizeof(sqlite3::select_all_authors), &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      Author author;
      assert(sqlite3_column_count(stmt) == 11);
      assign_author(stmt, author);
      results.push_back(author);
    }
    sqlite3_finalize(stmt);
  }
  return results;
}
//------------------------------------------------------------------------------
auto SQLite3Driver::properties() const -> QList<Property>
{
  QList<Property> results;
  sqlite3_stmt* stmt = 0;
  if (_db) {

    _error_code = sqlite3_prepare(_db, sqlite3::select_all_properties, sizeof(sqlite3::select_all_properties), &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      Property property;
      assert(sqlite3_column_count(stmt) == 5);
      assign_property(stmt, property);
    }
  }
  return results;
}
//------------------------------------------------------------------------------
auto SQLite3Driver::restrictions() const -> QList<Restriction>
{
  QList<Restriction> results;
  sqlite3_stmt* stmt = 0;
  if (_db) {
    _error_code = sqlite3_prepare(_db, sqlite3::select_all_restrictions, sizeof(sqlite3::select_all_restrictions), &stmt, nullptr);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      Restriction restriction;
      assert(sqlite3_column_count(stmt) == 5);
      assign_restriction(stmt, restriction);
    }
    sqlite3_finalize(stmt);
  }
  return results;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::select(Author& author) const
{
  sqlite3_stmt* stmt = 0;
  if (_db) {
    if (author.id != -1) {
      _error_code = sqlite3_prepare(_db, sqlite3::select_author_by_id, sizeof(sqlite3::select_author_by_id), &stmt, nullptr);
      _error_code = sqlite3_bind_int(stmt, 1, author.id);
    } else if (!author.email.isEmpty()) {
      _error_code = sqlite3_prepare(_db, sqlite3::select_author_by_email, sizeof(sqlite3::select_author_by_email), &stmt, nullptr);
      _error_code = sqlite3_bind_text(stmt, 1, author.email.toLatin1().data(), -1, SQLITE_STATIC);
    } else {
      return false;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      assert(sqlite3_column_count(stmt) == 5);
      assign_author(stmt, author);
    }
    sqlite3_finalize(stmt);
    return true;
  }
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::select(Property& property) const
{
  sqlite3_stmt* stmt = 0;
  if (_db) {
    if (property.id != -1) {
      _error_code = sqlite3_prepare(_db, sqlite3::select_property_by_id, sizeof(sqlite3::select_property_by_id), &stmt, nullptr);
      _error_code = sqlite3_bind_int(stmt, 1, property.id);
    } else if (!property.name.isEmpty()) {
      _error_code = sqlite3_prepare(_db, sqlite3::select_property_by_name, sizeof(sqlite3::select_property_by_name), &stmt, nullptr);
      _error_code = sqlite3_bind_text(stmt, 1, property.name.toLatin1().data(), -1, SQLITE_STATIC);
    } else {
      return false;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      assert(sqlite3_column_count(stmt) == 5);
      assign_property(stmt, property);
    }
    sqlite3_finalize(stmt);
    return true;
  }
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::select(Restriction& restriction) const
{
  sqlite3_stmt* stmt = 0;
  if (_db) {
    if (restriction.id != -1) {
      _error_code = sqlite3_prepare(_db, sqlite3::select_restriction_by_id, sizeof(sqlite3::select_restriction_by_id), &stmt, nullptr);
      _error_code = sqlite3_bind_int(stmt, 1, restriction.id);
    } else if (!restriction.name.isEmpty()) {
      _error_code = sqlite3_prepare(_db, sqlite3::select_restriction_by_name, sizeof(sqlite3::select_restriction_by_name), &stmt, nullptr);
      _error_code = sqlite3_bind_text(stmt, 1, restriction.name.toLatin1().data(), -1, SQLITE_STATIC);
    } else {
      return false;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      assert(sqlite3_column_count(stmt) == 5);
      assign_restriction(stmt, restriction);
    }
    sqlite3_finalize(stmt);
    return true;
  }
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::insert(Author& author)
{
  sqlite3_stmt* stmt = 0;
  if (_db) {
    if (!author.email.isEmpty()) {
      _error_code = sqlite3_prepare(_db, sqlite3::insert_into_authors, sizeof(sqlite3::insert_into_authors), &stmt, nullptr);
      _error_code = sqlite3_bind_text(stmt, 1, author.frist.toLatin1().data(), -1, SQLITE_STATIC);
      _error_code = sqlite3_bind_text(stmt, 1, author.last.toLatin1().data(), -1, SQLITE_STATIC);
      _error_code = sqlite3_bind_text(stmt, 1, author.email.toLatin1().data(), -1, SQLITE_STATIC);

      auto zipcode = (author.plus_4 != 0) ? QString("%1-%2").arg(author.zip).arg(author.plus_4) : QString("%1").arg(author.zip);
      _error_code = sqlite3_bind_text(stmt, 1, zipcode.toLatin1().data(), -1, SQLITE_STATIC);

      _error_code = sqlite3_bind_text(stmt, 1, author.state.toLatin1().data(), -1, SQLITE_STATIC);
      _error_code = sqlite3_bind_text(stmt, 1, author.country.toLatin1().data(), -1, SQLITE_STATIC);
      _error_code = sqlite3_bind_text(stmt, 1, author.phone.toLatin1().data(), -1, SQLITE_STATIC);
      _error_code = sqlite3_bind_text(stmt, 1, author.organization.toLatin1().data(), -1, SQLITE_STATIC);
    }
    if (SQLITE_DONE != sqlite3_step(stmt)) {
      sqlite3_finalize(stmt);
      return false;
    }
    sqlite3_finalize(stmt);
    return true;
  }
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::insert(Property& property)
{
  sqlite3_stmt* stmt = 0;
  if (_db) {
    if (!property.name.isEmpty()) {
      _error_code = sqlite3_prepare(_db, sqlite3::insert_into_properties, sizeof(sqlite3::insert_into_properties), &stmt, nullptr);
      _error_code = sqlite3_bind_text(stmt, 1, property.name.toLatin1().data(), -1, SQLITE_STATIC);
      _error_code = sqlite3_bind_text(stmt, 1, property.value .toLatin1().data(), -1, SQLITE_STATIC);
    }
    if (SQLITE_DONE != sqlite3_step(stmt)) {
      sqlite3_finalize(stmt);
      return false;
    }
    sqlite3_finalize(stmt);
    return true;
  }
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::insert(Restriction& restriction)
{
  sqlite3_stmt* stmt = 0;
  if (_db) {
    if (!restriction.name.isEmpty()) {
      _error_code = sqlite3_prepare(_db, sqlite3::insert_into_restrictions, sizeof(sqlite3::insert_into_restrictions), &stmt, nullptr);
      _error_code = sqlite3_bind_text(stmt, 1, restriction.name.toLatin1().data(), -1, SQLITE_STATIC);
      _error_code = sqlite3_bind_text(stmt, 1, restriction.value.toLatin1().data(), -1, SQLITE_STATIC);
    }
    if (SQLITE_DONE != sqlite3_step(stmt)) {
      sqlite3_finalize(stmt);
      return false;
    }
    sqlite3_finalize(stmt);
    return true;
  }
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::remove(Author& author)
{
  sqlite3_stmt* stmt = 0;
  if (_db) {
    if (author.id != -1) {
      _error_code = sqlite3_prepare(_db, sqlite3::delete_author_by_id, sizeof(sqlite3::delete_author_by_id), &stmt, nullptr);
      _error_code = sqlite3_bind_int(stmt, 1, author.id);
    } else if (!author.email.isEmpty()) {
      _error_code = sqlite3_prepare(_db, sqlite3::delete_author_by_email, sizeof(sqlite3::delete_author_by_email), &stmt, nullptr);
      _error_code = sqlite3_bind_text(stmt, 1, author.email.toLatin1().data(), -1, SQLITE_STATIC);
    } else {
      return false;
    }
    if  (SQLITE_DONE != sqlite3_step(stmt)) {
      sqlite3_finalize(stmt);
      return false; 
    }
    sqlite3_finalize(stmt);
    return true;
  }
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::remove(Property& property)
{
  sqlite3_stmt* stmt = 0;
  if (_db) {
    if (property.id != -1) {
      _error_code = sqlite3_prepare(_db, sqlite3::delete_property_by_id, sizeof(sqlite3::delete_property_by_id), &stmt, nullptr);
      _error_code = sqlite3_bind_int(stmt, 1, property.id);
    } else if (!property.name.isEmpty()) {
      _error_code = sqlite3_prepare(_db, sqlite3::delete_property_by_name, sizeof(sqlite3::delete_property_by_name), &stmt, nullptr);
      _error_code = sqlite3_bind_text(stmt, 1, property.name.toLatin1().data(), -1, SQLITE_STATIC);
    } else {
      return false;
    }
    if (SQLITE_DONE != sqlite3_step(stmt)) {
      sqlite3_finalize(stmt);
      return false;
    }
    sqlite3_finalize(stmt);
    return true;
  }
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::remove(Restriction& restriction)
{
  sqlite3_stmt* stmt = 0;
  if (_db) {
    if (restriction.id != -1) {
      _error_code = sqlite3_prepare(_db, sqlite3::delete_restriction_by_id, sizeof(sqlite3::delete_restriction_by_id), &stmt, nullptr);
      _error_code = sqlite3_bind_int(stmt, 1, restriction.id);
    } else if (!restriction.name.isEmpty()) {
      _error_code = sqlite3_prepare(_db, sqlite3::delete_restriction_by_name, sizeof(sqlite3::delete_restriction_by_name), &stmt, nullptr);
      _error_code = sqlite3_bind_text(stmt, 1, restriction.name.toLatin1().data(), -1, SQLITE_STATIC);
    } else {
      return false;
    }
    if (SQLITE_DONE != sqlite3_step(stmt)) {
      sqlite3_finalize(stmt);
      return false;
    }
    sqlite3_finalize(stmt);
    return true;
  }
  return false;
}
//------------------------------------------------------------------------------
}