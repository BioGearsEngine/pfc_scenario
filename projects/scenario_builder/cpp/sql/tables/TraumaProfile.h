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

#include "Image.h"

struct TraumaProfile;
struct Treatment;
struct Trauma;

struct TraumaOccurence : public QObject {
  Q_OBJECT
  Q_PROPERTY(Trauma* trauma MEMBER fk_trauma NOTIFY traumaChanged)
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
  Trauma* fk_trauma;
  QString description = "";
  QString location = "";
  QString severity = "";

  TraumaOccurence(QObject* parent = nullptr);
  TraumaOccurence(Trauma* trauma, Treatment* treatment, QObject* parent = nullptr);
  TraumaOccurence(const TraumaOccurence&) = delete;
  TraumaOccurence(TraumaOccurence&&) = delete;
  TraumaOccurence& operator=(const TraumaOccurence&) = delete;
  TraumaOccurence& operator=(TraumaOccurence&&) = delete;
  virtual ~TraumaOccurence() = default;

  bool operator==(const TraumaOccurence& rhs) const;
  bool operator!=(const TraumaOccurence& rhs) const;
  static Q_INVOKABLE TraumaOccurence* make();
  Q_INVOKABLE void assign(TraumaOccurence* rhs);
  void assign(const TraumaOccurence& rhs);
  void clear();
};

struct TraumaProfile : public QObject {
  Q_OBJECT
  Q_PROPERTY(int trauma_profile_id MEMBER id NOTIFY idChanged)
  Q_PROPERTY(QString uuid MEMBER uuid NOTIFY uuidChanged)
  Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)
  Q_PROPERTY(QString description MEMBER description NOTIFY descriptionChanged)
  Q_PROPERTY(QQmlListProperty<TraumaOccurence> traumas READ get_traumas NOTIFY traumasChanged);
  Q_PROPERTY(QStringList physiologyTree MEMBER physiologyTree NOTIFY physiologyTreeChanged)
  Q_PROPERTY(Image* treatment MEMBER fk_image NOTIFY imageChanged)

public:
  int32_t id = -1;
  QString uuid = "";
  QString name = "";
  QString description = "";
  QList<TraumaOccurence*> traumas;
  QStringList physiologyTree;
  Image* fk_image;

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

  Q_INVOKABLE void RemoveTrauma(int index);
  Q_INVOKABLE void ReplaceTrauma(int index, TraumaOccurence* value);
  // Q_INVOKABLE void RemoveLastPhysiologyState(int index);
  // Q_INVOKABLE void ReplacePhysiologyState(int index, QString* value);

signals:
  void idChanged();
  void uuidChanged();
  void nameChanged();
  void descriptionChanged();
  void traumasChanged();
  void physiologyTreeChanged();
  void imageChanged();

private:
  QQmlListProperty<TraumaOccurence> get_traumas();
  // QQmlListProperty<QString> get_physiology_tree();
  //! Helper functions for Traumas
  static void AppendTrauma(QQmlListProperty<TraumaOccurence>* list, TraumaOccurence* value);
  static auto GetTrauma(QQmlListProperty<TraumaOccurence>* list, int index) -> TraumaOccurence*;
  static void ClearTraumas(QQmlListProperty<TraumaOccurence>* list);
  static auto CountTraumas(QQmlListProperty<TraumaOccurence>* list) -> int;

  //! Helper functions for Traumas
  // static void AppendPhysiologyState(QQmlListProperty<QString>* list, QString* value);
  // static auto GetPhysiologyState(QQmlListProperty<QString>* list, int index) -> QString*;
  // static void ClearPhysiologyStates(QQmlListProperty<QString>* list);
  // static auto CountPhysiologyStates(QQmlListProperty<QString>* list) -> int;
};

#endif