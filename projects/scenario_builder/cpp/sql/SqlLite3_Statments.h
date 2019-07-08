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

  constexpr char* tables[] = {
    { "properties" },
    { "authors" },
    { "restrictions" }
  };

  constexpr auto list_tables = "SELECT * FROM sqlite_master WHERE type='table';";
  constexpr auto drop_table = "DROP TABLE IF EXISTS ?;";

  enum PROPERTY_COLUMNS {
    PROPERTY_ID,
    PROPERTY_NAME,
    PROPERTY_VALUE
  };

  constexpr auto create_properties_table = R"(
  CREATE TABLE properties (
    property_id Int,
    name Varchar(64) NOT NULL UNIQUE,
    value Varchar(255),
    created DATETIME,
    modified DATETIME, 
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
  CREATE TABLE authors (
    authors_id Int,
    name_first Varchar(25),
    name_last Varchar(25) NOT NULL ,
    email VarChar(45) NOT NULL UNIQUE,
    zipcode Varchar(10),
    state Varchar(64),
    country Varchar(64),
    phone Varchar(16),
    organization Varchar(128),    
    created DATETIME,
    modified DATETIME
  );
  )";

  enum RESTRICTION_COLUMNS {
    RESTRICTION_ID,
    RESTRICTION_NAME,
    RESTRICTION_VALUE
  };

  constexpr auto create_restrictions_table = R"(
  CREATE TABLE restrictions (
    restriction_id Int,
    name Varchar(64) NOT NULL UNIQUE,
    value Varchar(255)
    created DATETIME,s
    modified DATETIME
  );
  )";

  constexpr auto drop_all_properties = R"( DELETE FROM Properties; )";
  constexpr auto drop_all_authors = R"( DELETE FROM Authors; )";
  constexpr auto drop_all_restrictions = R"( DELETE FROM Restrictions; )";

  constexpr auto select_all_properties = R"( SELECT FROM Properties; )";
  constexpr auto select_all_authors = R"( SELECT FROM Authors; )";
  constexpr auto select_all_restrictions = R"( SELECT FROM Restrictions; )";

  constexpr auto select_property_by_id
    = R"( SELECT FROM Properties WHERE id = ?; )";
  constexpr auto select_author_by_id
    = R"( SELECT FROM Authors WHERE id = ?; )";
  constexpr auto select_restriction_by_id
    = R"( SELECT FROM Restrictions WHERE id = ?; )";

  constexpr auto select_property_by_name
    = R"( SELECT FROM Properties WHERE name = ?; )";
  constexpr auto select_author_by_email
    = R"( SELECT FROM Authors WHERE email = ?; )";
  constexpr auto select_restriction_by_name
    = R"( SELECT FROM Restrictions WHERE name = ?; )";

  constexpr auto insert_into_properties
    = R"( INSERT  OR REPLACE INTO Properties 
          ( name_first, name_last,email ,zipcode ,state ,country ,phone ,organization )
          VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?); 
        )";
  constexpr auto insert_into_authors
    = R"( INSERT  OR REPLACE INTO Authors  
          (name,value)
          VALUES (?, ?) ;
        )";
  constexpr auto insert_into_restrictions
    = R"( INSERT  OR REPLACE INTO Restrictions 
          (name,value)
          VALUES (?, ?);
         )";

  constexpr auto delete_property_by_id
    = R"( DELETE FROM Properties WHERE id = ?; )";
  constexpr auto delete_author_by_id
    = R"( DELETE FROM Authors WHERE id = ?; )";
  constexpr auto delete_restriction_by_id
    = R"( DELETE FROM Restrictions WHERE id = ?; )";

  constexpr auto delete_property_by_name
    = R"( DELETE FROM Properties WHERE name = ?; )";
  constexpr auto delete_author_by_email
    = R"( DELETE FROM Authors WHERE email = ?; )";
  constexpr auto delete_restriction_by_name
    = R"( DELETE FROM Restrictions WHERE name = ?; )";
}
}

#endif