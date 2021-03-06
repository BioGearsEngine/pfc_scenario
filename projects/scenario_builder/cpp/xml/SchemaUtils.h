#ifndef SCHEMA_HELPER_H
#define SCHEMA_HELPER_H

#include <QString>
#include <memory>

#include <pfc/schema/pfc_scenario.hxx>
#include <pfc/schema/military_scenario_1.0.0.hxx>
#include "sql/SqlLite3Driver.h"

struct Assessment;
struct Citation;
struct Equipment;
struct EquipmentMap;
struct Event;
struct Trauma;
struct Objective;
struct Restriction;
struct Role;
struct Scene;
struct Treatment;
struct TraumaProfile;
namespace pfc {


namespace schema {
  static auto make_uuid() -> ::xml_schema::string;
  static auto make_uuid(std::string) -> ::xml_schema::string;

  static auto make_string(QString) -> ::xml_schema::string;
  static auto make_time(QString) -> ::xml_schema::time;

  class PFC {
  public:
    static ScenarioSchema make_Scenario();
    static auto make_summary() -> std::unique_ptr<ScenarioSchema::summary_type>;
    static auto make_author() -> std::unique_ptr<ScenarioSchema::author_type>;
    static auto make_equipments() -> std::unique_ptr<ScenarioSchema::equipment_type>;
    static auto make_trauma_definitions() -> std::unique_ptr<ScenarioSchema::trauma_definitions_type>;
    static auto make_treatment_plans() -> std::unique_ptr<ScenarioSchema::treatment_plans_type>;
    static auto make_truama_profiles() -> std::unique_ptr<ScenarioSchema::trauma_profiles_type>;
    static auto make_syllabus() -> std::unique_ptr<ScenarioSchema::syllabus_type>;
    static auto make_medical_scenario() -> std::unique_ptr<ScenarioSchema::medical_scenario_type>;
    static auto make_citation_list() -> std::unique_ptr<ScenarioSchema::works_cited_type>;
    static auto make_medical_reference_list() -> std::unique_ptr<schema::medical_reference_list>;

    static auto make_trauma(Trauma const* const, pfc::SQLite3Driver* _db) -> std::unique_ptr<trauma>;
    static auto make_treatment_plan(Treatment const* const, pfc::SQLite3Driver* _db) -> std::unique_ptr<treatment_plan>;
    static auto make_learning_objective(Objective const* const, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::learning_objective>;
    static auto make_citation(Citation const* const) -> std::unique_ptr<schema::citation>;
    static auto make_location(Location const* const, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::location>;
    static auto make_scene(Scene const* const, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::scene>;
    static auto make_equipment(Equipment const* const, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::equipment>;
    static auto make_trauma_profile(TraumaProfile const* const, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::trauma_profile>;
    static auto make_assessment(Assessment const* const, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::assessment>;
    static auto make_role(Role const* const) -> std::unique_ptr<schema::role>;
    static auto make_event(Event const* const) -> std::unique_ptr<schema::event>;
    static auto make_item(EquipmentMap const* const) -> std::unique_ptr<schema::item>;
    static auto make_role_ref(RoleMap const* const) -> std::unique_ptr<::pfc::schema::role_ref>;

    //!
    //!  All references in the XML file are by UUID but in the database they are by FK
    //!  We need the _db to properly generate the translation
    //!
    static auto make_citation_ref_list(QList<Citation*> ref_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::citation_ref_list>;
    static auto make_cpg_ref_list(QList<Citation*> ref_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::cpg_ref_list>;
    static auto make_equipment_ref_list(QList<Equipment*> ref_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::equipment_ref_list>;
    static auto make_trauma_set_ref_list(QList<TraumaProfile*> ref_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::trauma_profile_ref_list>;
    static auto make_treatment_plan_ref_list(QList<Treatment*> ref_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::treatment_plan_ref_list>;
    static auto make_trauma_occurance_list(QList<TraumaOccurence*> ref_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::trauma_occurence_list>;

    static auto make_equipment_properties_list(QList<EquipmentParameter*> properties_list) -> std::unique_ptr<schema::equipment_properties_list>;
    static auto make_property_field_list(QList<ParameterField*> properties_list) -> std::unique_ptr<schema::property_field_list>;
    static auto make_property_field_list(QList<QString> properties_list) -> std::unique_ptr<schema::property_field_list>;
    static auto make_authors_list(QString name_list) -> schema::citation::authors_sequence;
    static auto make_event_category(QString cat) -> schema::event_category_enum;
    static auto make_event_fidelity(QString fed) -> schema::event_fidelity_enum;
    static auto make_property_value_list(QList<EquipmentParameter*> property_name_list, QString property_value_list) -> std::unique_ptr<schema::property_value_list>;

    static auto load_assessments(std::unique_ptr<schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<schema::ScenarioSchema>;
    static auto load_authors(std::unique_ptr<schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<schema::ScenarioSchema>;
    static auto load_citations(std::unique_ptr<schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<schema::ScenarioSchema>;
    static auto load_equipment(std::unique_ptr<schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<schema::ScenarioSchema>;
    static auto load_events(std::unique_ptr<schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<schema::ScenarioSchema>;
    static auto load_images(QList<QString>&& known_images,  pfc::SQLite3Driver& _db, bool& wasSuccessful) -> void;
    static auto load_trauma(std::unique_ptr<schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<schema::ScenarioSchema>;
    static auto load_trauma_profiles(std::unique_ptr<schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<schema::ScenarioSchema>;
    static auto load_locations(std::unique_ptr<schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<schema::ScenarioSchema>;
    static auto load_objectives(std::unique_ptr<schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<schema::ScenarioSchema>;
    //static auto load_properties(std::unique_ptr<schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<schema::ScenarioSchema>;
    static auto load_roles(std::unique_ptr<schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<schema::ScenarioSchema>;
    static auto load_scenes(std::unique_ptr<schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<schema::ScenarioSchema>;
    static auto load_treatments(std::unique_ptr<schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<schema::ScenarioSchema>;
  };

  class MSDL {
  public:
  };
} //namespace schema

} // namespace PFC
#endif //SCHEMA_HELPER_H