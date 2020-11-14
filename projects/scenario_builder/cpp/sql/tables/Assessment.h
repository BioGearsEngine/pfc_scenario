#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_ASSESSMENTS_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_ASSESSMENTS_H

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

struct Objective;

struct Assessment : public QObject {
  Q_OBJECT
  Q_PROPERTY(int assessment_id MEMBER id NOTIFY idChanged)
  Q_PROPERTY(QString uuid MEMBER uuid NOTIFY uuidChanged)
  Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)
  Q_PROPERTY(QString description MEMBER description NOTIFY descriptionChanged)
  Q_PROPERTY(QString type MEMBER type NOTIFY typeChanged)
  Q_PROPERTY(Objective* objective MEMBER fk_objective NOTIFY objectiveChanged)
  Q_PROPERTY(int availablePoints MEMBER available_points NOTIFY avaliablePointsChanged)
  Q_PROPERTY(QString criteria MEMBER criteria NOTIFY criteriaChanged)
signals:
  void idChanged();
  void uuidChanged();
  void nameChanged();
  void descriptionChanged();
  void typeChanged();
  void objectiveChanged();
  void avaliablePointsChanged();
  void criteriaChanged();

public:
  int32_t id = -1;
  QString uuid = "";
  QString name = "";
  QString description = "";
  QString type = "";
  Objective* fk_objective; // Forign Key to Objective this Assesses
  int32_t available_points = -1;
  QString criteria = "";

  Assessment(QObject* parent = nullptr);
  Assessment(Objective* objective, QObject* parent);
  Assessment(const Assessment&) = delete;
  Assessment(Assessment&&) = delete;
  Assessment& operator=(const Assessment&) = delete;
  Assessment& operator=(Assessment&&) = delete;
  virtual ~Assessment() = default;

  bool operator==(const Assessment& rhs) const;
  bool operator!=(const Assessment& rhs) const;

  static Q_INVOKABLE Assessment* make();

  Q_INVOKABLE void assign(Assessment* rhs);
  void assign(const Assessment& rhs);

  Q_INVOKABLE void clear();
  Q_INVOKABLE void clear(int index);
};
//----End Assessment

#endif
