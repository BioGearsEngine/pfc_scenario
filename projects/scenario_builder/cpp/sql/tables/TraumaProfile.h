#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_TRAUMAPROFILE_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_TRAUMAPROFILE_H

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

struct TraumaProfile;
struct Treatment;
struct Trauma;

struct TraumaOccurance : public QObject {
  Q_OBJECT
  Q_PROPERTY(int trauma_instance_id MEMBER id NOTIFY idChanged)
  Q_PROPERTY(Trauma* fk_trauma MEMBER fk_trauma NOTIFY traumaChanged)
  Q_PROPERTY(QString description MEMBER description NOTIFY descriptionChanged)
  Q_PROPERTY(QString location MEMBER location NOTIFY locationChanged)
  Q_PROPERTY(QString severity MEMBER severity NOTIFY severityChanged)

signals:
  void idChanged();
  void traumaChanged();
  void descriptionChanged();
  void locationChanged();
  void severityChanged();

public:
  int32_t id = -1;
  Trauma* fk_trauma;
  QString description = "";
  QString location = "";
  QString severity = "";

  TraumaOccurance(QObject* parent = nullptr);
  TraumaOccurance(Trauma* trauma, Treatment* treatment, QObject* parent = nullptr);
  TraumaOccurance(const TraumaOccurance&) = delete;
  TraumaOccurance(TraumaOccurance&&) = delete;
  TraumaOccurance& operator=(const TraumaOccurance&) = delete;
  TraumaOccurance& operator=(TraumaOccurance&&) = delete;
  virtual ~TraumaOccurance() = default;

  bool operator==(const TraumaOccurance& rhs) const;
  bool operator!=(const TraumaOccurance& rhs) const;
  static Q_INVOKABLE TraumaOccurance* make();
  Q_INVOKABLE void assign(TraumaOccurance* rhs);
  void assign(const TraumaOccurance& rhs);
  void clear();
};

struct TraumaProfile : public QObject {
  Q_OBJECT
  Q_PROPERTY(int trauma_profile_id MEMBER id NOTIFY idChanged)
  Q_PROPERTY(QString uuid MEMBER uuid NOTIFY uuidChanged)
  Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)
  Q_PROPERTY(QString description MEMBER description NOTIFY descriptionChanged)
  Q_PROPERTY(QQmlListProperty<TraumaOccurance> traumas READ get_traumas NOTIFY traumasChanged);
  Q_PROPERTY(QStringList physiologyTree MEMBER physiologyTree NOTIFY physiologyTreeChanged)

public:
  int32_t id = -1;
  QString uuid = "";
  QString name = "";
  QString description = "";
  QList<TraumaOccurance*> traumas;
  QStringList physiologyTree;

  TraumaProfile(QObject* parent = nullptr);
  TraumaProfile(const TraumaProfile&) = delete;
  TraumaProfile(TraumaProfile&&) = delete;
  TraumaProfile& operator=(const TraumaProfile&) = delete;
  TraumaProfile& operator=(TraumaProfile&&) = delete;
  virtual ~TraumaProfile() = default;

  bool operator==(const TraumaProfile& rhs) const;
  bool operator!=(const TraumaProfile& rhs) const;
  static Q_INVOKABLE TraumaProfile* make();
  Q_INVOKABLE void assign(TraumaProfile* rhs);
  void assign(const TraumaProfile& rhs);
  Q_INVOKABLE void clear();
  Q_INVOKABLE void clear(int);
    signals:
  void idChanged();
  void uuidChanged();
  void nameChanged();
  void descriptionChanged();
  void traumasChanged();
  void physiologyTreeChanged();

private:
  QQmlListProperty<TraumaOccurance> get_traumas();
  // QQmlListProperty<QString> get_physiology_tree();
  //! Helper functions for Traumas
  static void AppendTrauma(QQmlListProperty<TraumaOccurance>* list, TraumaOccurance* value);
  static auto GetTrauma(QQmlListProperty<TraumaOccurance>* list, int index) -> TraumaOccurance*;
  static void ClearTraumas(QQmlListProperty<TraumaOccurance>* list);
  static auto CountTraumas(QQmlListProperty<TraumaOccurance>* list) -> int;
  static void RemoveLastTrauma(QQmlListProperty<TraumaOccurance>* list);
  static void ReplaceTrauma(QQmlListProperty<TraumaOccurance>* list, int index, TraumaOccurance* value);

  //! Helper functions for Traumas
  // static void AppendPhysiologyState(QQmlListProperty<QString>* list, QString* value);
  // static auto GetPhysiologyState(QQmlListProperty<QString>* list, int index) -> QString*;
  // static void ClearPhysiologyStates(QQmlListProperty<QString>* list);
  // static auto CountPhysiologyStates(QQmlListProperty<QString>* list) -> int;
  // static void RemoveLastPhysiologyState(QQmlListProperty<QString>* list);
  // static void ReplacePhysiologyState(QQmlListProperty<QString>* list, int index, QString* value);
};

#endif