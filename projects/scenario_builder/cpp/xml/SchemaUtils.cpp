#include "SchemaUtils.h"

#include "../sql/SQLTables.h"

#include <iostream>
#include <mutex>

#include <QString>

namespace pfc {

namespace schema {

  auto make_uuid() -> ::xml_schema::string
  {
    static int counter = 0;
    static std::mutex mutex;

    std::lock_guard<std::mutex> guard{ mutex };

    return ::xml_schema::id(std::to_string(++counter));
  }
  //-------------------------------------------------------------------------------
  auto make_uuid(std::string input) -> ::xml_schema::string
  {
    return ::xml_schema::string(input);
  }
  //--------------------------------------------------------------------------------
  auto make_string(std::string input) -> ::xml_schema::string
  {
    return ::xml_schema::string(input);
  }
  //-------------------------------------------------------------------------------
  auto make_time(QString input) -> ::xml_schema::time
  {

    auto components = input.split(':');
    switch (components.size()) {
    case 0:
      return ::xml_schema::time{ 0, 0, 0 };
    case 1:
      return ::xml_schema::time{ 0, 0, components[0].toDouble() };
    case 2:
      return ::xml_schema::time{ 0, components[0].toUShort(), components[1].toDouble() };
    case 3:
    default:
      return ::xml_schema::time{ components[0].toUShort(), components[1].toUShort(), components[2].toDouble() };
    }
  }

  //-------------------------------------------------------------------------------
  ScenarioSchema PFC::make_Scenario()
  {
    return ScenarioSchema(make_equipments(),
                          make_trauma_definitions(),
                          make_treatment_plans(),
                          make_trauma_sets(),
                          make_syllabus(),
                          make_medical_scenario(),
                          make_citation_list());
  }
  //-------------------------------------------------------------------------------
  auto PFC::make_equipments() -> std::unique_ptr<ScenarioSchema::equipment_type>
  {
    return std::make_unique<schema::ScenarioSchema::equipment_type>();
  }
  //-------------------------------------------------------------------------------
  auto PFC::make_trauma_definitions() -> std::unique_ptr<ScenarioSchema::trauma_definitions_type>
  {
    return std::make_unique<schema::ScenarioSchema::trauma_definitions_type>();
  }

  auto PFC::make_treatment_plans() -> std::unique_ptr<ScenarioSchema::treatment_plans_type>
  {
    return std::make_unique<schema::ScenarioSchema::treatment_plans_type>();
  }
  //-------------------------------------------------------------------------------
  auto PFC::make_trauma_sets() -> std::unique_ptr<ScenarioSchema::trauma_sets_type>
  {
    return std::make_unique<schema::ScenarioSchema::trauma_sets_type>();
  }
  //-------------------------------------------------------------------------------
  auto PFC::make_syllabus() -> std::unique_ptr<ScenarioSchema::syllabus_type>
  {
    auto syllabus = std::make_unique<schema::ScenarioSchema::syllabus_type>(std::make_unique<schema::learning_objective_list>(),
                                                                            std::make_unique<schema::assessment_list>(0));
    return syllabus;
  }
  //-------------------------------------------------------------------------------
  auto PFC::make_medical_scenario() -> std::unique_ptr<ScenarioSchema::medical_scenario_type>
  {
    auto id = ::xml_schema::string("Medical Scenario 1");
    auto med_sc_roles = std::make_unique<schema::medical_scenario::roles_type>();
    auto med_sc_script = std::make_unique<schema::medical_scenario::training_script_type>();
    return std::make_unique<schema::ScenarioSchema::medical_scenario_type>(std::move(id), std::move(med_sc_roles), std::move(med_sc_script));
  }
  //-------------------------------------------------------------------------------
  auto PFC::make_citation_list() -> std::unique_ptr<ScenarioSchema::works_cited_type>
  {
    return std::make_unique<schema::ScenarioSchema::works_cited_type>();
  }
  //-------------------------------------------------------------------------------
  auto PFC::make_trauma(::pfc::Injury const* const input) -> std::unique_ptr<schema::trauma>
  {

    auto num_range = pfc::schema::trauma_severity_range();
    num_range.numeric_range(std::make_unique<pfc::schema::numeric_range>(input->severity_min, input->severity_max));

    auto trauma = std::make_unique<pfc::schema::trauma>(
      std::make_unique<pfc::schema::trauma::id_type>("Injury_" + std::to_string(input->id)),
      std::make_unique<pfc::schema::trauma::medical_name_type>(input->medical_name.toStdString()),
      std::make_unique<pfc::schema::citation_ref_list>(),
      std::make_unique<pfc::schema::trauma::description_type>(input->description.toStdString()),
      std::make_unique<pfc::schema::trauma::severity_range_type>(num_range));
    trauma->common_name(input->common_name.toStdString());
    trauma->citations(make_citation_ref_list(input->citations));

    return trauma;
  }
  //------------------------------------------------------------------------------
  auto PFC::make_citation_ref_list(QString ref_list) -> std::unique_ptr<schema::citation_ref_list>
  {
    auto citation_ref_list = std::make_unique<schema::citation_ref_list>();
    for (auto& token : ref_list.split(';')) {
      if (!token.isEmpty()) {
        citation_ref_list->citation_ref().push_back(schema::make_string("Citation_" + token.toStdString()));
      }
    }
    return citation_ref_list;
  }
  //-------------------------------------------------------------------------------
  auto PFC::make_cpg_ref_list(QString ref_list) -> std::unique_ptr<schema::cpg_ref_list>
  {
    auto cpg_ref_list = std::make_unique<schema::cpg_ref_list>();
    for (auto& token : ref_list.split(';')) {
      if (!token.isEmpty()) {
        cpg_ref_list->cpg_ref().push_back("CPG_" + token.toStdString());
      }
    }
    return cpg_ref_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_equipment_ref_list(QString ref_list) -> std::unique_ptr<schema::equipment_ref_list>
  {
    auto equipment_ref_list = std::make_unique<schema::equipment_ref_list>();
    for (auto& token : ref_list.split(';')) {
      if (!token.isEmpty()) {
        equipment_ref_list->equipment_refs().push_back("Equipment_" + token.toStdString());
      }
    }
    return equipment_ref_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_trauma_set_ref_list(QString ref_list) -> std::unique_ptr<schema::trauma_profile_ref_list>
  {
    auto trauma_profile_ref_list = std::make_unique<schema::trauma_profile_ref_list>();
    for (auto& token : ref_list.split(';')) {
      if (!token.isEmpty()) {
        trauma_profile_ref_list->trauma_profile().push_back("Profile_" + token.toStdString());
      }
    }
    return trauma_profile_ref_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_treatment_plan_ref_list(QString ref_list) -> std::unique_ptr<schema::treatment_plan_ref_list>
  {
    auto treatment_plan_ref_list = std::make_unique<schema::treatment_plan_ref_list>();
    for (auto& token : ref_list.split(';')) {
      if (!token.isEmpty()) {
        treatment_plan_ref_list->treatment_plan().push_back("Treatment_" + token.toStdString());
      }
    }
    return treatment_plan_ref_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_trauma_occurance_list(QString ref_list, QString occurance_list, QString severity_list) -> std::unique_ptr<schema::trauma_occurence_list>
  {
    auto trauma_occurence_list = std::make_unique<schema::trauma_occurence_list>();
    auto refs = ref_list.split(';');
    auto occurences = occurance_list.split(';');
    auto severities = severity_list.split(';');

    auto ref_count = refs.size();
    auto occurence_count = occurences.size();
    auto severity_count = severities.size();
    for (auto i = 0; i < refs.size(); ++i) {
      if (!refs[i].isEmpty()) {
        try {
          auto ref = refs.at(i);
          auto occurence = occurences.at(i);
          auto severity = severities.at(i);
          trauma_occurence_list->trauma().push_back(std::make_unique<schema::trauma_occurence>(make_string("Trauma_" + ref.toStdString()),
                                                                                               make_string(occurence.toStdString()),
                                                                                               make_string(""),
                                                                                               make_string(severity.toStdString())));
        } catch (...) {
          std::cout << "Error index mismatch between refs,occurances, and severities";
        }
      }
    }
    return trauma_occurence_list;
  }

  //-----------------------------------------------------------------------------
  auto PFC::make_properties_list(QString properties_list) -> std::unique_ptr<schema::properties_list>
  {
    auto property_list = std::make_unique<schema::properties_list>();
    for (auto& token : properties_list.split(';')) {
      if (!token.isEmpty()) {
        auto name_value = token.split(':');
        if (name_value.size() == 2) {
          property_list->property().push_back(std::make_unique<schema::property>(name_value[0].toStdString(),
                                                                                 name_value[1].toStdString()));
        } else if (name_value.size() == 1 && !name_value[0].isEmpty()) {
          property_list->property().push_back(std::make_unique<schema::property>(name_value[0].toStdString(),
                                                                                 "NUMBER"));
        }
      }
    }
    return property_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_authors_list(QString name_list) -> schema::citation::authors_sequence
  {
    auto author_list = schema::citation::authors_sequence{};
    for (auto& token : name_list.split(';')) {
      if (!token.isEmpty()) {
        author_list.push_back(token.toStdString());
      }
    }
    return author_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_event_category(QString cat) -> schema::event_category_enum
  {
    auto category = schema::event_category_enum::value::ACTION;
    if (cat == "ACTION") {
      category = schema::event_category_enum::value::ACTION;
    } else if (cat == "DIALOG") {
      category = schema::event_category_enum::value::DIALOG;
    } else if (cat == "MOVMENT") {
      category = schema::event_category_enum::value::MOVMENT;
    } else if (cat == "SOUND") {
      category = schema::event_category_enum::value::SOUND;
    } else if (cat == "ENVIRONMENT") {
      category = schema::event_category_enum::value::ENVIRONMENT;
    }
    return category;
  }

  auto PFC::make_event_fidelity(QString fed) -> schema::event_fidelity_enum
  {
    auto fidelity = schema::event_fidelity_enum::value::HIGH;
    if (fed == "HIGH") {
      fidelity = schema::event_fidelity_enum::value::HIGH;
    } else if (fed == "MEDIUM") {
      fidelity = schema::event_fidelity_enum::value::MEDIUM;
    } else if (fed == "LOW") {
      fidelity = schema::event_fidelity_enum::value::LOW;
    }
    return fidelity;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_property_value_list(QString value_list) -> std::unique_ptr<schema::property_value_list>
  {
    auto list = std::make_unique<schema::property_value_list>();
    auto vec = value_list.split(';');
    for (auto value : vec) {
      if (!value.isEmpty()) {
        list->value().push_back( std::make_unique<schema::property_value>(value.toStdString()));
      }
    }
    return list;
  }

  //-----------------------------------------------------------------------------
  auto PFC::make_treatment_plan(::pfc::Treatment const* const input) -> std::unique_ptr<treatment_plan>
  {
    auto treatment_citation_list = std::make_unique<pfc::schema::citation_list>();
    auto treatment = std::make_unique<pfc::schema::treatment_plan>(schema::make_string("Treatment_" + std::to_string(input->id)),
                                                                   schema::make_string(input->description.toStdString()),
                                                                   make_equipment_ref_list(input->equipment),
                                                                   make_medical_reference_list());

    treatment->common_name(input->common_name.toStdString());
    treatment->references().citations(make_citation_ref_list(input->citations));
    treatment->references().cpgs(make_cpg_ref_list(input->cpgs));

    return treatment;
  }
  //------------------------------------------------------------------------------
  auto PFC::make_learning_objective(::pfc::Objective const* const input) -> std::unique_ptr<schema::learning_objective>
  {
    auto objective = std::make_unique<pfc::schema::learning_objective>(schema::make_uuid("Objective_" + std::to_string(input->id)),
                                                                       schema::make_string(input->name.toStdString()),
                                                                       schema::make_string(input->description.toStdString()),
                                                                       std::make_unique<pfc::schema::learning_objective::references_type>(make_citation_ref_list(input->citations),
                                                                                                                                          make_cpg_ref_list(input->cpgs)),
                                                                       std::make_unique<pfc::schema::learning_objective::relates_to_type>(make_treatment_plan_ref_list(input->treatment_plans),
                                                                                                                                          make_trauma_set_ref_list(input->injury_profiles)));
    return objective;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_citation(::pfc::Citation const* const input) -> std::unique_ptr<schema::citation>
  {

    auto citation = std::make_unique<pfc::schema::citation>(make_uuid("Citation_" + std::to_string(input->id)),
                                                            input->title.toStdString(),
                                                            input->year.toStdString());
    citation->authors(make_authors_list(input->authors));
    return citation;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_scene(::pfc::Scene const* const input) -> std::unique_ptr<schema::scene>
  {
    return std::make_unique<pfc::schema::scene>(schema::make_uuid("Location_" + std::to_string(input->id)),
                                                schema::make_string(input->name.toStdString()),
                                                schema::make_string(input->description.toStdString()),
                                                schema::make_time(input->time_of_day),
                                                input->time_in_simulation,
                                                std::make_unique<pfc::schema::scene::events_type>(),
                                                std::make_unique<pfc::schema::scene::items_type>(),
                                                std::make_unique<pfc::schema::scene::roles_type>());
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_equipment(::pfc::Equipment const* const input) -> std::unique_ptr<schema::equipment>
  {
    auto equipment = std::make_unique<pfc::schema::equipment>(schema::make_uuid("Equipment_" + std::to_string(input->id)),
                                                              schema::make_string(input->name.toStdString()),
                                                              schema::make_string(input->description.toStdString()),
                                                              make_citation_ref_list(input->citations),
                                                              make_properties_list(input->properties));
    equipment->type(input->type);
    equipment->image(input->image.toStdString());
    return equipment;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_trauma_profile(::pfc::InjurySet const* const input) -> std::unique_ptr<schema::trauma_profile>
  {

    auto trauma = std::make_unique<schema::trauma_profile>(make_uuid("TraumaProfile_" + std::to_string(input->id)),
                                                           make_string(input->name.toStdString()),
                                                           make_trauma_occurance_list(input->injuries, input->locations, input->severities),
                                                           make_treatment_plan_ref_list(input->treatments));
    trauma->physiology_state(input->physiology_file.toStdString());
    return trauma;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_assessment(::pfc::Assessment const* const input) -> std::unique_ptr<schema::assessment>
  {
    auto assessment = std::make_unique<schema::assessment>(make_uuid("Assessment_" + std::to_string(input->id)),
                                                           make_uuid("Objective_" + input->objective.toStdString()),
                                                           make_string(input->name.toStdString()),
                                                           make_string(input->description.toStdString()),
                                                           input->available_points,
                                                           make_string(input->criteria.toStdString()));
    return assessment;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_role(::pfc::Role const* const input) -> std::unique_ptr<schema::role>
  {
    auto role = std::make_unique<schema::role>(make_uuid("Role_" + std::to_string(input->id)),
                                               make_string(input->name.toStdString()),
                                               make_string(input->short_name.toStdString()),
                                               make_string(input->description.toStdString()));
    role->trauma_profile_ref(make_string(input->trauma_profile.toStdString()));
    return role;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_event(::pfc::Event const* const input) -> std::unique_ptr<schema::event>
  {
    auto event = std::make_unique<schema::event>(make_uuid("Event_" + std::to_string(input->id)),
                                                 make_string(input->name.toStdString()),
                                                 make_event_category(input->category),
                                                 make_event_fidelity(input->fidelity),
                                                 make_string("Actor_" + std::to_string(input->fk_actor_1)),
                                                 make_string("Actor_" + std::to_string(input->fk_actor_2)),
                                                 make_string(input->equipment.toStdString()),
                                                 make_string(input->details.toStdString()));

    return event;
  }

  auto PFC::make_item(::pfc::EquipmentMap const* const input) -> std::unique_ptr<schema::item>
  {
    auto item = std::make_unique<schema::item>(make_string(input->name.toStdString()),
                                               make_string("Equipment_" + std::to_string(input->id)),
                                               make_string(input->notes.toStdString()),
                                               make_property_value_list(input->property_values));
    return item;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_role_ref(::pfc::Role const* const input) -> std::unique_ptr<::xml_schema::string>
  {
    return std::make_unique<::xml_schema::string>("Role_" + std::to_string(input->id));
  }

  //-----------------------------------------------------------------------------
  auto PFC::make_medical_reference_list() -> std::unique_ptr<schema::medical_reference_list>
  {
    return std::make_unique<schema::medical_reference_list>(
      std::make_unique<citation_ref_list>(),
      std::make_unique<cpg_ref_list>());
  }
  //----------These will be filled in incremental commits-----------------------------------
  bool PFC::load_assessments(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db)
  {
    auto assessments = scenario_schema->syllabus().learning_assessments().assessment();
    for (auto assessment : assessments) {
      pfc::Assessment temp;
      temp.name = QString::fromStdString(assessment.name().text_content());
      temp.description = QString::fromStdString(assessment.description().text_content());
      temp.type = (assessment.points_avaiable() == 1) ? QString("binary") : QString("partial");
      temp.available_points = assessment.points_avaiable();
      temp.criteria = QString::fromStdString(assessment.criteria().text_content());
      if(!_db.update_assessment(&temp)) {
        return false;
      }
    }
    return true;
  }
  //-----------------------------------------------------------------------------
  bool PFC::load_citations(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db)
  {
    auto citations = scenario_schema->works_cited().citation();
    for (auto citation : citations) {
      pfc::Citation temp;
      temp.title = QString::fromStdString(citation.title().text_content());
      temp.authors = QString::fromStdString(citation.authors().back().text_content());
      temp.year = QString::fromStdString(citation.date());
      temp.id = std::stoi(citation.uuid().text_content().substr(citation.uuid().text_content().find("_")));
      //temp.key
      //temp.publisher
      if(!_db.update_citation(&temp)){
        return false;
      }
    }
    return true;
  }
  //-----------------------------------------------------------------------------
  bool PFC::load_equipment(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db)
  {
    auto equipments = scenario_schema->equipment().equipment();
    for (auto equipment : equipments) {
      pfc::Equipment temp;
      temp.name = QString::fromStdString(equipment.name().text_content());
      temp.description = QString::fromStdString(equipment.description().text_content());
      temp.type = equipment.type().get();
      temp.image = QString::fromStdString(equipment.image()->text_content());
      temp.id = std::stoi(equipment.id().text_content().substr(equipment.id().text_content().find("_")));
      std::string citations;
      for (auto citation : equipment.citations().citation_ref()){
        citations += (citation.text_content()+";");
      }
      if(!citations.empty()){
        citations.pop_back();
      }
      temp.citations = QString::fromStdString(citations);
      if(!_db.update_equipment(&temp)){
        return false;
      }
    }
    return true;
  }
  //-----------------------------------------------------------------------------
  bool PFC::load_events(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db)
  {
    auto scenes = scenario_schema->medical_scenario().training_script().scene();
    for (auto scene : scenes) {
      for (auto event : scene.events().event()) {
        pfc::Event temp;
        temp.name = QString::fromStdString(event.name().text_content());
        temp.description = QString::fromStdString(event.details().text_content());
        temp.category = QString::fromStdString(event.category().text_content());
        temp.fidelity = QString::fromStdString(event.fidelity().text_content());
        temp.fk_actor_1 = std::stoi(event.actor_1().text_content().substr(event.actor_1().text_content().find("_")));
        temp.fk_actor_2 = std::stoi(event.actor_2().text_content().substr(event.actor_2().text_content().find("_")));
        temp.equipment = QString::fromStdString(event.equipment().text_content());
        if(!_db.update_event(&temp)){
          return false;
        }
      }
    }
    return true;
  }
  //-----------------------------------------------------------------------------
  bool PFC::load_injuries(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db)
  {
    auto injuries = scenario_schema->trauma_definitions().trauma();
    for (auto injury : injuries) {
      pfc::Injury temp;
      temp.medical_name = QString::fromStdString(injury.medical_name().text_content());
      temp.common_name = QString::fromStdString(injury.common_name()->text_content()); // Why is this different from medical_name at all?
      temp.description = QString::fromStdString(injury.description().text_content());
      temp.severity_min = injury.severity_range().numeric_range().get().min();
      temp.severity_max = injury.severity_range().numeric_range().get().max();
      std::string citations;
      for (auto citation : injury.citations().citation_ref()) {
        citations += ((citation.text_content().substr(citation.text_content().find("_")))+";");
      }
      if(!citations.empty()){
        citations.pop_back();
      }
      temp.citations = QString::fromStdString(citations);
      if(!_db.update_injury(&temp)){
        return false;
      }
    }
    return true;
  }
  //-----------------------------------------------------------------------------
  bool PFC::load_injury_sets(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db)
  {
    auto trauma_sets = scenario_schema->trauma_sets();
    auto trauma_profiles = trauma_sets.trauma_profile();
    for (auto trauma_profile : trauma_profiles) {
      pfc::InjurySet temp;
      temp.name = QString::fromStdString(trauma_profile.name().text_content());
      std::string injuries;
      std::string locations;
      std::string severities;
      std::string descriptions;
      for (auto injury : trauma_profile.injuries().trauma() ) {
        std::string injury_id = std::string(injury.id());
        injuries += (injury_id.substr(injury_id.find("_"))+";");
        std::string location = std::string(injury.location());
        locations += (location+";");
        std::string severity = std::string(injury.severity());
        severities += (severity+";");
        std::string description = std::string(injury.description());
        descriptions += (description+";");
      }
      if(!injuries.empty()){ //Have to remove last ';' but pop_back() is undefined behavior on empty string
        injuries.pop_back();
      }
      if(!locations.empty()){
        locations.pop_back();
      }
      if(!severities.empty()){
        severities.pop_back();
      }
      if(!descriptions.empty()){
        descriptions.pop_back();
      }
      temp.injuries = QString::fromStdString(injuries);
      temp.locations = QString::fromStdString(locations);
      temp.severities = QString::fromStdString(injuries);
      temp.description = QString::fromStdString(descriptions);
      if(!_db.update_injury_set(&temp)){
        return false;
      }
    }
    return true;
  }
  //-----------------------------------------------------------------------------
  bool PFC::load_locations(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db)
  {
    auto locations = scenario_schema->medical_scenario().training_script().scene();
    for (auto location : locations) {
      pfc::Location temp;
      temp.name = QString::fromStdString(std::string(location.name().text_content())+" Location");
      temp.scene_name = QString::fromStdString(location.name());
      temp.time_of_day = QString::fromStdString(location.time_of_day().text_content());
      if(!_db.update_location(&temp)){
        return false;
      }
    }
    return true;
  }
  //-----------------------------------------------------------------------------
  bool PFC::load_objectives(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db)
  {
    auto objectives = scenario_schema->syllabus().learning_objectives().objective();
    for (auto objective : objectives) {
      pfc::Objective temp;
      temp.name = QString::fromStdString(objective.name().text_content());
      temp.description = QString::fromStdString(objective.description().text_content());
      std::string citations;
      for (auto citation : objective.references().citations().citation_ref()){
        citations += ((citation.text_content().substr(citation.text_content().find("_")))+";");
      }
      if(!citations.empty()){
        citations.pop_back();
      }
      temp.citations = QString::fromStdString(citations);
      if(!_db.update_objective(&temp)){
        return false;
      }
    }
    return true;
  }
  //-----------------------------------------------------------------------------
  bool PFC::load_roles(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db)
  {
    auto roles = scenario_schema->medical_scenario().roles().role();
    for (auto role : roles) {
      pfc::Role temp;
      temp.name = QString::fromStdString(role.name().text_content());
      temp.description = QString::fromStdString(role.description().text_content());
      if(!_db.update_role(&temp)){
        return false;
      }
    }
    return true;
  }
  //-----------------------------------------------------------------------------
  bool PFC::load_scenes(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db)
  {
    auto scenes = scenario_schema->medical_scenario().training_script().scene();
    for (auto scene : scenes) {
      pfc::Scene temp;
      temp.name = QString::fromStdString(scene.name().text_content());
      temp.description = QString::fromStdString(scene.description().text_content());
      temp.time_of_day = QString::fromStdString(scene.time_of_day().text_content());
      temp.time_in_simulation = scene.time_in_simulation();
      //temp.weather - where do I get this?
      //temp.events - this is slightly more complicated to figure out
      //temp.items - same with all of the fields remaining really
      if(!_db.update_scene(&temp)){
        return false;
      }
    }
    return true;
  }
  //-----------------------------------------------------------------------------
  bool PFC::load_treatments(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db)
  {
    auto treatments = scenario_schema->treatment_plans().treatment_plan();
    for (auto treatment : treatments) {
      pfc::Treatment temp;
      //temp.medical_name = QString::fromStdString(treatment.medical_name().text_content());
      temp.common_name = QString::fromStdString(treatment.common_name()->text_content()); // Why is this different from medical_name at all?
      temp.description = QString::fromStdString(treatment.description().text_content());
      std::string citations;
      for (auto citation : treatment.references().citations().citation_ref()) {
        citations += ((citation.text_content().substr(citation.text_content().find("_"))) + ";");
      }
      if (!citations.empty()) {
        citations.pop_back();
      }
      std::string equipments;
      for (auto equipment : treatment.required_equipment().equipment_refs()){
        equipments += ((equipment.text_content().substr(equipment.text_content().find("_")))+";");
      }
      if (!equipments.empty()) {
        equipments.pop_back();
      }
      temp.citations = QString::fromStdString(citations);
      temp.equipment = QString::fromStdString(equipments);
      if(!_db.update_treatment(&temp)){
        return false;
      }
    }
    return true;
  }
  //-----------------------------------------------------------------------------

} //namespace schema

} // namespace PFC