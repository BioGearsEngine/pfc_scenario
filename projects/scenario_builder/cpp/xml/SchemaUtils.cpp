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
    auto id = ::xml_schema::id("");
    auto med_sc_roles = std::make_unique<schema::medical_scenario::roles_type>();
    auto med_sc_props = std::make_unique<schema::medical_scenario::props_type>();
    auto med_sc_script = std::make_unique<schema::medical_scenario::training_script_type>();
    return std::make_unique<schema::ScenarioSchema::medical_scenario_type>(std::move(id), std::move(med_sc_roles), std::move(med_sc_props), std::move(med_sc_script));
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
        } catch (std::exception e) {
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
                                                std::make_unique<pfc::schema::scene::events_type>());
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
  auto PFC::make_medical_reference_list() -> std::unique_ptr<schema::medical_reference_list>
  {
    return std::make_unique<schema::medical_reference_list>(
      std::make_unique<citation_ref_list>(),
      std::make_unique<cpg_ref_list>());
  }
  //-----------------------------------------------------------------------------
} //namespace schema

} // namespace PFC