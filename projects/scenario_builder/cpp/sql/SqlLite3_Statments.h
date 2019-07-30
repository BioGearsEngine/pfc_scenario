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

  constexpr char const * tables[] = {
     "properties",
     "authors",
     "restrictions",
     "references",
     "treatments",
     "equipments",
     "injuries",
     "assessments",
     "objectives",
     "locations",
     "roles",
     "props",
     "events"

  };

  constexpr auto list_tables = "SELECT * FROM sqlite_master WHERE type='table';";
  constexpr auto drop_table = "DROP TABLE IF EXISTS :table;";

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

  enum REFERENCE_COLUMNS {
    REFERENCE_ID,
    REFERENCE_KEY,
    REFERENCE_TITLE,
    REFERENCE_AUTHORS,
  };

  constexpr auto create_references_table = R"(
  CREATE TABLE IF NOT EXISTS references (
    reference_id INTEGER PRIMARY KEY,
    name Varchar(64) NOT NULL UNIQUE,
    key Varchar(64) NOT NULL,
    title TEXT,
    authors TEXT,
    value Varchar(255)
  );
  )";

  enum TREATMENT_COLUMNS {
    TREATMENT_ID,
    TREATMENT_MEDICAL_NAME,
    TREATMENT_COMMON_NAME,
    TREATMENT_DESCRIPTION,
    TREATMENT_EQUIPMENT_LIST,
    TREATMENT_REFERENCE_LIST,
  };

  constexpr auto create_treatments_table = R"(
  CREATE TABLE IF NOT EXISTS treatments (
    treatment_id INTEGER PRIMARY KEY,
    medical_name Varchar(64) NOT NULL UNIQUE,
    common_name Varchar(64) NOT NULL UNIQUE,
    description TEXT,
    equipment_list TEXT,
    reference_list TEXT
  );
  )";

  enum EQUIPMENT_COLUMNS {
    EQUIPMENT_ID,
    EQUIPMENT_MEDICAL_NAME,
    EQUIPMENT_COMMON_NAME,
    EQUIPMENT_DESCRIPTION,
    EQUIPMENT_EQUIPMENT_LIST,
    EQUIPMENT_REFERENCE_LIST,
  };

  constexpr auto create_equipment_table = R"(
  CREATE TABLE IF NOT EXISTS equipments (
    equipment_id INTEGER PRIMARY KEY,
    name Varchar(64) NOT NULL UNIQUE,
    purpose Varchar(64) NOT NULL UNIQUE,
    image TEXT
  );
  )";

  enum INJURY_COLUMNS {
    INJURY_ID,
    INJURY_MEDICAL_NAME,
    INJURY_COMMON_NAME,
    INJURY_DESCRIPTION,
    INJURY_EQUIPMENT_LIST,
    INJURY_REFERENCE_LIST,
  };

  constexpr auto create_injuries_table = R"(
  CREATE TABLE IF NOT EXISTS injuries ( 
    injury_id INTEGER PRIMARY KEY,
    medical_name Varchar(64) NOT NULL UNIQUE,
    common_name Varchar(64) NOT NULL UNIQUE,
    description TEXT,
    severity TEXT,
    reference_list TEXT
  );
  )";

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

  enum OBJECTIVE_COLUMNS {
    OBJECTIVE_ID,
    OBJECTIVE_NAME,
    OBJECTIVE_DESCRIPTION,
    OBJECTIVE_REFERENCE_LIST,
  };

  constexpr auto create_objectives_table = R"(
  CREATE TABLE IF NOT EXISTS objectives (
    objective_id INTEGER PRIMARY KEY,
    name Varchar(64) NOT NULL UNIQUE,
    description TEXT,
    reference_list TEXT
  );
  )";

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

  enum EVENT_COLUMNS {
    EVENT_ID,
  };

  constexpr auto create_events_table = R"(
  CREATE TABLE IF NOT EXISTS props (
    event_id INTEGER PRIMARY KEY,
  );
  )";

  constexpr auto drop_all_properties = R"( DELETE FROM properties; )";
  constexpr auto drop_all_authors = R"( DELETE FROM authors; )";
  constexpr auto drop_all_restrictions = R"( DELETE FROM restrictions; )";
  constexpr auto drop_all_objectives = R"( DELETE FROM objectives; )";
  
  constexpr auto count_properties = R"( SELECT COUNT(property_id) FROM properties; )";
  constexpr auto count_authors = R"( SELECT COUNT(author_id) FROM authors; )";
  constexpr auto count_restrictions = R"( SELECT COUNT(restriction_id) FROM restrictions; )";
  constexpr auto count_objectives = R"( SELECT COUNT(objective_id) FROM objectives; )";

  constexpr auto select_all_properties = R"( SELECT * FROM properties; )";
  constexpr auto select_all_authors = R"( SELECT * FROM authors; )";
  constexpr auto select_all_restrictions = R"( SELECT * FROM restrictions; )";
  constexpr auto select_all_objectives = R"( SELECT * FROM objectives; )";

  constexpr auto select_property_by_id
    = R"( SELECT * FROM properties WHERE property_id = :id ; )";
  constexpr auto select_author_by_id
    = R"( SELECT * FROM authors WHERE author_id = :id ; )";
  constexpr auto select_restriction_by_id
    = R"( SELECT * FROM restrictions WHERE restriction_id = :id ; )";
  constexpr auto select_objective_by_id
    = R"( SELECT * FROM objectives WHERE objective_id = :id ; )";

  constexpr auto select_property_by_name
    = R"( SELECT * FROM properties WHERE name = :name; )";
  constexpr auto select_author_by_email
    = R"( SELECT * FROM authors WHERE email = :email; )";
  constexpr auto select_restriction_by_name
    = R"( SELECT * FROM restrictions WHERE name = :name; )";
  constexpr auto select_objective_by_name
    = R"( SELECT * FROM objectives WHERE name = :name; )";


  constexpr auto insert_or_update_first_author
    = R"( INSERT  OR REPLACE INTO authors
                 (  author_id, name_first,  name_last,  email,  zipcode,  state,  country,  phone,  organization)
          VALUES (1, :name_first, :name_last, :email, :zipcode, :state, :country, :phone, :organization)
        )"
  ;

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

  constexpr auto insert_or_update_restrictions
    = R"( INSERT INTO restrictions 
          (name,value)
          VALUES (:name, :value)
          ON CONFLICT (name)
          DO UPDATE SET value = excluded.value;
         )";

  constexpr auto insert_or_update_properties
    = R"( INSERT  INTO properties 
          (name,value)
          VALUES (:name, :value)
          ON CONFLICT (name)
          DO UPDATE SET value = excluded.value;
         )";

    constexpr auto insert_or_update_objectives
    = R"( INSERT INTO objectives 
          (name,description,reference_list)
          VALUES (:name, :description, :references)
          ON CONFLICT (name)
          DO UPDATE SET name = excluded.name
                       , description = excluded.description
                       , reference_list = excluded.reference_list
         ;
         )";

  //constexpr auto insert_into_authors
  //  = R"( INSERT INTO authors 
  //            (  name_first,  name_last,  email,  zipcode,  state,  country,  phone,  organization)
  //     VALUES ( :name_first, :name_last, :email, :zipcode, :state, :country, :phone, :organization)
  //     ON CONFLICT(email) 
  //     DO UPDATE SET name_first = excluded.name_first
  //                  ,name_last  = excluded.name_last
  //                  ,zipcode    = excluded.zipcode
  //                  ,state      = excluded.state
  //                  ,country    = excluded.country
  //                  ,phone      = excluded.phone
  //                  ,organization = excluded.organization;
  //;)";



  constexpr auto delete_property_by_id
    = R"( DELETE FROM properties WHERE property_id = :id; )";
  constexpr auto delete_author_by_id
    = R"( DELETE FROM authors WHERE author_id = :id; )";
  constexpr auto delete_restriction_by_id
    = R"( DELETE FROM restrictions WHERE restriction_id = :id; )";
  constexpr auto delete_objective_by_id
    = R"( DELETE FROM objectives WHERE objective_id = :id; )";

  constexpr auto delete_property_by_name
    = R"( DELETE FROM properties WHERE name = :name; )";
  constexpr auto delete_author_by_email
    = R"( DELETE FROM authors WHERE email = :email; )";
  constexpr auto delete_restriction_by_name
    = R"( DELETE FROM restrictions WHERE name = :name; )";
  constexpr auto delete_objective_by_name
    = R"( DELETE FROM objectives WHERE name = :name; )";
}
}

#endif
