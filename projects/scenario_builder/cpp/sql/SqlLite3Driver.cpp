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

#include <fstream>
#include <iostream>

#include "SqlLite3_Statments.h"
#include "sqlite3ext.h"
#include <regex>

namespace pfc {
//------------------------------------------------------------------------------
//                      SQLiteDriver Functions
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
  bool creation_failure = false;
  std::map<char const*, char const*> create_statments = {
    { tables[AUTHORS], sqlite3::create_authors_table },
    { tables[ASSESSMENTS], sqlite3::create_assessments_table },
    { tables[CITATIONS], sqlite3::create_citations_table },
    { tables[CITATION_MAPS], sqlite3::create_citation_maps_table },
    { tables[EVENTS], sqlite3::create_events_table },
    { tables[EVENT_MAPS], sqlite3::create_event_maps_table },
    { tables[EQUIPMENTS], sqlite3::create_equipment_table },
    { tables[EQUIPMENT_MAPS], sqlite3::create_equipment_map_table },
    { tables[INJURIES], sqlite3::create_injuries_table },
    { tables[INJURY_SETS], sqlite3::create_injury_sets_table },
    { tables[LOCATIONS], sqlite3::create_locations_table },
    { tables[LOCATION_MAPS], sqlite3::create_location_maps_table },
    { tables[ROLE_MAPS], sqlite3::create_role_maps_table },
    { tables[RESTRICTION_MAPS], sqlite3::create_restriction_maps_table },
    { tables[OBJECTIVES], sqlite3::create_objectives_table },
    { tables[PROPERTIES], sqlite3::create_properties_table },
    { tables[RESTRICTIONS], sqlite3::create_restrictions_table },
    { tables[ROLES], sqlite3::create_roles_table },
    { tables[SCENES], sqlite3::create_scenes_table },
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
    query.bindValue(":table", tables[ASSESSMENTS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[CITATIONS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[CITATION_MAPS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[EVENTS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[EVENT_MAPS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[EQUIPMENTS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[EQUIPMENT_MAPS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[INJURIES]);
    iResult &= query.exec();
    query.bindValue(":table", tables[INJURY_SETS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[LOCATIONS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[LOCATION_MAPS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[OBJECTIVES]);
    iResult &= query.exec();
    query.bindValue(":table", tables[PROPERTIES]);
    iResult &= query.exec();
    query.bindValue(":table", tables[ROLES]);
    iResult &= query.exec();
    query.bindValue(":table", tables[ROLE_MAPS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[RESTRICTIONS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[RESTRICTION_MAPS]);
    iResult &= query.exec();
    query.bindValue(":table", tables[SCENES]);
    iResult &= query.exec();
    query.bindValue(":table", tables[TREATMENTS]);
    iResult &= query.exec();
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
    case ASSESSMENTS:
      query.bindValue(":table", tables[ASSESSMENTS]);
      break;
    case AUTHORS:
      query.bindValue(":table", tables[AUTHORS]);
      break;
    case CITATIONS:
      query.bindValue(":table", tables[CITATIONS]);
      break;
    case EQUIPMENTS:
      query.bindValue(":table", tables[EQUIPMENTS]);
      break;
    case EVENTS:
      query.bindValue(":table", tables[EVENTS]);
      break;
    case INJURIES:
      query.bindValue(":table", tables[INJURIES]);
      break;
    case LOCATIONS:
      query.bindValue(":table", tables[LOCATIONS]);
      break;
    case OBJECTIVES:
      query.bindValue(":table", tables[OBJECTIVES]);
      break;
    case PROPERTIES:
      query.bindValue(":table", tables[PROPERTIES]);
      break;
    case RESTRICTIONS:
      query.bindValue(":table", tables[RESTRICTIONS]);
      break;
    case ROLES:
      query.bindValue(":table", tables[ROLES]);
      break;
    case TREATMENTS:
      query.bindValue(":table", tables[TREATMENTS]);
      break;
    default:
      return false;
    }
    return query.exec();
  }
  return false;
}
//-----------------------------------------------------------------------------
int SQLite3Driver::nextID(Sqlite3Table table) const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    QString stmt = "SELECT MAX(%1_id) FROM %1s";
    switch (table) {
    case AUTHORS:
      stmt = stmt.arg("author");
      query.prepare(stmt);
      break;
    case ASSESSMENTS:
      stmt = stmt.arg("assessment");
      query.prepare(stmt);
      break;
    case EVENTS:
      query.prepare(stmt.arg("event"));
      break;
    case EQUIPMENTS:
      query.prepare(stmt.arg("equipment"));
      break;
    case INJURIES:
      query.prepare("SELECT MAX(injury_id) FROM injuries");
      break;
    case INJURY_SETS:
      query.prepare(stmt.arg("injury_set"));
      break;
    case LOCATIONS:
      query.prepare(stmt.arg("location"));
      break;
    case OBJECTIVES:
      query.prepare(stmt.arg("objective"));
      break;
    case PROPERTIES:
      query.prepare("SELECT MAX(property_id) FROM properties");
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
//----------------------------ASSESSMENT-----------------------------------------
inline void assign_assessment(const QSqlRecord& record, Assessment& assessment)
{
  assessment.id = record.value(ASSESSMENT_ID).toInt();
  assessment.name = record.value(ASSESSMENT_NAME).toString();
  assessment.description = record.value(ASSESSMENT_DESCRIPTION).toString();
  assessment.type = record.value(ASSESSMENT_TYPE).toString();
  assessment.available_points = record.value(ASSESSMENT_AVAILABLE_POINTS).toInt();
  assessment.criteria = record.value(ASSESSMENT_CRITERIA).toString();
}
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
      assert(record.count() == 6);
      assign_assessment(record, *assessment);
      _assessments.push_back(assessment.release());
    }
    _current_assessment = _assessments.begin();
    emit assessmentsChanged();
  }
}
bool SQLite3Driver::next_assessment(Assessment* assessment)
{
  if (_current_assessment == _assessments.end() || _assessments.empty()) {
    return false;
  }
  assessment->assign(*(*_current_assessment));
  ++_current_assessment;

  return true;
}
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
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
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
      const auto r = select_assessment(assessment);
      assessmentUpdated(assessment->id);
      return r;
    }
    assessmentUpdated(assessment->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_assessment(Assessment* assessment)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_assessment(assessment)) {
      query.prepare(sqlite3::delete_assessment_by_id);
      query.bindValue(":id", assessment->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      assessmentRemoved(assessment->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//----------------------------AUTHOR---------------------------------------------
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
bool SQLite3Driver::next_author(Author* author)
{
  if (_current_author == _authors.end() || _authors.empty()) {
    return false;
  }
  author->assign(*(*_current_author));
  ++_current_author;
  return true;
}
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
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_author(record, *author);
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
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
      if (-1 == author->id) {
        const auto r = select_author(author);
        authorUpdated(author->id);
        return r;
      }
      authorUpdated(author->id);
      return true;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
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
      authorUpdated(1);
      return true;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_author(Author* author)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_author(author)) {
      query.prepare(sqlite3::delete_author_by_id);
      query.bindValue(":id", author->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      authorRemoved(author->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//----------------------------CITATION--------------------------------------------
inline void assign_citation(const QSqlRecord& record, Citation& citation)
{
  citation.id = record.value(CITATION_ID).toInt();
  citation.key = record.value(CITATION_KEY).toString();
  citation.title = record.value(CITATION_TITLE).toString();
  citation.authors = record.value(CITATION_AUTHORS).toString();
  citation.year = record.value(CITATION_YEAR).toString();
  citation.publisher = record.value(CITATION_PUBLISHER).toString();
}
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
int SQLite3Driver::citation_count(Scene* scene) const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_citations_in_scene);
    query.bindValue(":id", scene->id);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
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
      assert(record.count() == 6);
      assign_citation(record, *citation);
      _citations.push_back(citation.release());
    }
    _current_citation = _citations.begin();
    emit citationsChanged();
  }
}
bool SQLite3Driver::next_citation(Citation* citation)
{
  if (_current_citation == _citations.end() || _citations.empty()) {
    return false;
  }
  citation->assign(*(*_current_citation));
  ++_current_citation;

  return true;
}
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
      query.bindValue(":key", citation->key);
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
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_citation(Citation* citation)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != citation->id) {
      query.prepare(sqlite3::update_citation_by_id);
      query.bindValue(":id", citation->id);
    } else if (!citation->key.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_citations);
    } else if (!citation->title.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_citations);
      auto first_author = citation->authors.split(";", QString::SkipEmptyParts);
      citation->key = (first_author.empty()) ? citation->title.toLower().simplified().remove(' ')
                                             : QString("%1%2").arg(first_author[0]).arg(citation->year);
    }
    query.bindValue(":key", citation->key);
    query.bindValue(":title", citation->title);
    query.bindValue(":authors", citation->authors);
    query.bindValue(":year", citation->year);
    query.bindValue(":publisher", citation->publisher);

    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == citation->id) {
      auto r = select_citation(citation);
      citationUpdated(citation->id);
      return r;
    }
    citationUpdated(citation->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_citation_in_scene(Scene* scene, Citation* citation)
{
  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (select_scene(scene)) {
      if (!select_citation(citation)) {
        update_citation(citation);
      }
      CitationMap map;
      map.fk_citation = citation->id;
      map.fk_scene = scene->id;
      if (!select_citation_map(&map)) {
        update_citation_map(&map);
      }
      return true;
    }
    qWarning() << "Scene not found";
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_citation(Citation* citation)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_citation(citation)) {
      query.prepare(sqlite3::delete_citation_by_id);
      query.bindValue(":id", citation->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      citationRemoved(citation->id);
      QSqlQuery query_map(_db);
      query_map.prepare(sqlite3::delete_citation_map_by_fk_citation);
      query_map.bindValue(":fk_citation", citation->id);
      if (!query_map.exec()) {
        qWarning() << query_map.lastError();
        return false;
      }
      return (remove_citation_from_equipment(citation->id) && 
		          remove_citation_from_treatments(citation->id) && 
		          remove_citation_from_injuries(citation->id) && 
		          remove_citation_from_objectives(citation->id));
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_citation_from_scene(Citation* citation, Scene* scene)
{
  pfc::CitationMap map;
  map.fk_scene = scene->id;
  map.fk_citation = citation->id;
  return remove_citation_map_by_fk(&map);
}
//----------------------------INJURY-------------------------------------------------
inline void assign_injury(const QSqlRecord& record, Injury& injury)
{
  injury.id = record.value(INJURY_ID).toInt();
  injury.medical_name = record.value(INJURY_MEDICAL_NAME).toString();
  injury.common_name = record.value(INJURY_COMMON_NAME).toString();
  injury.description = record.value(INJURY_DESCRIPTION).toString();
  injury.severity_min = record.value(INJURY_SEVERITY_MIN).toInt();
  injury.severity_max = record.value(INJURY_SEVERITY_MAX).toInt();
  injury.citations = record.value(INJURY_CITATIONS).toString();
}
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
      assert(record.count() == INJURY_COLUMN_COUNT);
      assign_injury(record, *injury);
      _injuries.push_back(injury.release());
    }
    _current_injury = _injuries.begin();
    emit injuriesChanged();
  }
}
bool SQLite3Driver::next_injury(Injury* injury)
{
  if (_current_injury == _injuries.end() || _injuries.empty()) {
    return false;
  }
  injury->assign(*(*_current_injury));
  ++_current_injury;

  return true;
}
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
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
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

    query.bindValue(":citations", injury->citations);
    query.bindValue(":medical_name", injury->medical_name);
    query.bindValue(":common_name", injury->common_name);
    query.bindValue(":description", injury->description);
    query.bindValue(":min", injury->severity_min);
    query.bindValue(":max", injury->severity_max);

    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == injury->id) {
      const auto r = select_injury(injury);
      injuryUpdated(injury->id);
      return r;
    }
    injuryUpdated(injury->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_injury(Injury* injury)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_injury(injury)) {
      query.prepare(sqlite3::delete_injury_by_id);
      query.bindValue(":id", injury->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      injuryRemoved(injury->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------INJURY_SET----------------------------------------
inline void assign_injury_set(const QSqlRecord& record, InjurySet& injury)
{
  injury.id = record.value(INJURY_SET_ID).toInt();
  injury.name = record.value(INJURY_SET_NAME).toString();
  injury.description = record.value(INJURY_SET_DESCRIPTION).toString();
  injury.injuries = record.value(INJURY_SET_INJURIES).toString();
  injury.locations = record.value(INJURY_SET_LOCATIONS).toString();
  injury.severities = record.value(INJURY_SET_SEVERITIES).toString();
}
int SQLite3Driver::injury_set_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_injury_sets);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::injury_sets()
{
  qDeleteAll(_injury_sets);
  _injury_sets.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_injury_sets);
    query.exec();
    while (query.next()) {
      auto set = std::make_unique<pfc::InjurySet>();
      auto record = query.record();
      assert(record.count() == INJURY_SET_COLUMN_COUNT);
      assign_injury_set(record, *set);
      _injury_sets.push_back(set.release());
    }
    _current_injury_set = _injury_sets.begin();
    emit injuriesChanged();
  }
}
bool SQLite3Driver::next_injury_set(InjurySet* injury_set)
{
  if (_current_injury_set == _injury_sets.end() || _injury_sets.empty()) {
    return false;
  }
  injury_set->assign(*(*_current_injury_set));
  ++_current_injury_set;

  return true;
}
bool SQLite3Driver::select_injury_set(InjurySet* set) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (set->id != -1) {
      query.prepare(sqlite3::select_injury_set_by_id);
      query.bindValue(":id", set->id);
    } else if (!set->name.isEmpty()) {
      query.prepare(sqlite3::select_injury_set_by_name);
      query.bindValue(":name", set->name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_injury_set(record, *set);
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_injury_set(InjurySet* injury_set)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != injury_set->id) {
      query.prepare(sqlite3::update_injury_set_by_id);
      query.bindValue(":id", injury_set->id);
    } else if (!injury_set->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_injury_sets);
    }
    query.bindValue(":name", injury_set->name);
    query.bindValue(":description", injury_set->description);
    query.bindValue(":injuries", injury_set->injuries);
    query.bindValue(":severities", injury_set->severities);
    query.bindValue(":locations", injury_set->locations);

    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == injury_set->id) {
      const auto r = select_injury_set(injury_set);
      injurySetUpdated(injury_set->id);
      return r;
    }
    injurySetUpdated(injury_set->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_injury_set(InjurySet* injury_set)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_injury_set(injury_set)) {
      query.prepare(sqlite3::delete_injury_set_by_id);
      query.bindValue(":id", injury_set->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      injurySetRemoved(injury_set->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------MAP------------------------------------------
inline void assign_role_map(const QSqlRecord& record, RoleMap& map)
{
  map.id = record.value(MAP_ID).toInt();
  map.fk_scene = record.value(MAP_FK_SCENE).toInt();
  map.fk_role = record.value(MAP_FK_ROLE).toInt();
}
int SQLite3Driver::role_map_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_role_maps);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::role_maps()
{
  qDeleteAll(_role_maps);
  _role_maps.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_role_maps);
    query.exec();
    while (query.next()) {
      auto map = std::make_unique<pfc::RoleMap>();
      auto record = query.record();
      assert(record.count() == MAP_COLUMN_COUNT);
      assign_role_map(record, *map);
      _role_maps.push_back(map.release());
    }
    _current_role_map = _role_maps.begin();
    emit mapsChanged();
  }
}
bool SQLite3Driver::next_role_map(RoleMap* map)
{
  if (_current_role_map == _role_maps.end() || _role_maps.empty()) {
    return false;
  }
  map->assign(*(*_current_role_map));
  ++_current_role_map;

  return true;
}
bool SQLite3Driver::select_role_map(RoleMap* map) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (map->id != -1) {
      query.prepare(sqlite3::select_role_map_by_id);
      query.bindValue(":id", map->id);
    } else if (map->fk_scene != -1) {
      query.prepare(sqlite3::select_role_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_role", map->fk_role);
    } else {
      qWarning() << "Provided Property has no id, fk_scene, or fk_role one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_role_map(record, *map);
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_role_map(RoleMap* map)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != map->id) {
      query.prepare(sqlite3::update_role_map_by_id);
      query.bindValue(":id", map->id);
    } else {
      query.prepare(sqlite3::insert_or_update_role_maps);
    }
    query.bindValue(":fk_scene", map->fk_scene);
    query.bindValue(":fk_role", map->fk_role);
    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == map->id) {
      const auto r = select_role_map(map);
      mapUpdated(map->id);
      return r;
    }
    mapUpdated(map->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_role_map(RoleMap* map)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_role_map(map)) {
      query.prepare(sqlite3::delete_role_map_by_id);
      query.bindValue(":id", map->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      mapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_role_map_by_fk(RoleMap* map)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_role_map(map)) {
      query.prepare(sqlite3::delete_role_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_role", map->fk_role);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      mapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------EVENT MAP------------------------------------------
inline void assign_event_map(const QSqlRecord& record, EventMap& map)
{
  map.id = record.value(EVENT_MAP_ID).toInt();
  map.fk_scene = record.value(EVENT_MAP_FK_SCENE).toInt();
  map.fk_event = record.value(EVENT_MAP_FK_EVENT).toInt();
}
int SQLite3Driver::event_map_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_event_maps);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::event_maps()
{
  qDeleteAll(_event_maps);
  _event_maps.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_event_maps);
    query.exec();
    while (query.next()) {
      auto map = std::make_unique<pfc::EventMap>();
      auto record = query.record();
      assert(record.count() == EVENT_MAP_COLUMN_COUNT);
      assign_event_map(record, *map);
      _event_maps.push_back(map.release());
    }
    _current_event_map = _event_maps.begin();
    emit eventMapsChanged();
  }
}
bool SQLite3Driver::next_event_map(EventMap* map)
{
  if (_current_event_map == _event_maps.end() || _event_maps.empty()) {
    return false;
  }
  map->assign(*(*_current_event_map));
  ++_current_event_map;

  return true;
}
bool SQLite3Driver::select_event_map(EventMap* map) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (map->id != -1) {
      query.prepare(sqlite3::select_event_map_by_id);
      query.bindValue(":id", map->id);
    } else if (map->fk_scene != -1) {
      query.prepare(sqlite3::select_event_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_event", map->fk_event);
    } else {
      qWarning() << "Provided Property has no id, fk_scene, or fk_event one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_event_map(record, *map);
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_event_map(EventMap* map)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != map->id) {
      query.prepare(sqlite3::update_event_map_by_id);
      query.bindValue(":id", map->id);
    } else {
      query.prepare(sqlite3::insert_or_update_event_maps);
    }
    query.bindValue(":fk_scene", map->fk_scene);
    query.bindValue(":fk_event", map->fk_event);
    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == map->id) {
      const auto r = select_event_map(map);
      eventMapUpdated(map->id);
      return r;
    }
    eventMapUpdated(map->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_event_map(EventMap* map)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_event_map(map)) {
      query.prepare(sqlite3::delete_event_map_by_id);
      query.bindValue(":id", map->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      eventMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_event_map_by_fk(EventMap* map)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_event_map(map)) {
      query.prepare(sqlite3::delete_event_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_event", map->fk_event);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      eventMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------LOCATION MAP------------------------------------------
inline void assign_location_map(const QSqlRecord& record, LocationMap& map)
{
  map.id = record.value(LOCATION_MAP_ID).toInt();
  map.fk_scene = record.value(LOCATION_MAP_FK_SCENE).toInt();
  map.fk_location = record.value(LOCATION_MAP_FK_LOCATION).toInt();
}
int SQLite3Driver::location_map_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_location_maps);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
int SQLite3Driver::location_map_count(Scene* scene) const //we currently only support a scene having one location, but this might change in the future
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_location_maps_in_scene);
    query.bindValue(":fk_scene", scene->id);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::location_maps()
{
  qDeleteAll(_location_maps);
  _location_maps.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_location_maps);
    query.exec();
    while (query.next()) {
      auto map = std::make_unique<pfc::LocationMap>();
      auto record = query.record();
      assert(record.count() == LOCATION_MAP_COLUMN_COUNT);
      assign_location_map(record, *map);
      _location_maps.push_back(map.release());
    }
    _current_location_map = _location_maps.begin();
    emit locationMapsChanged();
  }
}
bool SQLite3Driver::next_location_map(LocationMap* map)
{
  if (_current_location_map == _location_maps.end() || _location_maps.empty()) {
    return false;
  }
  map->assign(*(*_current_location_map));
  ++_current_location_map;

  return true;
}
bool SQLite3Driver::select_location_map(LocationMap* map) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (map->id != -1) {
      query.prepare(sqlite3::select_location_map_by_id);
      query.bindValue(":id", map->id);
    } else if (map->fk_scene != -1) {
      query.prepare(sqlite3::select_location_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_location", map->fk_location);
    } else {
      qWarning() << "Provided Property has no id, fk_scene, or fk_location one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_location_map(record, *map);
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_location_map(LocationMap* map)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != map->id) {
      query.prepare(sqlite3::update_location_map_by_id);
      query.bindValue(":id", map->id);
    } else {
      query.prepare(sqlite3::insert_or_update_location_maps);
    }
    query.bindValue(":fk_scene", map->fk_scene);
    query.bindValue(":fk_location", map->fk_location);
    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == map->id) {
      const auto r = select_location_map(map);
      locationMapUpdated(map->id);
      return r;
    }
    locationMapUpdated(map->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_location_map(LocationMap* map)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_location_map(map)) {
      query.prepare(sqlite3::delete_location_map_by_id);
      query.bindValue(":id", map->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      locationMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_location_map_by_fk(LocationMap* map)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_location_map(map)) {
      query.prepare(sqlite3::delete_location_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_location", map->fk_location);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      locationMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------CITATION MAP-----------------------------------------
inline void assign_citation_map(const QSqlRecord& record, CitationMap& map)
{
  map.id = record.value(CITATION_MAP_ID).toInt();
  map.fk_scene = record.value(CITATION_MAP_FK_SCENE).toInt();
  map.fk_citation = record.value(CITATION_MAP_FK_CITATION).toInt();
}
int SQLite3Driver::citation_map_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_citation_maps);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::citation_maps()
{
  qDeleteAll(_citation_maps);
  _citation_maps.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_citation_maps);
    query.exec();
    while (query.next()) {
      auto map = std::make_unique<pfc::CitationMap>();
      auto record = query.record();
      assert(record.count() == CITATION_MAP_COLUMN_COUNT);
      assign_citation_map(record, *map);
      _citation_maps.push_back(map.release());
    }
    _current_citation_map = _citation_maps.begin();
    emit citationMapsChanged();
  }
}
bool SQLite3Driver::next_citation_map(CitationMap* map)
{
  if (_current_citation_map == _citation_maps.end() || _citation_maps.empty()) {
    return false;
  }
  map->assign(*(*_current_citation_map));
  ++_current_citation_map;

  return true;
}
bool SQLite3Driver::select_citation_map(CitationMap* map) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (map->id != -1) {
      query.prepare(sqlite3::select_citation_map_by_id);
      query.bindValue(":id", map->id);
    } else if (map->fk_scene != -1) {
      query.prepare(sqlite3::select_citation_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_citation", map->fk_citation);
    } else {
      qWarning() << "Provided Property has no id, fk_scene, or fk_citation one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_citation_map(record, *map);
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_citation_map(CitationMap* map)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != map->id) {
      query.prepare(sqlite3::update_citation_map_by_id);
      query.bindValue(":id", map->id);
    } else {
      query.prepare(sqlite3::insert_or_update_citation_maps);
    }
    query.bindValue(":fk_scene", map->fk_scene);
    query.bindValue(":fk_citation", map->fk_citation);
    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == map->id) {
      const auto r = select_citation_map(map);
      citationMapUpdated(map->id);
      return r;
    }
    citationMapUpdated(map->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_citation_map(CitationMap* map)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_citation_map(map)) {
      query.prepare(sqlite3::delete_citation_map_by_id);
      query.bindValue(":id", map->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      citationMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_citation_map_by_fk(CitationMap* map)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_citation_map(map)) {
      query.prepare(sqlite3::delete_citation_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_citation", map->fk_citation);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      citationMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------EQUIPMENT MAP----------------------------------------
inline void assign_equipment_map(const QSqlRecord& record, EquipmentMap& map)
{
  map.id = record.value(EQUIPMENT_MAP_ID).toInt();
  map.fk_scene = record.value(EQUIPMENT_MAP_FK_SCENE).toInt();
  map.fk_equipment = record.value(EQUIPMENT_MAP_FK_EQUIPMENT).toInt();
  map.name = record.value(EQUIPMENT_MAP_NAME).toString();
  map.property_values = record.value(EQUIPMENT_MAP_ROPERTY_VALUES).toString();
  map.notes = record.value(EQUIPMENT_MAP_NOTES).toString();
}
inline void assign_equipment_from_scene(const QSqlRecord& record, EquipmentMap& map)
{
  map.id = record.value(0).toInt();
  map.fk_scene;
  map.fk_equipment;
  map.name = record.value(6).toString();
  map.property_values = record.value(7).toString();
  map.notes = record.value(8).toString();
}
int SQLite3Driver::equipment_map_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_equipment_map);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::equipment_maps()
{
  qDeleteAll(_equipment_maps);
  _equipment_maps.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_equipment_map);
    query.exec();
    while (query.next()) {
      auto map = std::make_unique<pfc::EquipmentMap>();
      auto record = query.record();
      assert(record.count() == EQUIPMENT_MAP_COLUMN_COUNT);
      assign_equipment_map(record, *map);
      _equipment_maps.push_back(map.release());
    }
    _current_equipment_map = _equipment_maps.begin();
    emit equipmentMapsChanged();
  }
}
bool SQLite3Driver::next_equipment_map(EquipmentMap* map)
{
  if (_current_equipment_map == _equipment_maps.end() || _equipment_maps.empty()) {
    return false;
  }
  map->assign(*(*_current_equipment_map));
  ++_current_equipment_map;

  return true;
}
bool SQLite3Driver::select_equipment_map(EquipmentMap* map) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (map->id != -1) {
      query.prepare(sqlite3::select_equipment_map_by_id);
      query.bindValue(":id", map->id);
    } else if (map->fk_scene != -1) {
      query.prepare(sqlite3::select_equipment_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_equipment", map->fk_equipment);
      query.bindValue(":name", map->name);
      query.bindValue(":values", map->property_values);
      query.bindValue(":notes", map->notes);
    } else {
      qWarning() << "Provided Property has no id, fk_scene, or fk_equipment one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_equipment_map(record, *map);
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_equipment_map(EquipmentMap* map)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != map->id) {
      query.prepare(sqlite3::update_equipment_map_by_id);
      query.bindValue(":id", map->id);
    } else {
      query.prepare(sqlite3::insert_or_update_equipment_map);
    }
    query.bindValue(":fk_scene", map->fk_scene);
    query.bindValue(":fk_equipment", map->fk_equipment);
    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == map->id) {
      const auto r = select_equipment_map(map);
      equipmentMapUpdated(map->id);
      return r;
    }
    equipmentMapUpdated(map->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_equipment_map(EquipmentMap* map)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_equipment_map(map)) {
      query.prepare(sqlite3::delete_equipment_map_by_id);
      query.bindValue(":id", map->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      equipmentMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_equipment_map_by_fk(EquipmentMap* map)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_equipment_map(map)) {
      query.prepare(sqlite3::delete_equipment_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_equipment", map->fk_equipment);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      equipmentMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------RESTRICTION MAP---------------------------------------
inline void assign_restriction_map(const QSqlRecord& record, RestrictionMap& map)
{
  map.id = record.value(RESTRICTION_MAP_ID).toInt();
  map.fk_scene = record.value(RESTRICTION_MAP_FK_SCENE).toInt();
  map.fk_restriction = record.value(RESTRICTION_MAP_FK_RESTRICTION).toInt();
}
int SQLite3Driver::restriction_map_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_restriction_maps);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::restriction_maps()
{
  qDeleteAll(_restriction_maps);
  _restriction_maps.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_restriction_maps);
    query.exec();
    while (query.next()) {
      auto map = std::make_unique<pfc::RestrictionMap>();
      auto record = query.record();
      assert(record.count() == RESTRICTION_MAP_COLUMN_COUNT);
      assign_restriction_map(record, *map);
      _restriction_maps.push_back(map.release());
    }
    _current_restriction_map = _restriction_maps.begin();
    emit restrictionMapChanged();
  }
}
bool SQLite3Driver::next_restriction_map(RestrictionMap* map)
{
  if (_current_restriction_map == _restriction_maps.end() || _restriction_maps.empty()) {
    return false;
  }
  map->assign(*(*_current_restriction_map));
  ++_current_restriction_map;

  return true;
}
bool SQLite3Driver::select_restriction_map(RestrictionMap* map) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (map->id != -1) {
      query.prepare(sqlite3::select_restriction_map_by_id);
      query.bindValue(":id", map->id);
    } else if (map->fk_scene != -1) {
      query.prepare(sqlite3::select_restriction_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_restriction", map->fk_restriction);
    } else {
      qWarning() << "Provided Property has no id, fk_scene, or fk_restriction one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_restriction_map(record, *map);
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_restriction_map(RestrictionMap* map)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != map->id) {
      query.prepare(sqlite3::update_restriction_map_by_id);
      query.bindValue(":id", map->id);
    } else {
      query.prepare(sqlite3::insert_or_update_restriction_maps);
    }
    query.bindValue(":fk_scene", map->fk_scene);
    query.bindValue(":fk_restriction", map->fk_restriction);
    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == map->id) {
      const auto r = select_restriction_map(map);
      restrictionMapUpdated(map->id);
      return r;
    }
    restrictionMapUpdated(map->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_restriction_map(RestrictionMap* map)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_restriction_map(map)) {
      query.prepare(sqlite3::delete_restriction_map_by_id);
      query.bindValue(":id", map->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      restrictionMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_restriction_map_by_fk(RestrictionMap* map)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_restriction_map(map)) {
      query.prepare(sqlite3::delete_restriction_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_restriction", map->fk_restriction);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      restrictionMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//----------------------------EVENT-------------------------------------------------
inline void assign_event(QSqlRecord& record, Event& event)
{
  event.id = record.value(EVENT_ID).toInt();
  event.name = record.value(EVENT_NAME).toString();
  event.description = record.value(EVENT_DESCRIPTION).toString();
  event.fidelity = record.value(EVENT_FIDELITY).toString();
  event.category = record.value(EVENT_CATEGORY).toString();
  event.fk_actor_1 = record.value(EVENT_ACTOR_1).toString();
  event.fk_actor_2 = record.value(EVENT_ACTOR_2).toString();
  event.equipment = record.value(EVENT_EQUIPMENT).toString();
  event.details = record.value(EVENT_DETAILS).toString();
}
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
int SQLite3Driver::event_count(Scene* scene) const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_events_in_scene);
    query.bindValue(":id", scene->id);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
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
      assert(record.count() == EVENT_COLUMN_COUNT);
      assign_event(record, *event);
      _events.push_back(event.release());
    }
    _current_event = _events.begin();
    emit eventsChanged();
  }
}
void SQLite3Driver::events_in_scene(Scene* scene)
{
  qDeleteAll(_events);
  _events.clear();

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_events_in_scene);
    query.bindValue(":scene_id", scene->id);
    query.exec();
    while (query.next()) {
      auto event = std::make_unique<pfc::Event>();
      auto record = query.record();
      assert(record.count() == EVENT_COLUMN_COUNT);
      assign_event(record, *event);
      _events.push_back(event.release());
    }
    _current_event = _events.begin();
    emit eventsChanged();
  }
}
bool SQLite3Driver::next_event(Event* event)
{
  if (_current_event == _events.end() || _events.empty()) {
    return false;
  }
  event->assign(*(*_current_event));
  ++_current_event;

  return true;
}
bool SQLite3Driver::select_event(Event* event) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (event->id != -1) {
      query.prepare(sqlite3::select_event_by_id);
      query.bindValue(":id", event->id);
    } else if (!event->name.isEmpty()) {
      query.prepare(sqlite3::select_event_by_name);
      query.bindValue(":name", event->name);
    } else {
      qWarning() << "Provided Event has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_event(record, *event);
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_event(Event* event)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != event->id) {
      query.prepare(sqlite3::update_event_by_id);
      query.bindValue(":id", event->id);
    } else if (!event->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_events);
    }
    query.bindValue(":equipment", event->equipment);
    query.bindValue(":name", event->name);
    query.bindValue(":description", event->description);
    query.bindValue(":category", event->category);
    query.bindValue(":fidelity", event->fidelity);
    query.bindValue(":actor_1", event->fk_actor_1);
    query.bindValue(":actor_2", event->fk_actor_2);
    query.bindValue(":details", event->details);
    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == event->id) {
      const auto r = select_event(event);
      eventUpdated(event->id);
      return r;
    }
    eventUpdated(event->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_event_in_scene(Scene* scene, Event* event)
{
  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (select_scene(scene)) {
      if (!select_event(event)) {
        update_event(event);
      }
      EventMap map;
      map.fk_event = event->id;
      map.fk_scene = scene->id;
      if (!select_event_map(&map)) {
        update_event_map(&map);
      }
      return true;
    }
    qWarning() << "Scene not found";
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_event(Event* event)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_event(event)) {
      query.prepare(sqlite3::delete_event_by_id);
      query.bindValue(":id", event->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      eventRemoved(event->id);
      QSqlQuery query_map(_db);
      query_map.prepare(sqlite3::delete_event_map_by_fk_event);
      query_map.bindValue(":fk_event", event->id);
      if (!query_map.exec()) {
        qWarning() << query_map.lastError();
        return false;
      }
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_event_from_scene(Event* event, Scene* scene)
{
  pfc::EventMap map;
  map.fk_scene = scene->id;
  map.fk_event = event->id;
  return remove_event_map_by_fk(&map);
}
//-----------------------------LOCATION------------------------------------------
inline void assign_location(const QSqlRecord& record, Location& location)
{
  location.id = record.value(LOCATION_ID).toInt();
  location.name = record.value(LOCATION_NAME).toString();
  location.scene_name = record.value(LOCATION_SCENE_NAME).toString();
  location.time_of_day = record.value(LOCATION_TIME_OF_DAY).toString();
  location.environment = record.value(LOCATION_ENVIRONMENT).toString();
}
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
      assert(record.count() == LOCATION_COLUMN_COUNT);
      assign_location(record, *location);
      _locations.push_back(location.release());
    }
    _current_location = _locations.begin();
    emit locationsChanged();
  }
}
bool SQLite3Driver::next_location(Location* location)
{
  if (_current_location == _locations.end() || _locations.empty()) {
    return false;
  }
  location->assign(*(*_current_location));
  ++_current_location;

  return true;
}
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
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_location(record, *location);
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
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
      const auto r = select_location(location);
      locationUpdated(location->id);
      return r;
    }
    locationUpdated(location->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
int SQLite3Driver::location_count(Scene* scene) const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_locations_in_scene);
    query.bindValue(":id", scene->id);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::locations_in_scene(Scene* scene)
{
  qDeleteAll(_locations);
  _locations.clear();

  if (_db.isOpen()) {
    std::vector<int32_t> fk_location;
    QSqlQuery location_map_query{ _db };
    location_map_query.prepare(sqlite3::select_location_map_by_fk_scene);
    location_map_query.bindValue(":fk_scene", scene->id);
    location_map_query.exec();
    while (location_map_query.next()) {
      auto location_map = std::make_unique<pfc::LocationMap>();
      auto location_map_record = location_map_query.record();
      assign_location_map(location_map_record, *location_map);
      fk_location.push_back(location_map->fk_location);
    }
    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_location_by_id);
    while (!fk_location.empty()) {
      query.bindValue(":id", fk_location.back());
      bool huh = query.exec();
      fk_location.pop_back();
      if (query.next()) {
        auto location = std::make_unique<pfc::Location>();
        auto record = query.record();
        assign_location(record, *location);
        _locations.push_back(location.release());
      }
    }
    _current_location = _locations.begin();
    emit locationsChanged();
  }
}
bool SQLite3Driver::update_location_in_scene(Scene* scene, Location* location)
{
  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (select_scene(scene)) {
      if (!select_location(location)) {
        update_location(location);
      }
      LocationMap map;
      map.fk_location = location->id;
      map.fk_scene = scene->id;
      if (location_map_count(scene) > 0) {
        QSqlQuery query_location_map(_db);
        query_location_map.prepare(sqlite3::delete_location_map_by_fk_scene);
        query_location_map.bindValue(":fk_scene", scene->id);
        if (!query_location_map.exec()) {
          qWarning() << query_location_map.lastError();
          return false;
        }
      }
      if (!select_location_map(&map)) {
        update_location_map(&map);
      } else {
        LocationMap prev_map;
        prev_map.fk_location = scene->id;
        remove_location_map(&prev_map);
        update_location_map(&map);
      }
      return true;
    }
    qWarning() << "Scene not found";
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}

bool SQLite3Driver::remove_location(Location* location)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_location(location)) {
      query.prepare(sqlite3::delete_location_by_id);
      query.bindValue(":id", location->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      locationRemoved(location->id);
      QSqlQuery query_map(_db);
      query_map.prepare(sqlite3::delete_location_map_by_fk_location);
      query_map.bindValue(":fk_location", location->id);
      if (!query_map.exec()) {
        qWarning() << query_map.lastError();
        return false;
      }
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}

bool SQLite3Driver::remove_location_from_scene(Location* location, Scene* scene)
{
  pfc::LocationMap map;
  map.fk_scene = scene->id;
  map.fk_location = location->id;
  return remove_location_map_by_fk(&map);
}
//----------------------------EQUIPMENT------------------------------------------
inline void assign_equipment(const QSqlRecord& record, Equipment& equipment)
{
  equipment.id = record.value(EQUIPMENT_ID).toInt();
  equipment.name = record.value(EQUIPMENT_NAME).toString();
  equipment.type = record.value(EQUIPMENT_TYPE).toInt();
  equipment.description = record.value(EQUIPMENT_DESCRIPTION).toString();
  equipment.citations = record.value(EQUIPMENT_CITATIONS).toString();
  equipment.image = record.value(EQUIPMENT_IMAGE).toString();
}
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
int SQLite3Driver::equipment_count(Scene* scene) const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_equipments_in_scene);
    query.bindValue(":id", scene->id);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
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
      assert(record.count() == EQUIPMENT_COLUMN_COUNT);
      assign_equipment(record, *equipment);
      _equipments.push_back(equipment.release());
    }
    _current_equipment = _equipments.begin();
    emit equipmentsChanged();
  }
}
void SQLite3Driver::equipment_in_scene(Scene* scene)
{
  qDeleteAll(_equipments);
  _equipments.clear();

  if (_db.isOpen()) {
    std::vector<int32_t> fk_equipment;
    QSqlQuery equipment_map_query{ _db };
    equipment_map_query.prepare(sqlite3::select_equipment_map_by_fk_scene);
    equipment_map_query.bindValue(":fk_scene", scene->id);
    equipment_map_query.exec();
    while (equipment_map_query.next()) {
      auto equipment_map = std::make_unique<pfc::EquipmentMap>();
      auto equipment_map_record = equipment_map_query.record();
      assign_equipment_map(equipment_map_record, *equipment_map);
      fk_equipment.push_back(equipment_map->fk_equipment);
    }
    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_equipment_by_id);
    while (!fk_equipment.empty()) {
      query.bindValue(":id", fk_equipment.back());
      bool huh = query.exec();
      fk_equipment.pop_back();
      if (query.next()) {
        auto equipment = std::make_unique<pfc::Equipment>();
        auto record = query.record();
        assign_equipment(record, *equipment);
        _equipments.push_back(equipment.release());
      }
    }
    _current_equipment = _equipments.begin();
    emit equipmentsChanged();
  }
}
bool SQLite3Driver::next_equipment(Equipment* equipment)
{
  if (_current_equipment == _equipments.end() || _equipments.empty()) {
    return false;
  }
  equipment->assign(*(*_current_equipment));
  ++_current_equipment;

  return true;
}
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
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
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
    query.bindValue(":name", equipment->name);
    query.bindValue(":type", equipment->type);
    query.bindValue(":description", equipment->description);
    query.bindValue(":citations", equipment->citations);
    query.bindValue(":image", equipment->image);

    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == equipment->id) {
      const auto r = select_equipment(equipment);
      equipmentUpdated(equipment->id);
      return r;
    }
    equipmentUpdated(equipment->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_equipment_in_scene(Scene* scene, Equipment* equipment)
{
  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (select_scene(scene)) {
      if (!select_equipment(equipment)) {
        update_equipment(equipment);
      }
      EquipmentMap map;
      map.fk_equipment = equipment->id;
      map.fk_scene = scene->id;
      if (!select_equipment_map(&map)) {
        update_equipment_map(&map);
      }
      return true;
    }
    qWarning() << "Scene not found";
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_equipment(Equipment* equipment)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_equipment(equipment)) {
      query.prepare(sqlite3::delete_equipment_by_id);
      query.bindValue(":id", equipment->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      equipmentRemoved(equipment->id);
      QSqlQuery query_map(_db);
      query_map.prepare(sqlite3::delete_equipment_map_by_fk_equipment);
      query_map.bindValue(":fk_equipment", equipment->id);
      if (!query_map.exec()) {
        qWarning() << query_map.lastError();
        return false;
      }
      return remove_equipment_from_treatments(equipment->id);
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_equipment_from_scene(Equipment* equipment, Scene* scene)
{
  pfc::EquipmentMap map;
  map.fk_scene = scene->id;
  map.fk_equipment = equipment->id;
  return remove_equipment_map_by_fk(&map);
}
//-----------------------------OBJECTIVE-----------------------------------------
inline void assign_objective(const QSqlRecord& record, Objective& objective)
{
  objective.id = record.value(OBJECTIVE_ID).toInt();
  objective.name = record.value(OBJECTIVE_NAME).toString();
  objective.description = record.value(OBJECTIVE_DESCRIPTION).toString();
  objective.citations = record.value(OBJECTIVE_CITATIONS).toString();
}
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
      assert(record.count() == OBJECTIVE_COLUMN_COUNT);
      assign_objective(record, *objective);
      _objectives.push_back(objective.release());
    }
    _current_objective = _objectives.begin();
    emit objectivesChanged();
  }
}
bool SQLite3Driver::next_objective(Objective* objective)
{
  if (_current_objective == _objectives.end() || _objectives.empty()) {
    return false;
  }
  objective->assign(*(*_current_objective));
  ++_current_objective;

  return true;
}
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
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
  } else {
    qWarning() << "No Database connection";
  }
  return false;
}
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

    query.bindValue(":name", objective->name);
    query.bindValue(":description", objective->description);
    query.bindValue(":citations", objective->citations);

    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == objective->id) {
      auto r = select_objective(objective);
      objectiveUpdated(objective->id);
      return r;
    }
    objectiveUpdated(objective->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_objective(Objective* objective)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_objective(objective)) {
      query.prepare(sqlite3::delete_objective_by_id);
      query.bindValue(":id", objective->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      objectiveRemoved(objective->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------PROPERTY------------------------------------------
inline void assign_property(const QSqlRecord& record, Property& property)
{
  property.id = record.value(PROPERTY_ID).toInt();
  property.name = record.value(PROPERTY_NAME).toString();
  property.value = record.value(PROPERTY_VALUE).toString();
}
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
      int ct = record.count();
      assert(record.count() == PROPERTY_COLUMN_COUNT);
      assign_property(record, *property);
      _properties.push_back(property.release());
    }
    _current_property = _properties.begin();
    emit propertiesChanged();
  }
}
bool SQLite3Driver::next_property(Property* property)
{
  if (_current_property == _properties.end() || _properties.empty()) {
    return false;
  }
  property->assign(*(*_current_property));
  ++_current_property;
  return true;
}
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
    if (query.exec()) {

      while (query.next()) {
        record = query.record();
        assign_property(record, *property);
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
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
      if (-1 == property->id) {
        const auto r = select_property(property);
        propertyUpdated(property->id);
        return r;
      }
      propertyUpdated(property->id);
      return true;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_property(Property* property)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_property(property)) {
      query.prepare(sqlite3::delete_property_by_id);
      query.bindValue(":id", property->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      propertyRemoved(property->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------ROLE----------------------------------------------
inline void assign_role(QSqlRecord& record, Role& role)
{
  role.id = record.value(ROLE_ID).toInt();
  role.name = record.value(ROLE_NAME).toString();
  role.description = record.value(ROLE_DESCRIPTION).toString();
}
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
int SQLite3Driver::role_count(Scene* scene) const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_roles_in_scene);
    query.bindValue(":id", scene->id);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
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
      assert(record.count() == ROLE_COLUMN_COUNT);
      assign_role(record, *role);
      _roles.push_back(role.release());
    }
    _current_role = _roles.begin();
    emit rolesChanged();
  }
}
void SQLite3Driver::roles_in_scene(Scene* scene)
{
  qDeleteAll(_roles);
  _roles.clear();

  if (_db.isOpen()) {
    std::vector<int32_t> fk_role;
    QSqlQuery map_query{ _db };
    map_query.prepare(sqlite3::select_role_map_by_fk_scene);
    map_query.bindValue(":fk_scene", scene->id);
    map_query.exec();
    while (map_query.next()) {
      auto map = std::make_unique<pfc::RoleMap>();
      auto map_record = map_query.record();
      assign_role_map(map_record, *map);
      fk_role.push_back(map->fk_role);
    }
    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_role_by_id);
    while (!fk_role.empty()) {
      query.bindValue(":id", fk_role.back());
      bool huh = query.exec();
      fk_role.pop_back();
      if (query.next()) {
        auto role = std::make_unique<pfc::Role>();
        auto record = query.record();
        assign_role(record, *role);
        int32_t iid = role->id;
        QString nuum = role->name;
        QString desc = role->description;
        _roles.push_back(role.release());
      }
    }
    _current_role = _roles.begin();
    emit rolesChanged();
  }
}
bool SQLite3Driver::next_role(Role* role)
{
  if (_current_role == _roles.end() || _roles.empty()) {
    return false;
  }
  role->assign(*(*_current_role));
  ++_current_role;

  return true;
}
bool SQLite3Driver::select_role(Role* role) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (role->id != -1) {
      query.prepare(sqlite3::select_role_by_id);
      query.bindValue(":id", role->id);
    } else if (!role->name.isEmpty()) {
      query.prepare(sqlite3::select_role_by_name);
      query.bindValue(":name", role->name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_role(record, *role);
        return true;
      }
    } else {

      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_role(Role* role)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != role->id) {
      query.prepare(sqlite3::update_role_by_id);
      query.bindValue(":id", role->id);
    } else if (!role->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_roles);
    }
    query.bindValue(":description", role->description);
    query.bindValue(":name", role->name);
    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == role->id) {
      const auto r = select_role(role);
      roleUpdated(role->id);
      return r;
    }
    roleUpdated(role->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_role_in_scene(Scene* scene, Role* role)
{
  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (select_scene(scene)) {
      if (!select_role(role)) {
        update_role(role);
      }
      RoleMap map;
      map.fk_role = role->id;
      map.fk_scene = scene->id;
      if (!select_role_map(&map)) {
        update_role_map(&map);
      }
      return true;
    }
    qWarning() << "Scene not found";
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_role(Role* role) // This deletes a role completely from the database
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_role(role)) {
      query.prepare(sqlite3::delete_role_by_id);
      query.bindValue(":id", role->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      roleRemoved(role->id);
      QSqlQuery query_map(_db);
      query_map.prepare(sqlite3::delete_role_map_by_fk_role);
      query_map.bindValue(":fk_role", role->id);
      if (!query_map.exec()) {
        qWarning() << query_map.lastError();
        return false;
      }
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_role_from_scene(Role* role, Scene* scene)
{
  pfc::RoleMap map;
  map.fk_scene = scene->id;
  map.fk_role = role->id;
  return remove_role_map_by_fk(&map);
}
//-----------------------------RESTRICTION---------------------------------------
inline void assign_restriction(const QSqlRecord& record, Restriction& restriction)
{
  restriction.id = record.value(RESTRICTION_ID).toInt();
  restriction.name = record.value(RESTRICTION_NAME).toString();
  restriction.value = record.value(RESTRICTION_VALUE).toString();
}
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
int SQLite3Driver::restriction_count(Scene* scene) const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_restrictions_in_scene);
    query.bindValue(":id", scene->id);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
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
      assert(record.count() == RESTRICTION_COLUMN_COUNT);
      assign_restriction(record, *restriction);
      _restrictions.push_back(restriction.release());
    }
    _current_restriction = _restrictions.begin();
    emit restrictionsChanged();
  }
}
bool SQLite3Driver::next_restriction(Restriction* restriction)
{
  if (_current_restriction == _restrictions.end() || _restrictions.empty()) {
    return false;
  }
  restriction->assign(*(*_current_restriction));
  ++_current_restriction;

  return true;
}
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
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_restriction(Restriction* restriction)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != restriction->id) {
      query.prepare(sqlite3::update_restriction_by_id);
      query.bindValue(":id", restriction->id);
    } else if (!restriction->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_restrictions);
    }

    query.bindValue(":name", restriction->name);
    query.bindValue(":value", restriction->value);
    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == restriction->id) {
      const auto r = select_restriction(restriction);
      restrictionUpdated(restriction->id);
      return r;
    }
    restrictionUpdated(restriction->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_restriction_in_scene(Scene* scene, Restriction* restriction)
{
  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (select_scene(scene)) {
      if (!select_restriction(restriction)) {
        update_restriction(restriction);
      }
      RestrictionMap map;
      map.fk_restriction = restriction->id;
      map.fk_scene = scene->id;
      if (!select_restriction_map(&map)) {
        update_restriction_map(&map);
      }
      return true;
    }
    qWarning() << "Scene not found";
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_restriction(Restriction* restriction)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_restriction(restriction)) {
      query.prepare(sqlite3::delete_restriction_by_id);
      query.bindValue(":id", restriction->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      restrictionRemoved(restriction->id);
      QSqlQuery query_map(_db);
      query_map.prepare(sqlite3::delete_restriction_map_by_fk_restriction);
      query_map.bindValue(":fk_restriction", restriction->id);
      if (!query_map.exec()) {
        qWarning() << query_map.lastError();
        return false;
      }
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_restriction_from_scene(Restriction* restriction, Scene* scene)
{
  pfc::RestrictionMap map;
  map.fk_scene = scene->id;
  map.fk_restriction = restriction->id;
  return remove_restriction_map_by_fk(&map);
}
//-----------------------------SCENE---------------------------------------------
inline void assign_scene(QSqlRecord& record, Scene& scene)
{
  scene.id = record.value(SCENE_ID).toInt();
  scene.name = record.value(SCENE_NAME).toString();
  scene.description = record.value(SCENE_DESCRIPTION).toString();
  scene.time_of_day = record.value(SCENE_TIME_OF_DAY).toString();
  scene.time_in_simulation = record.value(SCENE_TIME_IN_SIMULATION).toInt();
  scene.weather = record.value(SCENE_WEATHER).toString();
  scene.events = record.value(SCENE_EVENTS).toString();
  scene.items = record.value(SCENE_ITEMS).toString();
  scene.roles = record.value(SCENE_ROLES).toString();
  scene.details = record.value(SCENE_DETAILS).toString();
}
int SQLite3Driver::scene_count() const
{
  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::count_scenes);
    query.exec();
    if (query.next()) {
      auto record = query.record();
      assert(record.count() == 1);
      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::scenes()
{
  qDeleteAll(_scenes);
  _scenes.clear();

  if (_db.isOpen()) {

    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_all_scenes);
    query.exec();
    while (query.next()) {
      auto scene = std::make_unique<pfc::Scene>();
      auto record = query.record();
      assert(record.count() == SCENE_COLUMN_COUNT);
      assign_scene(record, *scene);
      _scenes.push_back(scene.release());
    }
    _current_scene = _scenes.begin();
    emit scenesChanged();
  }
}
bool SQLite3Driver::next_scene(Scene* scene)
{
  if (_current_scene == _scenes.end() || _scenes.empty()) {
    return false;
  }
  scene->assign(*(*_current_scene));
  ++_current_scene;

  return true;
}
bool SQLite3Driver::select_scene(Scene* scene) const
{

  if (_db.isOpen()) {
    QSqlQuery query(_db);
    QSqlRecord record;
    if (scene->id != -1) {
      query.prepare(sqlite3::select_scene_by_id);
      query.bindValue(":id", scene->id);
    } else if (!scene->name.isEmpty()) {
      query.prepare(sqlite3::select_scene_by_name);
      query.bindValue(":name", scene->name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_scene(record, *scene);
        return true;
      }
    } else {

      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_scene(Scene* scene)
{

  if (_db.isOpen()) {
    QSqlQuery query{ _db };
    if (-1 != scene->id) {
      query.prepare(sqlite3::update_scene_by_id);
      query.bindValue(":id", scene->id);
    } else if (!scene->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_scenes);
    }
    query.bindValue(":name", scene->name);
    query.bindValue(":description", scene->description);
    query.bindValue(":time_of_day", scene->time_of_day);
    query.bindValue(":time_in_simulation", scene->time_in_simulation);
    query.bindValue(":weather", scene->weather);
    query.bindValue(":events", scene->events);
    query.bindValue(":items", scene->items);
    query.bindValue(":roles", scene->roles);
    query.bindValue(":details", scene->details);
    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == scene->id) {
      const auto r = select_scene(scene);
      sceneUpdated(scene->id);
      return r;
    }
    sceneUpdated(scene->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_scene(Scene* scene)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_scene(scene)) {
      query.prepare(sqlite3::delete_scene_by_id);
      query.bindValue(":id", scene->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      sceneRemoved(scene->id);
      QSqlQuery query_map(_db);
      query_map.prepare(sqlite3::delete_role_map_by_fk_scene);
      query_map.bindValue(":fk_scene", scene->id);
      if (!query_map.exec()) {
        qWarning() << query_map.lastError();
        return false;
      }
      QSqlQuery query_event_map(_db);
      query_event_map.prepare(sqlite3::delete_event_map_by_fk_scene);
      query_event_map.bindValue(":fk_scene", scene->id);
      if (!query_event_map.exec()) {
        qWarning() << query_event_map.lastError();
        return false;
      }
      QSqlQuery query_restriction_map(_db);
      query_restriction_map.prepare(sqlite3::delete_restriction_map_by_fk_scene);
      query_restriction_map.bindValue(":fk_scene", scene->id);
      if (!query_restriction_map.exec()) {
        qWarning() << query_restriction_map.lastError();
        return false;
      }
      QSqlQuery query_equipment_map(_db);
      query_equipment_map.prepare(sqlite3::delete_equipment_map_by_fk_scene);
      query_equipment_map.bindValue(":fk_scene", scene->id);
      if (!query_equipment_map.exec()) {
        qWarning() << query_equipment_map.lastError();
        return false;
      }
      QSqlQuery query_citation_map(_db);
      query_citation_map.prepare(sqlite3::delete_citation_map_by_fk_scene);
      query_citation_map.bindValue(":fk_scene", scene->id);
      if (!query_citation_map.exec()) {
        qWarning() << query_citation_map.lastError();
        return false;
      }
      QSqlQuery query_location_map(_db);
      query_location_map.prepare(sqlite3::delete_location_map_by_fk_scene);
      query_location_map.bindValue(":fk_scene", scene->id);
      if (!query_location_map.exec()) {
        qWarning() << query_location_map.lastError();
        return false;
      }
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------TREATMENT-----------------------------------------
inline void assign_treatment(const QSqlRecord& record, Treatment& treatment)
{
  treatment.id = record.value(TREATMENT_ID).toInt();
  treatment.medical_name = record.value(TREATMENT_MEDICAL_NAME).toString();
  treatment.common_name = record.value(TREATMENT_COMMON_NAME).toString();
  treatment.description = record.value(TREATMENT_DESCRIPTION).toString();
  treatment.equipment = record.value(TREATMENT_EQUIPMENT).toString();
  treatment.citations = record.value(TREATMENT_CITATIONS).toString();
}
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
      assert(record.count() == TREATMENT_COLUMN_COUNT);
      assign_treatment(record, *treatment);
      _treatments.push_back(treatment.release());
    }
    _current_treatment = _treatments.begin();
    emit treatmentsChanged();
  }
}
bool SQLite3Driver::next_treatment(Treatment* treatment)
{
  if (_current_treatment == _treatments.end() || _treatments.empty()) {
    return false;
  }
  treatment->assign(*(*_current_treatment));
  ++_current_treatment;

  return true;
}
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
        return true;
      }
    } else {
      qWarning() << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
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
    query.bindValue(":citations", treatment->citations);
    query.bindValue(":equipment", treatment->equipment);
    query.bindValue(":medical_name", treatment->medical_name);
    query.bindValue(":common_name", treatment->common_name);
    query.bindValue(":description", treatment->description);

    if (!query.exec()) {
      qWarning() << query.lastError();
      return false;
    }
    if (-1 == treatment->id) {
      const auto r = select_treatment(treatment);
      treatmentUpdated(treatment->id);
      return r;
    }
    treatmentUpdated(treatment->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_treatment(Treatment* treatment)
{
  if (_db.isOpen()) {
    QSqlQuery query(_db);
    if (select_treatment(treatment)) {
      query.prepare(sqlite3::delete_treatment_by_id);
      query.bindValue(":id", treatment->id);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
      treatmentRemoved(treatment->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------BACK DOORS FOR NOW----------------------------------
std::vector<std::unique_ptr<Author>> SQLite3Driver::get_authors() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<Author>> author_list;
    QSqlQuery author_query{ _db };
    author_query.prepare(select_all_authors);
    author_query.exec();
    while (author_query.next()) {
      auto temp = std::make_unique<Author>();
      auto record = author_query.record();
      temp->id = record.value(0).toInt();
      temp->first = record.value(1).toString();
      temp->last = record.value(2).toString();
      temp->email = record.value(3).toString();
      temp->zip = record.value(4).toString();
      temp->state = record.value(5).toString();
      temp->country = record.value(6).toString();
      temp->phone = record.value(7).toString();
      temp->organization = record.value(8).toString();
      author_list.push_back(std::move(temp));
    }
    return author_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Assessment>> SQLite3Driver::get_assessments() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<Assessment>> assessment_list;
    QSqlQuery assessment_query{ _db };
    assessment_query.prepare(select_all_assessments);
    assessment_query.exec();
    while (assessment_query.next()) {
      auto temp = std::make_unique<Assessment>();
      auto record = assessment_query.record();
      temp->id = record.value(0).toInt();
      temp->name = record.value(1).toString();
      temp->description = record.value(2).toString();
      temp->type = record.value(3).toString();
      temp->available_points = record.value(4).toInt();
      temp->criteria = record.value(5).toString();
      assessment_list.push_back(std::move(temp));
    }
    return assessment_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Citation>> SQLite3Driver::get_citations() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<Citation>> citation_list;
    QSqlQuery citation_query{ _db };
    citation_query.prepare(select_all_citations);
    citation_query.exec();
    while (citation_query.next()) {
      auto temp = std::make_unique<Citation>();
      auto record = citation_query.record();
      temp->id = record.value(0).toInt();
      temp->key = record.value(1).toString();
      temp->title = record.value(2).toString();
      temp->authors = record.value(3).toString();
      temp->year = record.value(4).toString();
      temp->publisher = record.value(5).toString();
      citation_list.push_back(std::move(temp));
    }
    return citation_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Event>> SQLite3Driver::get_events() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<Event>> event_list;
    QSqlQuery event_query{ _db };
    event_query.prepare(select_all_events);
    event_query.exec();
    while (event_query.next()) {
      auto temp = std::make_unique<Event>();
      auto record = event_query.record();
      temp->id = record.value(0).toInt();
      temp->name = record.value(1).toString();
      temp->description = record.value(2).toString();
      temp->category = record.value(3).toString();
      temp->fidelity = record.value(4).toString();
      temp->fk_actor_1 = record.value(5).toString();
      temp->fk_actor_2 = record.value(6).toString();
      temp->equipment = record.value(7).toString();
      temp->details = record.value(8).toString();
      event_list.push_back(std::move(temp));
    }
    return event_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Event>> SQLite3Driver::get_events_in_scene(Scene const* const scene) const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<Event>> event_list;
    QSqlQuery event_query{ _db };
    event_query.prepare(select_events_in_scene);
    event_query.bindValue(":scene_id", scene->id);
    event_query.exec();
    while (event_query.next()) {
      auto temp = std::make_unique<Event>();
      auto record = event_query.record();
      temp->id = record.value(EVENT_ID).toInt();
      temp->name = record.value(EVENT_NAME).toString();
      //temp->location = record.value(2).toInt();
      //temp->actor = record.value(3).toInt();
      temp->equipment = record.value(EVENT_EQUIPMENT).toString();
      temp->details = record.value(EVENT_DETAILS).toString();
      event_list.push_back(std::move(temp));
    }
    return event_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Equipment>> SQLite3Driver::get_equipments() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<Equipment>> equipment_list;
    QSqlQuery equipment_query{ _db };
    equipment_query.prepare(select_all_equipments);
    equipment_query.exec();
    while (equipment_query.next()) {
      auto temp = std::make_unique<Equipment>();
      auto record = equipment_query.record();
      temp->id = record.value(EQUIPMENT_ID).toInt();
      temp->type = record.value(EQUIPMENT_TYPE).toInt();
      temp->name = record.value(EQUIPMENT_NAME).toString();
      temp->description = record.value(EQUIPMENT_DESCRIPTION).toString();
      temp->citations = record.value(EQUIPMENT_CITATIONS).toString();
      temp->image = record.value(EQUIPMENT_IMAGE).toString();
      equipment_list.push_back(std::move(temp));
    }
    return equipment_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<EquipmentMap>> SQLite3Driver::get_equipment_in_scene(Scene const* const scene) const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<EquipmentMap>> equipment_list;
    QSqlQuery scene_query{ _db };
    scene_query.prepare(select_equipment_in_scene);
    scene_query.bindValue(":scene_id", scene->id);
    scene_query.exec();
    while (scene_query.next()) {
      auto temp = std::make_unique<EquipmentMap>();
      auto record = scene_query.record();
      assign_equipment_from_scene(record, *temp);
      equipment_list.push_back(std::move(temp));
    }
    return equipment_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Injury>> SQLite3Driver::get_injuries() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<Injury>> injury_list;
    QSqlQuery injury_query{ _db };
    injury_query.prepare(select_all_injuries);
    injury_query.exec();
    while (injury_query.next()) {
      auto temp = std::make_unique<Injury>();
      auto record = injury_query.record();
      temp->id = record.value(0).toInt();
      temp->medical_name = record.value(1).toString();
      temp->common_name = record.value(2).toString();
      temp->description = record.value(3).toString();
      temp->citations = record.value(4).toString();
      temp->severity_min = record.value(5).toFloat();
      temp->severity_max = record.value(6).toFloat();
      injury_list.push_back(std::move(temp));
    }
    return injury_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<InjurySet>> SQLite3Driver::get_injury_sets() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<InjurySet>> injury_set_list;
    QSqlQuery injury_set_query{ _db };
    injury_set_query.prepare(select_all_injury_sets);
    injury_set_query.exec();
    while (injury_set_query.next()) {
      auto temp = std::make_unique<InjurySet>();
      auto record = injury_set_query.record();
      temp->id = record.value(0).toInt();
      temp->name = record.value(1).toString();
      temp->description = record.value(2).toString();
      temp->injuries = record.value(3).toString();
      temp->locations = record.value(4).toString();
      temp->severities = record.value(5).toString();
      injury_set_list.push_back(std::move(temp));
    }
    return injury_set_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<RoleMap>> SQLite3Driver::get_role_maps() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<RoleMap>> role_map_list;
    QSqlQuery role_map_query{ _db };
    role_map_query.prepare(select_all_role_maps);
    role_map_query.exec();
    while (role_map_query.next()) {
      auto temp = std::make_unique<RoleMap>();
      auto record = role_map_query.record();
      temp->id = record.value(0).toInt();
      temp->fk_scene = record.value(1).toInt();
      temp->fk_role = record.value(2).toInt();
      role_map_list.push_back(std::move(temp));
    }
    return role_map_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<EventMap>> SQLite3Driver::get_event_maps() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<EventMap>> event_map_list;
    QSqlQuery event_map_query{ _db };
    event_map_query.prepare(select_all_event_maps);
    event_map_query.exec();
    while (event_map_query.next()) {
      auto temp = std::make_unique<EventMap>();
      auto record = event_map_query.record();
      temp->id = record.value(0).toInt();
      temp->fk_scene = record.value(1).toInt();
      temp->fk_event = record.value(2).toInt();
      event_map_list.push_back(std::move(temp));
    }
    return event_map_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<LocationMap>> SQLite3Driver::get_location_maps() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<LocationMap>> location_map_list;
    QSqlQuery location_map_query{ _db };
    location_map_query.prepare(select_all_location_maps);
    location_map_query.exec();
    while (location_map_query.next()) {
      auto temp = std::make_unique<LocationMap>();
      auto record = location_map_query.record();
      temp->id = record.value(0).toInt();
      temp->fk_scene = record.value(1).toInt();
      temp->fk_location = record.value(2).toInt();
      location_map_list.push_back(std::move(temp));
    }
    return location_map_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<CitationMap>> SQLite3Driver::get_citation_maps() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<CitationMap>> citation_map_list;
    QSqlQuery citation_map_query{ _db };
    citation_map_query.prepare(select_all_citation_maps);
    citation_map_query.exec();
    while (citation_map_query.next()) {
      auto temp = std::make_unique<CitationMap>();
      auto record = citation_map_query.record();
      temp->id = record.value(0).toInt();
      temp->fk_scene = record.value(1).toInt();
      temp->fk_citation = record.value(2).toInt();
      citation_map_list.push_back(std::move(temp));
    }
    return citation_map_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<EquipmentMap>> SQLite3Driver::get_equipment_maps() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<EquipmentMap>> equipment_map_list;
    QSqlQuery equipment_map_query{ _db };
    equipment_map_query.prepare(select_all_equipment_map);
    equipment_map_query.exec();
    while (equipment_map_query.next()) {
      auto temp = std::make_unique<EquipmentMap>();
      auto record = equipment_map_query.record();
      temp->id = record.value(0).toInt();
      temp->fk_scene = record.value(1).toInt();
      temp->fk_equipment = record.value(2).toInt();
      equipment_map_list.push_back(std::move(temp));
    }
    return equipment_map_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<RestrictionMap>> SQLite3Driver::get_restriction_maps() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<RestrictionMap>> restriction_map_list;
    QSqlQuery restriction_map_query{ _db };
    restriction_map_query.prepare(select_all_restriction_maps);
    restriction_map_query.exec();
    while (restriction_map_query.next()) {
      auto temp = std::make_unique<RestrictionMap>();
      auto record = restriction_map_query.record();
      temp->id = record.value(0).toInt();
      temp->fk_scene = record.value(1).toInt();
      temp->fk_restriction = record.value(2).toInt();
      restriction_map_list.push_back(std::move(temp));
    }
    return restriction_map_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Objective>> SQLite3Driver::get_objectives() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<Objective>> objective_list;
    QSqlQuery objective_query{ _db };
    objective_query.prepare(select_all_objectives);
    objective_query.exec();
    while (objective_query.next()) {
      auto temp = std::make_unique<Objective>();
      auto record = objective_query.record();
      temp->id = record.value(0).toInt();
      temp->name = record.value(1).toString();
      temp->description = record.value(2).toString();
      temp->citations = record.value(3).toString();
      objective_list.push_back(std::move(temp));
    }
    return objective_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Location>> SQLite3Driver::get_locations() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<Location>> location_list;
    QSqlQuery location_query{ _db };
    location_query.prepare(select_all_locations);
    location_query.exec();
    while (location_query.next()) {
      auto temp = std::make_unique<Location>();
      auto record = location_query.record();
      temp->id = record.value(0).toInt();
      temp->name = record.value(1).toString();
      temp->scene_name = record.value(2).toString();
      temp->time_of_day = record.value(3).toString();
      temp->environment = record.value(4).toString();
      location_list.push_back(std::move(temp));
    }
    return location_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Location>> SQLite3Driver::get_locations_in_scene(Scene* scene) const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<Location>> location_list;
    std::vector<int32_t> fk_locations;
    QSqlQuery map_query{ _db };
    map_query.prepare(sqlite3::select_location_map_by_fk_scene);
    map_query.bindValue(":fk_scene", scene->id);
    map_query.exec();
    while (map_query.next()) {
      auto map = std::make_unique<pfc::LocationMap>();
      auto map_record = map_query.record();
      assign_location_map(map_record, *map);
      fk_locations.push_back(map->fk_location);
    }
    QSqlQuery query{ _db };
    query.prepare(sqlite3::select_location_by_id);
    for (auto& id : fk_locations) {
      query.bindValue(":id", id);
      if (query.next()) {
        auto location = std::make_unique<pfc::Location>();
        auto record = query.record();
        location->id = id;
        location->name = record.value(1).toString();
        location->scene_name = record.value(2).toString();
        location->time_of_day = record.value(3).toString();
        location->environment = record.value(4).toString();
        location_list.push_back(std::move(location));
      }
    }
    return location_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Property>> SQLite3Driver::get_properties() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<Property>> property_list;
    QSqlQuery property_query{ _db };
    property_query.prepare(select_all_properties);
    property_query.exec();
    while (property_query.next()) {
      auto temp = std::make_unique<Property>();
      auto record = property_query.record();
      temp->id = record.value(0).toInt();
      temp->name = record.value(1).toString();
      temp->value = record.value(2).toString();
      property_list.push_back(std::move(temp));
    }
    return property_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Restriction>> SQLite3Driver::get_restrictions() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<Restriction>> restriction_list;
    QSqlQuery restriction_query{ _db };
    restriction_query.prepare(select_all_restrictions);
    restriction_query.exec();
    while (restriction_query.next()) {
      auto temp = std::make_unique<Restriction>();
      auto record = restriction_query.record();
      temp->id = record.value(0).toInt();
      temp->name = record.value(1).toString();
      temp->value = record.value(2).toString();
      restriction_list.push_back(std::move(temp));
    }
    return restriction_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Role>> SQLite3Driver::get_roles() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<Role>> role_list;
    QSqlQuery role_query{ _db };
    role_query.prepare(select_all_roles);
    role_query.exec();
    while (role_query.next()) {
      auto temp = std::make_unique<Role>();
      auto record = role_query.record();
      temp->id = record.value(0).toInt();
      temp->name = record.value(1).toString();
      temp->description = record.value(2).toString();
      role_list.push_back(std::move(temp));
    }
    return role_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Role>> SQLite3Driver::get_roles_in_scene(Scene* scene) const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<Role>> role_list;
    QSqlQuery role_query{ _db };
    role_query.prepare(sqlite3::select_roles_in_scene);
    role_query.bindValue(":scene_id", scene->id);
    role_query.exec();
    while (role_query.next()) {
      auto role = std::make_unique<pfc::Role>();
      auto record = role_query.record();
      role->id = record.value(ROLE_ID).toInt();
      role->name = record.value(ROLE_NAME).toString();
      role->description = record.value(ROLE_DESCRIPTION).toString();
      role_list.push_back(std::move(role));
    }
    return role_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Treatment>> SQLite3Driver::get_treatments() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<Treatment>> treatment_list;
    QSqlQuery treatment_query{ _db };
    treatment_query.prepare(select_all_treatments);
    treatment_query.exec();
    while (treatment_query.next()) {
      auto temp = std::make_unique<Treatment>();
      auto record = treatment_query.record();
      temp->id = record.value(0).toInt();
      temp->medical_name = record.value(1).toString();
      temp->common_name = record.value(2).toString();
      temp->description = record.value(3).toString();
      temp->equipment = record.value(4).toString();
      temp->citations = record.value(5).toString();
      treatment_list.push_back(std::move(temp));
    }
    return treatment_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Scene>> SQLite3Driver::get_scenes() const
{
  if (_db.isOpen()) {
    std::vector<std::unique_ptr<Scene>> scene_list;
    QSqlQuery scene_query{ _db };
    scene_query.prepare(select_all_scenes);
    scene_query.exec();
    while (scene_query.next()) {
      auto temp = std::make_unique<Scene>();
      auto record = scene_query.record();
      temp->id = record.value(0).toInt();
      temp->name = record.value(1).toString();
      temp->description = record.value(2).toString();
      temp->time_of_day = record.value(3).toString();
      temp->time_in_simulation = record.value(4).toInt();
      temp->weather = record.value(5).toString();
      temp->events = record.value(6).toString();
      temp->items = record.value(7).toString();
      temp->roles = record.value(8).toString();
      temp->details = record.value(9).toString();
      scene_list.push_back(std::move(temp));
    }
    return scene_list;
  }
  throw std::runtime_error("No db connection");
}
//----------------------------------------------------------------------------
bool SQLite3Driver::remove_equipment_from_treatments(int equipment_id)
{
  return remove_equipment_from_treatments(std::to_string(equipment_id));
}
bool SQLite3Driver::remove_equipment_from_treatments(std::string equipment_id)
{
  auto treatments = get_treatments();
  if ( _db.isOpen() ) {
    for (auto& treatment : treatments) {
      std::string equipment = list_remove(treatment->equipment.toStdString(),equipment_id);
      QSqlQuery query{ _db };
      query.prepare(update_treatment_by_id);
      query.bindValue(":id", treatment->id);
      query.bindValue(":citations", treatment->citations);
      query.bindValue(":equipment", equipment.c_str());
      query.bindValue(":medical_name", treatment->medical_name);
      query.bindValue(":common_name", treatment->common_name);
      query.bindValue(":description", treatment->description);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
    }
    return true;
  }
  return false;
}
bool SQLite3Driver::remove_citation_from_treatments(int citation_id)
{
  return remove_citation_from_treatments(std::to_string(citation_id)); 
}
bool SQLite3Driver::remove_citation_from_treatments(std::string citation_id)
{
  auto treatments = get_treatments();
  if (_db.isOpen()) {
    for (auto& treatment : treatments) {
      std::string citations = list_remove(treatment->citations.toStdString(), citation_id);
      QSqlQuery query{ _db };
      query.prepare(update_treatment_by_id);
      query.bindValue(":id", treatment->id);
      query.bindValue(":citations", citations.c_str());
      query.bindValue(":equipment", treatment->equipment);
      query.bindValue(":medical_name", treatment->medical_name);
      query.bindValue(":common_name", treatment->common_name);
      query.bindValue(":description", treatment->description);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
    }
    return true;
  }
  return false;
}
bool SQLite3Driver::remove_citation_from_injuries(int citation_id)
{
  return remove_citation_from_injuries(std::to_string(citation_id));
}
bool SQLite3Driver::remove_citation_from_injuries(std::string citation_id)
{
  auto injuries = get_injuries();
  if (_db.isOpen()) {
    for (auto& injury : injuries) {
      std::string citations = list_remove(injury->citations.toStdString(), citation_id);
      QSqlQuery query{ _db };
      query.prepare(update_injury_by_id);
      query.bindValue(":id", injury->id);
      query.bindValue(":citations", citations.c_str());
      query.bindValue(":medical_name", injury->medical_name);
      query.bindValue(":common_name", injury->common_name);
      query.bindValue(":description", injury->description);
      query.bindValue(":min", injury->severity_min);
      query.bindValue(":max", injury->severity_max);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
    }
    return true;
  }
  return false;
}
bool SQLite3Driver::remove_citation_from_equipment(int citation_id)
{
  return remove_citation_from_equipment(std::to_string(citation_id));
}
bool SQLite3Driver::remove_citation_from_equipment(std::string citation_id)
{
  auto equipments = get_equipments();
  if (_db.isOpen()) {
    for (auto& equipment : equipments) {
      std::string citations = list_remove(equipment->citations.toStdString(), citation_id);
      QSqlQuery query{ _db };
      query.prepare(update_equipment_by_id);
      query.bindValue(":id", equipment->id);
      query.bindValue(":name", equipment->name);
      query.bindValue(":type", equipment->type);
      query.bindValue(":description", equipment->description);
      query.bindValue(":citations", citations.c_str());
      query.bindValue(":image", equipment->image);
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
    }
    return true;
  }
  return false;
}
bool SQLite3Driver::remove_citation_from_objectives(int citation_id)
{
  return remove_citation_from_objectives(std::to_string(citation_id));
}
bool SQLite3Driver::remove_citation_from_objectives(std::string citation_id)
{
  auto objectives = get_objectives();
  if (_db.isOpen()) {
    for (auto& objective : objectives) {
      std::string citations = list_remove(objective->citations.toStdString(), citation_id);
      QSqlQuery query{ _db };
      query.prepare(update_objective_by_id);
      query.bindValue(":id", objective->id);
      query.bindValue(":name", objective->name);
      query.bindValue(":description", objective->description);
      query.bindValue(":citations", citations.c_str());
      if (!query.exec()) {
        qWarning() << query.lastError();
        return false;
      }
    }
    return true;
  }
  return false;
}
bool SQLite3Driver::remove_injury_from_injury_sets(int injury_id)
{
  return remove_injury_from_injury_sets(std::to_string(injury_id));  
}
bool SQLite3Driver::remove_injury_from_injury_sets(std::string injury_id)
{
  return false;
}

std::string SQLite3Driver::list_remove(std::string list, std::string id) const
{
  std::string result;
  std::string temp_id;
  for (int i = 0;i < list.length();++i) {
    if (list[i] != ';') {
      temp_id += list[i];
    } else {
      if (temp_id != id && !temp_id.empty()) {
        result += (temp_id+";");
      }
      temp_id.clear();
    }
  }
  if (temp_id != id) {
    result += temp_id;
  }
  if (!result.empty() && result[result.length() - 1] == ';') {
    result.pop_back();
  }
  return result;
}

}
