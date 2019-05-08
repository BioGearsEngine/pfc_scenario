#include "Scenario.h"

#include <fstream>

#include <QRegularExpression>
#include <QDir>

#include "MilitaryScenario_1.0.0.hxx"
#include "MsdlComplexTypes_1.0.0.hxx"
namespace pfc {

struct Scenario::Implementation {
  std::unique_ptr<msdl_1::MilitaryScenarioType> scenario;

  QString path;
  QString filepath;
}; //End Struct Implementation

Scenario::Scenario()
  : _impl(std::make_unique<Implementation>())
{
  using namespace msdl_1;
  _impl->scenario = std::make_unique<msdl_1::MilitaryScenarioType>(
    std::make_unique<MilitaryScenarioType::ScenarioID_type>()
    , std::make_unique<MilitaryScenarioType::Options_type>(std::make_unique<OptionsType::MSDLVersion_type>())
    , std::make_unique<MilitaryScenarioType::ForceSides_type>()
    //TODO:Assign Defaults
  );
}
//-----------------------------------------------------------------------------
Scenario::~Scenario()
{
  _impl = nullptr;
}
//-----------------------------------------------------------------------------
Scenario::Scenario(std::string filename)
  : _impl(std::make_unique<Implementation>())
{
  using namespace msdl_1;
  _impl->scenario = MilitaryScenario(filename);
}
//-----------------------------------------------------------------------------
Scenario::Scenario(std::string path, std::string filename)
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
QString Scenario::ModificationDate() const
{
  return QString("%d-%d-%d").arg(_impl->scenario->ScenarioID().modificationDate().year()).arg(_impl->scenario->ScenarioID().modificationDate().month()).arg(_impl->scenario->ScenarioID().modificationDate().day());
}
//-----------------------------------------------------------------------------
QString Scenario::SecurityClassification() const
{
  if (!_impl->scenario->ScenarioID().securityClassification().idtag().present()) {
    _impl->scenario->ScenarioID().securityClassification().idtag("Unclassified");
  }
  return _impl->scenario->ScenarioID().securityClassification().idtag().get().text_content().c_str();
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
QString Scenario::UseLimitaiton() const
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
void Scenario::ModificationDate(QString date)
{
  //TODO:Migrate this to a date object or three INTS
  QRegularExpression re(R"((\d{4})-(\d{1,2})-(\d{1,2}))");
  QRegularExpressionMatch match = re.match(date);
  if (match.hasMatch()) {
    _impl->scenario->ScenarioID().modificationDate().year(match.captured(0).toInt());
    _impl->scenario->ScenarioID().modificationDate().month(match.captured(1).toInt());
    _impl->scenario->ScenarioID().modificationDate().day(match.captured(2).toInt());
  }
}
//-----------------------------------------------------------------------------
void Scenario::SecurityClassification(QString classification)
{
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
void Scenario::UseLimitaiton(QString limitation)
{
  _impl->scenario->ScenarioID().useLimitation(limitation.toStdString());
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
    msdl_1::MilitaryScenario(out,*_impl->scenario,map);
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
bool Scenario::SaveAs(std::string filename)
{
  if ( QDir(filename.c_str()).exists()) {
    xml_schema::namespace_infomap map;

    map["lib"].name = "urn:sisostds:scenario:military:data:draft:msdl:1";
    map["lib"].schema = "MilitaryScenario_1.0.0.xsd";

    auto out = std::ofstream(filename);

    std::string e;
    msdl_1::MilitaryScenario(out, *_impl->scenario, map);
    return true;
  }
  return false;
}
//-----------------------------------------------------------------------------
}