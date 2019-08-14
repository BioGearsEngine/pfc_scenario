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
  constexpr auto select_all_assessments = R"( SELECT * FROM assessments; )";

  constexpr auto select_assessment_by_id
    = R"( SELECT * FROM assessments WHERE assessment_id = :id ; )";
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
    = R"( SELECT * FROM assessments WHERE name = :name; )";
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
  constexpr auto select_all_authors = R"( SELECT * FROM authors; )";

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
    = R"( SELECT * FROM authors WHERE email = :email; )";

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
    description Varchar(64) NOT NULL UNIQUE,
    citations TEXT,
    image TEXT
  );
  )";
  constexpr auto drop_all_equipment = R"( DELETE FROM equipment; )";
  constexpr auto count_equipments = R"( SELECT COUNT(equipment_id) FROM equipments; )";
  constexpr auto select_all_equipments = R"( SELECT * FROM equipments; )";

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
    = R"( SELECT * FROM equipments WHERE name = :name )";
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
  constexpr auto select_all_events = R"( SELECT * FROM events; )";

  constexpr auto select_event_by_id
    = R"( SELECT * FROM events WHERE event_id = :id ; )";
  constexpr auto select_event_by_name
    = R"( SELECT * FROM events WHERE name = :name ; )";
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
  constexpr auto select_all_injuries = R"( SELECT * FROM injuries; )";

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
    = R"( SELECT * FROM injuries WHERE medical_name = :medical_name; )";
  constexpr auto select_injury_by_common_name
    = R"( SELECT * FROM injuries WHERE common_name = :common_name; )";
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
    INJURY_SET_COLUMN_COUNT
  };

  constexpr auto create_injury_sets_table = R"(
  CREATE TABLE IF NOT EXISTS injury_sets ( 
    injury_set_id INTEGER PRIMARY KEY,
    name Varchar(64) NOT NULL UNIQUE,
    description TEXT,
    injuries TEXT
  );
  )";

  constexpr auto drop_all_injury_sets = R"( DELETE FROM injury_sets; )";
  constexpr auto count_injury_sets = R"( SELECT COUNT(injury_set_id) FROM injury_sets; )";
  constexpr auto select_all_injury_sets = R"( SELECT * FROM injury_sets; )";

  constexpr auto select_injury_set_by_id
    = R"( SELECT * FROM injury_sets WHERE injury_set_id = :id ; )";
  constexpr auto update_injury_set_by_id
    = R"( UPDATE  injury_sets
          SET name = :name
              , description = :description
              , injuries = :injuries;
          WHERE injury_set_id = :id;
         )";
  constexpr auto delete_injury_set_by_id
    = R"( DELETE FROM injury_sets WHERE injury_set_id = :id; )";
  constexpr auto delete_injury_set_by_name
    = R"( DELETE FROM injury_sets WHERE name = :name; )";
  constexpr auto select_injury_set_by_name
    = R"( SELECT * FROM injury_sets WHERE name = :name; )";
  constexpr auto insert_or_update_injury_sets
    = R"( INSERT INTO injury_sets
          (name,description,injuries)
          VALUES (:name, :description, :injuries)
          ON CONFLICT (name)
          DO UPDATE SET name = excluded.name
                        , description = excluded.description
                        , injuries = excluded.injuries
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
    name Varchar(64) NOT NULL UNIQUE,
    scene_name Varchar(64) NOT NULL UNIQUE,
    time_of_day TEXT,
    environment TEXT
  );
  )";
  constexpr auto drop_all_locations = R"( DELETE FROM locations; )";
  constexpr auto count_locations = R"( SELECT COUNT(location_id) FROM locations; )";
  constexpr auto select_all_locations = R"( SELECT * FROM locations; )";

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
    = R"( DELETE FROM locations WHERE name = :name; )";
  constexpr auto select_location_by_name
    = R"( SELECT * FROM locations WHERE name = :name; )";
  constexpr auto select_location_by_scene_name
    = R"( SELECT * FROM locations WHERE scene_name = :scene_name; )";
  constexpr auto insert_or_update_locations
    = R"( INSERT INTO locations
          (name,scene_name,time_of_day,environment)
          VALUES (:name, :scene_name, :time_of_day, :environment)
          ON CONFLICT (name)
          DO UPDATE SET name = excluded.name
                       , scene_name = excluded.scene_name
                       , time_of_day = excluded.time_of_day
                       , environment = excluded.environment
         ;
         )";
  
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
  constexpr auto select_all_objectives = R"( SELECT * FROM objectives; )";

  constexpr auto select_objective_by_id
    = R"( SELECT * FROM objectives WHERE objective_id = :id ; )";
  constexpr auto select_objective_by_name
    = R"( SELECT * FROM objectives WHERE name = :name; )";

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
  //---------------------- PROP STATMENTS ------------------------
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
  constexpr auto select_all_props = R"( SELECT * FROM props; )";

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
  constexpr auto select_all_properties = R"( SELECT * FROM properties; )";

  constexpr auto select_property_by_id
    = R"( SELECT * FROM properties WHERE property_id = :id ; )";
  constexpr auto update_property_by_id
    = R"( UPDATE  properties
          SET name = :name
              , value = :value
          WHERE property_id = :id;
         )";
  constexpr auto select_property_by_name
    = R"( SELECT * FROM properties WHERE name = :name; )";

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
  
  //---------------------- CITATION STATMENTS ------------------------
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
  constexpr auto select_all_citations = R"( SELECT * FROM citations; )";

  constexpr auto select_citation_by_id
    = R"( SELECT * FROM citations WHERE citation_id = :id ; )";
  constexpr auto select_citation_by_key
    = R"( SELECT * FROM citations WHERE key = :key; )";
  constexpr auto select_citation_by_title
    = R"( SELECT * FROM citations WHERE title = :title; )";

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
  constexpr auto select_all_restrictions = R"( SELECT * FROM restrictions; )";

  constexpr auto select_restriction_by_id
    = R"( SELECT * FROM restrictions WHERE restriction_id = :id ; )";
  constexpr auto update_restriction_by_id
    = R"( UPDATE  restrictions
          SET name = :name
              , value = :value
          WHERE restriction_id = :id;
         )";
  constexpr auto select_restriction_by_name
    = R"( SELECT * FROM restrictions WHERE name = :name; )";

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
  constexpr auto select_all_roles = R"( SELECT * FROM roles; )";

  constexpr auto select_role_by_id
    = R"( SELECT * FROM roles WHERE role_id = :id ; )";
  constexpr auto update_role_by_id
    = R"( UPDATE  roles
          SET description = :description 
              , name = :name
          WHERE role_id = :id;
         )";
  constexpr auto select_role_by_name
    = R"( SELECT * FROM roles WHERE name = :name ; )";
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
  constexpr auto select_all_scenes = R"( SELECT * FROM scenes; )";

  constexpr auto select_scene_by_id
    = R"( SELECT * FROM scenes WHERE scene_id = :id ; )";
  constexpr auto select_scene_by_name
    = R"( SELECT * FROM scenes WHERE name = :name ; )";
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
  constexpr auto select_all_treatments = R"( SELECT * FROM treatments; )";

  constexpr auto select_treatment_by_medical_name
    = R"( SELECT * FROM treatments WHERE medical_name = :medical_name; )";
  constexpr auto select_treatment_by_common_name
    = R"( SELECT * FROM treatments WHERE common_name = :common_name; )";

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
