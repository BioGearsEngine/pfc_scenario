#include "SchemaUtils.h"

#include "../sql/SQLTables.h"

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
    return ::xml_schema::id(input);
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
      return ::xml_schema::time { 0, 0, components[0].toDouble() };
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
    return ScenarioSchema(make_conditions(),
                          make_treatment_plans(),
                          make_patient_states(),
                          make_syllabus(),
                          make_medical_scenario(),
                          make_citation_list());
  }
  //-------------------------------------------------------------------------------
  auto PFC::make_conditions() -> std::unique_ptr<ScenarioSchema::conditions_type>
  {
    //auto injury_id = std::make_unique<schema::injury::id_type>("");
    //auto injury_medical_name = std::make_unique<schema::injury::medical_name_type>();
    //auto injury_description = std::make_unique<schema::injury::description_type>();
    //auto num_range = schema::injury_severity_range();
    //auto injury_severity_range = std::make_unique<schema::injury::severity_range_type>(num_range);
    //auto injuries = std::make_unique<schema::injury>(std::move(injury_id), std::move(injury_medical_name), std::move(injury_description), std::move(injury_severity_range));
    return std::make_unique<schema::ScenarioSchema::conditions_type>();
  }

  auto PFC::make_treatment_plans() -> std::unique_ptr<ScenarioSchema::treatment_plans_type>
  {
    //auto treatment_id = std::make_unique<schema::treatment_plan::id_type>("");
    //auto treatment_description = std::make_unique<schema::treatment_plan::description_type>();
    //auto treatment_required_equipment = std::make_unique<schema::treatment_plan::required_equipment_type>();
    //auto treatment_plan = std::make_unique<pfc::schema::treatment_plan>(std::move(treatment_id), std::move(treatment_description), std::move(treatment_required_equipment));

    return std::make_unique<schema::ScenarioSchema::treatment_plans_type>();
  }
  //-------------------------------------------------------------------------------
  auto PFC::make_patient_states() -> std::unique_ptr<ScenarioSchema::patient_states_type>
  {
    return std::make_unique<schema::ScenarioSchema::patient_states_type>();
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
  auto PFC::make_injury(::pfc::Injury const* const input) -> std::unique_ptr<schema::injury>
  {

    auto num_range = pfc::schema::injury_severity_range();
    num_range.numeric_range(std::make_unique<pfc::schema::numeric_range>(input->severity_min, input->severity_max));

    auto injury = std::make_unique<pfc::schema::injury>(
      std::make_unique<pfc::schema::injury::id_type>("Injury_" + std::to_string(input->id + 1)),
      std::make_unique<pfc::schema::injury::medical_name_type>(input->medical_name.toStdString()),
      std::make_unique<pfc::schema::citation_ref_list>(),
      std::make_unique<pfc::schema::injury::description_type>(input->description.toStdString()),
      std::make_unique<pfc::schema::injury::severity_range_type>(num_range));
    injury->common_name(input->common_name.toStdString());
    injury->citations(make_citation_ref_list(input->common_name));

    return injury;
  }
  //------------------------------------------------------------------------------
  auto PFC::make_citation_ref_list(QString ref_list) -> std::unique_ptr<schema::citation_ref_list>
  {
    auto citation_ref_list = std::make_unique<schema::citation_ref_list>();
    for (auto& token : ref_list.split(';')) {
      citation_ref_list->citation_ref().push_back(token.toStdString());
    }
    return citation_ref_list;
  }
  //-------------------------------------------------------------------------------
  auto PFC::make_cpg_ref_list(QString ref_list) -> std::unique_ptr<schema::cpg_ref_list>
  {
    auto cpg_ref_list = std::make_unique<schema::cpg_ref_list>();
    for (auto& token : ref_list.split(';')) {
      cpg_ref_list->cpg_ref().push_back(token.toStdString());
    }
    return cpg_ref_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_equipment_ref_list(QString ref_list) -> std::unique_ptr<schema::equipment_ref_list>
  {
    auto equipment_ref_list = std::make_unique<schema::equipment_ref_list>();
    for (auto& token : ref_list.split(';')) {
      equipment_ref_list->equipment_refs().push_back(token.toStdString());
    }
    return equipment_ref_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_injury_set_ref_list(QString ref_list) -> std::unique_ptr<schema::injury_profile_ref_list>
  {
    auto injury_profile_ref_list = std::make_unique<schema::injury_profile_ref_list>();
    for (auto& token : ref_list.split(';')) {
      injury_profile_ref_list->injury_profile().push_back(token.toStdString());
    }
    return injury_profile_ref_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_treatment_plan_ref_list(QString ref_list) -> std::unique_ptr<schema::treatment_plan_ref_list>
  {
    auto treatment_plan_ref_list = std::make_unique<schema::treatment_plan_ref_list>();
    for (auto& token : ref_list.split(';')) {
      treatment_plan_ref_list->treatment_plan().push_back(token.toStdString());
    }
    return treatment_plan_ref_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_authors_list(QString name_list) -> schema::citation::authors_sequence
  {
    auto author_list = schema::citation::authors_sequence{};
    for (auto& token : name_list.split(';')) {
      author_list.push_back(token.toStdString());
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
                                                                                                                                          make_injury_set_ref_list(input->injury_profiles)));
    return objective;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_citation(::pfc::Citation const* const input) -> std::unique_ptr<schema::citation>
  {

    auto citation = std::make_unique<pfc::schema::citation>(make_uuid("citation_" + std::to_string(input->id)),
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
  auto PFC::make_medical_reference_list() -> std::unique_ptr<schema::medical_reference_list>
  {
    return std::make_unique<schema::medical_reference_list>(
      std::make_unique<citation_ref_list>(),
      std::make_unique<cpg_ref_list>());
  }
  //-----------------------------------------------------------------------------
} //namespace schema

} // namespace PFC