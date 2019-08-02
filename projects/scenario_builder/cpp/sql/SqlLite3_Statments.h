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
    "events",
    "equipments",
    "injuries",
    "locations",
    "objectives",
    "properties",
    "props",
    "citations",
    "restrictions",
    "roles",
    "treatments",

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
    ASSESSMENT_CRITERIA
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
  constexpr auto select_assessment_by_name
    = R"( SELECT * FROM assessments WHERE name = :name; )";
  constexpr auto insert_or_update_assessments
    = R"( INSERT INTO assessments
          (name,description,type,available_points,citations)
          VALUES (:name, :description, :type, :available_points, :citations)
          ON CONFLICT (name)
          DO UPDATE SET name = excluded.name
                       , description = excluded.description
                       , type = excluded.type
                       , available_points = excluded.available_points
                       , citations = excluded.citations
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
    EQUIPMENT_NAME,
    EQUIPMENT_DESCRIPTION,
    EQUIPMENT_EQUIPMENT_LIST,
    EQUIPMENT_CITATIONS,
  };

  constexpr auto create_equipment_table = R"(
  CREATE TABLE IF NOT EXISTS equipments (
    equipment_id INTEGER PRIMARY KEY,
    name Varchar(64) NOT NULL UNIQUE,
    purpose Varchar(64) NOT NULL UNIQUE,
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
              , description = :description
              , equipment_list = :equipment_list
              , citations = :citations
          WHERE equipment_id = :id;
         )";
  constexpr auto select_equipment_by_name
    = R"( SELECT * FROM equipments WHERE name = :name )";
  constexpr auto insert_or_update_equipments
    = R"( INSERT INTO equipments
          (medical_name,common_name,description,equipment_list,citations)
          VALUES (:name, :description, :equipment_list, :citations)
          ON CONFLICT (name)
          DO UPDATE SET name = excluded.name
                        , description = excluded.description
                        , equipment_list = excluded.equipment_list
                        , citations = excluded.citations
          ;         
          )";
  
  //---------------------- EVENT STATMENTS ------------------------
  enum EVENT_COLUMNS {
    EVENT_ID,
  };

  constexpr auto create_events_table = R"(
  CREATE TABLE IF NOT EXISTS events (
    event_id INTEGER PRIMARY KEY
  );
  )";
  constexpr auto drop_all_events = R"( DELETE FROM events; )";
  constexpr auto count_events = R"( SELECT COUNT(event_id) FROM events; )";
  constexpr auto select_all_events = R"( SELECT * FROM events; )";

  constexpr auto select_event_by_id
    = R"( SELECT * FROM events WHERE event_id = :id ; )";
  constexpr auto update_event_by_id
    = R"( UPDATE  events
          WHERE event_id = :id;
         )";
  constexpr auto insert_or_update_events
    = R"()";
  //---------------------- INJURY STATMENTS ------------------------
  enum INJURY_COLUMNS {
    INJURY_ID,
    INJURY_MEDICAL_NAME,
    INJURY_COMMON_NAME,
    INJURY_DESCRIPTION,
    INJURY_CITATIONS,
  };

  constexpr auto create_injuries_table = R"(
  CREATE TABLE IF NOT EXISTS injuries ( 
    injury_id INTEGER PRIMARY KEY,
    medical_name Varchar(64) NOT NULL UNIQUE,
    common_name Varchar(64) NOT NULL UNIQUE,
    description TEXT,
    severity TEXT,
    citations TEXT
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
              , equipment_list = :equipment_list
              , citations = :citations
          WHERE injury_id = :id;
         )";
  constexpr auto select_injury_by_medical_name
    = R"( SELECT * FROM injuries WHERE medical_name = :medical_name; )";
  constexpr auto select_injury_by_common_name
    = R"( SELECT * FROM injuries WHERE common_name = :common_name; )";
  constexpr auto insert_or_update_injuries
    = R"( INSERT INTO injuries
          (medical_name,common_name,description,citations)
          VALUES (:medical_name, :common_name, :description, :citations)
          ON CONFLICT (medical_name)
          DO UPDATE SET medical_name = excluded.medical_name
                        , common_name = excluded.common_name
                        , description = excluded.description
                        , citations = excluded.citations
          ;
          ON CONFLICT (common_name)
          DO UPDATE SET common_name = excluded.common_name
                        , medical_name = excluded.medical_name
                        , description = excluded.description
                        , citations = excluded.citations
          ;          
          )";
  

  //---------------------- LOCATION STATMENTS ------------------------
  enum LOCATION_COLUMNS {
    LOCATION_ID,
    LOCATION_NAME,
    LOCATION_SCENE_NAME,
    LOCATION_TIME_OF_DAY,
    LOCATION_ENVIRONMENT,
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
          ON CONFLICT (scene_name)
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
  constexpr auto insert_or_update_props
    = R"( INSERT INTO props 
          (name,equipment)
          VALUES (:name, :equipment)
          ON CONFLICT (name)
          DO UPDATE SET name = excluded.name
                       , equipment = excluded.equipment
         ;
         )";
  
  //---------------------- PROPERTY STATMENTS ------------------------
  enum PROPERTY_COLUMNS {
    PROPERTY_ID,
    PROPERTY_NAME,
    PROPERTY_VALUE
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
    CITATION_NAME,
    CITATION_KEY,
    CITATION_TITLE,
    CITATION_AUTHORS,
    CITATION_VALUE
  };

  constexpr auto create_citations_table = R"(
  CREATE TABLE IF NOT EXISTS citations (
    citation_id INTEGER PRIMARY KEY,
    name Varchar(64) NOT NULL UNIQUE,
    key Varchar(64) NOT NULL,
    title TEXT,
    authors TEXT,
    value Varchar(255)
  );
  )";
  constexpr auto drop_all_citations = R"( DELETE FROM citations; )";
  constexpr auto count_citations = R"( SELECT COUNT(citation_id) FROM citations; )";
  constexpr auto select_all_citations = R"( SELECT * FROM citations; )";

  constexpr auto select_citation_by_id
    = R"( SELECT * FROM citations WHERE citation_id = :id ; )";
  constexpr auto update_citation_by_id
    = R"( UPDATE  citations
          SET name = :name
              , key = :key
              , title = :title
              , authors = :authors
              , value = :value
          WHERE citation_id = :id;
         )";
  constexpr auto select_citation_by_name
    = R"( SELECT * FROM citations WHERE name = :name; )";
  constexpr auto insert_or_update_citations
    = R"( INSERT INTO citations
          (name,key,title,authors,value)
          VALUES (:name, :key,:title,:authors,:value)
          ON CONFLICT (name)
          DO UPDATE SET name = excluded.name
                       , key = excluded.key
                       , title = excluded.title
                       , authors = excluded.authors
                       , value = excluded.value
         ;
         )";


  //---------------------- RESTRICTION STATMENTS ------------------------
  enum RESTRICTION_COLUMNS {
    RESTRICTION_ID,
    RESTRICTION_NAME,
    RESTRICTION_VALUE
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
    ROLE_DESCRIPTION,
  };

  constexpr auto create_roles_table = R"(
  CREATE TABLE IF NOT EXISTS roles (
    role_id INTEGER PRIMARY KEY,
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
          WHERE role_id = :id;
         )";
  constexpr auto insert_or_update_roles
    = R"( INSERT INTO roles 
          (name,description)
          VALUES (:name, :description)
          ON CONFLICT (name)
          DO UPDATE SET name = excluded.name
                       , description = excluded.description
         ;
         )";
  
  //---------------------- TREATMENT STATEMENTS ------------------------
  enum TREATMENT_COLUMNS {
    TREATMENT_ID,
    TREATMENT_MEDICAL_NAME,
    TREATMENT_COMMON_NAME,
    TREATMENT_DESCRIPTION,
    TREATMENT_EQUIPMENT_LIST,
    TREATMENT_CITATIONS,
  };

  constexpr auto create_treatments_table = R"(
  CREATE TABLE IF NOT EXISTS treatments (
    treatment_id INTEGER PRIMARY KEY,
    medical_name Varchar(64) NOT NULL UNIQUE,
    common_name Varchar(64) NOT NULL UNIQUE,
    description TEXT,
    equipment_list TEXT,
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
              , equipment_list = :equipment_list
              , citations = :citations
          WHERE treatment_id = :id;
         )";
  constexpr auto insert_or_update_treatments
    = R"( INSERT INTO treatments
          (medical_name,common_name,description,equipment_list,citations)
          VALUES (:medical_name, :common_name, :description, :equipment_list, :citations)
          ON CONFLICT (medical_name)
          DO UPDATE SET medical_name = excluded.medical_name
                        , common_name = excluded.common_name
                        , description = excluded.description
                        , equipment_list = excluded.equipment_list
                        , citations = excluded.citations
          ;
          ON CONFLICT (common_name)
          DO UPDATE SET common_name = excluded.common_name
                        , medical_name = excluded.medical_name
                        , description = excluded.description
                        , equipment_list = excluded.equipment_list
                        , citations = excluded.citations
          ;          
          )";
  
  //-------------------------------------------------------------------


}
}

#endif
