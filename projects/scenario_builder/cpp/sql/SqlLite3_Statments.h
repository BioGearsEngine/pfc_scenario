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

  constexpr auto list_tables = "SELECT * FROM sqlite_master WHERE type='table';";
  constexpr auto drop_table = "DROP TABLE IF EXISTS :table;";

  //---------------------- ASSESSMENTS STATMENTS ------------------------
  enum ASSESSMENT_COLUMNS {
    ASSESSMENT_ID,
    ASSESSMENT_UUID,
    ASSESSMENT_NAME,
    ASSESSMENT_DESCRIPTION,
    ASSESSMENT_TYPE,
    ASSESSMENT_AVAILABLE_POINTS,
    ASSESSMENT_CRITERIA,
    ASSESSMENT_COLUMN_COUNT
  };

  constexpr auto create_assessments_table = R"(
    CREATE TABLE IF NOT EXISTS "assessments" (
	    "assessment_id"	INTEGER,
	    "uuid"	TEXT,
	    "name"	Varchar(64) NOT NULL UNIQUE,
	    "description"	TEXT,
	    "type"	TEXT,
	    "available_points"	INTEGER,
	    "criteria"	TEXT,
	    PRIMARY KEY("assessment_id"),
	    UNIQUE("uuid")
    );
  )";

  constexpr auto drop_all_assessments = R"( DELETE FROM assessments; )";
  constexpr auto count_assessments = R"( SELECT COUNT(assessment_id) FROM assessments; )";
  constexpr auto select_all_assessments = R"( SELECT * FROM assessments ORDER BY name; )";

  constexpr auto select_assessment_by_id
    = R"( SELECT * FROM assessments WHERE assessment_id = :id; )";
  constexpr auto update_assessment_by_id
    = R"( UPDATE  assessments 
          SET
                uuid = :uuid
              , name = :name
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
          (name,uuid,description,type,available_points,criteria)
          VALUES (:name, :uuid, :description, :type, :available_points, :criteria)
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
    AUTHOR_UUID,
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
    CREATE TABLE IF NOT EXISTS "authors" (
	    "author_id"	INTEGER,
	    "uuid"	TEXT,
	    "name_first"	Varchar(25),
	    "name_last"	Varchar(25) NOT NULL,
	    "email"	VarChar(45) NOT NULL UNIQUE,
	    "zipcode"	Varchar(10),
	    "state"	Varchar(64),
	    "country"	Varchar(64),
	    "phone"	Varchar(16),
	    "organization"	Varchar(128),
	    PRIMARY KEY("author_id"),
	    UNIQUE("uuid")
    );
  )";
  constexpr auto drop_all_authors = R"( DELETE FROM authors; )";
  constexpr auto count_authors = R"( SELECT COUNT(author_id) FROM authors; )";
  constexpr auto select_all_authors = R"( SELECT * FROM authors ORDER BY name_last; )";

  constexpr auto select_author_by_id
    = R"( SELECT * FROM authors WHERE author_id = :id ; )";
  constexpr auto update_author_by_id
    = R"( UPDATE  authors 
          SET
                uuid = :uuid
              , name_first = :name_first
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
                 (author_id,  uuid,  name_first,  name_last,  email,  zipcode,  state,  country,  phone,  organization)
          VALUES (1, :uuid, :name_first, :name_last, :email, :zipcode, :state, :country, :phone, :organization)
        )";

  constexpr auto insert_or_update_authors
    = R"( INSERT  INTO authors
                 (  name_first, uuid,  name_last,  email,  zipcode,  state,  country,  phone,  organization)
          VALUES ( :name_first, :uuid, :name_last, :email, :zipcode, :state, :country, :phone, :organization)
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
    EQUIPMENT_UUID,
    EQUIPMENT_TYPE,
    EQUIPMENT_NAME,
    EQUIPMENT_SUMMARY,
    EQUIPMENT_DESCRIPTION,
    EQUIPMENT_CITATIONS,
    EQUIPMENT_IMAGE,
    EQUIPMENT_PROPERTIES,
    EQUIPMENT_COLUMN_COUNT
  };
  // I know that the plural of equipment is 'equipment', but we've made it equipments to be less ambiguous
  constexpr auto create_equipment_table = R"(
    CREATE TABLE IF NOT EXISTS "equipments" (
	    "equipment_id"	INTEGER,
	    "uuid"	TEXT,
	    "type"	INTEGER NOT NULL DEFAULT 1,
	    "name"	Varchar(64) NOT NULL UNIQUE,
      "summary" Varchar(64) NOT NULL,
	    "description"	Varchar(64) NOT NULL,
	    "citations"	TEXT,
	    "image"	TEXT,
	    "properties"	TEXT,
	    PRIMARY KEY("equipment_id"),
	    UNIQUE("uuid")
    );
  )";
  constexpr auto drop_all_equipment = R"( DELETE FROM equipment; )";
  constexpr auto count_equipments = R"( SELECT COUNT(equipment_id) FROM equipments; )";
  constexpr auto count_equipments_in_scene = R"( SELECT COUNT(equipment_map_id) FROM equipment_map WHERE :id = fk_scene ; )";
  constexpr auto select_all_equipments = R"( SELECT *  FROM equipments ORDER BY name; )";

  constexpr auto select_equipment_by_id
    = R"( SELECT * FROM equipments WHERE equipment_id = :id ; )";
  constexpr auto update_equipment_by_id
    = R"( UPDATE  equipments
          SET
                uuid = :uuid
              , type = :type
              , name = :name
              , summary = :summary
              , description = :description
              , citations = :citations
              , image = :image
              , properties = :properties
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
          (name,uuid, type, summary, description,citations,image,properties)
          VALUES (:name, :uuid, :type, :summary, :description, :citations, :image, :properties)
          ON CONFLICT (name)
          DO UPDATE SET name = excluded.name
                        , type = excluded.type
                        , summary = excluded.summary
                        , description = excluded.description
                        , citations = excluded.citations
                        , image = excluded.image
                        , properties = excluded.properties
          ;         
          )";
  //---------------------- EVENT STATMENTS ------------------------
  enum EVENT_COLUMNS {
    EVENT_ID,
    EVENT_UUID,
    EVENT_NAME,
    EVENT_DESCRIPTION,
    EVENT_CATEGORY,
    EVENT_FIDELITY,
    EVENT_ACTOR_1,
    EVENT_ACTOR_2,
    EVENT_EQUIPMENT,
    EVENT_DETAILS,
    EVENT_COLUMN_COUNT
  };

  constexpr auto create_events_table = R"(
    CREATE TABLE IF NOT EXISTS "events" (
	    "event_id"	INTEGER,
	    "uuid"	TEXT,
	    "name"	Varchar(64) NOT NULL UNIQUE,
	    "description"	TEXT,
	    "category"	TEXT,
	    "fidelity"	TEXT,
	    "actor_1"	Varchar(64) NOT NULL,
	    "actor_2"	Varchar(64),
	    "equipment"	TEXT,
	    "details"	INTEGER,
	    PRIMARY KEY("event_id"),
	    UNIQUE("uuid")
    );
  )";
  constexpr auto drop_all_events = R"( DELETE FROM events; )";
  constexpr auto count_events = R"( SELECT COUNT(event_id) FROM events; )";
  constexpr auto count_events_in_scene = R"( SELECT COUNT(event_map_id) FROM event_maps WHERE :id = fk_scene ; )";
  constexpr auto select_all_events = R"( SELECT *  FROM events ORDER BY name; )";

  constexpr auto select_event_by_id
    = R"( SELECT * FROM events WHERE event_id = :id ; )";
  constexpr auto select_event_by_name
    = R"( SELECT * FROM events WHERE name = :name ORDER BY name; )";
  constexpr auto update_event_by_id
    = R"( UPDATE  events
            SET
                  uuid = :uuid
                , name = :name
                , description = :description
                , category = :category
                , fidelity = :fidelity
                , actor_1 = :actor_1
                , actor_2 = :actor_2
                , equipment = :equipment
                , details = :details
          WHERE event_id = :id;
         )";
  constexpr auto delete_event_by_id
    = R"( DELETE FROM events WHERE event_id = :id; )";
  constexpr auto insert_or_update_events
    = R"( INSERT INTO events
          (name, uuid, description, category, fidelity, actor_1, actor_2, equipment, details)
          VALUES (:name, :uuid, :description, :category, :fidelity, :actor_1, :actor_2, :equipment, :details)
          ON CONFLICT (name)
          DO UPDATE SET  name = excluded.name
                        , description = excluded.description
                        , category = excluded.category
                        , fidelity = excluded.fidelity
                        , actor_1 = excluded.actor_1
                        , actor_2 = excluded.actor_2
                        , equipment = excluded.equipment
                        , details = excluded.details
          ;          
          )";
  //---------------------- INJURY STATEMENTS ------------------------
  enum INJURY_COLUMNS {
    INJURY_ID,
    INJURY_UUID,
    INJURY_MEDICAL_NAME,
    INJURY_COMMON_NAME,
    INJURY_DESCRIPTION,
    INJURY_CITATIONS,
    INJURY_LOWER_BOUND,
    INJURY_UPPER_BOUND,
    INJURY_COLUMN_COUNT
  };

  constexpr auto create_traumas_table = R"(
    CREATE TABLE IF NOT EXISTS "traumas" (
	    "trauma_id"	INTEGER,
	    "uuid"	TEXT,
	    "medical_name"	Varchar(64) NOT NULL UNIQUE,
	    "common_name"	Varchar(64) NOT NULL UNIQUE,
	    "description"	TEXT,
	    "citations"	TEXT,
	    "lower_bound"	REAL DEFAULT 0.0,
	    "upper_bound"	REAL DEFAULT 1.0,
	    PRIMARY KEY("trauma_id"),
	    UNIQUE("uuid")
    );
  )";

  constexpr auto drop_all_traumas = R"( DELETE FROM traumas; )";
  constexpr auto count_traumas = R"( SELECT COUNT(trauma_id) FROM traumas; )";
  constexpr auto select_all_traumas = R"( SELECT * FROM traumas ORDER BY medical_name; )";

  constexpr auto select_trauma_by_id
    = R"( SELECT * FROM traumas WHERE trauma_id = :id ; )";
  constexpr auto update_trauma_by_id
    = R"( UPDATE  traumas
          SET
                uuid = :uuid
              , medical_name = :medical_name
              , common_name = :common_name
              , description = :description
              , citations = :citations
              , lower_bound  = :lower_bound
              , upper_bound  = :upper_bound
          WHERE trauma_id = :id;
         )";
  constexpr auto delete_trauma_by_id
    = R"( DELETE FROM traumas WHERE trauma_id = :id; )";
  constexpr auto delete_trauma_by_medical_name
    = R"( DELETE FROM traumas WHERE medical_name = :medical_name; )";
  constexpr auto delete_trauma_by_common_name
    = R"( DELETE FROM traumas WHERE common_name = :common_name; )";
  constexpr auto select_trauma_by_medical_name
    = R"( SELECT * FROM traumas WHERE medical_name = :medical_name ORDER BY medical_name; )";
  constexpr auto select_trauma_by_common_name
    = R"( SELECT * FROM traumas WHERE common_name = :common_name ORDER BY common_name; )";
  constexpr auto insert_or_update_traumas
    = R"( INSERT INTO traumas
          (medical_name, uuid, common_name, description, citations, lower_bound, upper_bound)
          VALUES (:medical_name, :uuid, :common_name, :description, :citations, :lower_bound, :upper_bound)
          ON CONFLICT (medical_name)
          DO UPDATE SET  common_name = excluded.common_name
                        , description = excluded.description
                        , citations = excluded.citations
                        , lower_bound= excluded.lower_bound
                        , upper_bound= excluded.upper_bound
          ;          
          )";
  
  //---------------------- Injury Set STATMENTS ------------------------
  enum INJURY_SET_COLUMNS {
    INJURY_SET_ID,
    INJURY_SET_UUID,
    INJURY_SET_NAME,
    INJURY_SET_DESCRIPTION,
    INJURY_SET_TRAUMAS,
    INJURY_SET_LOCATIONS,
    INJURY_SET_SEVERITIES,
    INJURY_SET_COLUMN_COUNT
  };
  //TODO: ADD SUPPORT FOR LOCATION AND SEVERITIES (PRIMITIVE List in the same order as the traumas should do it)

  constexpr auto create_trauma_profiles_table = R"(
    CREATE TABLE IF NOT EXISTS "trauma_profiles" (
	    "trauma_profile_id"	INTEGER,
	    "uuid"	TEXT,
	    "name"	Varchar(64) NOT NULL UNIQUE,
	    "description"	TEXT,
	    "traumas"	TEXT,
	    "locations"	TEXT,
	    "severities"	TEXT,
	    PRIMARY KEY("trauma_profile_id"),
	    UNIQUE("uuid")
    );
  )";

  constexpr auto drop_all_trauma_profile = R"( DELETE FROM trauma_profiles; )";
  constexpr auto count_trauma_profiles = R"( SELECT COUNT(trauma_profile_id) FROM trauma_profiles; )";
  constexpr auto select_all_trauma_profiles = R"( SELECT * FROM trauma_profiles ORDER BY name; )";

  constexpr auto select_trauma_profile_by_id
    = R"( SELECT * FROM trauma_profiles WHERE trauma_profile_id = :id ; )";
  constexpr auto update_trauma_profile_by_id
    = R"( UPDATE  trauma_profiles
          SET
                uuid = :uuid
              , name = :name
              , description = :description
              , traumas    = :traumas
              , locations   = :locations
              , severities  = :severities
          WHERE trauma_profile_id = :id;
         )";
  constexpr auto delete_trauma_profile_by_id
    = R"( DELETE FROM trauma_profiles WHERE trauma_profile_id = :id; )";
  constexpr auto delete_trauma_profile_by_name
    = R"( DELETE FROM trauma_profiles WHERE name = :name; )";
  constexpr auto select_trauma_profile_by_name
    = R"( SELECT * FROM trauma_profiles WHERE name = :name ORDER BY name; )";
  constexpr auto insert_or_update_trauma_profiles
    = R"( INSERT INTO trauma_profiles
          (name, uuid, description, traumas, locations, severities)
          VALUES (:name, :uuid, :description, :traumas, :locations, :severities)
          ON CONFLICT (name)
          DO UPDATE SET  name = excluded.name
                        , description = excluded.description
                        , traumas = excluded.traumas
                        , locations = excluded.locations
                        , severities = excluded.severities
          ;
          )";
  //---------------------- LOCATION STATMENTS ------------------------
  enum LOCATION_COLUMNS {
    LOCATION_ID,
    LOCATION_UUID,
    LOCATION_NAME,
    LOCATION_ENVIRONMENT,
    LOCATION_COLUMN_COUNT
  };

  constexpr auto create_locations_table = R"(
    CREATE TABLE IF NOT EXISTS "locations" (
	    "location_id"	INTEGER,
	    "uuid"	TEXT,
	    "name"	Varchar(64) NOT NULL,
	    "environment"	TEXT,
	    PRIMARY KEY("location_id"),
	    UNIQUE("uuid")
    );
  )";
  constexpr auto drop_all_locations = R"( DELETE FROM locations; )";
  constexpr auto count_locations = R"( SELECT COUNT(location_id) FROM locations; )";
  constexpr auto count_locations_in_scene = R"( SELECT COUNT(location_map_id) FROM location_maps WHERE :id = fk_scene ; )";
  constexpr auto select_all_locations = R"( SELECT * ORDER BY name; )";

  constexpr auto select_location_by_id
    = R"( SELECT * FROM locations WHERE location_id = :id ; )";
  constexpr auto update_location_by_id
    = R"( UPDATE  locations
          SET
                uuid = :uuid
              , name = :name
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
          (uuid, name, environment)
          VALUES (:uuid, :name, :environment)
          ;
         )";
  //--------------------------- MAP STATEMENTS ------------------------
  enum ROLE_MAP_COLUMNS {
    ROLE_MAP_ID,
    ROLE_MAP_FK_SCENE,
    ROLE_MAP_FK_ROLE,
    ROLE_MAP_COLUMN_COUNT
  };



  constexpr auto create_role_maps_table = R"(
  CREATE TABLE IF NOT EXISTS role_maps (
    map_id INTEGER PRIMARY KEY,
    fk_scene INTEGER,
    fk_role INTEGER
  );
  )";
  constexpr auto drop_all_role_maps = R"( DELETE FROM role_maps; )";
  constexpr auto count_role_maps = R"( SELECT COUNT(map_id) FROM role_maps; )";
  constexpr auto select_all_role_maps = R"( SELECT * FROM role_maps; )";

  constexpr auto select_role_map_by_id
    = R"( SELECT * FROM role_maps WHERE map_id = :id ; )";
  constexpr auto update_role_map_by_id
    = R"( UPDATE  role_maps
          SET   fk_scene = :fk_scene
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

  constexpr auto select_roles_in_scene
    = R"( SELECT roles.* FROM role_maps AS role_map 
          JOIN roles
          ON roles.role_id = role_map.fk_role
          WHERE role_map.fk_scene = :scene_id;)";

  constexpr auto select_scenes_with_role
    = R"( SELECT scenes.* FROM role_maps as role_map
          JOIN scenes
          ON scenes.scene_id = role_map.fk_scene
          WHERE role_map.fk_role= :role_id;)";
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
  constexpr auto select_all_event_maps = R"( SELECT * FROM event_maps; )";

  constexpr auto select_event_map_by_id
    = R"( SELECT * FROM event_maps WHERE event_map_id = :id ; )";
  constexpr auto update_event_map_by_id
    = R"( UPDATE  event_maps
          SET   fk_scene = :fk_scene
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

  constexpr auto select_events_in_scene
    = R"( SELECT events.* FROM event_maps AS event_map 
          JOIN events
          ON event_map.fk_event = events.event_id
          WHERE event_map.fk_scene = :scene_id;)";

  constexpr auto select_scenes_containing_event
    = R"( SELECT scenes.* FROM event_maps as event_map
          JOIN scenes
          ON scenes.scene_id = event_map.fk_scene
          WHERE event_map.fk_event = :event_id)";
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
  constexpr auto count_location_maps_in_scene = R"( SELECT COUNT(location_map_id) FROM location_maps WHERE fk_scene = :fk_scene; )";
  constexpr auto select_all_location_maps = R"( SELECT * FROM location_maps; )";

  constexpr auto select_location_map_by_id
    = R"( SELECT * FROM location_maps WHERE location_map_id = :id ; )";
  constexpr auto update_location_map_by_id
    = R"( UPDATE  location_maps
          SET   fk_scene = :fk_scene
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
  constexpr auto select_all_citation_maps = R"( SELECT * FROM citation_maps; )";

  constexpr auto select_citation_map_by_id
    = R"( SELECT * FROM citation_maps WHERE citation_map_id = :id ; )";
  constexpr auto update_citation_map_by_id
    = R"( UPDATE  citation_maps
          SET   fk_scene = :fk_scene
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
    EQUIPMENT_MAP_NAME,
    EQUIPMENT_MAP_VALUES ,
    EQUIPMENT_MAP_NOTES,
    EQUIPMENT_MAP_COLUMN_COUNT
  };

  constexpr auto create_equipment_map_table = R"(
  CREATE TABLE IF NOT EXISTS equipment_map (
    equipment_map_id INTEGER PRIMARY KEY,
    fk_scene INTEGER,
    fk_equipment INTEGER,
    name TEXT,
    property_values TEXT,
    notes TEXT,
	  UNIQUE("fk_scene","fk_equipment","name")
  );
  )";
  constexpr auto drop_all_equipment_map = R"( DELETE FROM equipment_map; )";
  constexpr auto count_equipment_map = R"( SELECT COUNT(equipment_map_id) FROM equipment_map; )";
  constexpr auto select_all_equipment_map = R"( SELECT * FROM equipment_map; )";

  constexpr auto select_equipment_map_by_id
    = R"( SELECT * FROM equipment_map WHERE equipment_map_id = :id ; )";
  constexpr auto update_equipment_map_by_id
    = R"( UPDATE  equipment_map
          SET   fk_scene = :fk_scene
              , fk_equipment = :fk_equipment
              , name = :name
              , property_values = :values
              , notes = :notes
          WHERE equipment_map_id = :id;
         )";
  constexpr auto delete_equipment_map_by_id
    = R"( DELETE FROM equipment_map WHERE equipment_map_id = :id; )";
  constexpr auto delete_all_equipment_from_a_scene
    = R"( DELETE FROM equipment_map WHERE fk_scene = :fk_scene; )";
  constexpr auto delete_this_equipment_across_all_scenes
    = R"( DELETE FROM equipment_map WHERE fk_equipment = :fk_equipment; )";
  constexpr auto delete_an_equipment_from_a_scene
    = R"( DELETE FROM equipment_map WHERE fk_equipment = :fk_equipment AND fk_scene = :fk_scene AND name = :name )";
  constexpr auto delete_all_occurrence_of_equipment_from_a_scene
    = R"( DELETE FROM equipment_map WHERE fk_equipment = :fk_equipment AND fk_scene = :fk_scene )";

  constexpr auto select_all_equipment_in_a_scene
    = R"( SELECT * FROM equipment_map WHERE fk_scene = :fk_scene; )";
  constexpr auto select_this_equipment_across_all_scenes
    = R"( SELECT * FROM equipment_map WHERE fk_equipment = :fk_equipment; )";
  constexpr auto select_equipment_from_scene_with_name
    = R"( SELECT * FROM equipment_map WHERE fk_equipment = :fk_equipment AND fk_scene = :fk_scene AND name = :name )";
  constexpr auto select_all_occurrences_of_this_equipment_in_a_scene
    = R"( SELECT * FROM equipment_map WHERE fk_equipment = :fk_equipment AND fk_scene = :fk_scene )";
  constexpr auto insert_or_update_equipment_map
    = R"( INSERT INTO equipment_map
          (fk_scene,fk_equipment,name,property_values,notes)
          VALUES (:fk_scene,:fk_equipment,:name,:values,:notes)
          ;
         )";

  constexpr auto select_equipment_in_scene
    = R"( SELECT *
          FROM equipment_map AS  map
          JOIN equipments AS eq
          ON eq.equipment_id = map.fk_equipment
          WHERE map.fk_scene = :scene_id;)";

  constexpr auto select_scenes_with_equipment
    = R"( SELECT  scene.* , map.name, map.property_values, map.notes
          FROM equipment_map AS map
          JOIN scenes AS scene
          ON scene.scene_id = map.fk_scene
          WHERE map.fk_equipment = :equipment_id;)";
  
  //---------------------- OBJECTIVE STATEMENTS ------------------------
  enum OBJECTIVE_COLUMNS {
    OBJECTIVE_ID,
    OBJECTIVE_UUID,
    OBJECTIVE_NAME,
    OBJECTIVE_DESCRIPTION,
    OBJECTIVE_CITATIONS,
    OBJECTIVE_COLUMN_COUNT
  };

  constexpr auto create_objectives_table = R"(
    CREATE TABLE IF NOT EXISTS "objectives" (
	    "objective_id"	INTEGER,
	    "uuid"	TEXT,
	    "name"	Varchar(64) NOT NULL UNIQUE,
	    "description"	TEXT,
	    "citations"	TEXT,
	    UNIQUE("uuid"),
	    PRIMARY KEY("objective_id")
    );
  )";
  constexpr auto drop_all_objectives = R"( DELETE FROM objectives; )";
  constexpr auto count_objectives = R"( SELECT COUNT(objective_id) FROM objectives; )";
  constexpr auto select_all_objectives = R"( SELECT * FROM objectives ORDER BY name; )";

  constexpr auto select_objective_by_id
    = R"( SELECT * FROM objectives WHERE objective_id = :id ; )";
  constexpr auto select_objective_by_name
    = R"( SELECT * FROM objectives WHERE name = :name ORDER BY name; )";

  constexpr auto update_objective_by_id
    = R"( UPDATE  objectives 
          SET   uuid = :uuid
              , name = :name
              , description = :description
              , citations = :citations
          WHERE objective_id = :id;
         )";
  constexpr auto insert_or_update_objective
    = R"( INSERT INTO objectives 
          (uuid, name,description,citations)
          VALUES (:uuid, :name, :description, :citations)
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
  //---------------------- PROPERTY STATEMENTS ------------------------
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
  constexpr auto select_all_properties = R"( SELECT * FROM properties ORDER BY property_id; )";

  constexpr auto select_property_by_id
    = R"( SELECT * FROM properties WHERE property_id = :id ; )";
  constexpr auto update_property_by_id
    = R"( UPDATE  properties
          SET   name = :name
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
    CITATION_UUID,
    CITATION_KEY,
    CITATION_TITLE,
    CITATION_AUTHORS,
    CITATION_YEAR,
    CITATION_PUBLISHER,
    CITATION_COLUMN_COUNT
  };

  constexpr auto create_citations_table = R"(
    CREATE TABLE IF NOT EXISTS "citations" (
	    "citation_id"	INTEGER,
	    "uuid"	TEXT,
	    "key"	Varchar(64) NOT NULL UNIQUE,
	    "title"	TEXT,
	    "authors"	TEXT,
	    "year"	TEXT,
	    "publisher"	TEXT,
	    PRIMARY KEY("citation_id"),
	    UNIQUE("uuid")
    );
  )";
  constexpr auto drop_all_citations = R"( DELETE FROM citations; )";
  constexpr auto count_citations = R"( SELECT COUNT(citation_id) FROM citations; )";
  constexpr auto count_citations_in_scene = R"( SELECT COUNT(citation_map_id) FROM citation_maps WHERE :id = fk_scene ; )";
  constexpr auto select_all_citations = R"( SELECT * FROM citations ORDER BY title; )";

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
          SET
              uuid    = :uuid
              , key     = :key
              , title   = :title
              , authors = :authors
              , year    = :year
              , publisher = :publisher
          WHERE citation_id = :id;
         )";
  constexpr auto update_citation_by_key
    = R"( UPDATE  citations
          SET key = :key
              , uuid = :uuid
              , title = :title
              , authors = :authors
              , year = :year
              , publisher = :publisher
          WHERE key = :key;
         )";

  constexpr auto insert_or_update_citations
    = R"( INSERT INTO citations
          (uuid, key, title, authors, year, publisher)
          VALUES (:uuid, :key, :title, :authors, :year, :publisher)
          ON CONFLICT (key)
          DO UPDATE SET
                         title = excluded.title
                       , authors = excluded.authors
                       , year = excluded.year
                       , publisher = excluded.publisher
         ;
         )";

  
  //---------------------- ROLE STATEMENTS ------------------------
  enum ROLE_COLUMNS {
    ROLE_ID,
    ROLE_UUID,
    ROLE_NAME,
    ROLE_DESCRIPTION,
    ROLE_COLUMN_COUNT
  };

  constexpr auto create_roles_table = R"(
    CREATE TABLE IF NOT EXISTS "roles" (
	    "role_id"	INTEGER,
	    "uuid"	TEXT,
	    "name"	Varchar(64) NOT NULL UNIQUE,
	    "description"	TEXT,
	    PRIMARY KEY("role_id"),
	    UNIQUE("uuid")
    );
  )";
  constexpr auto drop_all_roles = R"( DELETE FROM roles; )";
  constexpr auto count_roles = R"( SELECT COUNT(role_id) FROM roles; )";
  constexpr auto count_roles_in_scene = R"( SELECT COUNT(map_id) FROM role_maps WHERE :id = fk_scene ; )";
  constexpr auto select_all_roles = R"( SELECT * FROM roles ORDER BY name; )";

  constexpr auto select_role_by_id
    = R"( SELECT * FROM roles WHERE role_id = :id ; )";
  constexpr auto update_role_by_id
    = R"( UPDATE  roles
          SET
                uuid = : uuid
              , description = :description 
              , name = :name
          WHERE role_id = :id;
         )";
  constexpr auto select_role_by_name
    = R"( SELECT * FROM roles WHERE name = :name ORDER BY name; )";
  constexpr auto delete_role_by_id
    = R"( DELETE FROM roles WHERE role_id = :id; )";
  
  constexpr auto insert_or_update_roles
    = R"( INSERT INTO roles 
          (uuid, name,description)
          VALUES (:uuid, :name, :description)
          ON CONFLICT (name)
          DO UPDATE SET  name = excluded.name
                       , description = excluded.description
         )";


  //---------------------- SCENE STATEMENTS ------------------------
  enum SCENE_COLUMNS {
    SCENE_ID,
    SCENE_UUID,
    SCENE_NAME,
    SCENE_DESCRIPTION,
    SCENE_TIME_OF_DAY,
    SCENE_TIME_IN_SIMULATION,
    SCENE_WEATHER,
    SCENE_EVENTS,
    SCENE_ITEMS,
    SCENE_ROLES,
    SCENE_DETAILS,
    SCENE_COLUMN_COUNT
  };

  constexpr auto create_scenes_table = R"(
      CREATE TABLE IF NOT EXISTS "scenes" (
	      "scene_id"	INTEGER,
	      "uuid"	TEXT,
	      "name"	Varchar(64) NOT NULL UNIQUE,
	      "description"	TEXT NOT NULL,
	      "time_of_day"	Varchar(64) NOT NULL,
	      "time_in_simulation"	Varchar(64) NOT NULL,
	      "weather"	Varchar(64),
	      "events"	Varchar(64),
	      "items"	Varchar(64),
	      "roles"	Varchar(64),
	      "details"	TEXT NOT NULL,
	      UNIQUE("uuid"),
	      PRIMARY KEY("scene_id")
      );
  )";
  constexpr auto drop_all_scenes = R"( DELETE FROM scenes; )";
  constexpr auto count_scenes = R"( SELECT COUNT(scene_id) FROM scenes; )";
  constexpr auto select_all_scenes = R"( SELECT * FROM scenes ORDER BY name; )";

  constexpr auto select_scene_by_id
    = R"( SELECT * FROM scenes WHERE scene_id = :id ; )";
  constexpr auto select_scene_by_name
    = R"( SELECT * FROM scenes WHERE name = :name ORDER BY name; )";
  constexpr auto update_scene_by_id
    = R"( UPDATE  scenes
            SET
                  uuid = :uuid
                , name = :name
                , description = :description
                , time_of_day = :time_of_day
                , time_in_simulation = :time_in_simulation
                , weather = :weather
                , events = :events
                , items = :items
                , roles = :roles
                , details = :details
          WHERE scene_id = :id;
         )";
  constexpr auto delete_scene_by_id
    = R"( DELETE FROM scenes WHERE scene_id = :id; )";
  constexpr auto insert_or_update_scenes
    = R"( INSERT INTO scenes
          (name, uuid, description, time_of_day, time_in_simulation, weather, events, items, roles, details)
          VALUES (:name, :uuid, :description, :time_of_day, :time_in_simulation, :weather, :events, :items, :roles, :details)
          ON CONFLICT (name)
          DO UPDATE SET name = :name
                        , description = excluded.description
                        , time_of_day = excluded.time_of_day
                        , time_in_simulation = excluded.time_in_simulation
                        , weather = excluded.weather
                        , events = excluded.events
                        , items = excluded.items
                        , roles = excluded.roles
                        , details = excluded.details
          ;          
          )";
  //---------------------- TREATMENT STATEMENTS ------------------------
  enum TREATMENT_COLUMNS {
    TREATMENT_ID,
    TREATMENT_UUID,
    TREATMENT_MEDICAL_NAME,
    TREATMENT_COMMON_NAME,
    TREATMENT_DESCRIPTION,
    TREATMENT_EQUIPMENT,
    TREATMENT_CITATIONS,
    TREATMENT_CPGS,
    TREATMENT_COLUMN_COUNT
  };

  constexpr auto create_treatments_table = R"(
    CREATE TABLE IF NOT EXISTS "treatments" (
	    "treatment_id"	INTEGER,
	    "uuid"	TEXT,
	    "medical_name"	Varchar(64) NOT NULL UNIQUE,
	    "common_name"	Varchar(64) NOT NULL UNIQUE,
	    "description"	TEXT,
	    "equipment"	TEXT,
	    "citations"	TEXT,
      "cpgs"	TEXT,
	    PRIMARY KEY("treatment_id"),
	    UNIQUE("uuid")
    );
  )";

  constexpr auto drop_all_treatments = R"( DELETE FROM treatments; )";
  constexpr auto count_treatments = R"( SELECT COUNT(treatment_id) FROM treatments; )";
  constexpr auto select_all_treatments = R"( SELECT * FROM treatments ORDER BY medical_name; )";

  constexpr auto select_treatment_by_medical_name
    = R"( SELECT * FROM treatments WHERE medical_name = :medical_name ORDER BY medical_name; )";
  constexpr auto select_treatment_by_common_name
    = R"( SELECT * FROM treatments WHERE common_name = :common_name ORDER BY common_name; )";

  constexpr auto select_treatment_by_id
    = R"( SELECT * FROM treatments WHERE treatment_id = :id ; )";
  constexpr auto update_treatment_by_id
    = R"( UPDATE  treatments
          SET   uuid = :uuid
              , medical_name = :medical_name
              , common_name = :common_name
              , description = :description
              , equipment = :equipment
              , citations = :citations
              , cpgs = :cpgs
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
          (medical_name, uuid, common_name,description,equipment,citations,cpgs)
          VALUES (:medical_name, :uuid, :common_name, :description, :equipment, :citations, :cpgs)
          ON CONFLICT (medical_name)
          DO UPDATE SET   medical_name = excluded.medical_name
                        , common_name = excluded.common_name
                        , description = excluded.description
                        , equipment = excluded.equipment
                        , citations = excluded.citations
                        , cpgs = excluded.cpgs
          ;          
          )";
  
  //-------------------------------------------------------------------


}
}

#endif
