
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

#include "Scenario.h"

#include <fstream>
#include <iostream>

#include <QDebug>
#include <QDir>
#include <QRegularExpression>
#include <QUrl>

#include "MilitaryScenario_1.0.0.hxx"
#include "MsdlComplexTypes_1.0.0.hxx"
#include <xsd/cxx/tree/date-time.hxx>

#include "Actors.h"
#include "Locations.h"
#include "Objects.h"
#include "Scenes.h"

namespace pfc {

struct Scenario::Implementation {
  std::unique_ptr<msdl_1::MilitaryScenarioType> scenario;

  LocationSequence locations;
  ActorSequence actors;
  ObjectSequence objects;
  NarativeSequence scenes;

  QString path;
  QString filepath;
}; //End Struct Implementation

Scenario::Scenario(QObject* parent)
  : QAbstractItemModel(parent)
  , _impl(std::make_unique<Implementation>())
{
  reset();
}
//-----------------------------------------------------------------------------
Scenario::~Scenario()
{
  _impl = nullptr;
}
//-----------------------------------------------------------------------------
Scenario::Scenario(std::string filename, QObject* parent)
  : QAbstractItemModel(parent)
  , _impl(std::make_unique<Implementation>())
{
  using namespace msdl_1;
  _impl->scenario = MilitaryScenario(filename);
}
//-----------------------------------------------------------------------------
Scenario::Scenario(std::string path, std::string filename, QObject* parent)
  : QAbstractItemModel(parent)
{
  using namespace msdl_1;
  _impl->scenario = MilitaryScenario(path + "/" + filename);
}
//-----------------------------------------------------------------------------
QString Scenario::Name() const
{ 
  return _impl->scenario->ScenarioID().name().c_str();
}
//-----------------------------------------------------------------------------
QString Scenario::Type() const
{
  return _impl->scenario->ScenarioID().type().c_str();
}
//-----------------------------------------------------------------------------
QString Scenario::Version() const
{
  return _impl->scenario->ScenarioID().version().c_str();
}
//-----------------------------------------------------------------------------
QDate Scenario::ModificationDate() const
{

  return QDate(_impl->scenario->ScenarioID().modificationDate().year(), _impl->scenario->ScenarioID().modificationDate().month(), _impl->scenario->ScenarioID().modificationDate().day());
}
//-----------------------------------------------------------------------------
QString Scenario::SecurityClassification() const
{
  if (!_impl->scenario->ScenarioID().securityClassification().idtag().present()) {
    _impl->scenario->ScenarioID().securityClassification().idtag("");
  }
  return _impl->scenario->ScenarioID().securityClassification().idtag().get().c_str();
}
//-----------------------------------------------------------------------------
QString Scenario::ReleaseRestriction() const
{
  if (_impl->scenario->ScenarioID().releaseRestriction().empty()) {
    _impl->scenario->ScenarioID().releaseRestriction().push_back("None");
  }
  return _impl->scenario->ScenarioID().releaseRestriction().begin()->c_str();
}
//-----------------------------------------------------------------------------
QString Scenario::Purpose() const
{
  if (!_impl->scenario->ScenarioID().purpose().present()) {
    _impl->scenario->ScenarioID().purpose("");
  }
  return _impl->scenario->ScenarioID().purpose().get().c_str();
}
//-----------------------------------------------------------------------------
QString Scenario::Description() const
{
  return _impl->scenario->ScenarioID().description().c_str();
}
//-----------------------------------------------------------------------------
QString Scenario::UseLimitation() const
{
  if (!_impl->scenario->ScenarioID().useLimitation().present()) {
    _impl->scenario->ScenarioID().useLimitation(std::make_unique<schemas::modelID::modelIdentificationType::useLimitation_type>(""));
  }
  return _impl->scenario->ScenarioID().useLimitation().get().c_str();
}
//-----------------------------------------------------------------------------
void Scenario::Name(QString name)
{
  _impl->scenario->ScenarioID().name(name.toStdString());
}
//-----------------------------------------------------------------------------
void Scenario::Type(QString type)
{
  _impl->scenario->ScenarioID().type(type.toStdString());
}
//-----------------------------------------------------------------------------
void Scenario::Version(QString version)
{
  _impl->scenario->ScenarioID().version(version.toStdString());
}
//-----------------------------------------------------------------------------
bool Scenario::ModificationDate(QDate date)
{
  if (date.isValid()) {
    _impl->scenario->ScenarioID().modificationDate().year(date.year());
    _impl->scenario->ScenarioID().modificationDate().month(date.month());
    _impl->scenario->ScenarioID().modificationDate().day(date.day());
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
bool Scenario::ModificationDate(int year, unsigned short month, unsigned short day)
{
  //TODO:Range Validation
  if (year != 0) {
    _impl->scenario->ScenarioID().modificationDate().year(year);
    _impl->scenario->ScenarioID().modificationDate().month(month);
    _impl->scenario->ScenarioID().modificationDate().day(day);
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
void Scenario::SecurityClassification(QString classification)
{
  if (!_impl->scenario->ScenarioID().securityClassification().idtag().present()) {
    _impl->scenario->ScenarioID().securityClassification().idtag("");
  }
  _impl->scenario->ScenarioID().securityClassification().idtag(classification.toStdString());
}
//-----------------------------------------------------------------------------
void Scenario::ReleaseRestriction(QString rr)
{
  //TODO:Expand to support list of Restrictiosn
  if (_impl->scenario->ScenarioID().releaseRestriction().empty()) {
    _impl->scenario->ScenarioID().releaseRestriction().push_back(rr.toStdString());
  } else {
    *_impl->scenario->ScenarioID().releaseRestriction().begin() = (rr.toStdString());
  }
}
//-----------------------------------------------------------------------------
void Scenario::Purpose(QString purpose)
{
  _impl->scenario->ScenarioID().purpose(purpose.toStdString());
}
//-----------------------------------------------------------------------------
void Scenario::Description(QString description)
{
  _impl->scenario->ScenarioID().description(description.toStdString());
}
//-----------------------------------------------------------------------------
void Scenario::UseLimitation(QString limitation)
{
  _impl->scenario->ScenarioID().useLimitation(limitation.toStdString());
}
//-----------------------------------------------------------------------------
//!
//!  Conversts Data String to Valid Class Implemetnation Through XSD
//!
bool Scenario::marshal(QDataStream&)
{

  //TODO: XSD Map QDataStream to an StringStream then pass to XSD
  return false;
}
//-----------------------------------------------------------------------------
//!
//!  Conversts Class Implemetnation to Data Stream
//!
bool Scenario::unmarshal(QDataStream&)
{
  //TODO: XSD Map QDataStream to an StringStream then pass to XSD
  return false;
}
//-----------------------------------------------------------------------------
bool Scenario::Load()
{
  using namespace msdl_1;
  auto load_directory = QDir(_impl->path);
  auto load_path = load_directory.filePath(_impl->filepath);
  auto input = std::ifstream(load_path.toStdString());
  if (input.is_open()) {
    try {
    auto temp_scenario= msdl_1::MilitaryScenario(input);
      _impl->scenario = std::move(temp_scenario);
      //TODO: Refactor Sequence Storage
      //For testing the Sqeuence class use the underlying XML as a database, but this is going to be problimatic eventually
      //One day these groups will use sub fields to determine if they should be viewed or editable and so I feel we need to
      //Duplicate represnetation in either a sqlite database or a series of structs and allow cross-refernece
      //I'm leaning towards sqlite because it allows us to do forign key look up and simplify tables
      //Then on Marshall UnMarshall operations we simply mapp our SQL schema to the XML schema.

      _impl->locations = LocationSequence(_impl->scenario->Installations()->Installation());
      _impl->actors = ActorSequence(_impl->scenario->Organizations()->Units().Unit());
      _impl->objects = ObjectSequence(_impl->scenario->Organizations()->Equipment()->EquipmentItem());
      _impl->scenes = NarativeSequence(_impl->scenario->Installations()->Installation());
      return true;
    } catch (::xsd::cxx::tree::parsing<char> e) {
      std::cout << e;
    } catch (std::exception e) {
      std::cout << e.what();
      return false;
    }
  }
  return false;
 
}
//-----------------------------------------------------------------------------
bool Scenario::Load(QUrl uri)
{
  auto file = QFileInfo(uri.toLocalFile());
  _impl->path = file.dir().path();
  _impl->filepath = file.fileName();
  Load();
  return false;
}
//-----------------------------------------------------------------------------
bool Scenario::Save()
{
  auto save_directory = QDir(_impl->path);
  auto save_path = save_directory.filePath(_impl->filepath);
  if (QDir(save_path).exists()) {
    xml_schema::namespace_infomap map;


    map["lib"].name = "urn:sisostds:scenario:military:data:draft:msdl:1";
    map["lib"].schema = "MilitaryScenario_1.0.0.xsd";

    auto out = std::ofstream(save_path.toStdString());

    std::string e;
    msdl_1::MilitaryScenario(out, *_impl->scenario, map);
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
bool Scenario::SaveAs(QUrl filename)
{
  auto file = QFileInfo(filename.toLocalFile());
  QFileInfo path = file.path();
  if ( path.exists() && path.isDir() ) {
    xml_schema::namespace_infomap map;

    map["lib"].name = "urn:sisostds:scenario:military:data:draft:msdl:1";
    map["lib"].schema = "MilitaryScenario_1.0.0.xsd";

    auto out = std::ofstream(filename.toLocalFile().toStdString());
    if (out.is_open()) {
      std::string e;
      msdl_1::MilitaryScenario(out, *_impl->scenario, map);
      return true;
    }
  }
  return false;
}
//-----------------------------------------------------------------------------
void Scenario::reset()
{
  using namespace msdl_1;
  _impl->scenario = std::make_unique<msdl_1::MilitaryScenarioType>(
    std::make_unique<MilitaryScenarioType::ScenarioID_type>(schemas::modelID::modelIdentificationType(
      schemas::modelID::modelIdentificationType::name_type("New Scenario"),
      schemas::modelID::modelIdentificationType::type_type("Military Scenario"),
      schemas::modelID::modelIdentificationType::version_type("0.0.0"),
      schemas::modelID::modelIdentificationType::modificationDate_type(schemas::modelID::modificationDate(xml_schema::date(QDate::currentDate().year(), QDate::currentDate().month(), QDate::currentDate().day()))),
      schemas::modelID::modelIdentificationType::securityClassification_type("Unclassified"),
      schemas::modelID::modelIdentificationType::description_type())),
    std::make_unique<MilitaryScenarioType::Options_type>(std::make_unique<OptionsType::MSDLVersion_type>()),
    std::make_unique<MilitaryScenarioType::ForceSides_type>());
  //TODO:Assign Defaults

  _impl->scenario->Installations(std::make_unique<msdl_1::InstallationsType>());
  _impl->scenario->Organizations(std::make_unique<msdl_1::OrganizationsType>(std::make_unique<msdl_1::UnitsType>()));
  _impl->scenario->Organizations()->Equipment(std::make_unique<msdl_1::EquipmentType>());

  //TODO: Refactor Sequence Storage
  //For testing the Sqeuence class use the underlying XML as a database, but this is going to be problimatic eventually
  //One day these groups will use sub fields to determine if they should be viewed or editable and so I feel we need to
  //Duplicate represnetation in either a sqlite database or a series of structs and allow cross-refernece
  //I'm leaning towards sqlite because it allows us to do forign key look up and simplify tables
  //Then on Marshall UnMarshall operations we simply mapp our SQL schema to the XML schema.

  _impl->locations = LocationSequence(_impl->scenario->Installations()->Installation());
  _impl->actors = ActorSequence(_impl->scenario->Organizations()->Units().Unit());
  _impl->objects = ObjectSequence(_impl->scenario->Organizations()->Equipment()->EquipmentItem());
  _impl->scenes = NarativeSequence(_impl->scenario->Installations()->Installation());
}
  //-----------------------------------------------------------------------------
QModelIndex Scenario::index(int row, int column, const QModelIndex& parent) const
{
  if (0 <= row && row < 9) {
    return createIndex(row, column, const_cast<Scenario*>(this));
  } else {
    return QModelIndex();
  }
}
//-----------------------------------------------------------------------------
QModelIndex Scenario::parent(const QModelIndex& index) const
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
int Scenario::columnCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent)
  return 1;
}
//-----------------------------------------------------------------------------
int Scenario::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent)
  return 9;
}
//-----------------------------------------------------------------------------
QVariant Scenario::data(const QModelIndex& index, int role) const
{
  if (index.row() < 0 || index.row() > 9)
    return QVariant();

  if (index.row() == 0) {
    return (role == Qt::DisplayRole) ? Name() : QVariant("Name");
  } else if (index.row() == 1) {
    return (role == Qt::DisplayRole) ? Type() : QVariant("Type");
  } else if (index.row() == 2) {
    return (role == Qt::DisplayRole) ? Version() : QVariant("Version");
  } else if (index.row() == 3) {
    return (role == Qt::DisplayRole) ? ModificationDate() : QVariant("Modification Date");
  } else if (index.row() == 4) {
    return (role == Qt::DisplayRole) ? SecurityClassification() : QVariant("Security Classification");
  } else if (index.row() == 5) {
    return (role == Qt::DisplayRole) ? ReleaseRestriction() : QVariant("Release Restriction");
  } else if (index.row() == 6) {
    return (role == Qt::DisplayRole) ? Purpose() : QVariant("Purpose");
  } else if (index.row() == 7) {
    return (role == Qt::DisplayRole) ? Description() : QVariant("Description");
  } else if (index.row() == 8) {
    return (role == Qt::DisplayRole) ? UseLimitation() : QVariant("Use Limitation");
  }

  return QVariant();
}
//-----------------------------------------------------------------------------
Qt::ItemFlags Scenario::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
//-----------------------------------------------------------------------------
bool Scenario::setData(const QModelIndex& index, const QVariant& value, int role)
{
  bool dataAccepted = false;
  if (index.row() < 0 || index.row() > 9)
    return false;

  if (index.row() == 0) {
    if (role == Qt::DisplayRole) {
      Name(value.toString());
      emit(dataChanged(index, index, { Qt::EditRole, Qt::DisplayRole }));
    }
    dataAccepted = true;
  } else if (index.row() == 1) {
    if (role == Qt::DisplayRole) {
      Type(value.toString());
      emit(dataChanged(index, index, { Qt::EditRole, Qt::DisplayRole }));
    }
    dataAccepted = true;
  } else if (index.row() == 2) {
    if (role == Qt::DisplayRole) {
      Version(value.toString());
      emit(dataChanged(index, index, { Qt::EditRole, Qt::DisplayRole }));
    }
    dataAccepted = true;
  } else if (index.row() == 3) {
    if (role == Qt::DisplayRole) {
      ModificationDate(value.toDate());
      emit(dataChanged(index, index, { Qt::EditRole, Qt::DisplayRole }));
    }
    dataAccepted = true;
  } else if (index.row() == 4) {
    if (role == Qt::DisplayRole) {
      SecurityClassification(value.toString());
      emit(dataChanged(index, index, { Qt::EditRole, Qt::DisplayRole }));
    }
    dataAccepted = true;
  } else if (index.row() == 5) {
    if (role == Qt::DisplayRole) {
      ReleaseRestriction(value.toString());
      emit(dataChanged(index, index, { Qt::EditRole, Qt::DisplayRole }));
    }
    dataAccepted = true;
  } else if (index.row() == 6) {
    if (role == Qt::DisplayRole) {
      Purpose(value.toString());
      emit(dataChanged(index, index, { Qt::EditRole, Qt::DisplayRole }));
    }
    dataAccepted = true;
  } else if (index.row() == 7) {
    if (role == Qt::DisplayRole) {
      Description(value.toString());
      emit(dataChanged(index, index, { Qt::EditRole, Qt::DisplayRole }));
    }
    dataAccepted = true;
  } else if (index.row() == 8) {
    if (role == Qt::DisplayRole) {
      UseLimitation(value.toString());
      emit(dataChanged(index, index, { Qt::EditRole, Qt::DisplayRole }));
    }
    dataAccepted = true;
  }

  return dataAccepted;
}

//-----------------------------------------------------------------------------
LocationSequence* Scenario::Locations()
{
  return &_impl->locations;
}
//-----------------------------------------------------------------------------
ActorSequence* Scenario::Actors()
{
  return &_impl->actors;
}
//-----------------------------------------------------------------------------
ObjectSequence* Scenario::Objects()
{
  return &_impl->objects;
}
//-----------------------------------------------------------------------------
NarativeSequence* Scenario::Naratives()
{
  return &_impl->scenes;
}
//-----------------------------------------------------------------------------
QString Scenario::Filepath()
{
  return _impl->filepath;
}
//-----------------------------------------------------------------------------
void Scenario::Filepath(QString path)
{
  _impl->filepath = path;
}
//-----------------------------------------------------------------------------
bool Scenario::onDisk()
{
  return !_impl->filepath.isEmpty();
}
//-----------------------------------------------------------------------------
}
