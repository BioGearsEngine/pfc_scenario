#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_TRAUMA_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_TRAUMA_H

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

#include "Treatment.h"
#include <QList>
#include <QObject>
#include <QQmlListProperty>
#include <QString>

struct Citation;
struct Treatment;

struct Trauma : public QObject {
  Q_OBJECT
  Q_PROPERTY(int injury_id MEMBER id);
  Q_PROPERTY(QString uuid MEMBER uuid)
  Q_PROPERTY(QString medical_name MEMBER medical_name)
  Q_PROPERTY(QString common_name MEMBER common_name)
  Q_PROPERTY(QString description MEMBER description)
  Q_PROPERTY(QQmlListProperty<Citation> citations READ getCitations NOTIFY citationsChanged)
  Q_PROPERTY(float min MEMBER lower_bound)
  Q_PROPERTY(float max MEMBER upper_bound)
  Q_PROPERTY(Treatment* fk_treatment MEMBER fk_treatment)

public:
  int32_t id = -1;
  QString uuid = "";
  QString medical_name = "";
  QString common_name = "";
  QString description = "";
  QList<Citation*> citations;
  float lower_bound = 0;
  float upper_bound = 1;
  Treatment* fk_treatment;

  Trauma(QObject* parent = nullptr);

  Trauma(const Trauma&) = delete;
  Trauma(Trauma&&) = delete;
  Trauma& operator=(const Trauma&) = delete;
  Trauma& operator=(Trauma&&) = delete;
  virtual ~Trauma() = default;

  bool operator==(const Trauma& rhs) const;
  bool operator!=(const Trauma& rhs) const;

  static Q_INVOKABLE Trauma* make();

  Q_INVOKABLE void assign(Trauma* rhs);
  void assign(const Trauma& rhs);

  void clear();

signals:
  void citationsChanged();

private:
  //! Helper functions for Citations
  QQmlListProperty<Citation> getCitations();
  static void AppendCitation(QQmlListProperty<Citation>* list, Citation* value);
  static auto GetCitation(QQmlListProperty<Citation>* list, int index) -> Citation*;
  static void ClearCitations(QQmlListProperty<Citation>* list);
  static auto CountCitations(QQmlListProperty<Citation>* list) -> int;
  static void RemoveLastCitation(QQmlListProperty<Citation>* list);
  static void ReplaceCitation(QQmlListProperty<Citation>* list, int index, Citation* value);
};

#endif