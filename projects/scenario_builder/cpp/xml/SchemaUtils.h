#ifndef SCHEMA_HELPER_H
#define SCHEMA_HELPER_H

#include <QString>
#include <memory>

#include "../xsd/cpp/MilitaryScenario_1.0.0.hxx"
#include "../xsd/cpp/pfc_scenario_0.1.hxx"

namespace pfc {

struct Assessment;
struct Citation;
struct Equipment;
struct Injury;
struct Objective;
struct Restriction;
struct Role;
struct Scene;
struct Treatment;
struct InjurySet;

namespace schema {
  static auto make_uuid() -> ::xml_schema::string;
  static auto make_uuid(std::string) -> ::xml_schema::string;

  static auto make_string(std::string) -> ::xml_schema::string;
  static auto make_time(QString) -> ::xml_schema::time;

  class PFC {
  public:
    static ScenarioSchema make_Scenario();
    static auto make_equipments() -> std::unique_ptr<ScenarioSchema::equipment_type>;
    static auto make_trauma_definitions() -> std::unique_ptr<ScenarioSchema::trauma_definitions_type>;
    static auto make_treatment_plans() -> std::unique_ptr<ScenarioSchema::treatment_plans_type>;
    static auto make_trauma_sets() -> std::unique_ptr<ScenarioSchema::trauma_sets_type>;
    static auto make_syllabus() -> std::unique_ptr<ScenarioSchema::syllabus_type>;
    static auto make_medical_scenario() -> std::unique_ptr<ScenarioSchema::medical_scenario_type>;
    static auto make_citation_list() -> std::unique_ptr<ScenarioSchema::works_cited_type>;

    static auto make_medical_reference_list() -> std::unique_ptr<schema::medical_reference_list>;
    static auto make_trauma(::pfc::Injury const* const) -> std::unique_ptr<trauma>;
    static auto make_treatment_plan(::pfc::Treatment const* const) -> std::unique_ptr<treatment_plan>;
    static auto make_learning_objective(::pfc::Objective const* const) -> std::unique_ptr<schema::learning_objective>;
    static auto make_citation(::pfc::Citation const* const) -> std::unique_ptr<schema::citation>;
    static auto make_scene(::pfc::Scene const* const) -> std::unique_ptr<schema::scene>;
    static auto make_equipment(::pfc::Equipment const* const) -> std::unique_ptr<schema::equipment>;
    static auto make_trauma_profile(::pfc::InjurySet const* const) -> std::unique_ptr<schema::trauma_profile>;
    static auto make_assessment(::pfc::Assessment const* const) -> std::unique_ptr<schema::assessment>;

    static auto make_citation_ref_list(QString ref_list) -> std::unique_ptr<schema::citation_ref_list>;
    static auto make_cpg_ref_list(QString ref_list) -> std::unique_ptr<schema::cpg_ref_list>;
    static auto make_equipment_ref_list(QString ref_list) -> std::unique_ptr<schema::equipment_ref_list>;
    static auto make_trauma_set_ref_list(QString ref_list) -> std::unique_ptr<schema::trauma_profile_ref_list>;
    static auto make_treatment_plan_ref_list(QString ref_list) -> std::unique_ptr<schema::treatment_plan_ref_list>;
    static auto make_trauma_occurance_list(QString ref_list, QString occurance_list, QString severity_list) -> std::unique_ptr<schema::trauma_occurence_list>;
    static auto make_properties_list(QString properties_list) -> std::unique_ptr<schema::properties_list>;
    static auto make_authors_list(QString name_list) -> schema::citation::authors_sequence;
  };

  class MSDL {
  public:
  };
} //namespace schema

} // namespace PFC
#endif //SCHEMA_HELPER_H