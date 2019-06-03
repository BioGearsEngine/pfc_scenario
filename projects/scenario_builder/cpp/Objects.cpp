
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

#include "Objects.h"

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

struct ObjectSequence::Implementation {
  xsd::cxx::tree::sequence<::msdl_1::EquipmentItemType>* objects = nullptr;
  Implementation(xsd::cxx::tree::sequence<::msdl_1::EquipmentItemType>* i)
    : objects(i)
  {
  }
}; //End Struct Implementation

//-----------------------------------------------------------------------------
ObjectSequence::~ObjectSequence()
{
  _impl = nullptr;
}
//-----------------------------------------------------------------------------
ObjectSequence::ObjectSequence(QObject* parent)
  : QAbstractItemModel(parent)
  , _impl(std::make_unique<Implementation>(std::make_unique<xsd::cxx::tree::sequence<::msdl_1::EquipmentItemType>>().release()))
{
}
//-----------------------------------------------------------------------------
ObjectSequence::ObjectSequence(const ObjectSequence& obj)
  : QAbstractItemModel(nullptr)
  , _impl(std::make_unique<Implementation>(obj._impl->objects))
{
}
//-----------------------------------------------------------------------------
ObjectSequence::ObjectSequence(xsd::cxx::tree::sequence<::msdl_1::EquipmentItemType>& objects, QObject* parent)
  : QAbstractItemModel(parent)
  , _impl(std::make_unique<Implementation>(&objects))
{
  using namespace msdl_1;
}
//-----------------------------------------------------------------------------
ObjectSequence& ObjectSequence::operator=(const ObjectSequence& rhs)
{
  this->_impl = std::make_unique<Implementation>(rhs._impl->objects);
  return *this;
}
//-----------------------------------------------------------------------------
QModelIndex ObjectSequence::index(int row, int column, const QModelIndex& parent) const
{
  if (0 <= row && row < _impl->objects->size() && column == 0) {
    return createIndex(row, column, const_cast<ObjectSequence*>(this));
  } else {
    return QModelIndex();
  }
}
//-----------------------------------------------------------------------------
QModelIndex ObjectSequence::parent(const QModelIndex& index) const
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
int ObjectSequence::columnCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent)
  return 1;
}
//-----------------------------------------------------------------------------
int ObjectSequence::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent)
  return static_cast<int>(_impl->objects->size());
}
//-----------------------------------------------------------------------------
QVariant ObjectSequence::data(const QModelIndex& index, int role) const
{
  auto& objects = _impl->objects;
  if (index.row() < 0 || index.row() > objects->size()) {
    return QVariant();
  }

  auto& object = objects->at(index.row());
  QVariant result;
  switch (role) {
  case Qt::DisplayRole:
    result = QString(object.Name().get().c_str());
    break;
  case IdentityRole:
    result = QString(object.ObjectHandle().c_str());
    break;
  case SymbolRole:
    result = QString(object.SymbolIdentifier().c_str());
    break;
  case NameRole:
    result = QString(object.Name().get().c_str());
    break;
  case LatitudeRole:
    result = static_cast<double>(object.Disposition().Location().CoordinateData().GDC()->Latitude());
    break;
  case LongitudeRole:
    result = static_cast<double>(object.Disposition().Location().CoordinateData().GDC()->Longitude());
    break;
  case ElevationRole:
    result = static_cast<double>(object.Disposition().Location().CoordinateData().GDC()->ElevationAGL());
    break;
  case OrganicRelationsRole:
    result = QString(object.Relations().OrganicSuperiorHandle()->c_str());
    break;
  case DescriptionRole:
    result = QString(object.EquipmentSymbolModifiers()->AdditionalInfo()->c_str());
    break;
  default:
    result = QVariant();
  }
  return result;
}
//-----------------------------------------------------------------------------
Qt::ItemFlags ObjectSequence::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
//-----------------------------------------------------------------------------
bool ObjectSequence::setData(const QModelIndex& index, const QVariant& value, int role)
{
  auto& objects = _impl->objects;
  if (index.row() < 0 || index.row() > objects->size()) {
    return false;
  }

  auto& object = objects->at(index.row());
  switch (role) {
  case Qt::DisplayRole:
    if (object.Name().get() != value.toString().toStdString()) {
      object.Name(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case IdentityRole:
    if (object.ObjectHandle() != value.toString().toStdString()) {
      object.ObjectHandle(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case SymbolRole:
    if (object.SymbolIdentifier() != value.toString().toStdString()) {
      object.SymbolIdentifier(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case NameRole:
    if (object.Name().get() != value.toString().toStdString()) {
      object.Name(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case LatitudeRole:
    if (object.Disposition().Location().CoordinateData().GDC()->Latitude() != value.toDouble()) {
      double lat = value.toDouble();
      object.Disposition().Location().CoordinateData().GDC()->Latitude(lat);
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case LongitudeRole:
    if (object.Disposition().Location().CoordinateData().GDC()->Longitude() != value.toDouble()) {
      object.Disposition().Location().CoordinateData().GDC()->Longitude(value.toDouble());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case ElevationRole:
    if (object.Disposition().Location().CoordinateData().GDC()->ElevationAGL() != value.toDouble()) {
      object.Disposition().Location().CoordinateData().GDC()->ElevationAGL(value.toDouble());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case OrganicRelationsRole:
    if (object.Relations().OrganicSuperiorHandle().get() != value.toString().toStdString()) {
      object.Relations().OrganicSuperiorHandle(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case DescriptionRole:
    if (object.EquipmentSymbolModifiers()->AdditionalInfo().get() != value.toString().toStdString()) {
      object.EquipmentSymbolModifiers()->AdditionalInfo(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  default:
    return false;
  }
  emit objectModified();
  return true;
}
//-----------------------------------------------------------------------------
bool ObjectSequence::removeRows(int row, int count, const QModelIndex& parent)
{
  auto& objects = _impl->objects;
  beginRemoveRows(parent, row, row + count);
  if (row < objects->size() && row >= 0) {
    auto itr = objects->begin() + row;
    objects->erase(itr);
    emit objectDroped();
    emit countChanged();
    emit endRemoveRows();
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
bool ObjectSequence::insertRows(int row, int count, const QModelIndex& parent)
{
  static int newRowCount = 0;
  auto name = (newRowCount == 0) ? QString("New Object").toStdString()
                                 : QString("New Object %1").arg(newRowCount).toStdString();
  newRowCount++;
  auto uuid = boost::uuids::random_generator()();

  beginInsertRows(parent, row, row + count - 1);
  auto installation = std::make_unique<msdl_1::EquipmentItemType>(std::make_unique<msdl_1::EquipmentItemType::ObjectHandle_type>(boost::uuids::to_string(uuid)),
                                                                  std::make_unique<msdl_1::EquipmentItemType::SymbolIdentifier_type>(),
                                                                  std::make_unique<msdl_1::EquipmentItemType::Disposition_type>(
                                                                    std::make_unique<msdl_1::CoordinatesType>(
                                                                      std::make_unique<msdl_1::enumCoordinateSystemType>(msdl_1::enumCoordinateSystemType::GCC),
                                                                      std::make_unique<msdl_1::CoordinatesType::CoordinateData_type>())),
                                                                  std::make_unique<msdl_1::EquipmentRelationsType>(
                                                                    std::make_unique<msdl_1::OwnerType>(
                                                                      std::make_unique<msdl_1::OwnerType::OwnerChoice_type>(""),
                                                                      std::make_unique<msdl_1::OwnerType::OwnerData_type>())));

  installation->Relations().OrganicSuperiorHandle("");
  installation->EquipmentSymbolModifiers(std::make_unique<msdl_1::EquipmentSymbolModifiersType>(""));
  installation->EquipmentSymbolModifiers()->AdditionalInfo("");
  installation->Disposition().Location().CoordinateData().GDC(std::make_unique<msdl_1::GDCType>(1., 2., 3.));
  installation->Name(name);
  _impl->objects->push_back(std::move(installation));
  emit objectAdded();
  emit countChanged();
  emit endInsertRows();
  return true;
}
//-----------------------------------------------------------------------------
void ObjectSequence::new_object()
{
  insertRows(static_cast<int>(_impl->objects->size()), 1, QModelIndex());
}
//-----------------------------------------------------------------------------
void ObjectSequence::drop_object(int index)
{
  removeRows(index, 1, QModelIndex());
}
//-----------------------------------------------------------------------------
} //namespace pfc
