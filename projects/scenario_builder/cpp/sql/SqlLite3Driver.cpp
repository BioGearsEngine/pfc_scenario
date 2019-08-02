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
//------------------------------------------------------------------ASSIGN_START
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
  auto ref_list_s = record.value(OBJECTIVE_CITATIONS).toString();
  auto ref_list = ref_list_s.split(";");
  objective.citations.clear();
  for (auto& val : ref_list) {
    if (!val.isEmpty()) {
      objective.citations.push_back(val.toInt());
    }
  }
}
//------------------------------------------------------------------------------
inline void assign_citation(const QSqlRecord& record, Citation& citation)
{
  citation.id = record.value(CITATION_ID).toInt();
  citation.key = record.value(CITATION_KEY).toString();
  citation.title = record.value(CITATION_TITLE).toString();
  auto auth_list_s = record.value(CITATION_AUTHORS).toString();
  auto auth_list = auth_list_s.split(";");
  citation.authors.clear();
  for (auto& val : auth_list) {
    citation.authors.push_back(val);
  }
  citation.year = record.value(CITATION_YEAR).toString();
  citation.publisher = record.value(CITATION_PUBLISHER).toString();
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
  auto ref_list_s = record.value(TREATMENT_CITATIONS).toString();
  auto ref_list = ref_list_s.split(";");
  treatment.citations.clear();
  for (auto& val : ref_list) {
    treatment.citations.push_back(val.toInt());
  }
}
//------------------------------------------------------------------------------
inline void assign_equipment(const QSqlRecord& record, Equipment& equipment)
{
  equipment.id = record.value(EQUIPMENT_ID).toInt();
  equipment.name = record.value(EQUIPMENT_NAME).toString();
  equipment.description = record.value(EQUIPMENT_DESCRIPTION).toString();
  auto equip_list_s = record.value(EQUIPMENT_EQUIPMENT_LIST).toString();
  auto equip_list = equip_list_s.split(";");
  equipment.equipment_list.clear();
  for (auto& val : equip_list) {
    equipment.equipment_list.push_back(val);
  }
  auto ref_list_s = record.value(EQUIPMENT_CITATIONS).toString();
  auto ref_list = ref_list_s.split(";");
  equipment.citations.clear();
  for (auto& val : ref_list) {
    equipment.citations.push_back(val.toInt());
  }
}
//------------------------------------------------------------------------------
inline void assign_injury(const QSqlRecord& record, Injury& injury)
{
  injury.id = record.value(INJURY_ID).toInt();
  injury.medical_name = record.value(INJURY_MEDICAL_NAME).toString();
  injury.common_name = record.value(INJURY_COMMON_NAME).toString();
  injury.description = record.value(INJURY_DESCRIPTION).toString();
  auto ref_list_s = record.value(INJURY_CITATIONS).toString();
  auto ref_list = ref_list_s.split(";");
  injury.citations.clear();
  for (auto& val : ref_list) {
    injury.citations.push_back(val.toInt());
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
//--------------------------------------------------------------------ASSIGN_END
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
  bool creation_failure = false;
  std::map<char const*, char const*> create_statments = {
    { tables[AUTHORS], sqlite3::create_authors_table },
    { tables[ASSESSMENTS], sqlite3::create_assessments_table },
    { tables[EVENTS], sqlite3::create_events_table },
    { tables[EQUIPMENTS], sqlite3::create_equipment_table },
    { tables[INJURIES], sqlite3::create_injuries_table },
    { tables[LOCATIONS], sqlite3::create_locations_table },
    { tables[OBJECTIVES], sqlite3::create_objectives_table },
    { tables[PROPERTIES], sqlite3::create_properties_table },
    { tables[PROPS], sqlite3::create_props_table },
    { tables[CITATIONS], sqlite3::create_citations_table },
    { tables[RESTRICTIONS], sqlite3::create_restrictions_table },
    { tables[ROLES], sqlite3::create_roles_table },
    { tables[TREATMENTS], sqlite3::create_treatments_table }
  };

  if (_db.open()) {
    for (auto pair : create_statments) {
      query.prepare(pair.second);
      if (!query.exec()) {
        qCritical() << QString("Could not create table %1 \n\t --").arg(pair.first) << query.lastError();
        creation_failure = true;
        continue;
      }
    }
    return creation_failure;
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
    query.bindValue(":table", tables[CITATIONS]);
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
    case CITATIONS:
      query.bindValue(":table", tables[CITATIONS]);
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
//-------------------------------------------------------------------COUNT_START
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
int SQLite3Driver::citation_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_citations);
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
int SQLite3Driver::treatment_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_treatments);
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
int SQLite3Driver::equipment_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_equipments);
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
int SQLite3Driver::injury_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_injuries);
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
int SQLite3Driver::assessment_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_assessments);
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
int SQLite3Driver::location_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_locations);
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
int SQLite3Driver::role_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_roles);
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
int SQLite3Driver::prop_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_props);
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
int SQLite3Driver::event_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_events);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
//-----------------------------------------------------------------------------
int SQLite3Driver::nextID(Sqlite3Table table) const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    QString stmt = "SELECT MAX(%1_id) FROM %1s";
    switch (table) {
    case AUTHORS:
      query.prepare(stmt.arg("author"));
      break;
    case ASSESSMENTS:
      query.prepare(stmt.arg("assessment"));
      break;
    case EVENTS:
      query.prepare(stmt.arg("event"));
      break;
    case EQUIPMENTS:
      query.prepare(stmt.arg("equipment"));
      break;
    case INJURIES:
      query.prepare("SELECT MAX(injury) FROM injuries");
      break;
    case LOCATIONS:
      query.prepare(stmt.arg("loction"));
      break;
    case OBJECTIVES:
      query.prepare(stmt.arg("objective"));
      break;
    case PROPERTIES:
      query.prepare("SELECT MAX(property_id) FROM properties");
      break;
    case PROPS:
      query.prepare(stmt.arg("prop"));
      break;
    case CITATIONS:
      query.prepare(stmt.arg("citation"));
      break;
    case RESTRICTIONS:
      query.prepare(stmt.arg("restriction"));
      break;
    case ROLES:
      query.prepare(stmt.arg("role"));
      break;
    case TREATMENTS:
      query.prepare(stmt.arg("treatment"));
      break;
    default:
      return -1;
    }
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt() + 1;
    }
  }
  return -1;
}
//-----------------------------------------------------------------------------
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
    query.prepare(sqlite3::select_all_objectives);
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
void SQLite3Driver::citations()
{
  qDeleteAll(_citations);
  _citations.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_citations);
    query.exec();
    while (query.next()) {
      auto citation = std::make_unique<pfc::Citation>();
      auto record = query.record();
      assert(record.count() == 3);
      assign_citation(record, *citation);
      _citations.push_back(citation.release());
    }
    _current_citation = _citations.begin();
    emit restictionsChanged();
  }
}
//------------------------------------------------------------------------------
void SQLite3Driver::treatments()
{
  qDeleteAll(_treatments);
  _treatments.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_treatments);
    query.exec();
    while (query.next()) {
      auto treatment = std::make_unique<pfc::Treatment>();
      auto record = query.record();
      assert(record.count() == 3);
      assign_treatment(record, *treatment);
      _treatments.push_back(treatment.release());
    }
    _current_treatment = _treatments.begin();
    emit restictionsChanged();
  }
}
//------------------------------------------------------------------------------
void SQLite3Driver::equipments()
{
  qDeleteAll(_equipments);
  _equipments.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_equipments);
    query.exec();
    while (query.next()) {
      auto equipment = std::make_unique<pfc::Equipment>();
      auto record = query.record();
      assert(record.count() == 3);
      assign_equipment(record, *equipment);
      _equipments.push_back(equipment.release());
    }
    _current_equipment = _equipments.begin();
    emit restictionsChanged();
  }
}
//------------------------------------------------------------------------------
void SQLite3Driver::injuries()
{
  qDeleteAll(_injuries);
  _injuries.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_injuries);
    query.exec();
    while (query.next()) {
      auto injury = std::make_unique<pfc::Injury>();
      auto record = query.record();
      assert(record.count() == 3);
      assign_injury(record, *injury);
      _injuries.push_back(injury.release());
    }
    _current_injury = _injuries.begin();
    emit restictionsChanged();
  }
}
//------------------------------------------------------------------------------
void SQLite3Driver::assessments()
{
  qDeleteAll(_assessments);
  _assessments.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_assessments);
    query.exec();
    while (query.next()) {
      auto assessment = std::make_unique<pfc::Assessment>();
      auto record = query.record();
      assert(record.count() == 3);
      assign_assessment(record, *assessment);
      _assessments.push_back(assessment.release());
    }
    _current_assessment = _assessments.begin();
    emit restictionsChanged();
  }
}
//------------------------------------------------------------------------------
void SQLite3Driver::locations()
{
  qDeleteAll(_locations);
  _locations.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_locations);
    query.exec();
    while (query.next()) {
      auto location = std::make_unique<pfc::Location>();
      auto record = query.record();
      assert(record.count() == 3);
      assign_location(record, *location);
      _locations.push_back(location.release());
    }
    _current_location = _locations.begin();
    emit restictionsChanged();
  }
}
//------------------------------------------------------------------------------
void SQLite3Driver::roles()
{
  qDeleteAll(_roles);
  _roles.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_roles);
    query.exec();
    while (query.next()) {
      auto role = std::make_unique<pfc::Role>();
      auto record = query.record();
      assert(record.count() == 3);
      assign_role(record, *role);
      _roles.push_back(role.release());
    }
    _current_role = _roles.begin();
    emit restictionsChanged();
  }
}
//------------------------------------------------------------------------------
void SQLite3Driver::props()
{
  qDeleteAll(_props);
  _props.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_props);
    query.exec();
    while (query.next()) {
      auto prop = std::make_unique<pfc::Prop>();
      auto record = query.record();
      assert(record.count() == 3);
      assign_prop(record, *prop);
      _props.push_back(prop.release());
    }
    _current_prop = _props.begin();
    emit restictionsChanged();
  }
}
//------------------------------------------------------------------------------
void SQLite3Driver::events()
{
  qDeleteAll(_events);
  _events.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_events);
    query.exec();
    while (query.next()) {
      auto event = std::make_unique<pfc::Event>();
      auto record = query.record();
      assert(record.count() == 3);
      assign_event(record, *event);
      _events.push_back(event.release());
    }
    _current_event = _events.begin();
    emit restictionsChanged();
  }
}
//--------------------------------------------------------------------NEXT_START
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
bool SQLite3Driver::next_citation(Citation* citation)
{
  if (_current_citation == _citations.end() || _citations.empty()) {
    return false;
  }
  citation->assign(*(*_current_citation));
  ++_current_citation;

  return true;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::next_treatment(Treatment* treatment)
{
  if (_current_treatment == _treatments.end() || _treatments.empty()) {
    return false;
  }
  treatment->assign(*(*_current_treatment));
  ++_current_treatment;

  return true;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::next_equipment(Equipment* equipment)
{
  if (_current_equipment == _equipments.end() || _equipments.empty()) {
    return false;
  }
  equipment->assign(*(*_current_equipment));
  ++_current_equipment;

  return true;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::next_injury(Injury* injury)
{
  if (_current_injury == _injuries.end() || _injuries.empty()) {
    return false;
  }
  injury->assign(*(*_current_injury));
  ++_current_injury;

  return true;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::next_assessment(Assessment* assessment)
{
  if (_current_assessment == _assessments.end() || _assessments.empty()) {
    return false;
  }
  assessment->assign(*(*_current_assessment));
  ++_current_assessment;

  return true;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::next_location(Location* location)
{
  if (_current_location == _locations.end() || _locations.empty()) {
    return false;
  }
  location->assign(*(*_current_location));
  ++_current_location;

  return true;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::next_role(Role* role)
{
  if (_current_role == _roles.end() || _roles.empty()) {
    return false;
  }
  role->assign(*(*_current_role));
  ++_current_role;

  return true;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::next_prop(Prop* prop)
{
  if (_current_prop == _props.end() || _props.empty()) {
    return false;
  }
  prop->assign(*(*_current_prop));
  ++_current_prop;

  return true;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::next_event(Event* event)
{
  if (_current_event == _events.end() || _events.empty()) {
    return false;
  }
  event->assign(*(*_current_event));
  ++_current_event;

  return true;
}
//------------------------------------------------------------------SELECT_START
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
bool SQLite3Driver::select_citation(Citation* citation) const
{
  //TODO: Hitting the exact title is going to be pretty hard
  //TODO: Might want to add an alias or transform for the select where you remove all whitespace and downcase the title

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (citation->id != -1) {
      query.prepare(sqlite3::select_citation_by_id);
      query.bindValue(":id", citation->id);
    } else if (!citation->key.isEmpty()) {
      query.prepare(sqlite3::select_citation_by_key);
      query.bindValue(":name", citation->key);
    } else if (!citation->title.isEmpty()) {
      query.prepare(sqlite3::select_citation_by_title);
      query.bindValue(":title", citation->title);
    } else {
      qWarning() << "Provided Property has no id, key, or title one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_citation(record, *citation);
      }
      return true;
    }
    qWarning() << query.lastError();
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::select_treatment(Treatment* treatment) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (treatment->id != -1) {
      query.prepare(sqlite3::select_treatment_by_id);
      query.bindValue(":id", treatment->id);
    } else if (!treatment->medical_name.isEmpty()) {
      query.prepare(sqlite3::select_treatment_by_medical_name);
      query.bindValue(":medical_name", treatment->medical_name);
    } else if (!treatment->common_name.isEmpty()) {
      query.prepare(sqlite3::select_treatment_by_common_name);
      query.bindValue(":common_name", treatment->common_name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_treatment(record, *treatment);
      }
      return true;
    }
    qWarning() << query.lastError();
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::select_equipment(Equipment* equipment) const
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (equipment->id != -1) {
      query.prepare(sqlite3::select_equipment_by_id);
      query.bindValue(":id", equipment->id);
    } else if (!equipment->name.isEmpty()) {
      query.prepare(sqlite3::select_equipment_by_name);
      query.bindValue(":name", equipment->name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_equipment(record, *equipment);
      }
      return true;
    }
    qWarning() << query.lastError();
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::select_injury(Injury* injury) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (injury->id != -1) {
      query.prepare(sqlite3::select_injury_by_id);
      query.bindValue(":id", injury->id);
    } else if (!injury->medical_name.isEmpty()) {
      query.prepare(sqlite3::select_injury_by_medical_name);
      query.bindValue(":medical_name", injury->medical_name);
    } else if (!injury->common_name.isEmpty()) {
      query.prepare(sqlite3::select_injury_by_common_name);
      query.bindValue(":common_name", injury->common_name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_injury(record, *injury);
      }
      return true;
    }
    qWarning() << query.lastError();
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::select_assessment(Assessment* assessment) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (assessment->id != -1) {
      query.prepare(sqlite3::select_assessment_by_id);
      query.bindValue(":id", assessment->id);
    } else if (!assessment->name.isEmpty()) {
      query.prepare(sqlite3::select_assessment_by_name);
      query.bindValue(":name", assessment->name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_assessment(record, *assessment);
      }
      return true;
    }
    qWarning() << query.lastError();
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::select_location(Location* location) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (location->id != -1) {
      query.prepare(sqlite3::select_location_by_id);
      query.bindValue(":id", location->id);
    } else if (!location->name.isEmpty()) {
      query.prepare(sqlite3::select_location_by_name);
      query.bindValue(":name", location->name);
    } else if (!location->scene_name.isEmpty()) {
      query.prepare(sqlite3::select_location_by_scene_name);
      query.bindValue(":scene_name", location->scene_name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_location(record, *location);
      }
      return true;
    }
    qWarning() << query.lastError();
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::select_role(Role* role) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (role->id != -1) {
      query.prepare(sqlite3::select_role_by_id);
      query.bindValue(":id", role->id);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_role(record, *role);
      }
      return true;
    }
    qWarning() << query.lastError();
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::select_prop(Prop* prop) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (prop->id != -1) {
      query.prepare(sqlite3::select_prop_by_id);
      query.bindValue(":id", prop->id);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_prop(record, *prop);
      }
      return true;
    }
    qWarning() << query.lastError();
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::select_event(Event* event) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (event->id != -1) {
      query.prepare(sqlite3::select_event_by_id);
      query.bindValue(":id", event->id);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_event(record, *event);
      }
      return true;
    }
    qWarning() << query.lastError();
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------UPDATE_START
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
    QSqlQuery query{ _db };
    if (-1 != objective->id) {
      query.prepare(sqlite3::update_objective_by_id);
      query.bindValue(":id", objective->id);
    } else if (!objective->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_objective);
    }
    QString ref_list = "";
    for (auto& val : objective->citations) {
      ref_list += QString::number(val) + ";";
    }
    ref_list.chop(1);
    query.bindValue(":name", objective->name);
    query.bindValue(":description", objective->description);
    query.bindValue(":citations", ref_list);

    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == objective->id) {
      return select_objective(objective);
    }
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::update_citation(Citation* citation)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != citation->id) {
      query.prepare(sqlite3::update_citation_by_id);
      query.bindValue(":id", citation->id);
    } else if (!citation->key.isEmpty()) {
      query.prepare(sqlite3::update_citation_by_key);
    } else if (!citation->title.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_citations);
      citation->key = (citation->authors.empty()) ? citation->title.toLower().simplified().remove(' ')
                                                  : QString("%1%2").arg(citation->authors[0]).arg(citation->year);
    }
    QString auth_list = "";
    for (auto& val : citation->authors) {
      auth_list += val + ";";
    }
    auth_list.chop(1);

    query.bindValue(":key", citation->key);
    query.bindValue(":title", citation->title);
    query.bindValue(":authors", auth_list);
    query.bindValue(":year", citation->year);
    query.bindValue(":publisher", citation->publisher);

    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == citation->id) {
      return select_citation(citation);
    }
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::update_treatment(Treatment* treatment)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != treatment->id) {
      query.prepare(sqlite3::update_treatment_by_id);
      query.bindValue(":id", treatment->id);
    } else if (!treatment->medical_name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_treatments);
    } else if (!treatment->common_name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_treatments);
    }
    QString equip_list = "";
    for (auto& val : treatment->equipment_list) {
      equip_list += val + ";";
    }
    equip_list.chop(1);
    QString cite_list = "";
    for (auto& val : treatment->citations) {
      cite_list += val + ";";
    }
    cite_list.chop(1);
    query.bindValue(":medical_name", treatment->medical_name);
    query.bindValue(":common_name", treatment->common_name);
    query.bindValue(":description", treatment->description);

    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == treatment->id) {
      return select_treatment(treatment);
    }
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::update_equipment(Equipment* equipment)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != equipment->id) {
      query.prepare(sqlite3::update_equipment_by_id);
      query.bindValue(":id", equipment->id);
    } else if (!equipment->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_equipments);
    }
    QString equip_list = "";
    for (auto& val : equipment->equipment_list) {
      equip_list += val + ";";
    }
    equip_list.chop(1);
    QString cite_list = "";
    for (auto& val : equipment->citations) {
      cite_list += val + ";";
    }
    cite_list.chop(1);
    query.bindValue(":name", equipment->name);
    query.bindValue(":description", equipment->description);

    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == equipment->id) {
      return select_equipment(equipment);
    }
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::update_injury(Injury* injury)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != injury->id) {
      query.prepare(sqlite3::update_injury_by_id);
      query.bindValue(":id", injury->id);
    } else if (!injury->medical_name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_injuries);
    } else if (!injury->common_name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_injuries);
    }
    QString cite_list = "";
    for (auto& val : injury->citations) {
      cite_list += val + ";";
    }
    cite_list.chop(1);
    query.bindValue(":medical_name", injury->medical_name);
    query.bindValue(":common_name", injury->common_name);
    query.bindValue(":description", injury->description);

    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == injury->id) {
      return select_injury(injury);
    }
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::update_assessment(Assessment* assessment)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != assessment->id) {
      query.prepare(sqlite3::update_assessment_by_id);
      query.bindValue(":id", assessment->id);
    } else if (!assessment->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_assessments);
    }
    query.bindValue(":name", assessment->name);
    query.bindValue(":description", assessment->description);
    query.bindValue(":type", assessment->type);
    query.bindValue(":available_points", assessment->available_points);
    query.bindValue(":criteria", assessment->criteria);

    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == assessment->id) {
      return select_assessment(assessment);
    }
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::update_location(Location* location)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != location->id) {
      query.prepare(sqlite3::update_location_by_id);
      query.bindValue(":id", location->id);
    } else if (!location->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_locations);
    }
    query.bindValue(":name", location->name);
    query.bindValue(":scene_name", location->scene_name);
    query.bindValue(":time_of_day", location->time_of_day);
    query.bindValue(":environment", location->environment);

    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == location->id) {
      return select_location(location);
    }
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::update_role(Role* role)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != role->id) {
      query.prepare(sqlite3::update_role_by_id);
      query.bindValue(":id", role->id);
    }
    query.bindValue(":description", role->description);

    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == role->id) {
      return select_role(role);
    }
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::update_prop(Prop* prop)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != prop->id) {
      query.prepare(sqlite3::update_prop_by_id);
      query.bindValue(":id", prop->id);
    }
    query.bindValue(":equipment", prop->equipment);

    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == prop->id) {
      return select_prop(prop);
    }
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::update_event(Event* event)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != event->id) {
      query.prepare(sqlite3::update_event_by_id);
      query.bindValue(":id", event->id);
    }

    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == event->id) {
      return select_event(event);
    }
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------------------------------------------REMOVE_START
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
bool SQLite3Driver::remove_citation(Citation* citation)
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (citation->id != -1) {
      query.prepare(sqlite3::delete_citation_by_id);
      query.bindValue(":id", citation->id);
    } else if (!citation->key.isEmpty()) {
      query.prepare(sqlite3::delete_citation_by_key);
      query.bindValue(":key", citation->key);
    } else if (!citation->title.isEmpty()) {
      query.prepare(sqlite3::delete_citation_by_title);
      query.bindValue(":title", citation->title);
    } else {
      qWarning() << "Provided Citation has no id, key, or title one is required";
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
bool SQLite3Driver::remove_treatment(Treatment* treatment)
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (treatment->id != -1) {
      query.prepare(sqlite3::delete_treatment_by_id);
      query.bindValue(":id", treatment->id);
    } else if (!treatment->medical_name.isEmpty()) {
      query.prepare(sqlite3::delete_treatment_by_medical_name);
      query.bindValue(":medical_name", treatment->medical_name);
    } else if (!treatment->common_name.isEmpty()) {
      query.prepare(sqlite3::delete_treatment_by_common_name);
      query.bindValue(":common_name", treatment->common_name);
    } else {
      qWarning() << "Provided treatment has no id or name one is required";
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
bool SQLite3Driver::remove_equipment(Equipment* equipment)
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (equipment->id != -1) {
      query.prepare(sqlite3::delete_equipment_by_id);
      query.bindValue(":id", equipment->id);
    } else if (!equipment->name.isEmpty()) {
      query.prepare(sqlite3::delete_equipment_by_name);
      query.bindValue(":name", equipment->name);
    } else {
      qWarning() << "Provided equipment has no id or name one is required";
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
bool SQLite3Driver::remove_injury(Injury* injury)
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (injury->id != -1) {
      query.prepare(sqlite3::delete_injury_by_id);
      query.bindValue(":id", injury->id);
    } else if (!injury->medical_name.isEmpty()) {
      query.prepare(sqlite3::delete_injury_by_medical_name);
      query.bindValue(":medical_name", injury->medical_name);
    } else if (!injury->common_name.isEmpty()) {
      query.prepare(sqlite3::delete_injury_by_common_name);
      query.bindValue(":common_name", injury->common_name);
    } else {
      qWarning() << "Provided injury has no id or name one is required";
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
bool SQLite3Driver::remove_assessment(Assessment* assessment)
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (assessment->id != -1) {
      query.prepare(sqlite3::delete_assessment_by_id);
      query.bindValue(":id", assessment->id);
    } else if (!assessment->name.isEmpty()) {
      query.prepare(sqlite3::delete_assessment_by_name);
      query.bindValue(":name", assessment->name);
    } else {
      qWarning() << "Provided assessment has no id or name one is required";
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
bool SQLite3Driver::remove_location(Location* location)
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (location->id != -1) {
      query.prepare(sqlite3::delete_location_by_id);
      query.bindValue(":id", location->id);
    } else if (!location->name.isEmpty()) {
      query.prepare(sqlite3::delete_location_by_name);
      query.bindValue(":name", location->name);
    } else {
      qWarning() << "Provided location has no id or name one is required";
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
bool SQLite3Driver::remove_role(Role* role)
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (role->id != -1) {
      query.prepare(sqlite3::delete_role_by_id);
      query.bindValue(":id", role->id);
    } else {
      qWarning() << "Provided role has no id or name one is required";
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
bool SQLite3Driver::remove_prop(Prop* prop)
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (prop->id != -1) {
      query.prepare(sqlite3::delete_prop_by_id);
      query.bindValue(":id", prop->id);
    } else {
      qWarning() << "Provided prop has no id or name one is required";
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
bool SQLite3Driver::remove_event(Event* event)
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (event->id != -1) {
      query.prepare(sqlite3::delete_event_by_id);
      query.bindValue(":id", event->id);
    } else {
      qWarning() << "Provided event has no id or name one is required";
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
