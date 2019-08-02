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

#include "SQLTables.h"
namespace pfc {


class SQLite3Driver : public QObject {
public:
  Q_OBJECT

  Q_PROPERTY(QString name READ Name WRITE Name NOTIFY nameChanged)
  Q_PROPERTY(QString path READ Path WRITE Path NOTIFY pathChanged)

public:
  enum Sqlite3Table {
    AUTHORS,
    ASSESSMENTS,
    EVENTS,
    EQUIPMENTS,
    INJURIES,
    LOCATIONS,
    OBJECTIVES,
    PROPERTIES,
    PROPS,
    CITATIONS,
    RESTRICTIONS,
    ROLES,
    TREATMENTS,
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

  Q_INVOKABLE int author_count() const;
  Q_INVOKABLE int property_count() const;
  Q_INVOKABLE int restriction_count() const;
  Q_INVOKABLE int objective_count() const;
  Q_INVOKABLE int citation_count() const;
  Q_INVOKABLE int treatment_count() const;
  Q_INVOKABLE int equipment_count() const;
  Q_INVOKABLE int injury_count() const;
  Q_INVOKABLE int assessment_count() const;
  Q_INVOKABLE int location_count() const;
  Q_INVOKABLE int role_count() const;
  Q_INVOKABLE int prop_count() const;
  Q_INVOKABLE int event_count() const;

  Q_INVOKABLE int nextID(Sqlite3Table) const;

  Q_INVOKABLE void authors();
  Q_INVOKABLE void properties();
  Q_INVOKABLE void restrictions();
  Q_INVOKABLE void objectives();
  Q_INVOKABLE void citations();
  Q_INVOKABLE void treatments();
  Q_INVOKABLE void equipments();
  Q_INVOKABLE void injuries();
  Q_INVOKABLE void assessments();
  Q_INVOKABLE void locations();
  Q_INVOKABLE void roles();
  Q_INVOKABLE void props();
  Q_INVOKABLE void events();

  Q_INVOKABLE bool next_author(Author*);
  Q_INVOKABLE bool next_property(Property*);
  Q_INVOKABLE bool next_restriction(Restriction*);
  Q_INVOKABLE bool next_objective(Objective*);
  Q_INVOKABLE bool next_citation(Reference*);
  Q_INVOKABLE bool next_treatment(Treatment*);
  Q_INVOKABLE bool next_equipment(Equipment*);
  Q_INVOKABLE bool next_injury(Injury*);
  Q_INVOKABLE bool next_assessment(Assessment*);
  Q_INVOKABLE bool next_location(Location*);
  Q_INVOKABLE bool next_role(Role*);
  Q_INVOKABLE bool next_prop(Prop*);
  Q_INVOKABLE bool next_event(Event*);

  Q_INVOKABLE bool select_author(Author*) const;
  Q_INVOKABLE bool select_property(Property*) const;
  Q_INVOKABLE bool select_restriction(Restriction*) const;
  Q_INVOKABLE bool select_objective(Objective*) const;
  Q_INVOKABLE bool select_citation(Reference*) const;
  Q_INVOKABLE bool select_treatment(Treatment*) const;
  Q_INVOKABLE bool select_equipment(Equipment*) const;
  Q_INVOKABLE bool select_injury(Injury*) const;
  Q_INVOKABLE bool select_assessment(Assessment*) const;
  Q_INVOKABLE bool select_location(Location*) const;
  Q_INVOKABLE bool select_role(Role*) const;
  Q_INVOKABLE bool select_prop(Prop*) const;
  Q_INVOKABLE bool select_event(Event*) const;

  Q_INVOKABLE bool update_author(Author*);
  Q_INVOKABLE bool update_first_author(Author*);
  Q_INVOKABLE bool update_property(Property*);
  Q_INVOKABLE bool update_restriction(Restriction*);
  Q_INVOKABLE bool update_objective(Objective*);
  Q_INVOKABLE bool update_location(Location*);
  Q_INVOKABLE bool update_role(Role*);
  Q_INVOKABLE bool update_prop(Prop*);
  Q_INVOKABLE bool update_event(Event*);
  Q_INVOKABLE bool update_citation(Reference*);
  Q_INVOKABLE bool update_treatment(Treatment*);
  Q_INVOKABLE bool update_equipment(Equipment*);
  Q_INVOKABLE bool update_injury(Injury*);
  Q_INVOKABLE bool update_assessment(Assessment*);

  Q_INVOKABLE bool remove_author(Author*);
  Q_INVOKABLE bool remove_property(Property*);
  Q_INVOKABLE bool remove_restriction(Restriction*);
  Q_INVOKABLE bool remove_objective(Objective*);
  Q_INVOKABLE bool remove_citation(Reference*);
  Q_INVOKABLE bool remove_treatment(Treatment*);
  Q_INVOKABLE bool remove_equipment(Equipment*);
  Q_INVOKABLE bool remove_injury(Injury*);
  Q_INVOKABLE bool remove_assessment(Assessment*);
  Q_INVOKABLE bool remove_location(Location*);
  Q_INVOKABLE bool remove_role(Role*);
  Q_INVOKABLE bool remove_prop(Prop*);
  Q_INVOKABLE bool remove_event(Event*);

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
  void objectivesChanged();

private:
  bool open();

private:
  QString _db_name = "";
  QString _db_path = "./";
  QSqlDatabase _db;
  mutable int _error_code = 0;

  QList<Author*> _authors;
  QList<Property*> _properties;
  QList<Restriction*> _restrictions;
  QList<Objective*> _objectives;
  QList<Reference*> _citations;
  QList<Treatment*> _treatments;
  QList<Equipment*> _equipments;
  QList<Injury*> _injuries;
  QList<Assessment*> _assessments;
  QList<Location*> _locations;
  QList<Role*> _roles;
  QList<Prop*> _props;
  QList<Event*> _events;

  QList<Author*>::iterator _current_author;
  QList<Property*>::iterator _current_property;
  QList<Restriction*>::iterator _current_restriction;
  QList<Objective*>::iterator _current_objective;
  QList<Reference*>::iterator _current_citation;
  QList<Treatment*>::iterator _current_treatment;
  QList<Equipment*>::iterator _current_equipment;
  QList<Injury*>::iterator _current_injury;
  QList<Assessment*>::iterator _current_assessment;
  QList<Location*>::iterator _current_location;
  QList<Role*>::iterator _current_role;
  QList<Prop*>::iterator _current_prop;
  QList<Event*>::iterator _current_event;
};
}

#endif //PFC_SCENARIO_BUILDER_SQL_SQLLITE3_DRVER_H