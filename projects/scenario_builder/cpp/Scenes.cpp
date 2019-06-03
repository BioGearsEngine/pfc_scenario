
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

#include "Scenes.h"

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

struct NarativeSequence::Implementation {
  xsd::cxx::tree::sequence<::msdl_1::InstallationType>* installations = nullptr;
  Implementation(xsd::cxx::tree::sequence<::msdl_1::InstallationType>* i)
    : installations(i)
  {
  }
}; //End Struct Implementation

//-----------------------------------------------------------------------------
NarativeSequence::~NarativeSequence()
{
  _impl = nullptr;
}
//-----------------------------------------------------------------------------
NarativeSequence::NarativeSequence(QObject* parent)
  : QAbstractItemModel(parent)
  , _impl(std::make_unique<Implementation>(std::make_unique<xsd::cxx::tree::sequence<::msdl_1::InstallationType>>().release()))
{
}
//-----------------------------------------------------------------------------
NarativeSequence::NarativeSequence(const NarativeSequence& obj)
  : QAbstractItemModel(nullptr)
  , _impl(std::make_unique<Implementation>(obj._impl->installations))
{
}
//-----------------------------------------------------------------------------
NarativeSequence::NarativeSequence(xsd::cxx::tree::sequence<::msdl_1::InstallationType>& installations, QObject* parent)
  : QAbstractItemModel(parent)
  , _impl(std::make_unique<Implementation>(&installations))
{
  using namespace msdl_1;
}
//-----------------------------------------------------------------------------
NarativeSequence& NarativeSequence::operator=(const NarativeSequence& rhs)
{
  this->_impl = std::make_unique<Implementation>(rhs._impl->installations);
  return *this;
}
//-----------------------------------------------------------------------------
QModelIndex NarativeSequence::index(int row, int column, const QModelIndex& parent) const
{
  if (0 <= row && row < _impl->installations->size() && column == 0) {
    return createIndex(row, column, const_cast<NarativeSequence*>(this));
  } else {
    return QModelIndex();
  }
}
//-----------------------------------------------------------------------------
QModelIndex NarativeSequence::parent(const QModelIndex& index) const
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
int NarativeSequence::columnCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent)
  return 1;
}
//-----------------------------------------------------------------------------
int NarativeSequence::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent)
  return static_cast<int>(_impl->installations->size());
}
//-----------------------------------------------------------------------------
QVariant NarativeSequence::data(const QModelIndex& index, int role) const
{
  auto& installations = _impl->installations;
  if (index.row() < 0 || index.row() > installations->size()) {
    return QVariant();
  }

  auto& scene = installations->at(index.row());
  QVariant result;
  switch (role) {
  case Qt::DisplayRole:
    result = QString(scene.Name().get().c_str());
    break;
  case IdentityRole:
    result = QString(scene.ObjectHandle().c_str());
    break;
  case SymbolRole:
    result = QString(scene.SymbolIdentifier().c_str());
    break;
  case NameRole:
    result = QString(scene.Name().get().c_str());
    break;
  case OwnerRole:
    result = (scene.Owner().OwnerChoice() == msdl_1::enumForceOwnerType::FORCE_SIDE) ? QString(scene.Owner().OwnerData().ForceOwnerHandle().get().c_str()) : QString(scene.Owner().OwnerData().UnitOwnerHandle().get().c_str());
    break;
  case AffiliationRole:
    result = QString(scene.Affiliation().c_str());
    break;
  case LatitudeRole:
    result = static_cast<double>(scene.Location().CoordinateData().GDC()->Latitude());
    break;
  case LongitudeRole:
    result = static_cast<double>(scene.Location().CoordinateData().GDC()->Longitude());
    break;
  case ElevationRole:
    result = static_cast<double>(scene.Location().CoordinateData().GDC()->ElevationAGL());
    break;
  case DescriptionRole:
    result = QString(scene.InstallationSymbolModifiers()->AdditionalInfo()->c_str());
    break;
  default:
    result = QVariant();
  }
  return result;
}
//-----------------------------------------------------------------------------
Qt::ItemFlags NarativeSequence::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
//-----------------------------------------------------------------------------
bool NarativeSequence::setData(const QModelIndex& index, const QVariant& value, int role)
{
  auto& installations = _impl->installations;
  if (index.row() < 0 || index.row() > installations->size()) {
    return false;
  }

  auto& scene = installations->at(index.row());
  switch (role) {
  case Qt::DisplayRole:
    if (scene.Name().get() != value.toString().toStdString()) {
      scene.Name(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case IdentityRole:
    if (scene.ObjectHandle() != value.toString().toStdString()) {
      scene.ObjectHandle(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case SymbolRole:
    if (scene.SymbolIdentifier() != value.toString().toStdString()) {
      scene.SymbolIdentifier(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case NameRole:
    if (scene.Name().get() != value.toString().toStdString()) {
      scene.Name(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case OwnerRole:
    if (scene.Owner().OwnerChoice() == msdl_1::enumForceOwnerType::FORCE_SIDE) {
      if (scene.Owner().OwnerData().ForceOwnerHandle().get() != value.toString().toStdString()) {
        scene.Owner().OwnerData().ForceOwnerHandle(value.toString().toStdString());
        emit dataChanged(index, index, { Qt::EditRole, role });
      }
    } else {
      if (scene.Owner().OwnerData().UnitOwnerHandle()) {
        scene.Owner().OwnerData().UnitOwnerHandle(value.toString().toStdString());
        emit dataChanged(index, index, { Qt::EditRole, role });
      }
    }
    break;
  case AffiliationRole:
    if (scene.Affiliation() != value.toString().toStdString()) {
      scene.Affiliation(std::make_unique<msdl_1::InstallationType::Affiliation_type>(value.toString().toStdString()));
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case LatitudeRole:
    if (scene.Location().CoordinateData().GDC()->Latitude() != value.toDouble()) {
      double lat = value.toDouble();
      scene.Location().CoordinateData().GDC()->Latitude(lat);
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case LongitudeRole:
    if (scene.Location().CoordinateData().GDC()->Longitude() != value.toDouble()) {
      scene.Location().CoordinateData().GDC()->Longitude(value.toDouble());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case ElevationRole:
    if (scene.Location().CoordinateData().GDC()->ElevationAGL() != value.toDouble()) {
      scene.Location().CoordinateData().GDC()->ElevationAGL(value.toDouble());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case DescriptionRole:
    if (scene.InstallationSymbolModifiers()->AdditionalInfo().get() != value.toString().toStdString()) {
      scene.InstallationSymbolModifiers()->AdditionalInfo(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  default:
    return false;
  }
  emit sceneModified();
  return true;
}
//-----------------------------------------------------------------------------
bool NarativeSequence::removeRows(int row, int count, const QModelIndex& parent)
{
  auto& installations = _impl->installations;
  beginRemoveRows(parent, row, row + count);
  if (row < installations->size() && row >= 0) {
    auto itr = installations->begin() + row;
    installations->erase(itr);
    emit sceneDroped();
    emit countChanged();
    emit endRemoveRows();
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
bool NarativeSequence::insertRows(int row, int count, const QModelIndex& parent)
{
  static int newRowCount = 0;
  auto name = (newRowCount == 0) ? QString("New Location").toStdString()
                                 : QString("New Location %1").arg(newRowCount).toStdString();
  newRowCount++;
  auto uuid = boost::uuids::random_generator()();

  beginInsertRows(parent, row, row + count - 1);
  auto installation = std::make_unique<msdl_1::InstallationType>(std::make_unique<msdl_1::InstallationType::ObjectHandle_type>(boost::uuids::to_string(uuid)),
                                                                 std::make_unique<msdl_1::InstallationType::SymbolIdentifier_type>(),
                                                                 std::make_unique<msdl_1::InstallationType::Affiliation_type>(default_affiliation_value),
                                                                 std::make_unique<msdl_1::InstallationType::Owner_type>(std::make_unique<msdl_1::OwnerType::OwnerChoice_type>(default_owner_choice_value),
                                                                                                                        std::make_unique<msdl_1::OwnerType::OwnerData_type>()),
                                                                 std::make_unique<msdl_1::InstallationType::Location_type>(std::make_unique<msdl_1::enumCoordinateSystemType>(msdl_1::enumCoordinateSystemType::GCC),
                                                                                                                           std::make_unique<msdl_1::CoordinatesType::CoordinateData_type>()));

  installation->Owner().OwnerData().ForceOwnerHandle(default_owner_data_value);
  installation->Owner().OwnerData().UnitOwnerHandle(default_owner_data_value);
  installation->InstallationSymbolModifiers(std::make_unique<msdl_1::InstallationSymbolModifiersType>(""));
  installation->InstallationSymbolModifiers()->AdditionalInfo("");
  installation->Location().CoordinateData().GDC(std::make_unique<msdl_1::GDCType>(1., 2., 3.));
  installation->Name(name);
  _impl->installations->push_back(std::move(installation));
  emit sceneAdded();
  emit countChanged();
  emit endInsertRows();
  return true;
}
//-----------------------------------------------------------------------------
void NarativeSequence::new_scene()
{
  insertRows(static_cast<int>(_impl->installations->size()), 1, QModelIndex());
}
//-----------------------------------------------------------------------------
void NarativeSequence::drop_scene(int index)
{
  removeRows(index, 1, QModelIndex());
}
//-----------------------------------------------------------------------------
} //namespace pfc
