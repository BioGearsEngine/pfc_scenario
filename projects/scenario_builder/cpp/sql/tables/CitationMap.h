#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_CITATIONMAP_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_CITATIONMAP_H

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
#include <QString>

//!
//!  Map Types.
//!  Version 2.0 Will contain Pointers to the
//!  Classes they refer to
//!  Allowing them to be used like join statments

struct Scene;
struct Citation;

struct CitationMap : public QObject {
  Q_OBJECT
  Q_PROPERTY(int map_id MEMBER id)
  Q_PROPERTY(Scene* fk_scene MEMBER fk_scene)
  Q_PROPERTY(Citation* fk_citation MEMBER fk_citation)

public:
  int32_t id = -1;
  Scene* fk_scene = nullptr;
  Citation* fk_citation = nullptr;

  CitationMap(QObject* parent = nullptr);
  CitationMap(const CitationMap&) = delete;
  CitationMap(CitationMap&&) = delete;
  CitationMap& operator=(const CitationMap&) = delete;
  CitationMap& operator=(CitationMap&&) = delete;
  virtual ~CitationMap() = default;

  bool operator==(const CitationMap& rhs) const;
  bool operator!=(const CitationMap& rhs) const;

  static Q_INVOKABLE CitationMap* make();

  Q_INVOKABLE void assign(CitationMap* rhs);
  void assign(const CitationMap& rhs);

  void clear();
};

#endif