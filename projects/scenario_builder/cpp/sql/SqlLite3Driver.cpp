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

#include <memory>

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
  QRegularExpression zip_plus4(R"((\d{5})-(\d{4}))");
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
//------------------------------------------------------------------------------
inline void assign_property(const QSqlRecord& record, Property& property)
{
  property.id = record.value(PROPERTY_ID).toInt();
  property.name = record.value(PROPERTY_NAME).toString();
  property.value = record.value(PROPERTY_VALUE).toString();
}
//------------------------------------------------------------------------------
inline void assign_restriction(const QSqlRecord& record, Restriction& restriction)
{
  restriction.id = record.value(RESTRICTION_ID).toInt();
  restriction.name = record.value(RESTRICTION_NAME).toString();
  restriction.value = record.value(RESTRICTION_VALUE).toString();
}
//------------------------------------------------------------------------------
inline void assign_objective(const QSqlRecord& record, Objective& objective)
{
  objective.id = record.value(OBJECTIVE_ID).toInt();
  objective.name = record.value(OBJECTIVE_NAME).toString();
  objective.description = record.value(OBJECTIVE_DESCRIPTION).toString();
  auto ref_list_s = record.value(OBJECTIVE_REFERENCES).toString();
  auto ref_list = ref_list_s.split(";");
  objective.references.clear();
  for (auto& val : ref_list) {
    objective.references.push_back(val.toInt());
  }
}
//------------------------------------------------------------------------------
inline void assign_reference(const QSqlRecord& record, Reference& reference)
{
  reference.id = record.value(REFERENCE_ID).toInt();
  reference.key = record.value(REFERENCE_KEY).toString();
  reference.title = record.value(REFERENCE_TITLE).toString();
  auto auth_list_s = record.value(REFERENCE_AUTHORS).toString();
  auto auth_list = auth_list_s.split(";");
  reference.authors.clear();
  for (auto& val : auth_list) {
    reference.authors.push_back(val);
  }
  reference.value = record.value(REFERENCE_VALUE).toString();
}
//------------------------------------------------------------------------------
inline void assign_treatment(const QSqlRecord& record, Treatment& treatment)
{
  treatment.id = record.value(TREATMENT_ID).toInt();
  treatment.medical_name = record.value(TREATMENT_MEDICAL_NAME).toString();
  treatment.common_name = record.value(TREATMENT_COMMON_NAME).toString();
  treatment.description = record.value(TREATMENT_DESCRIPTION).toString();
  auto equip_list_s = record.value(TREATMENT_EQUIPMENT_LIST).toString();
  auto equip_list = equip_list_s.split(";");
  treatment.equipment_list.clear();
  for (auto& val : equip_list) {
    treatment.equipment_list.push_back(val);
  }
  auto ref_list_s = record.value(TREATMENT_REFERENCES).toString();
  auto ref_list = ref_list_s.split(";");
  treatment.references.clear();
  for (auto& val : ref_list) {
    treatment.references.push_back(val.toInt());
  }
}
//------------------------------------------------------------------------------
inline void assign_equipment(const QSqlRecord& record, Equipment& equipment)
{
  equipment.id = record.value(EQUIPMENT_ID).toInt();
  equipment.medical_name = record.value(EQUIPMENT_MEDICAL_NAME).toString();
  equipment.common_name = record.value(EQUIPMENT_COMMON_NAME).toString();
  equipment.description = record.value(EQUIPMENT_DESCRIPTION).toString();
  auto equip_list_s = record.value(EQUIPMENT_EQUIPMENT_LIST).toString();
  auto equip_list = equip_list_s.split(";");
  equipment.equipment_list.clear();
  for (auto& val : equip_list) {
    equipment.equipment_list.push_back(val);
  }
  auto ref_list_s = record.value(EQUIPMENT_REFERENCES).toString();
  auto ref_list = ref_list_s.split(";");
  equipment.references.clear();
  for (auto& val : ref_list) {
    equipment.references.push_back(val.toInt());
  }
}
//------------------------------------------------------------------------------
inline void assign_injury(const QSqlRecord& record, Injury& injury)
{
  injury.id = record.value(INJURY_ID).toInt();
  injury.medical_name = record.value(INJURY_MEDICAL_NAME).toString();
  injury.common_name = record.value(INJURY_COMMON_NAME).toString();
  injury.description = record.value(INJURY_DESCRIPTION).toString();
  auto equip_list_s = record.value(INJURY_EQUIPMENT_LIST).toString();
  auto equip_list = equip_list_s.split(";");
  injury.equipment_list.clear();
  for (auto& val : equip_list) {
    injury.equipment_list.push_back(val);
  }
  auto ref_list_s = record.value(INJURY_REFERENCES).toString();
  auto ref_list = ref_list_s.split(";");
  injury.references.clear();
  for (auto& val : ref_list) {
    injury.references.push_back(val.toInt());
  }
}
//------------------------------------------------------------------------------
inline void assign_assessment(const QSqlRecord& record, Assessment& assessment)
{
  assessment.id = record.value(ASSESSMENT_ID).toInt();
  assessment.name = record.value(ASSESSMENT_NAME).toString();
  assessment.description = record.value(ASSESSMENT_DESCRIPTION).toString();
  assessment.type = record.value(ASSESSMENT_TYPE).toString();
  assessment.available_points = record.value(ASSESSMENT_AVAILABLE_POINTS).toInt();
  assessment.criteria = record.value(ASSESSMENT_CRITERIA).toString();
}
//------------------------------------------------------------------------------
inline void assign_location(const QSqlRecord& record, Location& location)
{
  location.id = record.value(LOCATION_ID).toInt();
  location.name = record.value(LOCATION_NAME).toString();
  location.scene_name = record.value(LOCATION_SCENE_NAME).toString();
  location.time_of_day = record.value(LOCATION_TIME_OF_DAY).toString();
  location.environment = record.value(LOCATION_ENVIRONMENT).toString();
}
//------------------------------------------------------------------------------
inline void assign_role(QSqlRecord& record, Role& role)
{
  role.id = record.value(ROLE_ID).toInt();
  role.description = record.value(ROLE_DESCRIPTION).toString();
}
//------------------------------------------------------------------------------
inline void assign_prop(QSqlRecord& record, Prop& prop)
{
  prop.id = record.value(PROP_ID).toInt();
  prop.equipment = record.value(PROP_EQUIPMENT).toString();
}
//------------------------------------------------------------------------------
inline void assign_event(QSqlRecord& record, Event& event)
{
  event.id = record.value(EVENT_ID).toInt();
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
  if (!initialize_db()) {
    QSqlError("Unable to propertly initialize db!");
  }
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
  std::vector<char const*> create_statments = {
    sqlite3::create_properties_table,
    sqlite3::create_authors_table,
    sqlite3::create_restrictions_table,
    sqlite3::create_objectives_table,
    sqlite3::create_references_table,
    sqlite3::create_treatments_table,
    sqlite3::create_equipment_table,
    sqlite3::create_injuries_table,
    sqlite3::create_assessments_table,
    sqlite3::create_objectives_table,
    sqlite3::create_locations_table,
    sqlite3::create_roles_table,
    sqlite3::create_props_table,
    sqlite3::create_events_table
  };

  if (_db.open()) {
    for (auto statment : create_statments) {
      query.prepare(statment);
      if (!query.exec()) {
        qCritical() << query.lastError();
        return false;
      }
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
    iResult &= query.exec();
    query.bindValue(":table", tables[OBJECTIVES]);
    iResult &= query.exec();
    query.bindValue(":table", tables[REFERENCES]);
    iResult &= query.exec();
    query.bindValue(":table", tables[TREATMENTS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[EQUIPMENTS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[INJURIES]);
    iResult &= query.exec();
    query.bindValue(":table", tables[ASSESSMENTS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[OBJECTIVES]);
    iResult &= query.exec();
    query.bindValue(":table", tables[LOCATIONS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[ROLES]);
    iResult &= query.exec();
    query.bindValue(":table", tables[PROPS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[EVENTS]);
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
    case REFERENCES:
      query.bindValue(":table", tables[REFERENCES]);
      break;
    case TREATMENTS:
      query.bindValue(":table", tables[TREATMENTS]);
      break;
    case EQUIPMENTS:
      query.bindValue(":table", tables[EQUIPMENTS]);
      break;
    case INJURIES:
      query.bindValue(":table", tables[INJURIES]);
      break;
    case ASSESSMENTS:
      query.bindValue(":table", tables[ASSESSMENTS]);
      break;
    case OBJECTIVES:
      query.bindValue(":table", tables[OBJECTIVES]);
      break;
    case LOCATIONS:
      query.bindValue(":table", tables[LOCATIONS]);
      break;
    case ROLES:
      query.bindValue(":table", tables[ROLES]);
      break;
    case PROPS:
      query.bindValue(":table", tables[PROPS]);
      break;
    case EVENTS:
      query.bindValue(":table", tables[EVENTS]);
      break;
    default:
      return false;
    }
    return query.exec();
  }
  return false;
}
//------------------------------------------------------------------------------
int SQLite3Driver::author_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_authors);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
//------------------------------------------------------------------------------
int SQLite3Driver::restriction_count() const

{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_restrictions);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
//------------------------------------------------------------------------------
int SQLite3Driver::objective_count() const

{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_objectives);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
//------------------------------------------------------------------------------
int SQLite3Driver::property_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_properties);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
//------------------------------------------------------------------------------
void SQLite3Driver::authors()
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
    _current_author = _authors.begin();
    emit authorsChanged();
  }
}
//------------------------------------------------------------------------------
void SQLite3Driver::properties()
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
    _current_property = _properties.begin();
    emit propertiesChanged();
  }
}
//------------------------------------------------------------------------------
void SQLite3Driver::restrictions()
{
  qDeleteAll(_restrictions);
  _restrictions.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_restrictions);
    query.exec();
    while (query.next()) {
      auto restriction = std::make_unique<pfc::Restriction>();
      auto record = query.record();
      assert(record.count() == 3);
      assign_restriction(record, *restriction);
      _restrictions.push_back(restriction.release());
    }
    _current_restriction = _restrictions.begin();
    emit restictionsChanged();
  }
}
//------------------------------------------------------------------------------
void SQLite3Driver::objectives()
{
  qDeleteAll(_objectives);
  _objectives.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_restrictions);
    query.exec();
    while (query.next()) {
      auto objective = std::make_unique<pfc::Objective>();
      auto record = query.record();
      assert(record.count() == 4);
      assign_objective(record, *objective);
      _objectives.push_back(objective.release());
    }
    _current_objective = _objectives.begin();
    emit objectivesChanged();
  }
}
//------------------------------------------------------------------------------
bool SQLite3Driver::next_author(Author* author)
{
  if (_current_author == _authors.end() || _authors.empty()) {
    return false;
  }
  author->assign(*(*_current_author));
  ++_current_author;
  return true;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::next_property(Property* property)
{
  if (_current_property == _properties.end() || _properties.empty()) {
    return false;
  }
  property->assign(*(*_current_property));
  ++_current_property;
  return true;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::next_restriction(Restriction* restriction)
{
  if (_current_restriction == _restrictions.end() || _restrictions.empty()) {
    return false;
  }
  restriction->assign(*(*_current_restriction));
  ++_current_restriction;

  return true;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::next_objective(Objective* objective)
{
  if (_current_objective == _objectives.end() || _objectives.empty()) {
    return false;
  }
  objective->assign(*(*_current_objective));
  ++_current_objective;

  return true;
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
  qWarning() << "No Database connection";
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
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    query.exec();
    while (query.next()) {
      record = query.record();
      assign_property(record, *property);
    }
    return true;
  }
  qWarning() << "No Database connection";
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
      qWarning() << "Provided Property has no id or name one is required";
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
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::select_objective(Objective* objective) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (objective->id != -1) {
      query.prepare(sqlite3::select_objective_by_id);
      query.bindValue(":id", objective->id);
    } else if (!objective->name.isEmpty()) {
      query.prepare(sqlite3::select_objective_by_name);
      query.bindValue(":name", objective->name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_objective(record, *objective);
      }      
      return true;
    }
    qWarning() << query.lastError();
  }
  qWarning() << "No Database connection";
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
bool SQLite3Driver::update_objective(Objective* objective)
{

  if (_db.isOpen()) {
    if (!objective->name.isEmpty()) {
      QSqlQuery query{ _db };
      query.prepare(sqlite3::insert_or_update_objectives);
      query.bindValue(":name", objective->name);
      query.bindValue(":description", objective->description);

      QString ref_list = "";
      for (auto& val : objective->references) {
        ref_list += QString::number(val) + ";";
      }
      ref_list.chop(1);

      query.bindValue(":references", ref_list);
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
bool SQLite3Driver::remove_objective(Objective* objective)
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (objective->id != -1) {
      query.prepare(sqlite3::delete_objective_by_id);
      query.bindValue(":id", objective->id);
    } else if (!objective->name.isEmpty()) {
      query.prepare(sqlite3::delete_objective_by_name);
      query.bindValue(":name", objective->name);
    } else {
      qWarning() << "Provided Objective has no id or name one is required";
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
