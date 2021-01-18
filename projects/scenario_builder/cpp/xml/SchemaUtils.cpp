#include "SchemaUtils.h"

#include "../sql/SQLTables.h"

#include <iostream>
#include <mutex>
#include <string>

#include <QDebug>
#include <QString>
#include <QUuid>

namespace pfc {

namespace schema {
  //-----------------------------------------------------------------------------
  auto generate_location_name(std::string const scene_name) -> QString
  {
    return QString::fromStdString(scene_name + " Location");
  }
  //-----------------------------------------------------------------------------
  auto find_actor(std::string UUID, std::unique_ptr<::pfc::schema::ScenarioSchema>& scenario_schema, pfc::SQLite3Driver& _db) -> std::string
  {
    auto& roles = scenario_schema->medical_scenario().roles().role();
    for (auto& role : roles) {
      if (role.id() == UUID) {
        Role row;
        row.name = role.name().c_str();
        _db.select_role(&row);
        return std::to_string(row.id);
      }
    }
    return "";
  }
  //-----------------------------------------------------------------------------
  auto find_citation(std::string UUID, std::unique_ptr<::pfc::schema::ScenarioSchema>& scenario_schema, pfc::SQLite3Driver& _db) -> std::string
  {
    auto& citations = scenario_schema->works_cited().citation();
    for (auto& ref : citations) {
      if (ref.uuid() == UUID) {
        Citation citation;
        citation.title = ref.title().c_str();
        _db.select_citation(&citation);
        return std::to_string(citation.id);
      }
    }
    return "";
  }
  //-----------------------------------------------------------------------------
  auto find_equipment(std::string UUID, std::unique_ptr<::pfc::schema::ScenarioSchema>& scenario_schema, pfc::SQLite3Driver& _db) -> std::string
  {
    auto& equipments = scenario_schema->equipment().equipment();
    for (auto& ref : equipments) {
      if (ref.id() == UUID) { //ref.uuid() == UUID) {
        Equipment equipment;
        equipment.name = ref.name().c_str();
        _db.select_equipment(&equipment);
        return std::to_string(equipment.id);
        ;
      }
    }
    return "";
  }
  //-----------------------------------------------------------------------------
  auto find_injury(std::string UUID, std::unique_ptr<::pfc::schema::ScenarioSchema>& scenario_schema, pfc::SQLite3Driver& _db) -> std::string
  {
    auto& traumas = scenario_schema->trauma_definitions().trauma();
    for (auto& ref : traumas) {
      if (ref.id() == UUID) { //ref.uuid() == UUID) {
        Trauma trauma;
        trauma.medical_name = ref.medical_name().c_str();
        _db.select_trauma(&trauma);
        return std::to_string(trauma.id);
      }
    }
    return "";
  }

  //-------------------------------------------------------------------------------
  auto make_uuid() -> ::xml_schema::string
  {
    return QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
  }
  //-------------------------------------------------------------------------------
  auto make_uuid(std::string input) -> ::xml_schema::string
  {
    return QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString();
  }
  //--------------------------------------------------------------------------------
  auto make_string(QString input) -> ::xml_schema::string
  {
    return ::xml_schema::string(input.toStdString());
  }
  //-------------------------------------------------------------------------------
  auto make_time(QString input) -> ::xml_schema::time
  {

    auto components = input.split(':');
    switch (components.size()) {
    case 0:
      return ::xml_schema::time { 0, 0, 0 };
    case 1:
      return ::xml_schema::time { 0, 0, components[0].toDouble() };
    case 2:
      return ::xml_schema::time { 0, components[0].toUShort(), components[1].toDouble() };
    case 3:
    default:
      return ::xml_schema::time { components[0].toUShort(), components[1].toUShort(), components[2].toDouble() };
    }
  }
  //-------------------------------------------------------------------------------
  ScenarioSchema PFC::make_Scenario()
  {
    return ScenarioSchema(
      make_summary(),
      make_author(),
      make_equipments(),
      make_trauma_definitions(),
      make_treatment_plans(),
      make_trauma_sets(),
      make_syllabus(),
      make_medical_scenario(),
      make_citation_list());
  }
  //-------------------------------------------------------------------------------
  auto PFC::make_summary() -> std::unique_ptr<ScenarioSchema::summary_type>
  {
    return std::make_unique<schema::ScenarioSchema::summary_type>();
  }
  //-------------------------------------------------------------------------------
  auto PFC::make_author() -> std::unique_ptr<ScenarioSchema::author_type>
  {
    return std::make_unique<schema::ScenarioSchema::author_type>();
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
  auto PFC::make_trauma_sets() -> std::unique_ptr<ScenarioSchema::trauma_profiles_type>
  {
    return std::make_unique<schema::ScenarioSchema::trauma_profiles_type>();
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
    auto med_sc_locations = std::make_unique<schema::medical_scenario::locations_type>();
    return std::make_unique<schema::ScenarioSchema::medical_scenario_type>(std::move(id),
                                                                           std::move(med_sc_roles),
                                                                           std::move(med_sc_locations),
                                                                           std::move(med_sc_script));
  }
  //-------------------------------------------------------------------------------
  auto PFC::make_citation_list() -> std::unique_ptr<ScenarioSchema::works_cited_type>
  {
    return std::make_unique<schema::ScenarioSchema::works_cited_type>();
  }
  //-------------------------------------------------------------------------------
  auto PFC::make_trauma(Trauma const* const input, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::trauma>
  {

    auto num_range = pfc::schema::trauma_severity_range();
    num_range.numeric_range(std::make_unique<pfc::schema::numeric_range>(input->lower_bound, input->upper_bound));

    auto trauma = std::make_unique<pfc::schema::trauma>(
      std::make_unique<pfc::schema::trauma::id_type>(make_string(input->uuid)),
      std::make_unique<pfc::schema::trauma::medical_name_type>(make_string(input->medical_name)),
      std::make_unique<pfc::schema::citation_ref_list>(),
      std::make_unique<pfc::schema::trauma::description_type>(make_string(input->description)),
      std::make_unique<pfc::schema::trauma::severity_range_type>(num_range),
      std::make_unique<pfc::schema::trauma::treatments_type>());
    trauma->common_name(make_string(input->common_name));

    for (auto citation : input->citations) {
      trauma->citations().citation_ref().push_back(make_string(citation->uuid));
    }

    return trauma;
  }
  //------------------------------------------------------------------------------
  auto PFC::make_citation_ref_list(QList<Citation*> ref_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::citation_ref_list>
  {
    auto citation_ref_list = std::make_unique<schema::citation_ref_list>();
    for (auto citation : ref_list) {
      if (_db->select_citation(citation)) {
        citation_ref_list->citation_ref().push_back(schema::make_string(citation->uuid));
      }
    }
    return citation_ref_list;
  }
  //-------------------------------------------------------------------------------
  auto PFC::make_cpg_ref_list(QList<Citation*> ref_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::cpg_ref_list>
  {
    auto cpg_ref_list = std::make_unique<schema::cpg_ref_list>();
    for (auto& cpg : ref_list) {
      if (_db->select_citation(cpg)) {
        cpg_ref_list->cpg_ref().push_back(schema::make_string(cpg->uuid));
      }
    }
    return cpg_ref_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_equipment_ref_list(QList<Equipment*> ref_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::equipment_ref_list>
  {
    auto equipment_ref_list = std::make_unique<schema::equipment_ref_list>();
    for (auto equipment : ref_list) {
      if (_db->select_equipment(equipment)) {
        equipment_ref_list->equipment_refs().push_back(schema::make_string(equipment->uuid));
      }
    }
    return equipment_ref_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_trauma_set_ref_list(QList<TraumaProfile*> ref_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::trauma_profile_ref_list>
  {
    auto trauma_profile_ref_list = std::make_unique<schema::trauma_profile_ref_list>();

    for (auto trauma_profile : ref_list) {
      if (_db->select_trauma_profile(trauma_profile)) {
        trauma_profile_ref_list->trauma_profile().push_back(schema::make_string(trauma_profile->uuid));
      }
    }
    return trauma_profile_ref_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_treatment_plan_ref_list(QList<Treatment*> ref_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::treatment_plan_ref_list>
  {
    auto treatment_plan_ref_list = std::make_unique<schema::treatment_plan_ref_list>();

    for (auto& treatment_plan : ref_list) {
      if (_db->select_treatment(treatment_plan)) {
        treatment_plan_ref_list->treatment_plan().push_back(schema::make_string(treatment_plan->uuid));
      }
    }
    return treatment_plan_ref_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_trauma_occurance_list(QList<TraumaOccurence*> ref_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::trauma_occurence_list>
  {
    auto trauma_occurence_list = std::make_unique<schema::trauma_occurence_list>();

    for (auto occurence : ref_list) {
      if (_db->select_trauma(occurence->fk_trauma)) {

        auto const trauma = occurence->fk_trauma;

        trauma_occurence_list->trauma().push_back(std::make_unique<schema::trauma_occurence>(
          make_string(trauma->uuid),
          make_string(occurence->location),
          make_string(occurence->description),
          make_string(occurence->severity))

        );
      }
    }
    return trauma_occurence_list;
  }
  //-----------------------------------------------------------------------------------------------
  //!
  //!   The Database encodes Properties as the following
  //!   Property-List = <Name>:<Type>:[<field>...][;[<Name>:<Type>:[<Detail>...]..]
  //!   <field>       = <Name>,<Type>
  //!  Future subdivisions of fields can be comma delimited
  //!  Future support for quotes will allow the presence of delimiters in fields
  auto PFC::make_equipment_properties_list(QList<EquipmentParameter*> properties_list) -> std::unique_ptr<schema::equipment_properties_list>
  {
    auto property_list = std::make_unique<schema::equipment_properties_list>();

    for (auto& property : properties_list) {
      if (property->eType == Sustain::ENUM) {
        property_list->property().push_back(
          std::make_unique<equipment_property>(schema::make_string(property->name),
                                               schema::make_string(TypeToString(property->eType)),
                                               make_property_field_list(property->enumOptions)));
      } else {

        property_list->property().push_back(
          std::make_unique<equipment_property>(schema::make_string(property->name),
                                               schema::make_string(TypeToString(property->eType)),
                                               make_property_field_list(property->fields)));
      }
    }
    return property_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_property_field_list(QList<ParameterField*> parameter_field_list) -> std::unique_ptr<schema::property_field_list>
  {
    auto property_field_list = std::make_unique<schema::property_field_list>();
    for (auto field : parameter_field_list) {
      property_field_list->field().push_back(std::make_unique<schema::field_type>(
        schema::make_string(field->name),
        schema::make_string(TypeToString(field->eType))));
    }
    return property_field_list;
  }
  auto PFC::make_property_field_list(QList<QString> parameter_field_list) -> std::unique_ptr<schema::property_field_list>
  {
    auto property_field_list = std::make_unique<schema::property_field_list>();
    for (auto field : parameter_field_list) {
      property_field_list->field().push_back(std::make_unique<schema::field_type>(
        schema::make_string(field),
        schema::make_string("OPTION")));
    }
    return property_field_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_authors_list(QString name_list) -> schema::citation::authors_sequence
  {
    auto author_list = schema::citation::authors_sequence {};
    for (auto& token : name_list.split(';')) {
      if (!token.isEmpty()) {
        author_list.push_back(make_string(token));
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
  //-----------------------------------------------------------------------------
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
  auto PFC::make_property_value_list(QList<EquipmentParameter*> name_list, QString value_list) -> std::unique_ptr<schema::property_value_list>
  {
    //TODO: Get the names of each property
    auto list = std::make_unique<schema::property_value_list>();

    auto values = value_list.split(';');

    auto size = std::min(name_list.size(), values.size());
    for (auto i = 0; i < size; ++i) {
      auto value = (values[i].isEmpty() ? "" : values[i]);
      list->value().push_back(std::make_unique<schema::property_value>(make_string(name_list[i]->name), make_string(value)));
    }
    return list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_treatment_plan(Treatment const* const input, pfc::SQLite3Driver* _db) -> std::unique_ptr<treatment_plan>
  {
    auto treatment_citation_list = std::make_unique<pfc::schema::citation_list>();
    auto treatment = std::make_unique<pfc::schema::treatment_plan>(schema::make_string(input->uuid),
                                                                   schema::make_string(input->description),
                                                                   make_equipment_ref_list(input->equipment, _db),
                                                                   make_medical_reference_list());

    treatment->common_name(make_string(input->common_name));
    treatment->references().citations(make_citation_ref_list(input->citations, _db));
    treatment->references().cpgs(make_cpg_ref_list(input->cpgs, _db));
    treatment->medical_name(make_string(input->medical_name));

    return treatment;
  }
  //------------------------------------------------------------------------------
  auto PFC::make_learning_objective(Objective const* const input, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::learning_objective>
  {
    auto objective = std::make_unique<pfc::schema::learning_objective>(schema::make_string(input->uuid),
                                                                       schema::make_string(input->name),
                                                                       schema::make_string(input->description),
                                                                       std::make_unique<pfc::schema::learning_objective::references_type>(make_citation_ref_list(input->_citations, _db),
                                                                                                                                          make_cpg_ref_list(input->_cpgs, _db)),
                                                                       std::make_unique<pfc::schema::learning_objective::relates_to_type>(make_treatment_plan_ref_list(input->_treatment_plans, _db),
                                                                                                                                          make_trauma_set_ref_list(input->_trauma_profiles, _db)));
    return objective;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_citation(Citation const* const input) -> std::unique_ptr<schema::citation>
  {

    auto citation = std::make_unique<pfc::schema::citation>(make_string(input->uuid),
                                                            make_string(input->title),
                                                            make_string(input->year));
    citation->authors(make_authors_list(input->authors));
    citation->key().set(make_string(input->key));
    return citation;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_location(Location const* const input, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::location>
  {
    auto location = std::make_unique<pfc::schema::location>(schema::make_string(input->uuid),
                                                            schema::make_string(input->name),
                                                            schema::make_string(input->description),
                                                            schema::make_string(input->environment));
    return location;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_scene(Scene const* const input, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::scene>
  {

    Location* location = _db->getLocationOfScene(const_cast<Scene*>(input));
    auto scene = std::make_unique<pfc::schema::scene>(schema::make_string(input->uuid),
                                                      schema::make_string(location->uuid),
                                                      schema::make_string(input->name),
                                                      schema::make_string(input->description),
                                                      input->time_of_day,
                                                      input->time_in_simulation,
                                                      std::make_unique<pfc::schema::scene::events_type>(),
                                                      std::make_unique<pfc::schema::scene::items_type>(),
                                                      std::make_unique<pfc::schema::scene::roles_type>());

    scene->weather(schema::make_string(input->weather));
    //scene->details(schema::make_string(input->details));
    return scene;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_equipment(Equipment const* const input, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::equipment>
  {
    auto equipment = std::make_unique<pfc::schema::equipment>(schema::make_string(input->uuid),
                                                              schema::make_string(input->name),
                                                              schema::make_string(input->summary),
                                                              schema::make_string(input->description),
                                                              make_citation_ref_list(input->citations, _db),
                                                              make_equipment_properties_list(input->parameters));
    equipment->type(input->type);
    equipment->image(make_string(input->image));

    return equipment;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_trauma_profile(TraumaProfile const* const input, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::trauma_profile>
  {

    auto profile = std::make_unique<schema::trauma_profile>(make_string(input->uuid),
                                                            make_string(input->name),
                                                            make_trauma_occurance_list(input->traumas, _db));
    if (input->physiologyTree.size() > 0) {
      profile->physiology_state(make_string(input->physiologyTree[0]));
    }
    return profile;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_assessment(Assessment const* const input, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::assessment>
  {

    Objective objRef;

    if (!(input->fk_objective->id == -1)) {
      objRef.id = input->fk_objective->id;
      _db->select_objective(&objRef);
    }
    auto assessment = std::make_unique<schema::assessment>(make_string(input->uuid),
                                                           make_string(objRef.uuid),
                                                           make_string(input->name),
                                                           make_string(input->description),
                                                           input->available_points,
                                                           make_string(input->criteria));
    return assessment;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_role(Role const* const input) -> std::unique_ptr<schema::role>
  {
    auto role = std::make_unique<schema::role>(make_string(input->uuid),
                                               make_string(input->name),
                                               make_string(input->category),
                                               make_string(input->description));
    role->trauma_profile_ref(make_string(input->trauma_profile->uuid));
    return role;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_event(Event const* const input) -> std::unique_ptr<schema::event>
  {
    auto event = std::make_unique<schema::event>(make_string(input->uuid),
                                                 make_string(input->name),
                                                 make_event_category(input->category),
                                                 make_event_fidelity(input->fidelity),
                                                 make_string(input->fk_actor_1->uuid),
                                                 make_string(input->fk_actor_2->uuid),
                                                 make_string(input->fk_equipment->uuid),
                                                 make_string(input->details),
                                                 make_string(input->description));

    return event;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_item(EquipmentMap const* const input) -> std::unique_ptr<schema::item>
  {
    auto item = std::make_unique<schema::item>(make_string(input->name),
                                               make_string(input->equipment->uuid),
                                               make_string(input->notes),
                                               make_property_value_list(input->equipment->parameters, input->property_values));

    return item;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_role_ref(RoleMap const* const input) -> std::unique_ptr<::pfc::schema::role_ref>
  {
    auto role_ref =  std::make_unique<::pfc::schema::role_ref>(schema::make_string(input->fk_role->uuid));
    if ( !input->category.isEmpty() ) {
      role_ref->category(make_string(input->category)) ;
    }
    return role_ref;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_medical_reference_list() -> std::unique_ptr<schema::medical_reference_list>
  {
    return std::make_unique<schema::medical_reference_list>(
      std::make_unique<citation_ref_list>(),
      std::make_unique<cpg_ref_list>());
  }
  //--------------------------------------------
  auto PFC::load_authors(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<::pfc::schema::ScenarioSchema>
  {
    auto author = scenario_schema->author();
    Author temp;

    temp.uuid = (author.id().present()) ? QString::fromStdString(author.id().get())
                                        : QUuid::createUuid().toString(QUuid::WithoutBraces);

    temp.first = QString::fromStdString(author.first_name().get());
    temp.last = QString::fromStdString(author.last_name().get());
    temp.phone = QString::fromStdString(author.phone_number().get());
    temp.email = QString::fromStdString(author.email().get());
    temp.zip = QString::fromStdString(author.zip().get());
    temp.state = QString::fromStdString(author.state().get());
    temp.country = QString::fromStdString(author.country().get());
    if (!_db.update_author(&temp)) {
      wasSuccessful = false;
      return scenario_schema;
    }
    wasSuccessful = true;
    return scenario_schema;
  }
  //-----------------------------------------------------------------------------
  auto PFC::load_assessments(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<::pfc::schema::ScenarioSchema>
  {
    auto assessments = scenario_schema->syllabus().learning_assessments().assessment();
    for (auto assessment : assessments) {
      Assessment temp;
      temp.uuid = QString::fromStdString(assessment.id());
      temp.name = QString::fromStdString(assessment.name());
      temp.description = QString::fromStdString(assessment.description());
      temp.type = (assessment.points_avaiable() == 1) ? QString("binary") : QString("partial");
      temp.available_points = assessment.points_avaiable();
      temp.criteria = QString::fromStdString(assessment.criteria());
      if (!_db.update_assessment(&temp)) {
        wasSuccessful = false;
        return scenario_schema;
      }
    }
    wasSuccessful = true;
    return scenario_schema;
  }
  //-----------------------------------------------------------------------------
  auto PFC::load_citations(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<::pfc::schema::ScenarioSchema>
  {
    auto citations = scenario_schema->works_cited().citation();
    for (auto citation : citations) {
      Citation temp;
      temp.uuid = QString::fromStdString(citation.uuid());
      temp.title = QString::fromStdString(citation.title());
      temp.authors = QString::fromStdString(citation.authors().back());
      temp.year = QString::fromStdString(citation.date());
      temp.key = QString::fromStdString(citation.key().get());
      //temp.publisher
      if (!_db.update_citation(&temp)) {
        wasSuccessful = false;
        return scenario_schema;
      }
    }
    wasSuccessful = true;
    return scenario_schema;
  }
  //-----------------------------------------------------------------------------
  auto PFC::load_equipment(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<::pfc::schema::ScenarioSchema>
  {
    auto equipments = scenario_schema->equipment().equipment();
    for (auto equipment : equipments) {
      Equipment temp;
      temp.uuid = QString::fromStdString(equipment.id());
      temp.name = QString::fromStdString(equipment.name());
      temp.summary = QString::fromStdString(equipment.summary());
      temp.description = QString::fromStdString(equipment.description());
      temp.type = equipment.type().get();
      temp.image = QString::fromStdString(*equipment.image());

      Citation* citation;
      for (auto& citation_uuid : equipment.citations().citation_ref()) {
        citation = new Citation(&temp);
        citation->uuid = citation_uuid.c_str();
        _db.select_citation(citation);
        temp.citations.push_back(citation);
      }

      for (auto& property : equipment.properties().property()) {
        temp.appendParameter(QString::fromStdString(property.name()), TypeFromString(property.type()));
        for (auto field_info : property.fields().field()) {
          if (temp.parameters.back()->eType == Sustain::ENUM) {
            temp.parameters.back()->enumOptions.push_back(QString::fromStdString(field_info.name()));
          } else {
            temp.parameters.back()->appendField(field_info.name(), TypeFromString(field_info.type()));
          }
        }
      }

      if (!_db.update_equipment(&temp)) {
        wasSuccessful = false;
        return scenario_schema;
      }
    }
    wasSuccessful = true;
    return scenario_schema;
  }
  //-----------------------------------------------------------------------------
  auto PFC::load_events(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<::pfc::schema::ScenarioSchema>
  {
    auto scenes = scenario_schema->medical_scenario().training_script().scene();
    for (auto scene : scenes) {
      for (auto event : scene.events().event()) {
        Event temp;
        temp.name = QString::fromStdString(event.name());
        temp.uuid = QString::fromStdString(event.id());
        temp.description = QString::fromStdString(event.description());
        temp.category = QString::fromStdString(event.category());
        temp.fidelity = QString::fromStdString(event.fidelity());

        std::string actor1_id = find_actor(event.actor_1(), scenario_schema, _db);
        temp.fk_actor_1->uuid = QString::fromStdString(actor1_id);
        std::string actor2_id = find_actor(event.actor_2(), scenario_schema, _db);
        temp.fk_actor_2->uuid = QString::fromStdString(actor2_id);

        temp.fk_equipment->uuid = QString::fromStdString(event.equipment());
        temp.details = QString::fromStdString(event.details());

        if (!_db.update_event(&temp)) {
          wasSuccessful = false;
          return scenario_schema;
        }
      }
    }
    wasSuccessful = true;
    return scenario_schema;
  }
  //-----------------------------------------------------------------------------
  auto PFC::load_trauma(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<::pfc::schema::ScenarioSchema>
  {
    auto traumas = scenario_schema->trauma_definitions().trauma();
    for (auto& trauma : traumas) {
      Trauma temp;
      temp.uuid = QString::fromStdString(trauma.id());
      temp.medical_name = QString::fromStdString(trauma.medical_name());
      temp.common_name = QString::fromStdString(*trauma.common_name()); // Why is this different from medical_name at all?
      temp.description = QString::fromStdString(trauma.description());
      temp.lower_bound = trauma.severity_range().numeric_range().get().lower_bound();
      temp.upper_bound = trauma.severity_range().numeric_range().get().upper_bound();

      Citation* citation;
      for (auto citation_uuid : trauma.citations().citation_ref()) {
        citation = new Citation(&temp);
        citation->uuid = citation_uuid.c_str();
        _db.select_citation(citation);
        temp.citations.push_back(citation);
      }

      if (!_db.update_trauma(&temp)) {
        wasSuccessful = false;
        return scenario_schema;
      }
    }
    wasSuccessful = true;
    return scenario_schema;
  }
  //-----------------------------------------------------------------------------
  auto PFC::load_trauma_profiles(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<::pfc::schema::ScenarioSchema>
  {
    auto trauma_sets = scenario_schema->trauma_profiles();
    auto trauma_profiles = trauma_sets.trauma_profile();
    for (auto trauma_profile : trauma_profiles) {
      TraumaProfile temp;
      temp.uuid = QString::fromStdString(trauma_profile.id());
      temp.name = QString::fromStdString(trauma_profile.name());
      TraumaOccurence* trauma;
      for (auto& current_trauma : trauma_profile.traumas().trauma()) {
        trauma = new TraumaOccurence(&temp);
        trauma->fk_trauma->uuid = current_trauma.trauma_ref().c_str();
        trauma->location = current_trauma.location().c_str();
        trauma->severity = current_trauma.severity().c_str();
        trauma->description = current_trauma.description().c_str();
        _db.select_trauma(trauma->fk_trauma);
        temp.traumas.push_back(trauma);
      }

      if (!_db.update_trauma_profile(&temp)) {
        wasSuccessful = false;
        return scenario_schema;
      }
    }
    wasSuccessful = true;
    return scenario_schema;
  }
  //-----------------------------------------------------------------------------
  auto PFC::load_locations(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<::pfc::schema::ScenarioSchema>
  {
    auto locations = scenario_schema->medical_scenario().locations();
    for (auto location : locations.location()) {
      Location temp;
      temp.uuid = location.id().c_str();
      temp.name = location.name().c_str();
      temp.environment = location.environment().c_str();
      temp.description = location.description().c_str();
      if (!_db.update_location(&temp)) {
        wasSuccessful = false;
        return scenario_schema;
      }
    }
    wasSuccessful = true;
    return scenario_schema;
  }
  //-----------------------------------------------------------------------------
  auto PFC::load_objectives(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<::pfc::schema::ScenarioSchema>
  {
    auto objectives = scenario_schema->syllabus().learning_objectives().objective();
    for (auto objective : objectives) {
      Objective temp;
      temp.uuid = QString::fromStdString(objective.id());
      temp.name = QString::fromStdString(objective.name());
      temp.description = QString::fromStdString(objective.description());

      Citation* citation;
      for (auto citation_uuid : objective.references().citations().citation_ref()) {
        citation = new Citation(&temp);
        citation->uuid = citation_uuid.c_str();
        _db.select_citation(citation);
        temp._citations.push_back(citation);
      }

      if (!_db.update_objective(&temp)) {
        wasSuccessful = false;
        return scenario_schema;
      }
    }
    wasSuccessful = true;
    return scenario_schema;
  }
  //-----------------------------------------------------------------------------
  auto PFC::load_roles(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<::pfc::schema::ScenarioSchema>
  {
    auto roles = scenario_schema->medical_scenario().roles().role();
    for (auto role : roles) {
      Role temp;
      temp.uuid = QString::fromStdString(role.id());
      temp.name = QString::fromStdString(role.name());
      temp.description = QString::fromStdString(role.description());
      if (!_db.update_role(&temp)) {
        wasSuccessful = false;
        return scenario_schema;
      }
    }
    wasSuccessful = true;
    return scenario_schema;
  }
  //-----------------------------------------------------------------------------
  auto PFC::load_scenes(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<::pfc::schema::ScenarioSchema>
  {
    auto& scenes = scenario_schema->medical_scenario().training_script().scene();
    for (auto& scene : scenes) {
      Scene temp;
      temp.uuid = QString::fromStdString(scene.id());
      temp.name = QString::fromStdString(scene.name());
      temp.description = QString::fromStdString(scene.description());
      temp.time_of_day = scene.time_of_day();
      temp.time_in_simulation = scene.time_in_simulation();

      temp.weather = scene.weather().present() ? QString::fromStdString(scene.weather().get()) : "";

      if (!_db.update_scene(&temp, false)) {
        wasSuccessful = false;
        return scenario_schema;
      }

      Location temp_location;
      temp_location.id = -1;
      temp_location.uuid = scene.location_id().c_str();
      if (!_db.select_location(&temp_location)) {
        wasSuccessful = false;
        return scenario_schema;
      }

      LocationMap dbLocationMap;
      if (!_db.update_location_in_scene(&temp, &temp_location)) {
        wasSuccessful = false;
        return scenario_schema;
      }

      auto& events = scene.events().event();
      for (auto& event : events) {
        Event dbEvent;
        dbEvent.id = -1;
        dbEvent.uuid = event.id().c_str();
        if (_db.select_event(&dbEvent)) {
          EventMap map;
          map.fk_event->assign(dbEvent);
          map.fk_scene->assign(temp);
          _db.update_event_map(&map);
        }
      }

      auto items = scene.items().item();
      for (auto& item : items) {
        for (auto& equipment : scenario_schema->equipment().equipment()) {
          EquipmentMap eqMap;
          if (equipment.id() == item.equipment_id()) {
            Equipment eq;
            eq.name = equipment.name().c_str();
            if (_db.select_equipment(&eq)) {
              eqMap.scene->assign(temp);
              eqMap.equipment->assign(eq);
              eqMap.name = item.short_name().c_str();
              eqMap.notes = item.description().c_str();
              for (auto& property : item.properties().value()) {
                eqMap.property_values.append(property.value().c_str()).append(";");
              }

              if (!_db.update_equipment_map(&eqMap)) {
                qDebug() << "Error updating Equipment Map";
              }
            }
          }
        }
      }

      auto roles = scene.roles().role_ref();
      for (auto& parts : roles) {
        RoleMap rMap;
        for (auto& role : scenario_schema->medical_scenario().roles().role()) {
          if (role.id() == parts) {
            Role dbPart;
            dbPart.name = role.name().c_str();
            if (_db.select_role(&dbPart)) {
              rMap.fk_scene->assign(temp);
              rMap.fk_role->assign(dbPart);
              rMap.category = ( parts.category().present() )? parts.category().get().c_str() : "";
              if (!_db.update_role_map(&rMap)) {
                qDebug() << "Error updating Role Map";
              }
            }
          }
        }
      }
    }
    wasSuccessful = true;
    return scenario_schema;
  }
  //-----------------------------------------------------------------------------
  auto PFC::load_treatments(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<::pfc::schema::ScenarioSchema>
  {
    auto treatments = scenario_schema->treatment_plans().treatment_plan();
    for (auto treatment : treatments) {
      Treatment temp;
      temp.uuid = QString::fromStdString(treatment.id());
      temp.medical_name = QString::fromStdString(treatment.medical_name().get());
      temp.common_name = QString::fromStdString(*treatment.common_name()); // Why is this different from medical_name at all?
      temp.description = QString::fromStdString(treatment.description());

      Citation* citation;
      for (auto citation_uuid : treatment.references().citations().citation_ref()) {
        citation = new Citation(&temp);
        citation->uuid = citation_uuid.c_str();
        _db.select_citation(citation);
        temp.citations.push_back(citation);
      }

      Equipment* equipment;
      for (auto equipment_uuid : treatment.required_equipment().equipment_refs()) {
        equipment = new Equipment(&temp);
        equipment->uuid = equipment_uuid.c_str();
        _db.select_equipment(equipment);
        temp.equipment.push_back(equipment);
      }

      if (!_db.update_treatment(&temp)) {
        wasSuccessful = false;
        return scenario_schema;
      }
    }
    wasSuccessful = true;
    return scenario_schema;
  }
  //-----------------------------------------------------------------------------
} //namespace schema

} // namespace PFC