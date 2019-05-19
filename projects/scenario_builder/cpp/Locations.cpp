#include "Locations.h"

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

struct LocationSequence::Implementation {
  xsd::cxx::tree::sequence<::msdl_1::InstallationType>* installations = nullptr;
  Implementation(xsd::cxx::tree::sequence<::msdl_1::InstallationType>* i)
    : installations(i)
  {
  }
}; //End Struct Implementation

//-----------------------------------------------------------------------------
LocationSequence::~LocationSequence()
{
  _impl = nullptr;
}
//-----------------------------------------------------------------------------
LocationSequence::LocationSequence(QObject* parent)
  : QAbstractItemModel(parent)
  , _impl(std::make_unique<Implementation>(std::make_unique<xsd::cxx::tree::sequence<::msdl_1::InstallationType>>().release()))
{
}
//-----------------------------------------------------------------------------
LocationSequence::LocationSequence(const LocationSequence& obj)
  : QAbstractItemModel(nullptr)
  , _impl(std::make_unique<Implementation>(obj._impl->installations))
{
}
//-----------------------------------------------------------------------------
LocationSequence::LocationSequence(xsd::cxx::tree::sequence<::msdl_1::InstallationType>& installations, QObject* parent)
  : QAbstractItemModel(parent)
  , _impl(std::make_unique<Implementation>(&installations))
{
  using namespace msdl_1;
}
//-----------------------------------------------------------------------------
LocationSequence& LocationSequence::operator=(const LocationSequence& rhs)
{
  this->_impl = std::make_unique<Implementation>(rhs._impl->installations);
  return *this;
}
//-----------------------------------------------------------------------------
QModelIndex LocationSequence::index(int row, int column, const QModelIndex& parent) const
{
  if (0 <= row && row < _impl->installations->size() && column == 0) {
    return createIndex(row, column, const_cast<LocationSequence*>(this));
  } else {
    return QModelIndex();
  }
}
//-----------------------------------------------------------------------------
QModelIndex LocationSequence::parent(const QModelIndex& index) const
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
int LocationSequence::columnCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent)
  return 1;
}
//-----------------------------------------------------------------------------
int LocationSequence::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent)
  return static_cast<int>(_impl->installations->size());
}
//-----------------------------------------------------------------------------
QVariant LocationSequence::data(const QModelIndex& index, int role) const
{
  auto& installations = _impl->installations;
  if (index.row() < 0 || index.row() > installations->size()) {
    return QVariant();
  }

  auto& location = installations->at(index.row());
  QVariant result;
  switch (role) {
  case Qt::DisplayRole:
    result = QString(location.Name().get().c_str());
    break;
  case IdentityRole:
    result = QString(location.ObjectHandle().c_str());
    break;
  case SymbolRole:
    result = QString(location.SymbolIdentifier().c_str());
    break;
  case NameRole:
    result = QString(location.Name().get().c_str());
    break;
  case OwnerRole:
    result = (location.Owner().OwnerChoice() == msdl_1::enumForceOwnerType::FORCE_SIDE) ? 
      QString(location.Owner().OwnerData().ForceOwnerHandle().get().c_str()) : 
      QString(location.Owner().OwnerData().UnitOwnerHandle().get().c_str());
    break;
  case AffiliationRole:
    result = QString(location.Affiliation().c_str());
    break;
  case LatitudeRole:
    result = static_cast<double>(location.Location().CoordinateData().GDC()->Latitude());
    break;
  case LongitudeRole:
    result = static_cast<double>(location.Location().CoordinateData().GDC()->Longitude());
    break;
  case ElevationRole:
    result = static_cast<double>(location.Location().CoordinateData().GDC()->ElevationAGL());
    break;
  case DescriptionRole:
    result = QString(location.InstallationSymbolModifiers()->AdditionalInfo()->c_str());
    break;
  default:
    result = QVariant();
  }
  return result;
}
//-----------------------------------------------------------------------------
Qt::ItemFlags LocationSequence::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
//-----------------------------------------------------------------------------
bool LocationSequence::setData(const QModelIndex& index, const QVariant& value, int role)
{
  auto& installations = _impl->installations;
  if (index.row() < 0 || index.row() > installations->size()) {
    return false;
  }

  auto& location = installations->at(index.row());
  switch (role) {
  case Qt::DisplayRole:
    if (location.Name().get() != value.toString().toStdString()) {
      location.Name(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case IdentityRole:
    if (location.ObjectHandle() != value.toString().toStdString()) {
      location.ObjectHandle(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case SymbolRole:
    if (location.SymbolIdentifier() != value.toString().toStdString()) {
      location.SymbolIdentifier(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case NameRole:
    if (location.Name().get() != value.toString().toStdString()) {
      location.Name(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case OwnerRole:
    if (location.Owner().OwnerChoice() == msdl_1::enumForceOwnerType::FORCE_SIDE) {
      if (location.Owner().OwnerData().ForceOwnerHandle().get() != value.toString().toStdString()) {
        location.Owner().OwnerData().ForceOwnerHandle(value.toString().toStdString());
        emit dataChanged(index, index, { Qt::EditRole, role });
      }
    } else {
      if (location.Owner().OwnerData().UnitOwnerHandle()) {
        location.Owner().OwnerData().UnitOwnerHandle(value.toString().toStdString());
        emit dataChanged(index, index, { Qt::EditRole, role });
      }
    }
    break;
  case AffiliationRole:
    if (location.Affiliation() != value.toString().toStdString()) {
      location.Affiliation(std::make_unique<msdl_1::InstallationType::Affiliation_type>(value.toString().toStdString()));
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case LatitudeRole:
    if (location.Location().CoordinateData().GDC()->Latitude() != value.toDouble()) {
      double lat = value.toDouble();
      location.Location().CoordinateData().GDC()->Latitude(lat);
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case LongitudeRole:
    if (location.Location().CoordinateData().GDC()->Longitude() != value.toDouble()) {
      location.Location().CoordinateData().GDC()->Longitude(value.toDouble());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case ElevationRole:
    if (location.Location().CoordinateData().GDC()->ElevationAGL() != value.toDouble()) {
      location.Location().CoordinateData().GDC()->ElevationAGL(value.toDouble());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  case DescriptionRole:
    if (location.InstallationSymbolModifiers()->AdditionalInfo().get() != value.toString().toStdString()) {
      location.InstallationSymbolModifiers()->AdditionalInfo(value.toString().toStdString());
      emit dataChanged(index, index, { Qt::EditRole, role });
    }
    break;
  default:
    return false;
  }
  emit locationModified();
  return true;
}
//-----------------------------------------------------------------------------
bool LocationSequence::removeRows(int row, int count, const QModelIndex& parent)
{
  auto& installations = _impl->installations;
  beginRemoveRows(parent, row, row + count);
  if (row < installations->size() && row >= 0) {
    auto itr = installations->begin() + row;
    installations->erase(itr);
    emit locationDroped();
    emit countChanged();
    emit endRemoveRows();
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
bool LocationSequence::insertRows(int row, int count, const QModelIndex& parent)
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
  emit locationAdded();
  emit countChanged();
  emit endInsertRows();
  return true;
}
//-----------------------------------------------------------------------------
void LocationSequence::new_location()
{
  insertRows(static_cast<int>(_impl->installations->size()), 1, QModelIndex());
}
//-----------------------------------------------------------------------------
void LocationSequence::drop_location(int index)
{
  removeRows(index, 1, QModelIndex());
}
//-----------------------------------------------------------------------------
} //namespace pfc
