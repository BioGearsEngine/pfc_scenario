#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_OBJECTIVE_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_OBJECTIVE_H

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
#include <QQmlListProperty>
#include <QString>

struct Treatment;
struct TraumaProfile;
struct Citation;

struct Objective : public QObject {
  Q_OBJECT
  Q_PROPERTY(int objective_id MEMBER id NOTIFY idChanged)
  Q_PROPERTY(QString uuid MEMBER uuid NOTIFY uuidCHanged)
  Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)
  Q_PROPERTY(QString description MEMBER description NOTIFY descriptionChanged)

  Q_PROPERTY(QQmlListProperty<Citation> citations READ getCitations NOTIFY citationsChanged)
  Q_PROPERTY(QQmlListProperty<Citation> cpgs READ getCPGS NOTIFY cpgsChanged)
  Q_PROPERTY(QQmlListProperty<Treatment> treatment_plans READ getTreatments NOTIFY treatmentsChanged)
  Q_PROPERTY(QQmlListProperty<TraumaProfile> trauma_profile READ getTraumaProfiles NOTIFY traumaProfilesChanged)

public:
  int32_t id = -1;
  QString uuid = "";
  QString name = "";
  QString description = "";
  QList<Citation*> _citations;
  QList<Citation*> _cpgs;
  QList<Treatment*> _treatment_plans;
  QList<TraumaProfile*> _trauma_profiles;

  Objective(QObject* parent = nullptr);
  Objective(const Objective&) = delete;
  Objective(Objective&&) = delete;
  Objective& operator=(const Objective&) = delete;
  Objective& operator=(Objective&&) = delete;
  virtual ~Objective() = default;

  bool operator==(const Objective& rhs) const;
  bool operator!=(const Objective& rhs) const;

  static Q_INVOKABLE Objective* make();

  Q_INVOKABLE void assign(Objective* rhs);
  void assign(const Objective& rhs);

  void clear();

  Q_INVOKABLE void removeCitation(int index);
  Q_INVOKABLE void replaceCitation(int index, Citation* value);
  Q_INVOKABLE void removeCPG(int index);
  Q_INVOKABLE void replaceCPG(int index, Citation* value);
  Q_INVOKABLE void removeTreatment(int index);
  Q_INVOKABLE void replaceTreatment(int index, Treatment* value);
  Q_INVOKABLE void removeTraumaProfile(int index);
  Q_INVOKABLE void replaceTraumaProfile(int index, TraumaProfile* value);
signals:
  void idChanged();
  void uuidCHanged();
  void nameChanged();
  void descriptionChanged();
  void citationsChanged();
  void cpgsChanged();
  void treatmentsChanged();
  void traumaProfilesChanged();

private:

  //! Helper functions for Citations
  QQmlListProperty<Citation> getCitations();
  static void AppendCitation(QQmlListProperty<Citation>* list, Citation* value);
  static auto GetCitation(QQmlListProperty<Citation>* list, int index) -> Citation*;
  static void ClearCitations(QQmlListProperty<Citation>* list);
  static auto CountCitations(QQmlListProperty<Citation>* list) -> int;

  //! Helper functions for CPGs
  QQmlListProperty<Citation> getCPGS();
  static void AppendCPG(QQmlListProperty<Citation>* list, Citation* value);
  static auto GetCPG(QQmlListProperty<Citation>* list, int index) -> Citation*;
  static void ClearCPGs(QQmlListProperty<Citation>* list);
  static auto CountCPGs(QQmlListProperty<Citation>* list) -> int;

  //! Helper functions for Treatments
  QQmlListProperty<Treatment> getTreatments();
  static void AppendTreatment(QQmlListProperty<Treatment>* list, Treatment* value);
  static auto GetTreatment(QQmlListProperty<Treatment>* list, int index) -> Treatment*;
  static void ClearTreatments(QQmlListProperty<Treatment>* list);
  static auto CountTreatments(QQmlListProperty<Treatment>* list) -> int;

  //! Helper functions for TraumaProfiles
  QQmlListProperty<TraumaProfile> getTraumaProfiles();
  static void AppendTraumaProfile(QQmlListProperty<TraumaProfile>* list, TraumaProfile* value);
  static auto GetTraumaProfile(QQmlListProperty<TraumaProfile>* list, int index) -> TraumaProfile*;
  static void ClearTraumaProfiles(QQmlListProperty<TraumaProfile>* list);
  static auto CountTraumaProfiles(QQmlListProperty<TraumaProfile>* list) -> int;
};

#endif