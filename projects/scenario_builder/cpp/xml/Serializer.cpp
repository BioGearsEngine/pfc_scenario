#include "Serializer.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <regex>
#include <streambuf>
#include <string>

#include <QDebug>
#include <QDir>

// #include "../xsd/cpp/military_scenario_1.0.0.hxx"
#include "../xsd/cpp/pfc_scenario_0.3.hxx"
#include "SchemaUtils.h"

#include "mz.h"
#include "mz_os.h"
#include "mz_strm.h"
#include "mz_strm_buf.h"
#include "mz_strm_split.h"
#include "mz_zip.h"
#include "mz_zip_rw.h"

#include "mz_strm_mem.h"
#include <QSqlQuery>
#include <fstream>


//Include encoded header files.
#include "ver_0.3/military_scenario_1.0.0.xsd.h"
#include "ver_0.3/msdl_codes_1.0.0.xsd.h"
#include "ver_0.3/msdl_complex_types_1.0.0.xsd.h"
#include "ver_0.3/msdl_simple_types_1.0.0.xsd.h"
#include "ver_0.3/pfc_scenario.xsd.h"
#include "ver_0.3/pfc_scenario_complex_types.xsd.h"
#include "ver_0.3/extern/jc3iedm-3.1-codes-20061208.xsd.h"
#include "ver_0.3/extern/jc3iedm_meterological.xsd.h"
#include "ver_0.3/extern/model_id_v2006_final.xsd.h"


template <typename CharT, typename TraitsT = std::char_traits<CharT>>
class vectorwrapbuf : public std::basic_streambuf<CharT, TraitsT> {
public:
  vectorwrapbuf(std::vector<CharT>& vec)
  {
    this->setg(vec.data(), vec.data(), vec.data() + vec.size());
  }
};

namespace pfc {

xml_schema::date get_now();

Serializer::Serializer(QObject* parent)
  : QObject(parent)
{
}
//-------------------------------------------------------------------------------
Serializer::~Serializer()
{
}
//-------------------------------------------------------------------------------
bool Serializer::save() const
{
  QString filename = get_property(_db, "archive_file");
  if (!filename.contains("_NOTFOUND", Qt::CaseSensitive)) {
    return save_as(filename);
  }
  filename = get_property(_db, "scenario_title");
  if (!filename.contains("_NOTFOUND", Qt::CaseSensitive)) {
    filename = filename.replace(" ", "_") + ".pfc.zip";
    return save_as(filename);
  }
  filename = _sourceFile;
  return save_as(filename);
}
//-------------------------------------------------------------------------------
bool Serializer::save_as(const QString& filename) const
{
  if (!_db) {
    return false;
  }

  std::stringstream msdl_stream;
  std::stringstream pfc_stream;

  try {
    msdl_stream = generate_msdl_stream();
    pfc_stream = generate_pfc_stream();
  } catch (::xsd::cxx::tree::serialization<char> e) {
    std::stringstream ss;
    ss << e;
    qCritical() << ss.str().c_str();
  }

  mz_zip_file file_info;
  void* read_mem_stream = nullptr;
  void* write_mem_stream = nullptr;
  void* os_stream = nullptr;
  void* zip_handle = nullptr;
  int32_t written = 0;
  int32_t read = 0;
  int32_t text_size = 0;
  int32_t buffer_size = 0;
  int32_t err = MZ_OK;
  const char* password = "1234";

  std::string text_name = "scenario.xml";
  std::string text_ptr = "<XML><!-- Nothing in this file -->";

  memset(&file_info, 0, sizeof(file_info));

  /* Write zip to memory stream */
  mz_stream_mem_create(&write_mem_stream);
  mz_stream_mem_set_grow_size(write_mem_stream, 128 * 1024);
  mz_stream_open(write_mem_stream, nullptr, MZ_OPEN_MODE_CREATE);

  void* writer = nullptr;
  mz_zip_writer_create(&writer);
  mz_zip_writer_open_file(writer, filename.toStdString().c_str(), 0, false);

  text_name = "Scenario.msdl.xml";
  text_ptr = msdl_stream.str();
  file_info.version_madeby = MZ_VERSION_MADEBY;
  file_info.compression_method = MZ_COMPRESS_METHOD_DEFLATE;
  file_info.filename = text_name.c_str();
  file_info.uncompressed_size = text_ptr.length();
  mz_zip_writer_add_buffer(writer, (void*)text_ptr.c_str(), (int32_t)text_ptr.length(), &file_info);

  text_name = "Scenario.pfc.xml";
  text_ptr = pfc_stream.str();
  file_info.filename = text_name.c_str();
  file_info.uncompressed_size = text_ptr.length();
  mz_zip_writer_add_buffer(writer, (void*)text_ptr.c_str(), (int32_t)text_ptr.length(), &file_info);


  std::vector<std::tuple<char const*,size_t,unsigned char const *>> schemas = {
     {"xsd/pfc_scenario_0.3.xsd", io::size_of_pfc_scenario, io::pfc_scenario_text }
    ,{"xsd/pfc_scenario_complex_types_0.3.xsd",   io::size_of_pfc_scenario_complex_types ,io::pfc_scenario_complex_types_text}
    ,{"xsd/msdl_simple_types_1.0.0.xsd",          io::size_of_msdl_simple_types_1 ,io::msdl_simple_types_1_text}
    ,{"xsd/msdl_complex_types_1.0.0.xsd",         io::size_of_msdl_complex_types_1 ,io::msdl_complex_types_1_text}
    ,{"xsd/msdl_codes_1.0.0.xsd",                 io::size_of_msdl_codes_1 ,io::msdl_codes_1_text}
    ,{"xsd/military_scenario_1.0.0.xsd",          io::size_of_military_scenario_1 ,io::military_scenario_1_text}
    ,{"xsd/extern/jc3iedm_meterological.xsd",     io::size_of_jc3iedm_meterological , io::jc3iedm_meterological_text}
    ,{"xsd/extern/jc3iedm-3.1-codes-20061208.xsd",io::size_of_jc3iedm_3 ,io::jc3iedm_3_text}
    ,{"xsd/extern/model_id_v2006_final.xsd",      io::size_of_model_id_v2006_final ,io::model_id_v2006_final_text}
  };
  for (auto& tuple : schemas) {
  file_info.filename = std::get<0>(tuple);
    file_info.uncompressed_size = std::get<1>(tuple);
    mz_zip_writer_add_buffer(writer, (void*)std::get<2>(tuple), (int32_t)std::get<1>(tuple), &file_info);
  }

  mz_zip_writer_close(writer);
  mz_zip_writer_delete(&writer);

  return update_property(_db, "archive_file", filename.toStdString());
}
//-------------------------------------------------------------------------------
bool Serializer::load(const QString& filename)
{

  if (!_db) {
    //Short Circuit We do not have a valid database ptr
    return false;
  }

  SQLite3Driver scratch_db{ "loading.sqlite" };
  scratch_db.open(scratch_db.Name());
  scratch_db.clear_db();
  scratch_db.initialize_db();

  mz_zip_file* file_info = NULL;
  uint32_t ratio = 0;
  int16_t level = 0;
  int32_t err = MZ_OK;
  const char* string_method = NULL;
  void* reader = NULL;

  mz_zip_reader_create(&reader);
  err |= mz_zip_reader_open_file(reader, filename.toStdString().c_str());
  if (err != MZ_OK) {
    qInfo() << QString("Error %1 opening archive %2\n").arg(err).arg(filename);
    mz_zip_reader_delete(&reader);

    return err;
  }

  // Note: What I really want to do here is find all archives ending in xsd/* and move them to tmp/xsd.  Minizip has a
  // Function mz_zip_reader_save_all which would let me do this, but would also grab every image file. So instead I'm going to iterate over
  // All contained files and store two list images and xsd. While I was writing this I ran in to a problem where saving files to disk invalidated future calls to mz_reader_close_entry.
  // My quick solution was just to close/delete the reader and reopen it.  Loading xml files must be done after the xsd/ files are saved but the entries come after so not being able to rewind was a major issue.
  std::string destination = "";
  err = mz_zip_reader_goto_first_entry(reader);
  do {
    err = mz_zip_reader_entry_get_info(reader, &file_info);
    if (strncmp(file_info->filename, "xsd/", 4) == 0) {
      //NOTE: Might want to strip xsd:/
      _known_schemas.push_back(file_info->filename);
      destination = "tmp/";
      destination += file_info->filename;
      mz_zip_reader_entry_save_file(reader, destination.c_str());
    } else if (strncmp(file_info->filename, "images/", 7) == 0) {
      //NOTE: Might want to strip images:/
      _known_images.push_back(file_info->filename);
    }
    err = mz_zip_reader_goto_next_entry(reader);
  } while (err == MZ_OK);
  mz_zip_reader_delete(&reader);

  //Recreating Reader
  reader = nullptr;
  mz_zip_reader_create(&reader);
  err = mz_zip_reader_open_file(reader, filename.toStdString().c_str());
  if (err != MZ_OK) {
    qInfo() << QString("Error %1 opening archive %2").arg(err).arg(filename);
    mz_zip_reader_delete(&reader);

    return err;
  }

  QString fallback = QDir::currentPath();
  QDir::setCurrent(QStringLiteral("tmp/xsd"));
  //TODO: Restore MSDL Support

  if (mz_zip_reader_locate_entry(reader, "Scenario.msdl.xml", 1) == MZ_OK) {
    if (mz_zip_reader_entry_get_info(reader, &file_info) == MZ_OK && mz_zip_reader_entry_open(reader) == MZ_OK) {
      //NOTE: At this point I want to pull all scenarios in to memory for parsing.  Then Close down the archive
      //At the time of this function being written the average size of an archive was < 1MB, but it is easy to invision a world
      //Where they grow to < 1GB.  However we would need to refactor a great many things reguarding file io to handle such an occurance.

      //If Scenarios grow beyond 50Mbs concider streamed reading and background loading to speed up launching of the application.

      std::vector<char> buffer;
      buffer.resize(file_info->uncompressed_size);
      auto bytes_read = mz_zip_reader_entry_read(reader, &buffer[0], file_info->uncompressed_size);
      vectorwrapbuf<char> schema_buffer{ buffer };
      std::istream i_stream(&schema_buffer);
      mz_zip_reader_entry_close(reader);

      try { // If the parsing fails this prints out every error
        //      auto msdl_schema = msdl_1::MilitaryScenario(i_stream);
        //      auto scenario_id = msdl_schema->ScenarioID();
        //
        //      auto name = scenario_id.name();
        //      auto domain = scenario_id.applicationDomain();
        //      auto version = scenario_id.version();
        //      auto security = scenario_id.securityClassification();
        //      auto description = scenario_id.description();
        //
        //      auto success = update_property("scenario_title", name);
        //
        //      if (domain.present()) {
        //        success &= update_property("scenario_domain", *domain);
        //      }
        //      success &= update_property("scenario_version", version);
        //      success &= update_property("scenario_security", security);
        //      success &= update_property("scenario_description", description);
        //
      } catch (const xml_schema::exception& e) {
        std::cout << e << '\n';
        //
        mz_zip_reader_delete(&reader); //Removing the Reader if this moves below the try remember you must always cal this function before leaving this function
        QDir::setCurrent(fallback);
        return false;
      }
    }
  } else {
    mz_zip_reader_delete(&reader);
    printf("Could not find %s in archive %s\n", "Scenario.pfc.xml", filename.toStdString().c_str());

    QDir::setCurrent(fallback);
    return false;
  }

  if (mz_zip_reader_locate_entry(reader, "Scenario.pfc.xml", 1) == MZ_OK) {
    if (mz_zip_reader_entry_get_info(reader, &file_info) == MZ_OK && mz_zip_reader_entry_open(reader) == MZ_OK) {

      //NOTE: At this point I want to pull all scenarios in to memory for parsing.  Then Close down the archive
      //At the time of this function being written the average size of an archive was < 1MB, but it is easy to invision a world
      //Where they grow to < 1GB.  However we would need to refactor a great many things reguarding file io to handle such an occurance.

      //If Scenarios grow beyond 50Mbs concider streamed reading and background loading to speed up launching of the application.

      std::vector<char> buffer;
      buffer.resize(file_info->uncompressed_size);
      auto bytes_read = mz_zip_reader_entry_read(reader, &buffer[0], file_info->uncompressed_size);
      vectorwrapbuf<char> schema_buffer{ buffer };
      std::istream i_stream(&schema_buffer);
      mz_zip_reader_entry_close(reader);


      try { // If the parsing fails this prints out every error
        auto scenario_schema = pfc::schema::Scenario(i_stream);
        bool successful = false;
        scratch_db.ready();
        scenario_schema = pfc::schema::PFC::load_authors(std::move(scenario_schema), scratch_db, successful);
        scratch_db.ready();
        if (successful) {
          update_property(&scratch_db, "scenario_title", scenario_schema->summary().title()->c_str());
          update_property(&scratch_db, "scenario_version", scenario_schema->summary().version()->c_str());
          update_property(&scratch_db, "scenario_security", scenario_schema->summary().classification()->c_str());
          update_property(&scratch_db, "scenario_description", scenario_schema->summary().description()->c_str());
          update_property(&scratch_db, "scenario_keywords", scenario_schema->summary().keywords()->c_str());
          update_property(&scratch_db, "scenario_domain", scenario_schema->summary().domain()->c_str());
          update_property(&scratch_db, "scenario_limitations", scenario_schema->summary().limitations()->c_str());
        } else {
          throw std::runtime_error("Failed to load properties");
        }
        if (successful) {
          scenario_schema = pfc::schema::PFC::load_assessments(std::move(scenario_schema), scratch_db, successful);
        } else {
          throw std::runtime_error("Failed to load authors");
        }
        if (successful) {
          scenario_schema = pfc::schema::PFC::load_citations(std::move(scenario_schema), scratch_db, successful);
        } else {
          throw std::runtime_error("Failed to load citations");
        }
        if (successful) {
          scenario_schema = pfc::schema::PFC::load_equipment(std::move(scenario_schema), scratch_db, successful);
        } else {
          throw std::runtime_error("Failed to load equipment");
        }

        if (successful) {
          scenario_schema = pfc::schema::PFC::load_injuries(std::move(scenario_schema), scratch_db, successful);
        } else {
          throw std::runtime_error("Failed to load injuries");
        }
        if (successful) {
          scenario_schema = pfc::schema::PFC::load_injury_sets(std::move(scenario_schema), scratch_db, successful);
        } else {
          throw std::runtime_error("Failed to load injury sets");
        }
        if (successful) {
          scenario_schema = pfc::schema::PFC::load_objectives(std::move(scenario_schema), scratch_db, successful);
        } else {
          throw std::runtime_error("Failed to load objectives");
        }
        if (successful) {
          scenario_schema = pfc::schema::PFC::load_locations(std::move(scenario_schema), scratch_db, successful);
        } else {
          throw std::runtime_error("Failed to load locations");
        }
        if (successful) {
          scenario_schema = pfc::schema::PFC::load_roles(std::move(scenario_schema), scratch_db, successful);
        } else {
          throw std::runtime_error("Failed to load roles");
        }
        if (successful) {
          scenario_schema = pfc::schema::PFC::load_treatments(std::move(scenario_schema), scratch_db, successful);
        } else {
          throw std::runtime_error("Failed to load treatments");
        }
        if (successful) {
          scenario_schema = pfc::schema::PFC::load_events(std::move(scenario_schema), scratch_db, successful);
        } else {
          throw std::runtime_error("Failed to load events");
        }
        if (successful) {
          scenario_schema = pfc::schema::PFC::load_scenes(std::move(scenario_schema), scratch_db, successful);
        } else {
          throw std::runtime_error("Failed to load scenes");
        }
        mz_zip_reader_delete(&reader); //Removing the Reader if this moves below the try remember you must always cal this function before leaving this function

        QDir::setCurrent(fallback);

        scratch_db.close();
        _db->close();

        QFile target{ _db->Path() + "/" + _db->Name() };
        QFile source{ scratch_db.Path() + "/" + scratch_db.Name() };

        if (target.exists()) {
          target.remove();
        }
        source.copy(target.fileName());
        _db->open();
        _db->refresh();
        return  target.error() == QFile::NoError && source.error() == QFile::NoError;


      } catch (const xml_schema::exception& e) {
        std::cout << e << '\n';

        mz_zip_reader_delete(&reader); //Removing the Reader if this moves below the try remember you must always cal this function before leaving this function
        QDir::setCurrent(fallback);
        return false;
      }
      mz_zip_reader_delete(&reader); //Removing the Reader if this moves below the try remember you must always cal this function before leaving this function
      QDir::setCurrent(fallback);
      return update_property(_db, "archive_file", filename.toStdString());
    }
  } else {
    mz_zip_reader_delete(&reader);
    printf("Could not find %s in archive %s\n", "Scenario.pfc.xml", filename.toStdString().c_str());

    QDir::setCurrent(fallback);
    return false;
  }

  QDir::setCurrent(fallback);
  return false;
}
//-------------------------------------------------------------------------------
bool Serializer::update_property(SQLite3Driver* db, const std::string& name, const std::string& value) const
{
  Property prop;
  prop.name = name.c_str();
  prop.value = value.c_str();
  if (db) {
    return db->update_property(&prop);
  }
  return false;
}
//-------------------------------------------------------------------------------
QString Serializer::get_property(SQLite3Driver* db, const QString& name) const
{
  Property prop;
  prop.name = name;
  if (db) {
    return (db->select_property(&prop)) ? prop.value : QString("%1_NOTFOUND").arg(name);
  }
  return QString("%1_NOTFOUND").arg(name);
}
//-------------------------------------------------------------------------------
auto Serializer::generate_msdl_stream() const -> std::stringstream
{

  Property titleProperty, domainProperty, versionProperty, securityProperty, descriptionProperty;
  titleProperty.name = "scenario_title";
  domainProperty.name = "scenario_domain";
  versionProperty.name = "scenario_version";
  securityProperty.name = "scenario_security";
  descriptionProperty.name = "scenario_description";

  _db->select_property(&titleProperty);
  _db->select_property(&domainProperty);
  _db->select_property(&versionProperty);
  _db->select_property(&securityProperty);
  _db->select_property(&descriptionProperty);

  std::string name = titleProperty.value.toStdString();
  std::string domain = domainProperty.value.toStdString();
  std::string version = versionProperty.value.toStdString();
  std::string security = securityProperty.value.toStdString();
  std::string description = descriptionProperty.value.toStdString();

  // auto scenarioID_nameType = msdl_1::MilitaryScenarioType::ScenarioID_type::name_type(name);
  // auto scenarioID_typeType = msdl_1::MilitaryScenarioType::ScenarioID_type::type_type(domain);
  // auto scenarioID_versionType = msdl_1::MilitaryScenarioType::ScenarioID_type::version_type(version);
  // auto scenarioID_modificationDateType = msdl_1::MilitaryScenarioType::ScenarioID_type::modificationDate_type(get_now());
  // auto scenarioID_securityClassificationType = msdl_1::MilitaryScenarioType::ScenarioID_type::securityClassification_type(security);
  // auto scenarioID_descriptionType = msdl_1::MilitaryScenarioType::ScenarioID_type::description_type(description);
  //
  // auto msdl_scenario_id = std::make_unique<msdl_1::MilitaryScenarioType::ScenarioID_type>(scenarioID_nameType, scenarioID_typeType, scenarioID_versionType, scenarioID_modificationDateType, scenarioID_securityClassificationType, scenarioID_descriptionType);
  // auto msdl_military_version = std::make_unique<msdl_1::MilitaryScenarioType::Options_type>(msdl_1::MilitaryScenarioType::Options_type::MSDLVersion_type(""));
  // auto msdl_force_sides = std::make_unique<msdl_1::MilitaryScenarioType::ForceSides_type>();
  // auto msdl = msdl_1::MilitaryScenarioType(std::move(msdl_scenario_id), std::move(msdl_military_version), std::move(msdl_force_sides));

  xml_schema::namespace_infomap info;
  info[""].name = "urn:sisostds:scenario:military:data:draft:msdl:1";
  info[""].schema = "military_scenario_1.0.0.xsd";

  std::stringstream msdl_content;
  // msdl_1::MilitaryScenario(msdl_content, msdl, info);
  return msdl_content;
}
//-------------------------------------------------------------------------------
auto Serializer::generate_pfc_stream() const -> std::stringstream
{
  using namespace ::pfc::schema;

  auto pfc_scenario = PFC::make_Scenario();

  //0. <Author>
  for (auto& author : _db->get_authors()) { // For now there should only ever be one author
    pfc_scenario.author().email(author->email.toStdString());
    pfc_scenario.author().first_name(author->first.toStdString());
    pfc_scenario.author().last_name(author->last.toStdString());
    pfc_scenario.author().phone_number(author->phone.toStdString());
    pfc_scenario.author().zip(author->zip.toStdString());
    pfc_scenario.author().state(author->state.toStdString());
    pfc_scenario.author().country(author->country.toStdString());
  }

  pfc_scenario.summary().title(get_property(_db, "scenario_title").toStdString());
  pfc_scenario.summary().version(get_property(_db, "scenario_version").toStdString());
  pfc_scenario.summary().classification(get_property(_db, "scenario_security").toStdString());
  pfc_scenario.summary().description(get_property(_db, "scenario_description").toStdString());
  pfc_scenario.summary().limitations(get_property(_db, "scenario_limitations").toStdString());
  pfc_scenario.summary().domain(get_property(_db, "scenario_domain").toStdString().c_str());
  pfc_scenario.summary().keywords(get_property(_db, "scenario_keywords").toStdString().c_str());

  //1. <Equipment>
  for (auto& equipment : _db->get_equipments()) {
    pfc_scenario.equipment().equipment().push_back(PFC::make_equipment(equipment.get()));
  }
  //2. <conditions>
  for (auto& injury : _db->get_injuries()) {
    pfc_scenario.trauma_definitions().trauma().push_back(PFC::make_trauma(injury.get()));
  }

  //3. <treatment_plans>
  for (auto& treatment : _db->get_treatments()) {
    pfc_scenario.treatment_plans().treatment_plan().push_back(PFC::make_treatment_plan(treatment.get()));
  }

  //4. Populate <trauma_sets>
  for (auto& trauma : _db->get_injury_sets()) {
    pfc_scenario.trauma_sets().trauma_profile().push_back(PFC::make_trauma_profile(trauma.get()));
  }
  //5. <syllabus>
  //5.1 <syllabus><learning_objective>
  for (auto& objective : _db->get_objectives()) {
    pfc_scenario.syllabus().learning_objectives().objective().push_back(PFC::make_learning_objective(objective.get()));
  }
  //5.2 <syllabus><assessment>
  int32_t total_points = 0;
  for (auto& assessment : _db->get_assessments()) {
    total_points += assessment->available_points;
    pfc_scenario.syllabus().learning_assessments().assessment().push_back(PFC::make_assessment(assessment.get()));
  }
  pfc_scenario.syllabus().learning_assessments().total_points(total_points);

  //6.  <medical-scenario>
  //6.1 <medical-scenario><scenes>
  for (auto& scene : _db->get_scenes()) {
    auto scene_ptr = PFC::make_scene(scene.get());
    //6.1.1 <medical-scenario><scenes><events>
    for (auto& event : _db->get_events_in_scene(scene.get())) {
      scene_ptr->events().event().push_back(PFC::make_event(event.get()));
    }
    //6.1.2 <medical-scenario><scenes><items>
    for (auto& item : _db->get_equipment_in_scene(scene.get())) {
      scene_ptr->items().item().push_back(PFC::make_item(item.get()));
    }
    ////6.1.3 <medical-scenario><scenes><roles>
    for (auto& role : _db->get_roles_in_scene(scene.get())) {
      if (role->id != -1) {
        scene_ptr->roles().role_ref().push_back(PFC::make_role_ref(role.get()));
      }
    }
    pfc_scenario.medical_scenario().training_script().scene().push_back(std::move(scene_ptr));
  }
  //6.2 <medical-scenario><roles>
  for (auto& role : _db->get_roles()) {
    pfc_scenario.medical_scenario().roles().role().push_back(PFC::make_role(role.get()));
  }

  //7. <works-cited>
  for (auto& citation : _db->get_citations()) {
    pfc_scenario.works_cited().citation().push_back(PFC::make_citation(citation.get()));
  }

  xml_schema::namespace_infomap info;
  info[""].name = "com:ara:pfc:training:1";
  info[""].schema = "pfc_scenario_0.3.xsd";
  std::stringstream pfc_content;
  Scenario(pfc_content, pfc_scenario, info);
  return pfc_content;
}
//-------------------------------------------------------------------------------
xml_schema::date get_now()
{
  std::time_t t = std::time(nullptr);
  auto tm = std::localtime(&t);

  return xml_schema::date{ static_cast<int>(tm->tm_year),
                           static_cast<unsigned short>(tm->tm_mon),
                           static_cast<unsigned short>(tm->tm_yday),
                           static_cast<short>(tm->tm_hour),
                           static_cast<short>(tm->tm_min) };
}
//-------------------------------------------------------------------------------
}
// namespace pfc
