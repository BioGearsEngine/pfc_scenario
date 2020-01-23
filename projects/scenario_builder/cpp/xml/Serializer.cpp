#include "Serializer.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <regex>
#include <string>

#include <QDebug>

#include "../xsd/cpp/MilitaryScenario_1.0.0.hxx"
#include "../xsd/cpp/pfc_scenario_0.1.hxx"

#include "mz.h"
#include "mz_os.h"
#include "mz_strm.h"
#include "mz_strm_buf.h"
#include "mz_strm_split.h"
#include "mz_zip.h"
#include "mz_zip_rw.h"

#include "mz_strm_mem.h"
#include <QSqlQuery>

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
  if (!_db) {
    return false;
  }
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

  printf("      Packed     Unpacked Ratio Method   Attribs Date     Time  CRC-32     Name\n");
  printf("      ------     -------- ----- ------   ------- ----     ----  ------     ----\n");

  /* Enumerate all entries in the archive */
  do {
    err = mz_zip_reader_entry_get_info(reader, &file_info);

    if (err != MZ_OK) {
      printf("Error %" PRId32 " getting entry info in archive\n", err);
      break;
    }

    ratio = 0;
    if (file_info->uncompressed_size > 0)
      ratio = (uint32_t)((file_info->compressed_size * 100) / file_info->uncompressed_size);

    /* Display a '*' if the file is encrypted */
    if (file_info->flag & MZ_ZIP_FLAG_ENCRYPTED)
      crypt = '*';
    else
      crypt = ' ';

    switch (file_info->compression_method) {
    case MZ_COMPRESS_METHOD_STORE:
      string_method = "Stored";
      break;
    case MZ_COMPRESS_METHOD_DEFLATE:
      level = (int16_t)((file_info->flag & 0x6) / 2);
      if (level == 0)
        string_method = "Defl:N";
      else if (level == 1)
        string_method = "Defl:X";
      else if ((level == 2) || (level == 3))
        string_method = "Defl:F"; /* 2: fast , 3: extra fast */
      else
        string_method = "Defl:?";
      break;
    case MZ_COMPRESS_METHOD_BZIP2:
      string_method = "BZip2";
      break;
    case MZ_COMPRESS_METHOD_LZMA:
      string_method = "LZMA";
      break;
    default:
      string_method = "?";
    }

    mz_zip_time_t_to_tm(file_info->modified_date, &tmu_date);

    /* Print entry information */
    printf("%12" PRId64 " %12" PRId64 "  %3" PRIu32 "%% %6s%c %8" PRIx32 " %2.2" PRIu32
           "-%2.2" PRIu32 "-%2.2" PRIu32 " %2.2" PRIu32 ":%2.2" PRIu32 " %8.8" PRIx32 "   %s\n",
           file_info->compressed_size, file_info->uncompressed_size, ratio,
           string_method, crypt, file_info->external_fa,
           (uint32_t)tmu_date.tm_mon + 1, (uint32_t)tmu_date.tm_mday,
           (uint32_t)tmu_date.tm_year % 100,
           (uint32_t)tmu_date.tm_hour, (uint32_t)tmu_date.tm_min,
           file_info->crc, file_info->filename);

    err = mz_zip_reader_goto_next_entry(reader);

    if (err != MZ_OK && err != MZ_END_OF_LIST) {
      printf("Error %" PRId32 " going to next entry in archive\n", err);
      break;
    }
  } while (err == MZ_OK);

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

  std::vector<Property*> property_list = driver->get_properties();
  qInfo() << property_list[0]->name;
  qInfo() << property_list[0]->value;
  Property titleProperty,domainProperty,versionProperty,securityProperty,descriptionProperty;
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
  info[""].schema = "MilitaryScenario_1.0.0.xsd";

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
  using pfc::schema::ScenarioSchema;
  namespace pfcs = pfc::schema;

  // Injury Variables
  std::vector<Injury*> injury_list = driver->get_injuries();
  auto conditions = std::make_unique<pfcs::ScenarioSchema::conditions_type>();
  for (auto i = 0;i < injury_list.size();++i) {
    std::string medical_name = injury_list[i]->medical_name.toStdString();
    std::string common_name = injury_list[i]->common_name.toStdString();
    std::string description = injury_list[i]->description.toStdString();
    std::string citations = injury_list[i]->citations.toStdString();
    float severity_min = injury_list[i]->severity_min;
    float severity_max = injury_list[i]->severity_max;
    auto injury_id = std::make_unique<pfcs::injury::id_type>("Injury_"+std::to_string(i+1));
    auto injury_medical_name = std::make_unique<pfcs::injury::medical_name_type>(medical_name);
    auto injury_description = std::make_unique<pfcs::injury::description_type>(description);
    auto num_range = pfcs::injury_severity_range();
    num_range.numeric_range(std::make_unique<pfc::schema::numeric_range>(severity_min, severity_max));
    auto injury_severity_range = std::make_unique<pfcs::injury::severity_range_type>(num_range);
    auto injuries = std::make_unique<pfcs::injury>(std::move(injury_id), std::move(injury_medical_name), std::move(injury_description), std::move(injury_severity_range));
    conditions->injury().push_back(std::move(injuries));
  }

  // Treatment Variables
  std::vector<Treatment*> treatment_list = driver->get_treatments();
  auto treatment_plans = std::make_unique<pfcs::treatment_plan_definition_list>();
  for (auto i = 0;i < treatment_list.size();++i) {
    std::string treatment_medical_name = treatment_list[i]->medical_name.toStdString();
    std::string trtmnt_common_name = treatment_list[i]->common_name.toStdString();
    std::string treatment_desc = treatment_list[i]->description.toStdString();
    std::string treatment_equipment = treatment_list[i]->equipment.toStdString();
    std::string trtmnt_citations = treatment_list[i]->citations.toStdString();
    auto treatment_id = std::make_unique<pfcs::treatment_plan::id_type>("Treatment_"+std::to_string(i+1));
    auto treatment_description = std::make_unique<pfcs::treatment_plan::description_type>(treatment_desc);
    auto treatment_common_name = std::make_unique<pfcs::treatment_plan::common_name_type>(trtmnt_common_name); // This isn't in the constructor right now but we could add it later
    auto treatment_required_equipment = std::make_unique<pfcs::treatment_plan::required_equipment_type>();
    treatment_required_equipment->equipment_refs().push_back(treatment_equipment);
    auto treatment_citation_list = std::make_unique<pfcs::citation_list>();
    auto treatment_plan = std::make_unique<pfc::schema::treatment_plan>(std::move(treatment_id), std::move(treatment_description), std::move(treatment_required_equipment));
    treatment_plans->treatment_plan().push_back(std::move(treatment_plan));
  }

  auto patient_states = std::make_unique<pfcs::ScenarioSchema::patient_states_type>();
  
//Learning Objective
  auto assessment_points = pfcs::assessment_criteria_list::total_points_type();
  auto assessment_criteira = std::make_unique<pfcs::assessment_criteria_list>(std::move(assessment_points));
  auto assessment_objectives = std::make_unique<pfcs::learning_objective_list>();
  auto syllabus = std::make_unique<pfcs::ScenarioSchema::syllabus_type>(std::move(assessment_objectives),std::move(assessment_points)); // What is total assessment points? Is it a summation?
  auto& assessment_criteria = syllabus->learning_assessments();
  auto& learning_objectives = syllabus->learning_objectives().objective();

  std::vector<Objective*> objective_list = driver->get_objectives();
  for (auto i = 0;i < objective_list.size();++i) {
    auto learning_objective_id = std::make_unique<pfcs::learning_objective::id_type>("Objective_"+std::to_string(i));
    auto learning_objective_name = std::make_unique<pfcs::learning_objective::name_type>(objective_list[i]->name.toStdString());
    auto lo_ref_citations = std::make_unique<pfcs::citation_ref_list>();
    lo_ref_citations->citation_ref().push_back(objective_list[i]->citations.toStdString());
    auto lo_ref_cpg_type = std::make_unique<pfcs::cpg_list::citation_ref_type>();
    auto lo_ref_cpg_name = std::make_unique<pfcs::cpg_list::name_type>();
    auto lo_ref_cpgs = std::make_unique<pfcs::cpg_list>(std::move(lo_ref_cpg_name), std::move(lo_ref_cpg_type)); //lo_ref_citations is a citation_list
    auto learning_objective_refs = std::make_unique<pfcs::learning_objective::references_type>(std::move(lo_ref_citations), std::move(lo_ref_cpgs));
    auto lo_ip_ref_list = std::make_unique<pfcs::injury_profile_reference_list>(); //lo_ref_cpgs is a cpg_list
    auto lo_tp_ref_list = std::make_unique<pfcs::treatment_plan_reference_list>();
    auto learning_objective_relates = std::make_unique<pfcs::learning_objective::relates_to_type>(std::move(lo_tp_ref_list), std::move(lo_ip_ref_list));
    auto learning_objective = std::make_unique<pfcs::learning_objective>(std::move(learning_objective_id), std::move(learning_objective_name), std::move(learning_objective_refs), std::move(learning_objective_relates));
    learning_objectives.push_back( std::move(learning_objective));
  }
  //syllabus->learning_objectives().objective(learning_objectives);

  //scene(const location_id_type&,
  //      const description_type&,
  //      const time_of_day_type&,
  //      const time_in_simulation_type&,
  //      const events_type&);
  //event(const category_type&,
  //      const fedelity_type&,
  //      const details_type&);
  auto scenes = std::make_unique<pfcs::scene_list>();
  std::vector<Scene*> scene_list = driver->get_scenes();
  for (auto i = 0;i < scene_list.size();++i) {
    std::string scene_id = std::to_string(scene_list[i]->id);
    std::string scene_name = scene_list[i]->name.toStdString();
    std::vector<Location*> locations = driver->get_locations_in_scene(scene_list[i]);                              // Currently we only support 1 singular location being mapped to a scene, 
    auto loc_id = std::make_unique<pfcs::scene::location_id_type>("Location_" + std::to_string(locations[0]->id)); //however in the future we may want to update that, for now there's no need 
    auto desc = std::make_unique<pfcs::scene::description_type>("Scene Description "+std::to_string(i));       // to iterate since the vector will have max 1 element
    auto t_o_d = std::make_unique<pfcs::scene::time_of_day_type>(0/*Hours*/, 0/*Minutes*/, 0/*Seconds*/); // Not sure how to set this, check back later
    auto t_i_s = pfcs::scene::time_in_simulation_type(60/*One Hour*/);
    auto cat = std::make_unique<pfcs::event::category_type>(pfcs::event_category_enum::ACTION); // Arbitrary pick, come back to this
    auto fid = std::make_unique<pfcs::event::fidelity_type>(pfcs::event_fidelity_enum::LOW);
    auto det = std::make_unique<pfcs::event::details_type>(locations[0]->environment.toStdString());
    auto ev = std::make_unique<pfcs::event>(std::move(cat),std::move(fid),std::move(det));
    auto ev_list = std::make_unique<pfcs::scene::events_type>();
    ev_list->event().push_back(std::move(ev));
    auto scene = std::make_unique<pfcs::scene>(std::move(loc_id),std::move(desc),std::move(t_o_d),std::move(t_i_s),std::move(ev_list));
    //how do I assigned time_of_day and time_in_simulation?
  }
  std::string scene_id = std::to_string(scene_list[0]->id);
  std::string scene_name = scene_list[0]->name.toStdString();
  auto id = ::xml_schema::id(scene_id);
  auto med_sc_roles = std::make_unique<pfcs::medical_scenario::roles_type>();
  auto med_sc_props = std::make_unique<pfcs::medical_scenario::props_type>();
  auto med_sc_script = std::make_unique<pfcs::medical_scenario::training_script_type>();
  auto medical_scenario = std::make_unique<ScenarioSchema::medical_scenario_type>(std::move(id), std::move(med_sc_roles), std::move(med_sc_props), std::move(med_sc_script));


//
  auto lo_ref_citation_list = std::make_unique<pfcs::citation_list>();
  std::vector<Citation*> citation_vector = _db->get_citations();
 
  for (auto i = 0; i < citation_vector.size(); ++i) {
    auto& cit = citation_vector[i];
    auto key = "citation_" + std::to_string(i);
    auto citation = std::make_unique<pfcs::citation>(key, cit->title.toStdString(), cit->year.toStdString());
    citation->authors().push_back(cit->authors.toStdString());
    lo_ref_citation_list->citation().push_back(std::move(citation));
  }
//

  auto pfc = ScenarioSchema(std::move(conditions), std::move(treatment_plans), std::move(patient_states), std::move(syllabus), std::move(medical_scenario), std::move(lo_ref_citation_list));
  _pfc_content.str("");

  xml_schema::namespace_infomap info;
  info[""].name = "urn:sisostds:scenario:military:data:draft:msdl:1";
  info[""].schema = "MilitaryScenario_1.0.0.xsd";
  QSqlDatabase _db;
  if (_db.isValid()) {
    _db = QSqlDatabase::addDatabase("QSQLITE");
  }
  try {
    Scenario(_pfc_content, pfc, info);
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
