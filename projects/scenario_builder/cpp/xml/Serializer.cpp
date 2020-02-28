#include "Serializer.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <regex>
#include <string>

#include <QDebug>

#include "../xsd/cpp/military_scenario_1.0.0.hxx"
#include "../xsd/cpp/pfc_scenario_0.2.hxx"
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
bool Serializer::save(SQLite3Driver* driver)
{
  generate_msdl_stream(driver);
  generate_pfc_stream(driver);
  if (!_db) {
    return false;
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
  mz_zip_writer_open_file(writer, "PFC_Scenario.zip", 0, false);
  text_name = "Scenario.msdl.xml";
  text_ptr = _msdl_content.str();
  file_info.version_madeby = MZ_VERSION_MADEBY;
  file_info.compression_method = MZ_COMPRESS_METHOD_DEFLATE;
  file_info.filename = text_name.c_str();
  file_info.uncompressed_size = text_ptr.length();
  std::cout << file_info.uncompressed_size << "\n";
  std::cout << text_ptr << "\n";
  mz_zip_writer_add_buffer(writer, (void*)text_ptr.c_str(), (int32_t)text_ptr.length(), &file_info);

  text_name = "Scenario.pfc.xml";
  text_ptr = _pfc_content.str();
  file_info.filename = text_name.c_str();
  file_info.uncompressed_size = text_ptr.length();
  std::cout << file_info.uncompressed_size << "\n";
  std::cout << text_ptr << "\n";
  mz_zip_writer_add_buffer(writer, (void*)text_ptr.c_str(), (int32_t)text_ptr.length(), &file_info);

  mz_zip_writer_close(writer);
  mz_zip_writer_delete(&writer);

  return true;
}
//-------------------------------------------------------------------------------
bool Serializer::load(const QString& filename)
{
  if (!_db ) {
    return false;
  }
  _db->open(_db->Name());
  _db->clear_db();
  _db->close();
  _db->initialize_db();

  mz_zip_file* file_info = NULL;
  uint32_t ratio = 0;
  int16_t level = 0;
  int32_t err = MZ_OK;
  struct tm tmu_date;
  const char* string_method = NULL;
  char crypt = ' ';
  void* reader = NULL;

  mz_zip_reader_create(&reader);
  err = mz_zip_reader_open_file(reader, filename.toStdString().c_str());
  if (err != MZ_OK) {
    qInfo() << QString("Error %1 opening archive %2\n").arg(err).arg(filename);
    mz_zip_reader_delete(&reader);
    return err;
  }

  err = mz_zip_reader_goto_first_entry(reader);

  if (err != MZ_OK && err != MZ_END_OF_LIST) {
    printf("Error %" PRId32 " going to first entry in archive\n", err);
    mz_zip_reader_delete(&reader);
    return err;
  }

  std::filebuf file_buf;
  file_buf.open("Scenario.pfc.xml", std::ios::in);
  std::istream i_stream(&file_buf);
  //
  try { // If the parsing fails this prints out every error
    auto scenario_schema = pfc::schema::Scenario(i_stream);
    bool successful = false;
    scenario_schema = pfc::schema::PFC::load_authors(std::move(scenario_schema), *_db, successful);
    if (successful) {
      scenario_schema = pfc::schema::PFC::load_assessments(std::move(scenario_schema), *_db, successful);
    } else {
      throw std::runtime_error("Failed to load authors");
    }
    if (successful) {
      scenario_schema = pfc::schema::PFC::load_citations(std::move(scenario_schema), *_db, successful);
    } else {
      throw std::runtime_error("Failed to load citations");
    }
    if (successful) {
      scenario_schema = pfc::schema::PFC::load_equipment(std::move(scenario_schema), *_db, successful);
    } else {
      throw std::runtime_error("Failed to load equipment");
    }

    if (successful) {
      scenario_schema = pfc::schema::PFC::load_injuries(std::move(scenario_schema), *_db, successful);
    } else {
      throw std::runtime_error("Failed to load injuries");
    }
    if (successful) {
      scenario_schema = pfc::schema::PFC::load_injury_sets(std::move(scenario_schema), *_db, successful);
    } else {
      throw std::runtime_error("Failed to load injury sets");
    }
    if (successful) {
      scenario_schema = pfc::schema::PFC::load_objectives(std::move(scenario_schema), *_db, successful);
    } else {
      throw std::runtime_error("Failed to load objectives");
    }
    if (successful) {
      scenario_schema = pfc::schema::PFC::load_locations(std::move(scenario_schema), *_db, successful);
    } else {
      throw std::runtime_error("Failed to load locations");
    }
    if (successful) {
      scenario_schema = pfc::schema::PFC::load_roles(std::move(scenario_schema), *_db, successful);
    } else {
      throw std::runtime_error("Failed to load roles");
    }
    if (successful) {
      scenario_schema = pfc::schema::PFC::load_treatments(std::move(scenario_schema), *_db, successful);
    } else {
      throw std::runtime_error("Failed to load treatments");
    }
    if (successful) {
      scenario_schema = pfc::schema::PFC::load_events(std::move(scenario_schema), *_db, successful);
    } else {
      throw std::runtime_error("Failed to load events");
    }
    if (successful) {
      scenario_schema = pfc::schema::PFC::load_scenes(std::move(scenario_schema), *_db, successful);
    } else {
      throw std::runtime_error("Failed to load scenes");
    }
  } catch (const xml_schema::exception& e) {
    std::cout << e << '\n';
    return false;
  }
  //
  file_buf.close();

  mz_zip_reader_delete(&reader);

  if (err == MZ_END_OF_LIST)
    return MZ_OK;

  return true;
}
//-------------------------------------------------------------------------------
QString Serializer::get_property(const QString& name)
{
  Property prop;
  prop.name = name;
  if (_db) {
    return (_db->select_property(&prop)) ? prop.name : "N/A";
  }
  return "N/A";
}
//-------------------------------------------------------------------------------
void Serializer::generate_msdl_stream(SQLite3Driver* driver)
{

  auto property_list = driver->get_properties();
  qInfo() << property_list[0]->name;
  qInfo() << property_list[0]->value;
  Property titleProperty, domainProperty, versionProperty, securityProperty, descriptionProperty;
  titleProperty.name = "scenario_title";
  domainProperty.name = "scenario_domain";
  versionProperty.name = "scenario_version";
  securityProperty.name = "scenario_security";
  descriptionProperty.name = "scenario_description";
  driver->select_property(&titleProperty);
  driver->select_property(&domainProperty);
  driver->select_property(&versionProperty);
  driver->select_property(&securityProperty);
  driver->select_property(&descriptionProperty);
  std::string name = titleProperty.value.toStdString();
  std::string domain = domainProperty.value.toStdString();
  std::string version = versionProperty.value.toStdString();
  std::string security = securityProperty.value.toStdString();
  std::string description = descriptionProperty.value.toStdString();

  auto scenarioID_nameType = msdl_1::MilitaryScenarioType::ScenarioID_type::name_type(name);
  auto scenarioID_typeType = msdl_1::MilitaryScenarioType::ScenarioID_type::type_type(domain);
  auto scenarioID_versionType = msdl_1::MilitaryScenarioType::ScenarioID_type::version_type(version);
  auto scenarioID_modificationDateType = msdl_1::MilitaryScenarioType::ScenarioID_type::modificationDate_type(get_now());
  auto scenarioID_securityClassificationType = msdl_1::MilitaryScenarioType::ScenarioID_type::securityClassification_type(security);
  auto scenarioID_descriptionType = msdl_1::MilitaryScenarioType::ScenarioID_type::description_type(description);

  auto msdl_scenario_id = std::make_unique<msdl_1::MilitaryScenarioType::ScenarioID_type>(scenarioID_nameType, scenarioID_typeType, scenarioID_versionType, scenarioID_modificationDateType, scenarioID_securityClassificationType, scenarioID_descriptionType);
  auto msdl_military_version = std::make_unique<msdl_1::MilitaryScenarioType::Options_type>(msdl_1::MilitaryScenarioType::Options_type::MSDLVersion_type(""));
  auto msdl_force_sides = std::make_unique<msdl_1::MilitaryScenarioType::ForceSides_type>();
  auto msdl = msdl_1::MilitaryScenarioType(std::move(msdl_scenario_id), std::move(msdl_military_version), std::move(msdl_force_sides));

  _msdl_content.str("");

  xml_schema::namespace_infomap info;
  info[""].name = "urn:sisostds:scenario:military:data:draft:msdl:1";
  info[""].schema = "military_scenario_1.0.0.xsd";

  try {
    msdl_1::MilitaryScenario(_msdl_content, msdl, info);
  } catch (std::exception e) {
    std::cout << e.what() << std::endl;
    _msdl_content << e.what();
  }
}
//-------------------------------------------------------------------------------
void Serializer::generate_pfc_stream(SQLite3Driver* driver)
{
  using namespace ::pfc::schema;

  auto pfc_scenario = PFC::make_Scenario();

  //0. <Author>
  for (auto& author : driver->get_authors()) { // For now there should only ever be one author
    pfc_scenario.author().email() = author->email.toStdString();
    pfc_scenario.author().first_name() = author->first.toStdString();
    pfc_scenario.author().last_name() = author->last.toStdString();
    pfc_scenario.author().phone_number() = author->phone.toStdString();
    pfc_scenario.author().zip() = author->zip.toStdString();
    pfc_scenario.author().state() = author->state.toStdString();
    pfc_scenario.author().country() = author->country.toStdString();
  }

  //0.5 <Maps>
  auto citation_maps = _db->get_citation_maps();
  std::string citation_map_list;
  for (int i = 0; i < citation_maps.size(); ++i) {
    std::string map = '(' + std::to_string(citation_maps[i]->fk_scene) + ',' + std::to_string(citation_maps[i]->fk_citation) + ')';
    citation_map_list += map;
  }
  auto event_maps = _db->get_event_maps();
  std::string event_map_list;
  for (int i = 0; i < event_maps.size(); ++i) {
    std::string map = '(' + std::to_string(event_maps[i]->fk_scene) + ',' + std::to_string(event_maps[i]->fk_event) + ')';
    event_map_list += map;
  }
  auto equipment_maps = _db->get_equipment_maps();
  std::string equipment_map_list;
  for (int i = 0; i < equipment_maps.size(); ++i) {
    std::string map = '(' + std::to_string(equipment_maps[i]->fk_scene) + ',' + std::to_string(equipment_maps[i]->fk_equipment) + ')';
    equipment_map_list += map;
  }
  auto location_maps = _db->get_location_maps();
  std::string location_map_list;
  for (int i = 0; i < location_maps.size(); ++i) {
    std::string map = '(' + std::to_string(location_maps[i]->fk_scene) + ',' + std::to_string(location_maps[i]->fk_location) + ')';
    location_map_list += map;
  }
  auto role_maps = _db->get_role_maps();
  std::string role_map_list;
  for (int i = 0; i < role_maps.size(); ++i) {
    std::string map = '(' + std::to_string(role_maps[i]->fk_scene) + ',' + std::to_string(role_maps[i]->fk_role) + ')';
    role_map_list += map;
  }
  pfc_scenario.maps().citation_maps() = citation_map_list;
  pfc_scenario.maps().event_maps() = event_map_list;
  pfc_scenario.maps().equipment_maps() = equipment_map_list;
  pfc_scenario.maps().location_maps() = location_map_list;
  pfc_scenario.maps().role_maps() = role_map_list;

  //1. <Equipment>
  for (auto& equipment : driver->get_equipments()) {
    pfc_scenario.equipment().equipment().push_back(PFC::make_equipment(equipment.get()));
  }
  //2. <conditions>
  for (auto& injury : driver->get_injuries()) {
    pfc_scenario.trauma_definitions().trauma().push_back(PFC::make_trauma(injury.get()));
  }

  //3. <treatment_plans>
  for (auto& treatment : driver->get_treatments()) {
    pfc_scenario.treatment_plans().treatment_plan().push_back(PFC::make_treatment_plan(treatment.get()));
  }

  //4. Populate <trauma_sets>
  for (auto& trauma : driver->get_injury_sets()) {
    pfc_scenario.trauma_sets().trauma_profile().push_back(PFC::make_trauma_profile(trauma.get()));
  }
  //5. <syllabus>
  //5.1 <syllabus><learning_objective>
  for (auto& objective : driver->get_objectives()) {
    pfc_scenario.syllabus().learning_objectives().objective().push_back(PFC::make_learning_objective(objective.get()));
  }
  //5.2 <syllabus><assessment>
  int32_t total_points = 0;
  for (auto& assessment : driver->get_assessments()) {
    total_points += assessment->available_points;
    pfc_scenario.syllabus().learning_assessments().assessment().push_back(PFC::make_assessment(assessment.get()));
  }
  pfc_scenario.syllabus().learning_assessments().total_points(total_points);

  //6.  <medical-scenario>
  //6.1 <medical-scenario><scenes>
  for (auto& scene : driver->get_scenes()) {
    auto scene_ptr = PFC::make_scene(scene.get());
    //6.1.1 <medical-scenario><scenes><events>
    for (auto& event : driver->get_events_in_scene(scene.get())) {
      scene_ptr->events().event().push_back(PFC::make_event(event.get()));
    }
    //6.1.2 <medical-scenario><scenes><items>
    for (auto& item : driver->get_equipment_in_scene(scene.get())) {
      scene_ptr->items().item().push_back(PFC::make_item(item.get()));
    }
    ////6.1.3 <medical-scenario><scenes><roles>
    for (auto& role : driver->get_roles_in_scene(scene.get())) {
      if (role->id != -1) {
        scene_ptr->roles().role_ref().push_back(PFC::make_role_ref(role.get()));
      }
    }
    pfc_scenario.medical_scenario().training_script().scene().push_back(std::move(scene_ptr));
  }
  //6.2 <medical-scenario><roles>
  for (auto& role : driver->get_roles()) {
    pfc_scenario.medical_scenario().roles().role().push_back(PFC::make_role(role.get()));
  }

  //7. <works-cited>
  for (auto& citation : driver->get_citations()) {
    pfc_scenario.works_cited().citation().push_back(PFC::make_citation(citation.get()));
  }

  _pfc_content.str("");

  xml_schema::namespace_infomap info;
  info[""].name = "com:ara:pfc:training:1";
  info[""].schema = "pfc_scenario_0.2.xsd";
  QSqlDatabase _db;
  if (_db.isValid()) {
    _db = QSqlDatabase::addDatabase("QSQLITE");
  }
  try {
    Scenario(_pfc_content, pfc_scenario, info);
  } catch (std::exception e) {
    std::cout << e.what() << std::endl;
    _pfc_content << e.what();
  }
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
