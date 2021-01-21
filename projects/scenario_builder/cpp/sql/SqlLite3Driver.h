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

#include <memory>
#include <vector>

#include <QList>
#include <QObject>
#include <QQmlListProperty>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
#include <QVariant>

#include "SQLTables.h"

namespace pfc {
class Serializer;

//!  \class SQLite3Driver
//!
//!  \brief  QtSql based Sqlite3 Driver for sotring working version of internal database
//!          Each table has a series of functions designed to interact with the database
//!          SQLTables has a series of structs designed to pass between this class and QML
//!
//!          For instance to interact with the table Scenss you will use
//!          void scenes()
//!          std::vector<std::unique_ptr<Scene>> get_scenes()
//!          bool update_scene(scene);
//!          bool remove_scene(scene);
//!          bool select_scene(scene);
//!          bool next_scene();

//!          The idea is that when scenes prepopulates an interable list of scenes and resets the current scene to the begin();
//!          Next scene changed an interal pointer to the current scene in the table and allows QML to iterate over each scene like a for loop
//!

//!          In adition to this a Scene can be constructed and pasted to select,update,remove   to interact with the specific scene by id or unique pair.
//!          This is the primary way that QML currently uses this class.

class SQLite3Driver : public QObject {
  friend Serializer;

public:
  Q_OBJECT

  Q_PROPERTY(QString name READ Name WRITE Name NOTIFY nameChanged)
  Q_PROPERTY(QString path READ Path WRITE Path NOTIFY pathChanged)

  Q_PROPERTY(QQmlListProperty<Author>       authors       READ getAuthors    NOTIFY authorsChanged)
  Q_PROPERTY(QQmlListProperty<Property>     properties    READ getProperties NOTIFY propertiesChanged)
  Q_PROPERTY(QQmlListProperty<Objective>    objectives    READ getObjectives NOTIFY objectivesChanged)
  Q_PROPERTY(QQmlListProperty<Citation>     citations     READ getCitations  NOTIFY citationsChanged)
  Q_PROPERTY(QQmlListProperty<Treatment>    treatments    READ getTreatments NOTIFY treatmentsChanged)
  Q_PROPERTY(QQmlListProperty<Equipment>    equipment     READ getEquipment  NOTIFY equipmentChanged)
  Q_PROPERTY(QQmlListProperty<Trauma>       traumas       READ getTraumas    NOTIFY traumasChanged)
  Q_PROPERTY(QQmlListProperty<TraumaProfile> traumaProfiles READ getTraumaProfiles NOTIFY traumaProfilesChanged)
  Q_PROPERTY(QQmlListProperty<Assessment>   assessments   READ getAssessments NOTIFY assessmentsChanged)
  Q_PROPERTY(QQmlListProperty<Location>     locations     READ getLocations   NOTIFY locationsChanged)
  Q_PROPERTY(QQmlListProperty<RoleMap>      roleMaps      READ getRoleMaps    NOTIFY roleMapsChanged)
  Q_PROPERTY(QQmlListProperty<EventMap>     eventMaps     READ getEventMaps   NOTIFY eventMapsChanged)
  Q_PROPERTY(QQmlListProperty<LocationMap>  locationMaps  READ getLocationMaps  NOTIFY locationMapsChanged)
  Q_PROPERTY(QQmlListProperty<CitationMap>  citationMaps  READ getCitationMaps  NOTIFY citationMapsChanged)
  Q_PROPERTY(QQmlListProperty<EquipmentMap> equipmentMaps READ getEquipmentMaps NOTIFY equipmentMapsChanged)
  Q_PROPERTY(QQmlListProperty<Role>         roles         READ getRoles  NOTIFY rolesChanged)
  Q_PROPERTY(QQmlListProperty<Event>        events        READ getEvents NOTIFY eventsChanged)
  Q_PROPERTY(QQmlListProperty<Scene>        scenes        READ getScenes NOTIFY scenesChanged)

public:
  enum Sqlite3Table {
    AUTHORS,
    ASSESSMENTS,
    CITATIONS,
    CITATION_MAPS,
    EVENTS,
    EVENT_MAPS,
    EQUIPMENTS,
    EQUIPMENT_MAPS,
    TRAUMAS,
    TRAUMA_PROFILES,
    LOCATIONS,
    LOCATION_MAPS,
    OBJECTIVES,
    PROPERTIES,
    ROLES,
    ROLE_MAPS,
    RESTRICTIONS,
    RESTRICTION_MAPS,
    SCENES,
    TREATMENTS
  };
  Q_ENUM(Sqlite3Table)

  std::map<Sqlite3Table, char const*> tables = {
    { AUTHORS, "authors" },
    { ASSESSMENTS, "assessments" },
    { CITATIONS, "citations" },
    { CITATION_MAPS, "citation_maps" },
    { EVENTS, "events" },
    { EVENT_MAPS, "event_maps" },
    { EQUIPMENTS, "equipments" },
    { EQUIPMENT_MAPS, "equipment_map" },
    { TRAUMAS, "traumas" },
    { TRAUMA_PROFILES, "trauma_profiles" },
    { LOCATIONS, "locations" },
    { LOCATION_MAPS, "location_maps" },
    { OBJECTIVES, "objectives" },
    { PROPERTIES, "properties" },
    { ROLES, "roles" },
    { ROLE_MAPS, "role_maps" },
    { RESTRICTIONS, "restrictions" },
    { RESTRICTION_MAPS, "restriction_maps" },
    { SCENES, "scenes" },
    { TREATMENTS, "treatments" }
  };

  explicit SQLite3Driver(QObject* parent = nullptr);
  SQLite3Driver(const std::string& dbName, const std::string& path = "./", QObject* parent = nullptr);
  SQLite3Driver(const SQLite3Driver&) = delete;
  SQLite3Driver(SQLite3Driver&&) = delete;
  SQLite3Driver& operator=(const SQLite3Driver&) = delete;
  SQLite3Driver& operator=(SQLite3Driver&&) = delete;
  ~SQLite3Driver();

  Q_INVOKABLE bool ready();
  Q_INVOKABLE bool open(QString db_name);
  Q_INVOKABLE void close();

  Q_INVOKABLE bool initialize_db();

  Q_INVOKABLE bool populate_citations();
  Q_INVOKABLE bool populate_db();
  Q_INVOKABLE bool populate_equipment();
  Q_INVOKABLE bool populate_traumas();
  Q_INVOKABLE bool populate_treatments();

  Q_INVOKABLE bool clear_db();
  Q_INVOKABLE bool clear_table(enum SQLite3Driver::Sqlite3Table);

  Q_INVOKABLE int author_count() const;
  Q_INVOKABLE int property_count() const;
  Q_INVOKABLE int objective_count() const;
  Q_INVOKABLE int citation_count() const;
  Q_INVOKABLE int treatment_count() const;
  Q_INVOKABLE int equipment_count() const;
  Q_INVOKABLE int equipment_count(Scene const*) const;
  Q_INVOKABLE int trauma_count() const;
  Q_INVOKABLE int trauma_profile_count() const;
  Q_INVOKABLE int assessment_count() const;
  Q_INVOKABLE int location_count() const;
  Q_INVOKABLE int location_count(Scene const*) const;
  Q_INVOKABLE int role_map_count() const;
  Q_INVOKABLE int event_map_count() const;
  Q_INVOKABLE int location_map_count() const;
  Q_INVOKABLE int location_map_count(Scene const*) const;
  Q_INVOKABLE int citation_map_count() const;
  Q_INVOKABLE int equipment_map_count() const;
  Q_INVOKABLE int role_count() const;
  Q_INVOKABLE int role_count(Scene const*) const;
  Q_INVOKABLE int event_count() const;
  Q_INVOKABLE int event_count(Scene const*) const;
  Q_INVOKABLE int citation_count(Scene const*) const;
  Q_INVOKABLE int scene_count() const;

  Q_INVOKABLE int nextID(Sqlite3Table) const;
  

  //!
  //!  Calling a table function updates the QList of known cash hits
  //!  This should allow us to convert the QSQL Database to a
  //!  QListModel using our structs;
  //!
  //!  A stronger refactor for 2.0 Would be to rewrite this entire class as a QSqlTableModel. Our Average UI screen
  //!  Is really just a complex VIEW of no more then a 3 Table Join.
  //!  Updating is more complicated, but can be derived from forign_key relations in the view  I strongly advise
  //!  Removing this coding layer in the next refactor in favor of some AbstractDataModel which can automatically update
  //!  fields in QML and hopefully signal element changes accordingly to all pages in the QML UI
  //!
  //!  https://wiki.qt.io/How_to_Use_a_QSqlQueryModel_in_QML

  QList<Author*> authors() const;
  QList<Property*> properties() const;
  QList<Objective*> objectives() const;
  QList<Citation*> citations() const;
  QList<Treatment*> treatments() const;
  QList<Equipment*> equipments() const;
  QList<Trauma*> traumas() const;
  QList<TraumaProfile*> trauma_profiles() const;
  QList<Assessment*> assessments() const;
  QList<Location*> locations() const;
  QList<Role*> roles() const;
  QList<RoleMap*> roles_in_scene(Scene const* scene) const;
  QList<RoleMap*> role_maps() const;
  QList<EventMap*> event_maps() const;
  QList<LocationMap*> location_maps() const;
  QList<CitationMap*> citation_maps() const;
  QList<EquipmentMap*> equipment_maps() const;
  QList<Event*> events() const;
  QList<Event*> events_in_scene(Scene const* scene) const;
  
  QList<EquipmentMap*> equipment_in_scene(Scene const* scene) const;
  QList<Scene*> scenes() const;

  Q_INVOKABLE QQmlListProperty<Author> getAuthors() const;
  Q_INVOKABLE QString getProperty(QString name) const;
  Q_INVOKABLE QQmlListProperty<Property> getProperties() const;
  Q_INVOKABLE QQmlListProperty<Objective> getObjectives() const;
  Q_INVOKABLE QQmlListProperty<Citation> getCitations() const;
  Q_INVOKABLE QQmlListProperty<Treatment> getTreatments() const;
  Q_INVOKABLE QQmlListProperty<Equipment> getEquipment() const;
  Q_INVOKABLE QQmlListProperty<Trauma> getTraumas() const;
  Q_INVOKABLE QQmlListProperty<TraumaProfile> getTraumaProfiles() const;
  Q_INVOKABLE QQmlListProperty<Assessment> getAssessments() const;
  Q_INVOKABLE QQmlListProperty<Location> getLocations() const;
  Q_INVOKABLE Location* getLocationOfScene(Scene* scene) const;
  Q_INVOKABLE QQmlListProperty<RoleMap> getRoleMaps() const;
  Q_INVOKABLE QQmlListProperty<EventMap> getEventMaps() const;
  Q_INVOKABLE QQmlListProperty<LocationMap> getLocationMaps() const;
  Q_INVOKABLE QQmlListProperty<CitationMap> getCitationMaps() const;
  Q_INVOKABLE QQmlListProperty<EquipmentMap> getEquipmentMaps() const;
  Q_INVOKABLE QQmlListProperty<Role> getRoles() const;
  Q_INVOKABLE QQmlListProperty<RoleMap> getRolesInScene(Scene* scene) const;
  Q_INVOKABLE QQmlListProperty<Event> getEvents() const;
  Q_INVOKABLE QQmlListProperty<Scene> getScenes() const;

  Q_INVOKABLE bool select_author(Author*) const;
  Q_INVOKABLE bool select_property(Property*) const;
  Q_INVOKABLE bool select_objective(Objective*) const;
  Q_INVOKABLE bool select_citation(Citation*) const;
  Q_INVOKABLE bool select_treatment(Treatment*) const;
  Q_INVOKABLE bool select_equipment(Equipment*) const;
  Q_INVOKABLE bool select_trauma(Trauma*) const;
  Q_INVOKABLE bool select_trauma_profile(TraumaProfile*) const;
  Q_INVOKABLE bool select_assessment(Assessment*) const;
  Q_INVOKABLE bool select_location(Location*) const;
  Q_INVOKABLE bool select_role_map(RoleMap*) const;
  Q_INVOKABLE bool select_event_map(EventMap*) const;
  Q_INVOKABLE bool select_location_map(LocationMap*) const;
  Q_INVOKABLE bool select_citation_map(CitationMap*) const;
  Q_INVOKABLE bool select_equipment_map(EquipmentMap*) const;
  Q_INVOKABLE bool select_role(Role*) const;
  Q_INVOKABLE bool select_event(Event*) const;
  Q_INVOKABLE bool select_scene(Scene*) const;

  Q_INVOKABLE bool update_author(Author*);
  Q_INVOKABLE bool update_first_author(Author*);
  Q_INVOKABLE bool update_property(Property*);
  Q_INVOKABLE bool update_objective(Objective*);
  Q_INVOKABLE bool update_location(Location*);
  Q_INVOKABLE bool update_role_map(RoleMap*);
  Q_INVOKABLE bool update_event_map(EventMap*);
  Q_INVOKABLE bool update_location_map(LocationMap*);
  Q_INVOKABLE bool update_citation_map(CitationMap*);
  Q_INVOKABLE bool update_equipment_map(EquipmentMap*);
  Q_INVOKABLE bool update_role(Role*);
  Q_INVOKABLE bool update_role_in_scene(Scene*, Role*);
  Q_INVOKABLE bool update_location_in_scene(Scene*, Location*);

  Q_INVOKABLE bool update_event(Event*);
  Q_INVOKABLE bool update_event_in_scene(Scene*, Event*);
  Q_INVOKABLE bool update_citation(Citation*);
  Q_INVOKABLE bool update_citation_in_scene(Scene*, Citation*);
  Q_INVOKABLE bool update_treatment(Treatment*);
  Q_INVOKABLE bool update_equipment(Equipment*);
  Q_INVOKABLE bool update_equipment_in_scene(EquipmentMap*);
  Q_INVOKABLE bool update_trauma(Trauma*);
  Q_INVOKABLE bool update_trauma_profile(TraumaProfile*);
  Q_INVOKABLE bool update_assessment(Assessment*);
  Q_INVOKABLE bool update_scene(Scene*, bool createLocationIfMissing = true);

  Q_INVOKABLE bool remove_author(Author*);
  Q_INVOKABLE bool remove_property(Property*);
  Q_INVOKABLE bool remove_objective(Objective*);
  Q_INVOKABLE bool remove_citation(Citation*);
  Q_INVOKABLE bool remove_citation_from_scene(Citation*, Scene*);
  Q_INVOKABLE bool remove_treatment(Treatment*);
  Q_INVOKABLE bool remove_equipment(Equipment*);
  Q_INVOKABLE bool remove_equipment_from_scene(Equipment*, Scene*);
  Q_INVOKABLE bool remove_trauma(Trauma*);
  Q_INVOKABLE bool remove_trauma_profile(TraumaProfile*);
  Q_INVOKABLE bool remove_assessment(Assessment*);
  Q_INVOKABLE bool remove_location(Location*);
  Q_INVOKABLE bool remove_location_from_scene(Location*, Scene*);
  Q_INVOKABLE bool remove_role_map(RoleMap*);
  Q_INVOKABLE bool remove_event_map(EventMap*);
  Q_INVOKABLE bool remove_location_map(LocationMap*);
  Q_INVOKABLE bool remove_citation_map(CitationMap*);
  Q_INVOKABLE bool remove_equipment_map(EquipmentMap*);
  Q_INVOKABLE bool remove_role_map_by_fk(RoleMap*);
  Q_INVOKABLE bool remove_event_map_by_fk(EventMap*);
  Q_INVOKABLE bool remove_location_map_by_fk(LocationMap*);
  Q_INVOKABLE bool remove_citation_map_by_fk(CitationMap*);
  
  Q_INVOKABLE bool remove_equipment_map_by_fk(EquipmentMap*);
  Q_INVOKABLE bool remove_role(Role*);
  Q_INVOKABLE bool remove_role_from_scene(Role*, Scene*);
  Q_INVOKABLE bool remove_event(Event*);
  Q_INVOKABLE bool remove_event_from_scene(Event*, Scene*);
  Q_INVOKABLE bool remove_scene(Scene*);
  
  Q_INVOKABLE bool remove_equipment_from_treatments(int);
  Q_INVOKABLE bool remove_equipment_from_treatments(std::string);
  Q_INVOKABLE bool remove_citation_from_treatments(int);
  Q_INVOKABLE bool remove_citation_from_treatments(std::string);
  Q_INVOKABLE bool remove_citation_from_traumas(int);
  Q_INVOKABLE bool remove_citation_from_traumas(std::string);
  Q_INVOKABLE bool remove_citation_from_equipment(int);
  Q_INVOKABLE bool remove_citation_from_equipment(std::string);
  Q_INVOKABLE bool remove_citation_from_objectives(int);
  Q_INVOKABLE bool remove_citation_from_objectives(std::string);
  Q_INVOKABLE bool remove_trauma_from_trauma_profiles(int);
  Q_INVOKABLE bool remove_trauma_from_trauma_profiles(std::string);

  Q_INVOKABLE void establish_settings();
  Q_INVOKABLE void log_scenario_file(std::string);
  Q_INVOKABLE void log_scenario_file(QString);
  Q_INVOKABLE QList<QString> get_recent_scenario_files();

  std::string list_remove(std::string, std::string) const;
  std::string list_remove_index(std::string, int) const;
  int list_find(std::string, std::string) const;

  Q_INVOKABLE int raw_error() const { return QSqlDatabase::database(_db_name).lastError().type(); };
  Q_INVOKABLE QString error_message() const { return QSqlDatabase::database(_db_name).lastError().text(); }
  Q_INVOKABLE bool success() const { return !error(); }
  Q_INVOKABLE bool error() const { return QSqlDatabase::database(_db_name).lastError().isValid(); };

  QString Path() const { return _db_path; };
  QString Name() const { return _db_name; };

  void Path(const QString p) { _db_path = p; };
  void Name(const QString n) { _db_name = n; };

signals:
  void nameChanged();
  void pathChanged();

  //Additional signal for any change to a table
  //Possible used when you want to update secondary relations that might
  //Invalidate your entire QML model
  //
   void authorsChanged();
  void assessmentsChanged();
  void eventsChanged();
  void equipmentChanged();
  void traumasChanged();
  void traumaProfilesChanged();
  void treatmentsChanged();;
  void locationsChanged();
  void objectivesChanged();
  void propertiesChanged();
  void citationsChanged();
  void rolesChanged();
  void scenesChanged();
  void roleMapsChanged();
  void eventMapsChanged();
  void locationMapsChanged();
  void citationMapsChanged();
  void equipmentMapsChanged();
  
  //Signals the removal of an author
  //This is intended for QML to prune models when possible.
  void authorRemoved(int index);
  void assessmentRemoved(int index);
  void eventRemoved(int index);
  void equipmentRemoved(int index);
  void traumaRemoved(int index);
  void traumaProfileRemoved(int index);
  void locationRemoved(int index);
  void mapRemoved(int index);
  void eventMapRemoved(int index);
  void locationMapRemoved(int index);
  void citationMapRemoved(int index);
  void equipmentMapRemoved(int index);
  void objectiveRemoved(int index);
  void propertyRemoved(int index);
  void citationRemoved(int index);
  void roleRemoved(int index);
  void sceneRemoved(int index);
  void treatmentRemoved(int index);

  //Signals that a table was Updated and which index has changed
  //This is intended for QML optimization of model cashes.
  void authorUpdated(int index);
  void assessmentUpdated(int index);
  void eventUpdated(int index);
  void equipmentUpdated(int index);
  void traumaUpdated(int index);
  void traumaProfileUpdated(int index);
  void locationUpdated(int index);
  void mapUpdated(int index);
  void eventMapUpdated(int index);
  void locationMapUpdated(int index);
  void citationMapUpdated(int index);
  void equipmentMapUpdated(int index);
  void objectiveUpdated(int index);
  void propertyUpdated(int index);
  void citationUpdated(int index);
  void roleUpdated(int index);
  void sceneUpdated(int index);
  void treatmentUpdated(int index);

  void refresh();

private:
  bool open();

  void assign_assessment(const QSqlRecord& record, Assessment& assessment) const;
  void assign_author(const QSqlRecord& record, Author& author) const;
  void assign_citation(const QSqlRecord& record, Citation& citation) const;
  void assign_citation_map(const QSqlRecord& record, CitationMap& map) const;
  void assign_equipment(const QSqlRecord& record, Equipment& equipment) const;
  void assign_equipment_map(const QSqlRecord& record, EquipmentMap& map, Scene const* scene = nullptr, Equipment const* equipment = nullptr) const;
  void assign_event(QSqlRecord& record, Event& event) const;
  void assign_event_map(const QSqlRecord& record, EventMap& map) const;
  void assign_location(const QSqlRecord& record, Location& location) const;
  void assign_location_map(const QSqlRecord& record, LocationMap& map) const;
  void assign_objective(const QSqlRecord& record, Objective& objective) const;
  void assign_property(const QSqlRecord& record, Property& property) const;
  void assign_trauma(const QSqlRecord& record, Trauma& trauma) const;
  void assign_trauma_profile(const QSqlRecord& record, TraumaProfile& trauma) const;
  void assign_treatment(const QSqlRecord& record, Treatment& treatment) const;
  void assign_scene(QSqlRecord& record, Scene& scene) const;
  void assign_role(QSqlRecord& record, Role& role) const;
  void assign_role_map(const QSqlRecord& record, RoleMap& map) const;

private:
  QString _db_name = "";
  QString _db_path = "./";
  mutable int _error_code = 0;

  std::vector<std::string> recent_files;
};
}

#endif //PFC_SCENARIO_BUILDER_SQL_SQLLITE3_DRVER_H
