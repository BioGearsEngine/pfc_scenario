#include "Actors.h"

#include <string>

#include <QDebug>
#include <QDir>
#include <QRegularExpression>

#include <boost/uuid/uuid.hpp> // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp> // streaming operators etc.

#include "MilitaryScenario_1.0.0.hxx"
#include "MsdlComplexTypes_1.0.0.hxx"
#include <xsd/cxx/tree/date-time.hxx>

constexpr char default_affiliation_value[] = "Unknown";
constexpr char default_owner_choice_value[] = "FORCE_SIDE";
const auto default_owner_data_value = msdl_1::patternUUIDRef32("");

namespace pfc {

struct ActorSequence::Implementation {
  xsd::cxx::tree::sequence<::msdl_1::UnitType>* actors = nullptr;
  Implementation(xsd::cxx::tree::sequence<::msdl_1::UnitType>* i)
    : actors(i)
  {
  }
}; //End Struct Implementation

//-----------------------------------------------------------------------------
ActorSequence::~ActorSequence()
{
  _impl = nullptr;
}
//-----------------------------------------------------------------------------
ActorSequence::ActorSequence(QObject* parent)
  : QAbstractItemModel(parent)
  , _impl(std::make_unique<Implementation>(std::make_unique<xsd::cxx::tree::sequence<::msdl_1::UnitType>>().release()))
{
}
//-----------------------------------------------------------------------------
ActorSequence::ActorSequence(const ActorSequence& obj)
  : QAbstractItemModel(nullptr)
  , _impl(std::make_unique<Implementation>(obj._impl->actors))
{
}
//-----------------------------------------------------------------------------
ActorSequence::ActorSequence(xsd::cxx::tree::sequence<::msdl_1::UnitType>& actors, QObject* parent)
  : QAbstractItemModel(parent)
  , _impl(std::make_unique<Implementation>(&actors))
{
  using namespace msdl_1;
}
//-----------------------------------------------------------------------------
ActorSequence& ActorSequence::operator=(const ActorSequence& rhs)
{
  this->_impl = std::make_unique<Implementation>(rhs._impl->actors);
  return *this;
}
//-----------------------------------------------------------------------------
QModelIndex ActorSequence::index(int row, int column, const QModelIndex& parent) const
{
  if (0 <= row && row < _impl->actors->size() && column == 0) {
    return createIndex(row, column, const_cast<ActorSequence*>(this));
  } else {
    return QModelIndex();
  }
}
//-----------------------------------------------------------------------------
QModelIndex ActorSequence::parent(const QModelIndex& index) const
{
  if (!index.isValid())
    return QModelIndex();
  return QModelIndex();

  //TODO: In order to actually track children we need to move beyond a model of a flat model
  //ModelItem *childItem = static_cast<ModelItem*>( index.internalPointer() );
  //ModelItem *parentItem = childItem->parent();

  //if ( parentItem == rootItem )
  //    return QModelIndex();

  //return createIndex( parentItem->row(), 0, parentItem );
}
//-----------------------------------------------------------------------------
int ActorSequence::columnCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent)
  return 1;
}
//-----------------------------------------------------------------------------
int ActorSequence::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent)
  return static_cast<int>(_impl->actors->size());
}
//-----------------------------------------------------------------------------
QVariant ActorSequence::data(const QModelIndex& index, int role) const
{
  auto& actors = _impl->actors;
  if (index.row() < 0 || index.row() > actors->size()) {
    return QVariant();
  }

  auto& actor = actors->at(index.row());
  QVariant result;
  switch (role) {
  case Qt::DisplayRole:
    result = QString(actor.Name().get().c_str());
    break;
  case IdentityRole:
    result = QString(actor.ObjectHandle().c_str());
    break;
  case SymbolRole:
    result = QString(actor.SymbolIdentifier().c_str());
    break;
  case NameRole:
    result = QString(actor.Name().get().c_str());
    break;
  case LatitudeRole:
    result = static_cast<double>(actor.Disposition()->Location()->CoordinateData().GDC()->Latitude());
    break;
  case LongitudeRole:
    result = static_cast<double>(actor.Disposition()->Location()->CoordinateData().GDC()->Longitude());
    break;
  case ElevationRole:
    result = static_cast<double>(actor.Disposition()->Location()->CoordinateData().GDC()->ElevationAGL());
    break;
  case ForceRelationsRole:
    result = QString(actor.Relations().ForceRelation().ForceRelationData().ForceSideHandle()->c_str());
  break;
case OrganicRelationsRole:
    result = (actor.Relations().OrganicRelation()->OrganicRelationChoice() == msdl_1::enumForceOwnerType::UNIT) ? 
      QString(actor.Relations().OrganicRelation()->OrganicRelationData().OrganicSuperiorHandle()->c_str()) : 
      QString(actor.Relations().OrganicRelation()->OrganicRelationData().OrganicForceSideHandle()->c_str());
  break;
case SupportRelationsRole:
  //TODO:Implement this using column or something
  result = actor.Relations().SupportRelations()->SupportRelation().size();
  break;
case DescriptionRole:
  result = QString(actor.UnitSymbolModifiers()->AdditionalInfo()->c_str());
  break;
default:
  result = QVariant();
}
return result;
}
//-----------------------------------------------------------------------------
Qt::ItemFlags ActorSequence::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
//-----------------------------------------------------------------------------
bool ActorSequence::setData(const QModelIndex& index, const QVariant& value, int role)
{
  auto& actors = _impl->actors;
  if (index.row() < 0 || index.row() > actors->size()) {
    return false;
  }

  auto& actor = actors->at(index.row());
  switch (role) {
  case Qt::DisplayRole:
    if (actor.Name().get() != value.toString().toStdString()) {
      actor.Name(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case IdentityRole:
    if (actor.ObjectHandle() != value.toString().toStdString()) {
      actor.ObjectHandle(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case SymbolRole:
    if (actor.SymbolIdentifier() != value.toString().toStdString()) {
      actor.SymbolIdentifier(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case NameRole:
    if (actor.Name().get() != value.toString().toStdString()) {
      actor.Name(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case LatitudeRole:
    if (actor.Disposition()->Location()->CoordinateData().GDC()->Latitude() != value.toDouble()) {
      double lat = value.toDouble();
      actor.Disposition()->Location()->CoordinateData().GDC()->Latitude(lat);
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case LongitudeRole:
    if (actor.Disposition()->Location()->CoordinateData().GDC()->Longitude() != value.toDouble()) {
      actor.Disposition()->Location()->CoordinateData().GDC()->Longitude(value.toDouble());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case ElevationRole:
    if (actor.Disposition()->Location()->CoordinateData().GDC()->ElevationAGL() != value.toDouble()) {
      actor.Disposition()->Location()->CoordinateData().GDC()->ElevationAGL(value.toDouble());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case ForceRelationsRole:
    if (actor.Relations().ForceRelation().ForceRelationData().ForceSideHandle().get() != value.toString().toStdString()) {
      actor.Relations().ForceRelation().ForceRelationData().ForceSideHandle(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
  case OrganicRelationsRole:
    if (actor.Relations().OrganicRelation()->OrganicRelationChoice() == msdl_1::enumForceOwnerType::UNIT) {
      if (actor.Relations().OrganicRelation()->OrganicRelationData().OrganicSuperiorHandle().get() != value.toString().toStdString()) {
        actor.Relations().OrganicRelation()->OrganicRelationData().OrganicSuperiorHandle(value.toString().toStdString());
        emit dataChanged(index, index, { Qt::EditRole, role });
      }
    } else {
      if (actor.Relations().OrganicRelation()->OrganicRelationData().OrganicForceSideHandle().get() != value.toString().toStdString()) {
        actor.Relations().OrganicRelation()->OrganicRelationData().OrganicForceSideHandle(value.toString().toStdString());
        emit dataChanged(index, index, { Qt::EditRole, role });
      }
    }
    break;
  case SupportRelationsRole:
    //TODO:Implement this using column or something
    //actor.Relations().SupportRelations()->SupportRelation();
    break;
  case DescriptionRole:
    if (actor.UnitSymbolModifiers()->AdditionalInfo().get() != value.toString().toStdString()) {
      actor.UnitSymbolModifiers()->AdditionalInfo(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  default:
    return false;
  }
  emit actorModified();
  return true;
}
//-----------------------------------------------------------------------------
bool ActorSequence::removeRows(int row, int count, const QModelIndex& parent)
{
  auto& actors = _impl->actors;
  beginRemoveRows(parent, row, row + count);
  if (row < actors->size() && row >= 0) {
    auto itr = actors->begin() + row;
    actors->erase(itr);
    emit actorDroped();
    emit countChanged();
    emit endRemoveRows();
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
bool ActorSequence::insertRows(int row, int count, const QModelIndex& parent)
{
  static int newRowCount = 0;
  auto name = (newRowCount == 0) ? QString("New Actor").toStdString()
                                 : QString("New Actor %1").arg(newRowCount).toStdString();
  newRowCount++;
  auto uuid = boost::uuids::random_generator()();

  beginInsertRows(parent, row, row + count - 1);
  //UnitType(const ObjectHandle_type&,
  //         const SymbolIdentifier_type&,
  //         const Relations_type&);
  auto actor = std::make_unique<msdl_1::UnitType>(std::make_unique<msdl_1::UnitType::ObjectHandle_type>(boost::uuids::to_string(uuid)),
                                                  std::make_unique<msdl_1::UnitType::SymbolIdentifier_type>(),
                                                  std::make_unique<msdl_1::UnitRelationsType>(
                                                    std::make_unique<msdl_1::ForceRelationType>(
                                                      std::make_unique<msdl_1::enumForceOwnerType>(msdl_1::enumForceOwnerType::FORCE_SIDE),
                                                      std::make_unique<::msdl_1::ForceRelationDataType>())));

  actor->Disposition(std::make_unique<msdl_1::UnitDispositionType>());
  actor->Disposition()->Location(std::make_unique<msdl_1::CoordinatesType>(
    std::make_unique<msdl_1::enumCoordinateSystemType>(msdl_1::enumCoordinateSystemType::GDC),
    std::make_unique<msdl_1::CoordinatePointType>()));
  actor->Disposition()->Location()->CoordinateData().GDC(msdl_1::GDCType(0., 0., 0.));

  actor->Relations().OrganicRelation(std::make_unique<msdl_1::OrganicRelationType>(
    std::make_unique<msdl_1::OrganicRelationDataType>(),
    std::make_unique<msdl_1::enumForceOwnerType>(msdl_1::enumForceOwnerType::FORCE_SIDE)));
  actor->Relations().OrganicRelation()->OrganicRelationChoice(msdl_1::enumForceOwnerType::FORCE_SIDE);
  actor->Relations().OrganicRelation()->OrganicRelationData().OrganicSuperiorHandle("");
  actor->Relations().OrganicRelation()->OrganicRelationData().OrganicForceSideHandle("");
  actor->Relations().ForceRelation(std::make_unique<msdl_1::ForceRelationType>(
    std::make_unique<msdl_1::enumForceOwnerType>(msdl_1::enumForceOwnerType::FORCE_SIDE),
    std::make_unique<msdl_1::ForceRelationDataType>()));
  actor->Relations().ForceRelation().ForceRelationData().ForceSideHandle("");
  actor->Relations().SupportRelations(std::make_unique<msdl_1::SupportRelationsType>());

  actor->UnitSymbolModifiers(std::make_unique<msdl_1::UnitSymbolModifiersType>(""));
  actor->UnitSymbolModifiers()->AdditionalInfo("");
  actor->Name(name);
  _impl->actors->push_back(std::move(actor));
  emit actorAdded();
  emit countChanged();
  emit endInsertRows();
  return true;
}
//-----------------------------------------------------------------------------
void ActorSequence::new_actor()
{
  insertRows(static_cast<int>(_impl->actors->size()), 1, QModelIndex());
}
//-----------------------------------------------------------------------------
void ActorSequence::drop_actor(int index)
{
  removeRows(index, 1, QModelIndex());
}
//-----------------------------------------------------------------------------
} //namespace pfc
