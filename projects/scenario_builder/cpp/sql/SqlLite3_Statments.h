#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_STATMENTS_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_STATMENTS_H

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

namespace pfc {
inline namespace sqlite3 {

  constexpr char const* tables[] = {
    "authors",
    "assessments",
    "citations",
    "events",
    "equipments",
    "injuries",
    "injury_sets",
    "locations",
    "objectives",
    "properties",
    "props",
    "restrictions",
    "roles",
    "treatments",
    "scenes",
    "role_maps",
    "event_maps",
    "prop_maps",
    "restriction_maps"
  };

  constexpr auto list_tables = "SELECT * FROM sqlite_master WHERE type='table';";
  constexpr auto drop_table = "DROP TABLE IF EXISTS :table;";

  //---------------------- ASSESSMENTS STATMENTS ------------------------
  enum ASSESSMENT_COLUMNS {
    ASSESSMENT_ID,
    ASSESSMENT_NAME,
    ASSESSMENT_DESCRIPTION,
    ASSESSMENT_TYPE,
    ASSESSMENT_AVAILABLE_POINTS,
    ASSESSMENT_CRITERIA,
    ASSESSMENT_COLUMN_COUNT
  };

  constexpr auto create_assessments_table = R"(
  CREATE TABLE IF NOT EXISTS assessments (
    assessment_id INTEGER PRIMARY KEY,
    name Varchar(64) NOT NULL UNIQUE,
    description TEXT,
    type TEXT,
    available_points INTEGER,
    criteria TEXT
  );
  )";

  constexpr auto drop_all_assessments = R"( DELETE FROM assessments; )";
  constexpr auto count_assessments = R"( SELECT COUNT(assessment_id) FROM assessments; )";
  constexpr auto select_all_assessments = R"( SELECT assessment_id,name,description,type,available_points,criteria FROM assessments ORDER BY name; )";

  constexpr auto select_assessment_by_id
    = R"( SELECT * FROM assessments WHERE assessment_id = :id; )";
  constexpr auto update_assessment_by_id
    = R"( UPDATE  assessments 
          SET name = :name
              , description = :description
              , type = :type
              , available_points = :available_points
              , criteria = :criteria
          WHERE assessment_id = :id;
         )";
  constexpr auto delete_assessment_by_id
      = R"( DELETE FROM assessments WHERE assessment_id = :id; )";
  constexpr auto delete_assessment_by_name
    = R"( DELETE FROM assessments WHERE name = :name; )";
  constexpr auto select_assessment_by_name
    = R"( SELECT * FROM assessments WHERE name = :name ORDER BY name; )";
  constexpr auto insert_or_update_assessments
    = R"( INSERT INTO assessments
          (name,description,type,available_points,criteria)
          VALUES (:name, :description, :type, :available_points, :criteria)
          ON CONFLICT (name)
          DO UPDATE SET name = excluded.name
                       , description = excluded.description
                       , type = excluded.type
                       , available_points = excluded.available_points
                       , criteria = excluded.criteria
         ;
         )";
  
  //---------------------- AUTHOR STATMENTS ------------------------
  enum AUTHOR_COLUMNS {
    AUTHOR_ID,
    AUTHOR_FIRST_NAME,
    AUTHOR_LAST_NAME,
    AUTHOR_EMAIL,
    AUTHOR_ZIPCODE,
    AUTHOR_STATE,
    AUTHOR_COUNTRY,
    AUTHOR_PHONE,
    AUTHOR_ORGANIZATION,
    AUTHOR_COLUMN_COUNT
  };
  constexpr auto create_authors_table = R"(
  CREATE TABLE IF NOT EXISTS authors(
    author_id INTEGER PRIMARY KEY,
    name_first Varchar(25),
    name_last Varchar(25) NOT NULL ,
    email VarChar(45) NOT NULL UNIQUE,
    zipcode Varchar(10),
    state Varchar(64),
    country Varchar(64),
    phone Varchar(16),
    organization Varchar(128)
  );
  )";
  constexpr auto drop_all_authors = R"( DELETE FROM authors; )";
  constexpr auto count_authors = R"( SELECT COUNT(author_id) FROM authors; )";
  constexpr auto select_all_authors = R"( SELECT author_id,name_first,name_last,email,zipcode,state,country,phone,organization FROM authors ORDER BY name_last; )";

  constexpr auto select_author_by_id
    = R"( SELECT * FROM authors WHERE author_id = :id ; )";
  constexpr auto update_author_by_id
    = R"( UPDATE  authors 
          SET name_first = :name_first
              , name_last = :name_last
              , email = :email
              , zipcode = :zipcode
              , state = :state
              , country = :country
              , phone = :phone
              , organization = :organization
          WHERE author_id = :id;
         )";
  constexpr auto select_author_by_email
    = R"( SELECT * FROM authors WHERE email = :email ORDER BY name_last; )";

  constexpr auto insert_or_update_first_author
    = R"( INSERT  OR REPLACE INTO authors
                 (  author_id, name_first,  name_last,  email,  zipcode,  state,  country,  phone,  organization)
          VALUES (1, :name_first, :name_last, :email, :zipcode, :state, :country, :phone, :organization)
        )";

  constexpr auto insert_or_update_authors
    = R"( INSERT  INTO authors
                 (  name_first,  name_last,  email,  zipcode,  state,  country,  phone,  organization)
          VALUES ( :name_first, :name_last, :email, :zipcode, :state, :country, :phone, :organization)
          ON CONFLICT(email) 
          DO UPDATE SET 
                     name_first = excluded.name_first
                    ,name_last  = excluded.name_last
                    ,zipcode    = excluded.zipcode  
                    ,state      = excluded.state
                    ,country    = excluded.country
                    ,phone      = excluded.phone
                    ,organization = excluded.organization;
        )";

  constexpr auto delete_author_by_id
    = R"( DELETE FROM authors WHERE author_id = :id; )";
  constexpr auto delete_author_by_email
    = R"( DELETE FROM authors WHERE email = :email; )";

  //---------------------- EQUIPMENT STATMENTS ------------------------
  enum EQUIPMENT_COLUMNS {
    EQUIPMENT_ID,
    EQUIPMENT_TYPE,
    EQUIPMENT_NAME,
    EQUIPMENT_DESCRIPTION,
    EQUIPMENT_CITATIONS,
    EQUIPMENT_IMAGE,
    EQUIPMENT_COLUMN_COUNT
  };
  // I know that the plural of equipment is 'equipment', but we've made it equipments to be less ambiguous
  constexpr auto create_equipment_table = R"(
  CREATE TABLE IF NOT EXISTS equipments (
    equipment_id INTEGER PRIMARY KEY,
    type INTEGER NOT NULL DEFAULT 1,
    name Varchar(64) NOT NULL UNIQUE,
    description Varchar(64) NOT NULL,
    citations TEXT,
    image TEXT
  );
  )";
  constexpr auto drop_all_equipment = R"( DELETE FROM equipment; )";
  constexpr auto count_equipments = R"( SELECT COUNT(equipment_id) FROM equipments; )";
  constexpr auto count_equipments_in_scene = R"( SELECT COUNT(equipment_map_id) FROM equipment_maps WHERE :id = fk_scene ; )";
  constexpr auto select_all_equipments = R"( SELECT equipment_id,type,name,description,citations,image FROM equipments ORDER BY name; )";

  constexpr auto select_equipment_by_id
    = R"( SELECT * FROM equipments WHERE equipment_id = :id ; )";
  constexpr auto update_equipment_by_id
    = R"( UPDATE  equipments
          SET name = :name
              , type = :type
              , description = :description
              , citations = :citations
              , image = :image
          WHERE equipment_id = :id;
         )";
  constexpr auto delete_equipment_by_id
    = R"( DELETE FROM equipments WHERE equipment_id = :id; )";
  constexpr auto delete_equipment_by_name
    = R"( DELETE FROM equipments WHERE name = :name ; )";
  constexpr auto select_equipment_by_name
    = R"( SELECT * FROM equipments WHERE name = :name ORDER BY name;)";
  constexpr auto insert_or_update_equipments
    = R"( INSERT INTO equipments
          (name,type,description,citations,image)
          VALUES (:name, :type, :description, :citations, :image)
          ON CONFLICT (name)
          DO UPDATE SET name = excluded.name
                        , type = excluded.type
                        , description = excluded.description
                        , citations = excluded.citations
                        , image = excluded.image
          ;         
          )";
  
  //---------------------- EVENT STATMENTS ------------------------
  enum EVENT_COLUMNS {
    EVENT_ID,
    EVENT_NAME,
    EVENT_LOCATION,
    EVENT_ACTOR,
    EVENT_EQUIPMENT,
    EVENT_DESCRIPTION,
    EVENT_COLUMN_COUNT
  };

  constexpr auto create_events_table = R"(
  CREATE TABLE IF NOT EXISTS events (
    event_id INTEGER PRIMARY KEY,
    name Varchar(64) NOT NULL UNIQUE,
    location INTEGER, 
    actor INTEGER,
    equipment TEXT,
    description TEXT
  );
  )";
  constexpr auto drop_all_events = R"( DELETE FROM events; )";
  constexpr auto count_events = R"( SELECT COUNT(event_id) FROM events; )";
  constexpr auto count_events_in_scene = R"( SELECT COUNT(event_map_id) FROM event_maps WHERE :id = fk_scene ; )";
  constexpr auto select_all_events = R"( SELECT event_id,name,location,actor,equipment,description FROM events ORDER BY name; )";

  constexpr auto select_event_by_id
    = R"( SELECT * FROM events WHERE event_id = :id ; )";
  constexpr auto select_event_by_name
    = R"( SELECT * FROM events WHERE name = :name ORDER BY name; )";
  constexpr auto update_event_by_id
    = R"( UPDATE  events
            SET name = :name
                , location = :location
                , actor = :actor
                , equipment = :equipment
                , description = :description
          WHERE event_id = :id;
         )";
  constexpr auto delete_event_by_id
    = R"( DELETE FROM events WHERE event_id = :id; )";
  constexpr auto insert_or_update_events
    = R"( INSERT INTO events
          (name, location, actor, equipment, description)
          VALUES (:name, :location, :actor, :equipment, :description)
          ON CONFLICT (name)
          DO UPDATE SET name = excluded.name
                        , location = excluded.location
                        , actor = excluded.actor
                        , equipment = excluded.equipment
                        , description = excluded.description
          ;          
          )";
  //---------------------- INJURY STATEMENTS ------------------------
  enum INJURY_COLUMNS {
    INJURY_ID,
    INJURY_MEDICAL_NAME,
    INJURY_COMMON_NAME,
    INJURY_DESCRIPTION,
    INJURY_CITATIONS,
    INJURY_SEVERITY_MIN,
    INJURY_SEVERITY_MAX,
    INJURY_COLUMN_COUNT
  };

  constexpr auto create_injuries_table = R"(
  CREATE TABLE IF NOT EXISTS injuries ( 
    injury_id INTEGER PRIMARY KEY,
    medical_name Varchar(64) NOT NULL UNIQUE,
    common_name Varchar(64) NOT NULL UNIQUE,
    description TEXT,
    citations TEXT,
    min REAL DEFAULT 0.0,
    max REAL DEFAULT 1.0
  );
  )";

  constexpr auto drop_all_injuries = R"( DELETE FROM injuries; )";
  constexpr auto count_injuries = R"( SELECT COUNT(injury_id) FROM injuries; )";
  constexpr auto select_all_injuries = R"( SELECT injury_id,medical_name,common_name,description,citations,min,max FROM injuries ORDER BY medical_name; )";

  constexpr auto select_injury_by_id
    = R"( SELECT * FROM injuries WHERE injury_id = :id ; )";
  constexpr auto update_injury_by_id
    = R"( UPDATE  injuries
          SET medical_name = :medical_name
              , common_name = :common_name
              , description = :description
              , citations = :citations
              , min  = :min
              , max  = :max
          WHERE injury_id = :id;
         )";
  constexpr auto delete_injury_by_id
    = R"( DELETE FROM injuries WHERE injury_id = :id; )";
  constexpr auto delete_injury_by_medical_name
    = R"( DELETE FROM injuries WHERE medical_name = :medical_name; )";
  constexpr auto delete_injury_by_common_name
    = R"( DELETE FROM injuries WHERE common_name = :common_name; )";
  constexpr auto select_injury_by_medical_name
    = R"( SELECT * FROM injuries WHERE medical_name = :medical_name ORDER BY medical_name; )";
  constexpr auto select_injury_by_common_name
    = R"( SELECT * FROM injuries WHERE common_name = :common_name ORDER BY common_name; )";
  constexpr auto insert_or_update_injuries
    = R"( INSERT INTO injuries
          (medical_name,common_name,description,citations,min,max)
          VALUES (:medical_name, :common_name, :description, :citations, :min, :max)
          ON CONFLICT (medical_name)
          DO UPDATE SET common_name = excluded.common_name
                        , description = excluded.description
                        , citations = excluded.citations
                        , min= excluded.min
                        , max= excluded.max
          ;          
          )";
  
  //---------------------- Injury Set STATMENTS ------------------------
  enum INJURY_SET_COLUMNS {
    INJURY_SET_ID,
    INJURY_SET_NAME,
    INJURY_SET_DESCRIPTION,
    INJURY_SET_INJURIES,
    INJURY_SET_LOCATIONS,
    INJURY_SET_SEVERITIES,
    INJURY_SET_COLUMN_COUNT
  };
  //TODO: ADD SUPPORT FOR LOCATION AND SEVERITIES (PRIMITIVE List in the same order as the injuries should do it)

  constexpr auto create_injury_sets_table = R"(
  CREATE TABLE IF NOT EXISTS injury_sets ( 
    injury_set_id INTEGER PRIMARY KEY,
    name Varchar(64) NOT NULL UNIQUE,
    description TEXT,
    injuries TEXT,
    locations TEXT,
    severities TEXT
  );
  )";

  constexpr auto drop_all_injury_sets = R"( DELETE FROM injury_sets; )";
  constexpr auto count_injury_sets = R"( SELECT COUNT(injury_set_id) FROM injury_sets; )";
  constexpr auto select_all_injury_sets = R"( SELECT injury_set_id,name,description,injuries,locations,severities FROM injury_sets ORDER BY name; )";

  constexpr auto select_injury_set_by_id
    = R"( SELECT * FROM injury_sets WHERE injury_set_id = :id ; )";
  constexpr auto update_injury_set_by_id
    = R"( UPDATE  injury_sets
          SET name = :name
              , description = :description
              , injuries    = :injuries
              , locations   = :locations
              , severities  = :severities
          WHERE injury_set_id = :id;
         )";
  constexpr auto delete_injury_set_by_id
    = R"( DELETE FROM injury_sets WHERE injury_set_id = :id; )";
  constexpr auto delete_injury_set_by_name
    = R"( DELETE FROM injury_sets WHERE name = :name; )";
  constexpr auto select_injury_set_by_name
    = R"( SELECT * FROM injury_sets WHERE name = :name ORDER BY name; )";
  constexpr auto insert_or_update_injury_sets
    = R"( INSERT INTO injury_sets
          (name, description, injuries, locations, severities)
          VALUES (:name, :description, :injuries, :locations, :severities)
          ON CONFLICT (name)
          DO UPDATE SET name = excluded.name
                        , description = excluded.description
                        , injuries = excluded.injuries
                        , locations = excluded.locations
                        , severities = excluded.severities
          ;
          )";
  //---------------------- LOCATION STATMENTS ------------------------
  enum LOCATION_COLUMNS {
    LOCATION_ID,
    LOCATION_NAME,
    LOCATION_SCENE_NAME,
    LOCATION_TIME_OF_DAY,
    LOCATION_ENVIRONMENT,
    LOCATION_COLUMN_COUNT
  };

  constexpr auto create_locations_table = R"(
  CREATE TABLE IF NOT EXISTS locations (
    location_id INTEGER PRIMARY KEY,
    name Varchar(64) NOT NULL,
    scene_name Varchar(64),
    time_of_day TEXT,
    environment TEXT
  );
  )";
  constexpr auto drop_all_locations = R"( DELETE FROM locations; )";
  constexpr auto count_locations = R"( SELECT COUNT(location_id) FROM locations; )";
  constexpr auto count_locations_in_scene = R"( SELECT COUNT(location_map_id) FROM location_maps WHERE :id = fk_scene ; )";
  constexpr auto select_all_locations = R"( SELECT location_id,name,scene_name,time_of_day,environment FROM locations ORDER BY name; )";

  constexpr auto select_location_by_id
    = R"( SELECT * FROM locations WHERE location_id = :id ; )";
  constexpr auto update_location_by_id
    = R"( UPDATE  locations
          SET name = :name
              , scene_name = :scene_name
              , time_of_day = :time_of_day
              , environment = :environment
          WHERE location_id = :id;
         )";
  constexpr auto delete_location_by_id
    = R"( DELETE FROM locations WHERE location_id = :id; )";
  constexpr auto delete_location_by_name
    = "R( DELETE FROM locations WHERE location_id = :id; )";
  constexpr auto select_location_by_name
    = R"( SELECT * FROM locations WHERE name = :name ORDER BY name; )";
  constexpr auto insert_or_update_locations
    = R"( INSERT INTO locations
          (name,scene_name,time_of_day,environment)
          VALUES (:name, :scene_name, :time_of_day, :environment)
          ;
         )";
  //--------------------------- MAP STATEMENTS ------------------------
  enum MAP_COLUMNS {
    MAP_ID,
    MAP_FK_SCENE,
    MAP_FK_ROLE,
    MAP_COLUMN_COUNT
  };

//SELECT* FROM MAP_Scene_Role AS role_map
//    JOIN Roles
//      ON Roles.role_id
//    = role_map.role
//        WHERE role_map.scene
//    = 2;

  constexpr auto create_role_maps_table = R"(
  CREATE TABLE IF NOT EXISTS role_maps (
    map_id INTEGER PRIMARY KEY,
    fk_scene INTEGER,
    fk_role INTEGER
  );
  )";
  constexpr auto drop_all_role_maps = R"( DELETE FROM role_maps; )";
  constexpr auto count_role_maps = R"( SELECT COUNT(map_id) FROM role_maps; )";
  constexpr auto select_all_role_maps = R"( SELECT map_id,fk_scene,fk_role FROM role_maps; )";

  constexpr auto select_role_map_by_id
    = R"( SELECT * FROM role_maps WHERE map_id = :id ; )";
  constexpr auto update_role_map_by_id
    = R"( UPDATE  role_maps
          SET fk_scene = :fk_scene
              , fk_role = :fk_role
          WHERE map_id = :id;
         )";
  constexpr auto delete_role_map_by_id
    = R"( DELETE FROM role_maps WHERE map_id = :id; )";
  constexpr auto delete_role_map_by_fk_scene
    = R"( DELETE FROM role_maps WHERE fk_scene = :fk_scene; )";
  constexpr auto delete_role_map_by_fk_role
    = R"( DELETE FROM role_maps WHERE fk_role = :fk_role; )";
  constexpr auto delete_role_map_by_fk
    = R"( DELETE FROM role_maps WHERE fk_role = :fk_role AND fk_scene = :fk_scene )";
  constexpr auto select_role_map_by_fk_scene
    = R"( SELECT * FROM role_maps WHERE fk_scene = :fk_scene; )";
  constexpr auto select_role_map_by_fk_role
    = R"( SELECT * FROM role_maps WHERE fk_role = :fk_role; )";
  constexpr auto select_role_map_by_fk
    = R"( SELECT * FROM role_maps WHERE fk_role = :fk_role AND fk_scene = :fk_scene )";
  constexpr auto insert_or_update_role_maps
    = R"( INSERT INTO role_maps
          (fk_scene,fk_role)
          VALUES (:fk_scene,:fk_role)
          ;
         )";

  constexpr auto select_scene_roles_by_fk_scene
    = R"( SELECT * FROM role_maps AS role_map 
          JOIN roles
          ON roles.role_id = role_map.role
          WHERE role_map.fk_scene = :id;)";

  constexpr auto select_role_scenes_by_fk_role
    = R"( SELECT * FROM role_maps as role_map
          JOIN scenes
          ON scenes.scene_id = role_map.scene
          WHERE role_map.fk_role = :id)";
  //--------------------------- EVENT MAP STATEMENTS ------------------
  enum EVENT_MAP_COLUMNS {
    EVENT_MAP_ID,
    EVENT_MAP_FK_SCENE,
    EVENT_MAP_FK_EVENT,
    EVENT_MAP_COLUMN_COUNT
  };

  constexpr auto create_event_maps_table = R"(
  CREATE TABLE IF NOT EXISTS event_maps (
    event_map_id INTEGER PRIMARY KEY,
    fk_scene INTEGER,
    fk_event INTEGER
  );
  )";
  constexpr auto drop_all_event_maps = R"( DELETE FROM event_maps; )";
  constexpr auto count_event_maps = R"( SELECT COUNT(event_map_id) FROM event_maps; )";
  constexpr auto select_all_event_maps = R"( SELECT event_map_id,fk_scene,fk_event FROM event_maps; )";

  constexpr auto select_event_map_by_id
    = R"( SELECT * FROM event_maps WHERE event_map_id = :id ; )";
  constexpr auto update_event_map_by_id
    = R"( UPDATE  event_maps
          SET fk_scene = :fk_scene
              , fk_event = :fk_event
          WHERE event_map_id = :id;
         )";
  constexpr auto delete_event_map_by_id
    = R"( DELETE FROM event_maps WHERE event_map_id = :id; )";
  constexpr auto delete_event_map_by_fk_scene
    = R"( DELETE FROM event_maps WHERE fk_scene = :fk_scene; )";
  constexpr auto delete_event_map_by_fk_event
    = R"( DELETE FROM event_maps WHERE fk_event = :fk_event; )";
  constexpr auto delete_event_map_by_fk
    = R"( DELETE FROM event_maps WHERE fk_event = :fk_event AND fk_scene = :fk_scene )";
  constexpr auto select_event_map_by_fk_scene
    = R"( SELECT * FROM event_maps WHERE fk_scene = :fk_scene; )";
  constexpr auto select_event_map_by_fk_event
    = R"( SELECT * FROM event_maps WHERE fk_event = :fk_event; )";
  constexpr auto select_event_map_by_fk
    = R"( SELECT * FROM event_maps WHERE fk_event = :fk_event AND fk_scene = :fk_scene )";
  constexpr auto insert_or_update_event_maps
    = R"( INSERT INTO event_maps
          (fk_scene,fk_event)
          VALUES (:fk_scene,:fk_event)
          ;
         )";

  constexpr auto select_scene_events_by_fk_scene
    = R"( SELECT * FROM event_maps AS event_map 
          JOIN roles
          ON roles.role_id = event_map.role
          WHERE event_map.fk_scene = :id;)";

  constexpr auto select_event_scenes_by_fk_event
    = R"( SELECT * FROM event_maps as event_map
          JOIN scenes
          ON scenes.scene_id = event_map.scene
          WHERE event_map.fk_event = :id)";
  //--------------------------- LOCATION MAP STATEMENTS ------------------
  enum LOCATION_MAP_COLUMNS {
    LOCATION_MAP_ID,
    LOCATION_MAP_FK_SCENE,
    LOCATION_MAP_FK_LOCATION,
    LOCATION_MAP_COLUMN_COUNT
  };

  constexpr auto create_location_maps_table = R"(
  CREATE TABLE IF NOT EXISTS location_maps (
    location_map_id INTEGER PRIMARY KEY,
    fk_scene INTEGER,
    fk_location INTEGER
  );
  )";
  constexpr auto drop_all_location_maps = R"( DELETE FROM location_maps; )";
  constexpr auto count_location_maps = R"( SELECT COUNT(location_map_id) FROM location_maps; )";
  constexpr auto select_all_location_maps = R"( SELECT location_map_id,fk_scene,fk_location FROM location_maps; )";

  constexpr auto select_location_map_by_id
    = R"( SELECT * FROM location_maps WHERE location_map_id = :id ; )";
  constexpr auto update_location_map_by_id
    = R"( UPDATE  location_maps
          SET fk_scene = :fk_scene
              , fk_location = :fk_location
          WHERE location_map_id = :id;
         )";
  constexpr auto delete_location_map_by_id
    = R"( DELETE FROM location_maps WHERE location_map_id = :id; )";
  constexpr auto delete_location_map_by_fk_scene
    = R"( DELETE FROM location_maps WHERE fk_scene = :fk_scene; )";
  constexpr auto delete_location_map_by_fk_location
    = R"( DELETE FROM location_maps WHERE fk_location = :fk_location; )";
  constexpr auto delete_location_map_by_fk
    = R"( DELETE FROM location_maps WHERE fk_location = :fk_location AND fk_scene = :fk_scene )";
  constexpr auto select_location_map_by_fk_scene
    = R"( SELECT * FROM location_maps WHERE fk_scene = :fk_scene; )";
  constexpr auto select_location_map_by_fk_location
    = R"( SELECT * FROM location_maps WHERE fk_location = :fk_location; )";
  constexpr auto select_location_map_by_fk
    = R"( SELECT * FROM location_maps WHERE fk_location = :fk_location AND fk_scene = :fk_scene )";
  constexpr auto insert_or_update_location_maps
    = R"( INSERT INTO location_maps
          (fk_scene,fk_location)
          VALUES (:fk_scene,:fk_location)
          ;
         )";

  constexpr auto select_scene_locations_by_fk_scene
    = R"( SELECT * FROM location_maps AS location_map 
          JOIN roles
          ON roles.role_id = location_map.role
          WHERE location_map.fk_scene = :id;)";

  constexpr auto select_location_scenes_by_fk_location
    = R"( SELECT * FROM location_maps as location_map
          JOIN scenes
          ON scenes.scene_id = location_map.scene
          WHERE location_map.fk_location = :id)";
  //--------------------------- PROP MAP STATEMENTS ------------------
  enum PROP_MAP_COLUMNS {
    PROP_MAP_ID,
    PROP_MAP_FK_SCENE,
    PROP_MAP_FK_PROP,
    PROP_MAP_COLUMN_COUNT
  };

  constexpr auto create_prop_maps_table = R"(
  CREATE TABLE IF NOT EXISTS prop_maps (
    prop_map_id INTEGER PRIMARY KEY,
    fk_scene INTEGER,
    fk_prop INTEGER
  );
  )";
  constexpr auto drop_all_prop_maps = R"( DELETE FROM prop_maps; )";
  constexpr auto count_prop_maps = R"( SELECT COUNT(prop_map_id) FROM prop_maps; )";
  constexpr auto select_all_prop_maps = R"( SELECT prop_map_id,fk_scene,fk_prop FROM prop_maps; )";

  constexpr auto select_prop_map_by_id
    = R"( SELECT * FROM prop_maps WHERE prop_map_id = :id ; )";
  constexpr auto update_prop_map_by_id
    = R"( UPDATE  prop_maps
          SET fk_scene = :fk_scene
              , fk_prop = :fk_prop
          WHERE prop_map_id = :id;
         )";
  constexpr auto delete_prop_map_by_id
    = R"( DELETE FROM prop_maps WHERE prop_map_id = :id; )";
  constexpr auto delete_prop_map_by_fk_scene
    = R"( DELETE FROM prop_maps WHERE fk_scene = :fk_scene; )";
  constexpr auto delete_prop_map_by_fk_prop
    = R"( DELETE FROM prop_maps WHERE fk_prop = :fk_prop; )";
  constexpr auto delete_prop_map_by_fk
    = R"( DELETE FROM prop_maps WHERE fk_prop = :fk_prop AND fk_scene = :fk_scene )";
  constexpr auto select_prop_map_by_fk_scene
    = R"( SELECT * FROM prop_maps WHERE fk_scene = :fk_scene; )";
  constexpr auto select_prop_map_by_fk_prop
    = R"( SELECT * FROM prop_maps WHERE fk_prop = :fk_prop; )";
  constexpr auto select_prop_map_by_fk
    = R"( SELECT * FROM prop_maps WHERE fk_prop = :fk_prop AND fk_scene = :fk_scene )";
  constexpr auto insert_or_update_prop_maps
    = R"( INSERT INTO prop_maps
          (fk_scene,fk_prop)
          VALUES (:fk_scene,:fk_prop)
          ;
         )";

  constexpr auto select_scene_props_by_fk_scene
    = R"( SELECT * FROM prop_maps AS prop_map 
          JOIN roles
          ON roles.role_id = prop_map.role
          WHERE prop_map.fk_scene = :id;)";

  constexpr auto select_prop_scenes_by_fk_prop
    = R"( SELECT * FROM prop_maps as prop_map
          JOIN scenes
          ON scenes.scene_id = prop_map.scene
          WHERE prop_map.fk_prop = :id)";
  //--------------------------- CITATION MAP STATEMENTS ------------------
  enum CITATION_MAP_COLUMNS {
    CITATION_MAP_ID,
    CITATION_MAP_FK_SCENE,
    CITATION_MAP_FK_CITATION,
    CITATION_MAP_COLUMN_COUNT
  };

  constexpr auto create_citation_maps_table = R"(
  CREATE TABLE IF NOT EXISTS citation_maps (
    citation_map_id INTEGER PRIMARY KEY,
    fk_scene INTEGER,
    fk_citation INTEGER
  );
  )";
  constexpr auto drop_all_citation_maps = R"( DELETE FROM citation_maps; )";
  constexpr auto count_citation_maps = R"( SELECT COUNT(citation_map_id) FROM citation_maps; )";
  constexpr auto select_all_citation_maps = R"( SELECT citation_map_id,fk_scene,fk_citation FROM citation_maps; )";

  constexpr auto select_citation_map_by_id
    = R"( SELECT * FROM citation_maps WHERE citation_map_id = :id ; )";
  constexpr auto update_citation_map_by_id
    = R"( UPDATE  citation_maps
          SET fk_scene = :fk_scene
              , fk_citation = :fk_citation
          WHERE citation_map_id = :id;
         )";
  constexpr auto delete_citation_map_by_id
    = R"( DELETE FROM citation_maps WHERE citation_map_id = :id; )";
  constexpr auto delete_citation_map_by_fk_scene
    = R"( DELETE FROM citation_maps WHERE fk_scene = :fk_scene; )";
  constexpr auto delete_citation_map_by_fk_citation
    = R"( DELETE FROM citation_maps WHERE fk_citation = :fk_citation; )";
  constexpr auto delete_citation_map_by_fk
    = R"( DELETE FROM citation_maps WHERE fk_citation = :fk_citation AND fk_scene = :fk_scene )";
  constexpr auto select_citation_map_by_fk_scene
    = R"( SELECT * FROM citation_maps WHERE fk_scene = :fk_scene; )";
  constexpr auto select_citation_map_by_fk_citation
    = R"( SELECT * FROM citation_maps WHERE fk_citation = :fk_citation; )";
  constexpr auto select_citation_map_by_fk
    = R"( SELECT * FROM citation_maps WHERE fk_citation = :fk_citation AND fk_scene = :fk_scene )";
  constexpr auto insert_or_update_citation_maps
    = R"( INSERT INTO citation_maps
          (fk_scene,fk_citation)
          VALUES (:fk_scene,:fk_citation)
          ;
         )";

  constexpr auto select_scene_citations_by_fk_scene
    = R"( SELECT * FROM citation_maps AS citation_map 
          JOIN roles
          ON roles.role_id = citation_map.role
          WHERE citation_map.fk_scene = :id;)";

  constexpr auto select_citation_scenes_by_fk_citation
    = R"( SELECT * FROM citation_maps as citation_map
          JOIN scenes
          ON scenes.scene_id = citation_map.scene
          WHERE citation_map.fk_citation = :id)";
  //--------------------------- EQUIPMENT MAP STATEMENTS ------------------
  enum EQUIPMENT_MAP_COLUMNS {
    EQUIPMENT_MAP_ID,
    EQUIPMENT_MAP_FK_SCENE,
    EQUIPMENT_MAP_FK_EQUIPMENT,
    EQUIPMENT_MAP_COLUMN_COUNT
  };

  constexpr auto create_equipment_maps_table = R"(
  CREATE TABLE IF NOT EXISTS equipment_maps (
    equipment_map_id INTEGER PRIMARY KEY,
    fk_scene INTEGER,
    fk_equipment INTEGER
  );
  )";
  constexpr auto drop_all_equipment_maps = R"( DELETE FROM equipment_maps; )";
  constexpr auto count_equipment_maps = R"( SELECT COUNT(equipment_map_id) FROM equipment_maps; )";
  constexpr auto select_all_equipment_maps = R"( SELECT equipment_map_id,fk_scene,fk_equipment FROM equipment_maps; )";

  constexpr auto select_equipment_map_by_id
    = R"( SELECT * FROM equipment_maps WHERE equipment_map_id = :id ; )";
  constexpr auto update_equipment_map_by_id
    = R"( UPDATE  equipment_maps
          SET fk_scene = :fk_scene
              , fk_equipment = :fk_equipment
          WHERE equipment_map_id = :id;
         )";
  constexpr auto delete_equipment_map_by_id
    = R"( DELETE FROM equipment_maps WHERE equipment_map_id = :id; )";
  constexpr auto delete_equipment_map_by_fk_scene
    = R"( DELETE FROM equipment_maps WHERE fk_scene = :fk_scene; )";
  constexpr auto delete_equipment_map_by_fk_equipment
    = R"( DELETE FROM equipment_maps WHERE fk_equipment = :fk_equipment; )";
  constexpr auto delete_equipment_map_by_fk
    = R"( DELETE FROM equipment_maps WHERE fk_equipment = :fk_equipment AND fk_scene = :fk_scene )";
  constexpr auto select_equipment_map_by_fk_scene
    = R"( SELECT * FROM equipment_maps WHERE fk_scene = :fk_scene; )";
  constexpr auto select_equipment_map_by_fk_equipment
    = R"( SELECT * FROM equipment_maps WHERE fk_equipment = :fk_equipment; )";
  constexpr auto select_equipment_map_by_fk
    = R"( SELECT * FROM equipment_maps WHERE fk_equipment = :fk_equipment AND fk_scene = :fk_scene )";
  constexpr auto insert_or_update_equipment_maps
    = R"( INSERT INTO equipment_maps
          (fk_scene,fk_equipment)
          VALUES (:fk_scene,:fk_equipment)
          ;
         )";

  constexpr auto select_scene_equipments_by_fk_scene
    = R"( SELECT * FROM equipment_maps AS equipment_map 
          JOIN roles
          ON roles.role_id = equipment_map.role
          WHERE equipment_map.fk_scene = :id;)";

  constexpr auto select_equipment_scenes_by_fk_equipment
    = R"( SELECT * FROM equipment_maps as equipment_map
          JOIN scenes
          ON scenes.scene_id = equipment_map.scene
          WHERE equipment_map.fk_equipment = :id)";
  //--------------------------- RESTRICTION MAP STATEMENTS ------------------
  enum RESTRICTION_MAP_COLUMNS {
    RESTRICTION_MAP_ID,
    RESTRICTION_MAP_FK_SCENE,
    RESTRICTION_MAP_FK_RESTRICTION,
    RESTRICTION_MAP_COLUMN_COUNT
  };

  constexpr auto create_restriction_maps_table = R"(
  CREATE TABLE IF NOT EXISTS restriction_maps (
    restriction_map_id INTEGER PRIMARY KEY,
    fk_scene INTEGER,
    fk_restriction INTEGER
  );
  )";
  constexpr auto drop_all_restriction_maps = R"( DELETE FROM restriction_maps; )";
  constexpr auto count_restriction_maps = R"( SELECT COUNT(restriction_map_id) FROM restriction_maps; )";
  constexpr auto select_all_restriction_maps = R"( SELECT restriction_map_id,fk_scene,fk_restriction FROM restriction_maps; )";

  constexpr auto select_restriction_map_by_id
    = R"( SELECT * FROM restriction_maps WHERE restriction_map_id = :id ; )";
  constexpr auto update_restriction_map_by_id
    = R"( UPDATE  restriction_maps
          SET fk_scene = :fk_scene
              , fk_restriction = :fk_restriction
          WHERE restriction_map_id = :id;
         )";
  constexpr auto delete_restriction_map_by_id
    = R"( DELETE FROM restriction_maps WHERE restriction_map_id = :id; )";
  constexpr auto delete_restriction_map_by_fk_scene
    = R"( DELETE FROM restriction_maps WHERE fk_scene = :fk_scene; )";
  constexpr auto delete_restriction_map_by_fk_restriction
    = R"( DELETE FROM restriction_maps WHERE fk_restriction = :fk_restriction; )";
  constexpr auto delete_restriction_map_by_fk
    = R"( DELETE FROM restriction_maps WHERE fk_restriction = :fk_restriction AND fk_scene = :fk_scene )";
  constexpr auto select_restriction_map_by_fk_scene
    = R"( SELECT * FROM restriction_maps WHERE fk_scene = :fk_scene; )";
  constexpr auto select_restriction_map_by_fk_restriction
    = R"( SELECT * FROM restriction_maps WHERE fk_restriction = :fk_restriction; )";
  constexpr auto select_restriction_map_by_fk
    = R"( SELECT * FROM restriction_maps WHERE fk_restriction = :fk_restriction AND fk_scene = :fk_scene )";
  constexpr auto insert_or_update_restriction_maps
    = R"( INSERT INTO restriction_maps
          (fk_scene,fk_restriction)
          VALUES (:fk_scene,:fk_restriction)
          ;
         )";

  constexpr auto select_scene_restrictions_by_fk_scene
    = R"( SELECT * FROM restriction_maps AS restriction_map 
          JOIN roles
          ON roles.role_id = restriction_map.role
          WHERE restriction_map.fk_scene = :id;)";

  constexpr auto select_restriction_scenes_by_fk_restriction
    = R"( SELECT * FROM restriction_maps as restriction_map
          JOIN scenes
          ON scenes.scene_id = restriction_map.scene
          WHERE restriction_map.fk_restriction = :id)";
  //---------------------- OBJECTIVE STATMENTS ------------------------
  enum OBJECTIVE_COLUMNS {
    OBJECTIVE_ID,
    OBJECTIVE_NAME,
    OBJECTIVE_DESCRIPTION,
    OBJECTIVE_CITATIONS,
    OBJECTIVE_COLUMN_COUNT
  };

  constexpr auto create_objectives_table = R"(
  CREATE TABLE IF NOT EXISTS objectives (
    objective_id INTEGER PRIMARY KEY,
    name Varchar(64) NOT NULL UNIQUE,
    description TEXT,
    citations TEXT
  );
  )";
  constexpr auto drop_all_objectives = R"( DELETE FROM objectives; )";
  constexpr auto count_objectives = R"( SELECT COUNT(objective_id) FROM objectives; )";
  constexpr auto select_all_objectives = R"( SELECT objective_id,name,description,citations FROM objectives ORDER BY name; )";

  constexpr auto select_objective_by_id
    = R"( SELECT * FROM objectives WHERE objective_id = :id ; )";
  constexpr auto select_objective_by_name
    = R"( SELECT * FROM objectives WHERE name = :name ORDER BY name; )";

  constexpr auto update_objective_by_id
    = R"( UPDATE  objectives 
          SET name = :name
              , description = :description
              , citations = :citations
          WHERE objective_id = :id;
         )";
  constexpr auto insert_or_update_objective
    = R"( INSERT INTO objectives 
          (name,description,citations)
          VALUES (:name, :description, :citations)
          ON CONFLICT (name)
          DO UPDATE SET name = excluded.name
                       , description = excluded.description
                       , citations = excluded.citations
         ;
         )";

  constexpr auto delete_objective_by_id
    = R"( DELETE FROM objectives WHERE objective_id = :id; )";
  constexpr auto delete_objective_by_name
    = R"( DELETE FROM objectives WHERE name = :name; )";
  //---------------------- PROP STATEMENTS ------------------------
  enum PROP_COLUMNS {
    PROP_ID,
    PROP_EQUIPMENT,
    PROP_COLUMN_COUNT
  };

  constexpr auto create_props_table = R"(
  CREATE TABLE IF NOT EXISTS props (
    prop_id INTEGER PRIMARY KEY,
    equipment TEXT
  );
  )";
  constexpr auto drop_all_props = R"( DELETE FROM props; )";
  constexpr auto count_props = R"( SELECT COUNT(prop_id) FROM props; )";
  constexpr auto count_props_in_scene = R"( SELECT COUNT(prop_map_id) FROM prop_maps WHERE :id = fk_scene ; )";
  constexpr auto select_all_props = R"( SELECT prop_id,equipment FROM props ORDER BY equipment; )";

  constexpr auto select_prop_by_id
    = R"( SELECT * FROM props WHERE prop_id = :id ; )";
  constexpr auto update_prop_by_id
    = R"( UPDATE  props
          SET equipment = :equipment
          WHERE prop_id = :id;
         )";
  constexpr auto delete_prop_by_id
    = R"( DELETE FROM props WHERE prop_id = :id; )";

  constexpr auto insert_or_update_props
    = R"( INSERT INTO props 
          (equipment)
          VALUES (:equipment)
         )";
  
  //---------------------- PROPERTY STATMENTS ------------------------
  enum PROPERTY_COLUMNS {
    PROPERTY_ID,
    PROPERTY_NAME,
    PROPERTY_VALUE,
    PROPERTY_COLUMN_COUNT
  };

  constexpr auto create_properties_table = R"(
  CREATE TABLE IF NOT EXISTS properties(
    property_id INTEGER PRIMARY KEY,
    name Varchar(64) NOT NULL UNIQUE,
    value TEXT
  );
  )";
  constexpr auto drop_all_properties = R"( DELETE FROM properties; )";
  constexpr auto count_properties = R"( SELECT COUNT(property_id) FROM properties; )";
  constexpr auto select_all_properties = R"( SELECT property_id,name,value FROM properties ORDER BY property_id; )";

  constexpr auto select_property_by_id
    = R"( SELECT * FROM properties WHERE property_id = :id ; )";
  constexpr auto update_property_by_id
    = R"( UPDATE  properties
          SET name = :name
              , value = :value
          WHERE property_id = :id;
         )";
  constexpr auto select_property_by_name
    = R"( SELECT * FROM properties WHERE name = :name ORDER BY name; )";

  constexpr auto insert_or_update_properties
    = R"( INSERT  INTO properties 
          (name,value)
          VALUES (:name, :value)
          ON CONFLICT (name)
          DO UPDATE SET value = excluded.value;
         )";

  constexpr auto delete_property_by_id
    = R"( DELETE FROM properties WHERE property_id = :id; )";

  
  constexpr auto delete_property_by_name
    = R"( DELETE FROM properties WHERE name = :name; )";
  
  //---------------------- CITATION STATEMENTS ------------------------
  enum CITATION_COLUMNS {
    CITATION_ID,
    CITATION_KEY,
    CITATION_TITLE,
    CITATION_AUTHORS,
    CITATION_YEAR,
    CITATION_PUBLISHER,
    CITATION_COLUMN_COUNT
  };

  constexpr auto create_citations_table = R"(
  CREATE TABLE IF NOT EXISTS citations (
    citation_id INTEGER PRIMARY KEY,
    key Varchar(64) NOT NULL UNIQUE,
    title TEXT,
    authors TEXT,
    year TEXT,
    publisher TEXT
  );
  )";
  constexpr auto drop_all_citations = R"( DELETE FROM citations; )";
  constexpr auto count_citations = R"( SELECT COUNT(citation_id) FROM citations; )";
  constexpr auto count_citations_in_scene = R"( SELECT COUNT(citation_map_id) FROM citation_maps WHERE :id = fk_scene ; )";
  constexpr auto select_all_citations = R"( SELECT citation_id,key,title,authors,year,publisher FROM citations ORDER BY title; )";

  constexpr auto select_citation_by_id
    = R"( SELECT * FROM citations WHERE citation_id = :id ; )";
  constexpr auto select_citation_by_key
    = R"( SELECT * FROM citations WHERE key = :key ORDER BY title; )";
  constexpr auto select_citation_by_title
    = R"( SELECT * FROM citations WHERE title = :title ORDER BY title; )";

  constexpr auto delete_citation_by_id
    = R"( DELETE FROM citations WHERE citation_id = :id; )";
  constexpr auto delete_citation_by_key
    = R"( DELETE FROM citations WHERE key = :key; )";
  constexpr auto delete_citation_by_title
    = R"( DELETE FROM citations WHERE title = :title; )";

  constexpr auto update_citation_by_id
    = R"( UPDATE  citations
          SET key       = :key
              , title   = :title
              , authors = :authors
              , year    = :year
              , publisher = :publisher
          WHERE citation_id = :id;
         )";
  constexpr auto update_citation_by_key
    = R"( UPDATE  citations
          SET key = :key
              , title = :title
              , authors = :authors
              , year = :year
              , publisher = :publisher
          WHERE key = :key;
         )";

  constexpr auto insert_or_update_citations
    = R"( INSERT INTO citations
          (key, title, authors, year, publisher)
          VALUES (:key, :title, :authors, :year, :publisher)
          ON CONFLICT (key)
          DO UPDATE SET key = excluded.key
                       , title = excluded.title
                       , authors = excluded.authors
                       , year = excluded.year
                       , publisher = excluded.publisher
         ;
         )";


  //---------------------- RESTRICTION STATMENTS ------------------------
  enum RESTRICTION_COLUMNS {
    RESTRICTION_ID,
    RESTRICTION_NAME,
    RESTRICTION_VALUE,
    RESTRICTION_COLUMN_COUNT
  };

  constexpr auto create_restrictions_table = R"(
  CREATE TABLE IF NOT EXISTS restrictions (
    restriction_id INTEGER PRIMARY KEY,
    name Varchar(64) NOT NULL UNIQUE,
    value Varchar(255)
  );
  )";
  constexpr auto drop_all_restrictions = R"( DELETE FROM restrictions; )";
  constexpr auto count_restrictions = R"( SELECT COUNT(restriction_id) FROM restrictions; )";
  constexpr auto count_restrictions_in_scene = R"( SELECT COUNT(restriction_map_id) FROM restriction_maps WHERE :id = fk_scene ; )";
  constexpr auto select_all_restrictions = R"( SELECT restriction_id,name,value FROM restrictions ORDER BY name; )";

  constexpr auto select_restriction_by_id
    = R"( SELECT * FROM restrictions WHERE restriction_id = :id ; )";
  constexpr auto update_restriction_by_id
    = R"( UPDATE  restrictions
          SET name = :name
              , value = :value
          WHERE restriction_id = :id;
         )";
  constexpr auto select_restriction_by_name
    = R"( SELECT * FROM restrictions WHERE name = :name ORDER BY name; )";

  constexpr auto insert_or_update_restrictions
    = R"( INSERT INTO restrictions 
          (name,value)
          VALUES (:name, :value)
          ON CONFLICT (name)
          DO UPDATE SET value = excluded.value;
         )";
  constexpr auto delete_restriction_by_id
    = R"( DELETE FROM restrictions WHERE restriction_id = :id; )";
  constexpr auto delete_restriction_by_name
    = R"( DELETE FROM restrictions WHERE name = :name; )";
  
  //---------------------- ROLE STATMENTS ------------------------
  enum ROLE_COLUMNS {
    ROLE_ID,
    ROLE_NAME,
    ROLE_DESCRIPTION,
    ROLE_COLUMN_COUNT
  };

  constexpr auto create_roles_table = R"(
  CREATE TABLE IF NOT EXISTS roles (
    role_id INTEGER PRIMARY KEY,
    name Varchar(64) NOT NULL UNIQUE,
    description TEXT
  );
  )";
  constexpr auto drop_all_roles = R"( DELETE FROM roles; )";
  constexpr auto count_roles = R"( SELECT COUNT(role_id) FROM roles; )";
  constexpr auto count_roles_in_scene = R"( SELECT COUNT(map_id) FROM role_maps WHERE :id = fk_scene ; )";
  constexpr auto select_all_roles = R"( SELECT role_id,name,description FROM roles ORDER BY name; )";

  constexpr auto select_role_by_id
    = R"( SELECT * FROM roles WHERE role_id = :id ; )";
  constexpr auto update_role_by_id
    = R"( UPDATE  roles
          SET description = :description 
              , name = :name
          WHERE role_id = :id;
         )";
  constexpr auto select_role_by_name
    = R"( SELECT * FROM roles WHERE name = :name ORDER BY name; )";
  constexpr auto delete_role_by_id
    = R"( DELETE FROM roles WHERE role_id = :id; )";
  
  constexpr auto insert_or_update_roles
    = R"( INSERT INTO roles 
          (name,description)
          VALUES (:name, :description)
          ON CONFLICT (name)
          DO UPDATE SET name = excluded.name
                       , description = excluded.description
         ;
         )";

  //---------------------- SCENE STATEMENTS ------------------------
  enum SCENE_COLUMNS {
    SCENE_ID,
    SCENE_NAME,
    //SCENE_LOCATION,
    //SCENE_ACTOR,
    //SCENE_EQUIPMENT,
    //SCENE_DESCRIPTION,
    SCENE_COLUMN_COUNT
  };

  constexpr auto create_scenes_table = R"(
  CREATE TABLE IF NOT EXISTS scenes (
    scene_id INTEGER PRIMARY KEY,
    name Varchar(64) NOT NULL UNIQUE
  );
  )";
  constexpr auto drop_all_scenes = R"( DELETE FROM scenes; )";
  constexpr auto count_scenes = R"( SELECT COUNT(scene_id) FROM scenes; )";
  constexpr auto select_all_scenes = R"( SELECT scene_id,name FROM scenes ORDER BY name; )";

  constexpr auto select_scene_by_id
    = R"( SELECT * FROM scenes WHERE scene_id = :id ; )";
  constexpr auto select_scene_by_name
    = R"( SELECT * FROM scenes WHERE name = :name ORDER BY name; )";
  constexpr auto update_scene_by_id
    = R"( UPDATE  scenes
            SET name = :name
          WHERE scene_id = :id;
         )";
  constexpr auto delete_scene_by_id
    = R"( DELETE FROM scenes WHERE scene_id = :id; )";
  constexpr auto insert_or_update_scenes
    = R"( INSERT INTO scenes
          (name)
          VALUES (:name)
          ON CONFLICT (name)
          DO UPDATE SET name = excluded.name
          ;          
          )";
  //---------------------- TREATMENT STATEMENTS ------------------------
  enum TREATMENT_COLUMNS {
    TREATMENT_ID,
    TREATMENT_MEDICAL_NAME,
    TREATMENT_COMMON_NAME,
    TREATMENT_DESCRIPTION,
    TREATMENT_EQUIPMENT,
    TREATMENT_CITATIONS,
    TREATMENT_COLUMN_COUNT
  };

  constexpr auto create_treatments_table = R"(
  CREATE TABLE IF NOT EXISTS treatments (
    treatment_id INTEGER PRIMARY KEY,
    medical_name Varchar(64) NOT NULL UNIQUE,
    common_name Varchar(64) NOT NULL UNIQUE,
    description TEXT,
    equipment TEXT,
    citations TEXT
  );
  )";

  constexpr auto drop_all_treatments = R"( DELETE FROM treatments; )";
  constexpr auto count_treatments = R"( SELECT COUNT(treatment_id) FROM treatments; )";
  constexpr auto select_all_treatments = R"( SELECT treatment_id,medical_name,common_name,description,equipment,citations FROM treatments ORDER BY medical_name; )";

  constexpr auto select_treatment_by_medical_name
    = R"( SELECT * FROM treatments WHERE medical_name = :medical_name ORDER BY medical_name; )";
  constexpr auto select_treatment_by_common_name
    = R"( SELECT * FROM treatments WHERE common_name = :common_name ORDER BY common_name; )";

  constexpr auto select_treatment_by_id
    = R"( SELECT * FROM treatments WHERE treatment_id = :id ; )";
  constexpr auto update_treatment_by_id
    = R"( UPDATE  treatments
          SET medical_name = :medical_name
              , common_name = :common_name
              , description = :description
              , equipment = :equipment
              , citations = :citations
          WHERE treatment_id = :id;
         )";
  constexpr auto delete_treatment_by_id
    = R"( DELETE FROM treatments WHERE treatment_id = :id; )";
  constexpr auto delete_treatment_by_medical_name
    = R"( DELETE FROM treatments WHERE medical_name = :medical_name; )";
  constexpr auto delete_treatment_by_common_name
    = R"( DELETE FROM treatments WHERE common_name = :common_name; )";
  constexpr auto insert_or_update_treatments
    = R"( INSERT INTO treatments
          (medical_name,common_name,description,equipment,citations)
          VALUES (:medical_name, :common_name, :description, :equipment, :citations)
          ON CONFLICT (medical_name)
          DO UPDATE SET medical_name = excluded.medical_name
                        , common_name = excluded.common_name
                        , description = excluded.description
                        , equipment = excluded.equipment
                        , citations = excluded.citations
          ;          
          )";
  
  //-------------------------------------------------------------------


}
}

#endif
