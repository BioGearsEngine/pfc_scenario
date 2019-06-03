#ifndef PFC_SCENARIO_BUILDER_ACTOR_H
#define PFC_SCENARIO_BUILDER_ACTOR_H

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

#include <QAbstractItemModel>
#include <QDate>
#include <QDebug>
#include <QElapsedTimer>
#include <QString>
#include <QVariant>

#include "MilitaryScenario_1.0.0.hxx"
#include "MsdlComplexTypes_1.0.0.hxx"

namespace pfc {

class ActorSequence : public QAbstractItemModel {
  Q_OBJECT

  Q_PROPERTY(int32_t count READ rowCount() NOTIFY countChanged)

public:
  ActorSequence(QObject* parent = nullptr);
  virtual ~ActorSequence();
  ActorSequence(::xsd::cxx::tree::sequence<msdl_1::UnitType>&, QObject* parent = nullptr);
  ActorSequence(const ActorSequence&);
  ActorSequence(ActorSequence&&) = default;
  ActorSequence& operator=(const ActorSequence&);
  ActorSequence& operator=(ActorSequence&&) = default;

  enum ActorRoles {
    IdentityRole = Qt::UserRole + 1,
    SymbolRole,
    NameRole,
    LatitudeRole,
    LongitudeRole,
    ElevationRole,
    ForceRelationsRole,
    OrganicRelationsRole,
    SupportRelationsRole,
    DescriptionRole,
  };

  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex& index) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
  bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
  bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

public slots:
  void new_actor();
  void drop_actor(int);

signals:
  void countChanged();
  int32_t actorAdded();
  int32_t actorDroped();
  int32_t actorModified();

protected:
  QHash<int, QByteArray> roleNames() const
  {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "content";
    roles[Qt::DecorationRole] = "decoration";
    roles[Qt::EditRole] = "edit";
    roles[Qt::ToolTipRole] = "toolTip";
    roles[Qt::StatusTipRole] = "statusTip";
    roles[Qt::WhatsThisRole] = "whatsThis";
    roles[IdentityRole] = "id";
    roles[SymbolRole] = "symbol";
    roles[NameRole] = "name";
    roles[LatitudeRole] = "lat";
    roles[LongitudeRole] = "lon";
    roles[ElevationRole] = "alt";
    roles[ForceRelationsRole] = "forceRelation";
    roles[OrganicRelationsRole] = "organicRelation";
    roles[SupportRelationsRole] = "supportRelation";
    roles[DescriptionRole] = "description";

    return roles;
  }

private:
  struct Implementation;
  std::unique_ptr<Implementation> _impl;

};//class Actor
}//namespace pfc
#endif //PFC_SCENARIO_BUILDER_ACTOR_H
