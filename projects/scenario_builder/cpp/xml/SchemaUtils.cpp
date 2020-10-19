#include "SchemaUtils.h"

#include "../sql/SQLTables.h"

#include <iostream>
#include <mutex>

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
    auto& injuries = scenario_schema->trauma_definitions().trauma();
    for (auto& ref : injuries) {
      if (ref.id() == UUID) { //ref.uuid() == UUID) {
        Injury injury;
        injury.medical_name = ref.medical_name().c_str();
        _db.select_injury(&injury);
        return std::to_string(injury.id);
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
  auto PFC::make_trauma(Injury const* const input, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::trauma>
  {

    auto num_range = pfc::schema::trauma_severity_range();
    num_range.numeric_range(std::make_unique<pfc::schema::numeric_range>(input->lower_bound, input->upper_bound));

    auto trauma = std::make_unique<pfc::schema::trauma>(
      std::make_unique<pfc::schema::trauma::id_type>("Injury_" + std::to_string(input->id)),
      std::make_unique<pfc::schema::trauma::medical_name_type>(input->medical_name.toStdString()),
      std::make_unique<pfc::schema::citation_ref_list>(),
      std::make_unique<pfc::schema::trauma::description_type>(input->description.toStdString()),
      std::make_unique<pfc::schema::trauma::severity_range_type>(num_range));
    trauma->common_name(input->common_name.toStdString());
    trauma->citations(make_citation_ref_list(input->citations, _db));

    return trauma;
  }
  //------------------------------------------------------------------------------
  auto PFC::make_citation_ref_list(QString ref_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::citation_ref_list>
  {
    auto citation_ref_list = std::make_unique<schema::citation_ref_list>();
    Citation currentCitation;
    for (auto& token : ref_list.split(';')) {
      currentCitation.id = token.toInt();
      if (_db->select_citation(&currentCitation)) {
        citation_ref_list->citation_ref().push_back(schema::make_string(currentCitation.uuid));
      }
    }
    return citation_ref_list;
  }
  //-------------------------------------------------------------------------------
  auto PFC::make_cpg_ref_list(QString ref_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::cpg_ref_list>
  {
    auto cpg_ref_list = std::make_unique<schema::cpg_ref_list>();
    Citation currentCPG;
    for (auto& token : ref_list.split(';')) {
      currentCPG.id = token.toInt();
      if (_db->select_citation(&currentCPG)) {
        cpg_ref_list->cpg_ref().push_back(schema::make_string(currentCPG.uuid));
      }
    }
    return cpg_ref_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_equipment_ref_list(QString ref_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::equipment_ref_list>
  {
    auto equipment_ref_list = std::make_unique<schema::equipment_ref_list>();
    Equipment currentItem;
    for (auto& token : ref_list.split(';')) {
      currentItem.id = token.toInt();
      if (_db->select_equipment(&currentItem)) {
        equipment_ref_list->equipment_refs().push_back(schema::make_string(currentItem.uuid));
      }
    }
    return equipment_ref_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_trauma_set_ref_list(QString ref_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::trauma_profile_ref_list>
  {
    auto trauma_profile_ref_list = std::make_unique<schema::trauma_profile_ref_list>();
    InjurySet currentInjurySet;
    for (auto& token : ref_list.split(';')) {
      currentInjurySet.id = token.toInt();
      if (_db->select_injury_set(&currentInjurySet)) {
        trauma_profile_ref_list->trauma_profile().push_back(schema::make_string(currentInjurySet.uuid));
      }
    }
    return trauma_profile_ref_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_treatment_plan_ref_list(QString ref_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::treatment_plan_ref_list>
  {
    auto treatment_plan_ref_list = std::make_unique<schema::treatment_plan_ref_list>();
    Treatment currentTreatment;
    for (auto& token : ref_list.split(';')) {
      currentTreatment.id = token.toInt();
      if (_db->select_treatment(&currentTreatment)) {
        treatment_plan_ref_list->treatment_plan().push_back(schema::make_string(currentTreatment.uuid));
      }
    }
    return treatment_plan_ref_list;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_trauma_occurance_list(QString ref_list, QString occurance_list, QString severity_list, QString description_list, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::trauma_occurence_list>
  {
    auto trauma_occurence_list = std::make_unique<schema::trauma_occurence_list>();
    auto refs = ref_list.split(';');
    auto occurences = occurance_list.split(';');
    auto severities = severity_list.split(';');
    auto descriptions = description_list.split(';');

    auto ref_count = refs.size();
    auto occurence_count = occurences.size();
    auto severity_count = severities.size();
    auto description_count = descriptions.size();
    //NOTE: THis is unsafe when the database is confused and the indexes do not add up, but what are you going to do
    Injury currentInjury;
    for (auto i = 0; i < refs.size(); ++i) {
      currentInjury.id = refs.at(i).toInt();
      if (_db->select_injury(&currentInjury)) {
        try {
          auto occurence = occurences.at(i);
          auto severity = severities.at(i);
          auto description = descriptions.at(i);
          trauma_occurence_list->trauma().push_back(std::make_unique<schema::trauma_occurence>(make_string(currentInjury.uuid),
                                                                                               make_string(occurence),
                                                                                               make_string(description),
                                                                                               make_string(severity)));
        } catch (...) {
          std::cout << "Error index mismatch between refs,occurances, and severities";
        }
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
  auto PFC::make_equipment_properties_list(QString properties_list) -> std::unique_ptr<schema::equipment_properties_list>
  {
    auto property_list = std::make_unique<schema::equipment_properties_list>();

    QString name = "";
    QString type = "";
    QString field = "";
    std::string field_name;
    std::string field_type;
    for (auto& property : properties_list.split(';')) {
      if (!property.isEmpty()) {
        auto tokens = property.split(':');
        if (tokens.size() >= 1) {
          name = tokens.at(0);
          type = tokens.at(1);

          property_list->property().push_back(std::make_unique<::pfc::schema::equipment_property>(
            make_string(name),
            std::string("UNKNOWN"),
            std::make_unique<::pfc::schema::property_field_list>()));

          if (0 == type.compare("integral", Qt::CaseInsensitive)) {
            property_list->property().back().type("INTEGRAL");
          } else if (0 == type.compare("scalar", Qt::CaseInsensitive)) {

            property_list->property().back().type("SCALAR");
            if (tokens.size() > 2) {
              field = tokens.at(2);
              auto field_values = field.split(",");
              if (field_values.size() == 2) {
                field_name = field_values.at(0).toStdString();
                field_type = field_values.at(1).toStdString();
                property_list->property().back().fields().field().push_back(std::make_unique<::pfc::schema::field_type>(field_name, field_type));
              }
            }
          } else if (0 == type.compare("boolean", Qt::CaseInsensitive) || type.compare("bool", Qt::CaseInsensitive)) {
            property_list->property().back().type("BOOLEAN");
          } else {
            property_list->property().pop_back();
          }
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
  auto PFC::make_property_value_list(QString name_list, QString value_list) -> std::unique_ptr<schema::property_value_list>
  {
    //TODO: Get the names of each property
    auto list = std::make_unique<schema::property_value_list>();
    auto names = name_list.split(';');
    auto values = value_list.split(';');

    auto size = std::min(names.size(), values.size());
    for (auto i = 0; i < size; ++i) {
      auto name = (names[i].isEmpty() ? "" : names[i].split(":").at(0));//TODO: Verify split doesn't need a guard
      auto value = (values[i].isEmpty() ? "" : values[i]);
      list->value().push_back(std::make_unique<schema::property_value>(name.toStdString(), value.toStdString()));
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

    treatment->common_name(input->common_name.toStdString());
    treatment->references().citations(make_citation_ref_list(input->citations, _db));
    treatment->references().cpgs(make_cpg_ref_list(input->cpgs, _db));
    treatment->medical_name(input->medical_name.toStdString());

    return treatment;
  }
  //------------------------------------------------------------------------------
  auto PFC::make_learning_objective(Objective const* const input, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::learning_objective>
  {
    auto objective = std::make_unique<pfc::schema::learning_objective>(schema::make_string(input->uuid),
                                                                       schema::make_string(input->name),
                                                                       schema::make_string(input->description),
                                                                       std::make_unique<pfc::schema::learning_objective::references_type>(make_citation_ref_list(input->citations, _db),
                                                                                                                                          make_cpg_ref_list(input->cpgs, _db)),
                                                                       std::make_unique<pfc::schema::learning_objective::relates_to_type>(make_treatment_plan_ref_list(input->treatment_plans, _db),
                                                                                                                                          make_trauma_set_ref_list(input->injury_profiles, _db)));
    return objective;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_citation(Citation const* const input) -> std::unique_ptr<schema::citation>
  {

    auto citation = std::make_unique<pfc::schema::citation>(make_string(input->uuid),
                                                            input->title.toStdString(),
                                                            input->year.toStdString());
    citation->authors(make_authors_list(input->authors));
    citation->key().set(input->key.toStdString());
    return citation;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_scene(Scene const* const input, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::scene>
  {

    /*
      <xs : element name = "id" type = "xs:string" />
      <xs : element name = "location-id" type = "xs:string" />
      <xs : element name = "name" type = "xs:string" />
      <xs : element name = "description" type = "xs:string" />
      <xs : element name = "details" type = "xs:string" minOccurs = "0" />
      <xs : element name = "weather" type = "xs:string" minOccurs = "0" />
      <xs : element name = "time-of-day" type = "xs:integer" />
      <xs : element name = "time-in-simulation" type = "xs:integer" />
      <xs:element name="events" type="event-list"/>
      <xs : element name = "items" type = "item-list" />
      <xs : element name = "roles" type = "role-ref-list" />
*/
    Location loc;
    loc.name = generate_location_name(input->name.toStdString());
    _db->select_location(&loc);
    auto scene = std::make_unique<pfc::schema::scene>(schema::make_string(input->uuid),
                                                      schema::make_string(loc.uuid),
                                                      schema::make_string(input->name),
                                                      schema::make_string(input->description),
                                                      input->time_of_day.toInt(),
                                                      input->time_in_simulation,
                                                      std::make_unique<pfc::schema::scene::events_type>(),
                                                      std::make_unique<pfc::schema::scene::items_type>(),
                                                      std::make_unique<pfc::schema::scene::roles_type>());
    scene->weather(schema::make_string(input->weather));
    scene->details(schema::make_string(input->details));
    return scene;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_equipment(Equipment const* const input, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::equipment>
  {
    auto equipment = std::make_unique<pfc::schema::equipment>(schema::make_string(input->uuid),
                                                              schema::make_string(input->name),
                                                              schema::make_string(input->description),
                                                              make_citation_ref_list(input->citations, _db),
                                                              make_equipment_properties_list(input->properties));
    equipment->type(input->type);
    equipment->image(input->image.toStdString());

    return equipment;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_trauma_profile(InjurySet const* const input, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::trauma_profile>
  {

    auto trauma = std::make_unique<schema::trauma_profile>(make_string(input->uuid),
                                                           make_string(input->name),
                                                           make_trauma_occurance_list(input->injuries, input->locations, input->severities, input->description, _db),
                                                           make_treatment_plan_ref_list(input->treatments, _db));
    trauma->physiology_state(input->physiology_file.toStdString());
    return trauma;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_assessment(Assessment const* const input, pfc::SQLite3Driver* _db) -> std::unique_ptr<schema::assessment>
  {

    Objective objRef;
    objRef.id = input->objective.toInt();
    _db->select_objective(&objRef);
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
                                               make_string(input->short_name),
                                               make_string(input->description));
    role->trauma_profile_ref(make_string(input->trauma_profile));
    return role;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_event(Event const* const input) -> std::unique_ptr<schema::event>
  {
    auto event = std::make_unique<schema::event>(make_string(input->uuid),
                                                 make_string(input->name),
                                                 make_event_category(input->category),
                                                 make_event_fidelity(input->fidelity),
                                                 make_string(input->fk_actor_1),
                                                 make_string(input->fk_actor_2),
                                                 make_string(input->equipment),
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
                                               make_property_value_list(input->equipment->properties, input->property_values));

    return item;
  }
  //-----------------------------------------------------------------------------
  auto PFC::make_role_ref(Role const* const input) -> std::unique_ptr<::xml_schema::string>
  {
    return std::make_unique<::xml_schema::string>(schema::make_string(input->uuid));
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
      temp.description = QString::fromStdString(equipment.description());
      temp.type = equipment.type().get();
      temp.image = QString::fromStdString(*equipment.image());

      std::string citations;
      for (auto& citation : equipment.citations().citation_ref()) {
        std::string citation_id = find_citation(citation, scenario_schema, _db);
        citations += citation_id + ";";
      }
      if (!citations.empty()) {
        citations.pop_back();
      }
      temp.citations = QString::fromStdString(citations);

      std::string properties;
      for (auto& property : equipment.properties().property()) {
        properties += property.name() + ":" + property.type();
        for (auto field : property.fields().field()) {
          properties += ":" + field.name() + "," + field.type();
        }
        properties += ";";
      }
      if (!properties.empty()) {
        properties.pop_back();
      }
      temp.properties = QString::fromStdString(properties);

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
        temp.description = QString::fromStdString(event.description());
        temp.category = QString::fromStdString(event.category());
        temp.fidelity = QString::fromStdString(event.fidelity());

        std::string actor1_id = find_actor(event.actor_1(), scenario_schema, _db);
        temp.fk_actor_1 = QString::fromStdString(actor1_id);
        std::string actor2_id = find_actor(event.actor_2(), scenario_schema, _db);
        temp.fk_actor_2 = QString::fromStdString(actor2_id);

        temp.equipment = QString::fromStdString(event.equipment());
        temp.details = QString::fromStdString(event.details());

        //TODO: Convert UUID to FK
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
  auto PFC::load_injuries(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<::pfc::schema::ScenarioSchema>
  {
    auto injuries = scenario_schema->trauma_definitions().trauma();
    for (auto& injury : injuries) {
      Injury temp;
      temp.uuid = QString::fromStdString(injury.id());
      temp.medical_name = QString::fromStdString(injury.medical_name());
      temp.common_name = QString::fromStdString(*injury.common_name()); // Why is this different from medical_name at all?
      temp.description = QString::fromStdString(injury.description());
      temp.lower_bound = injury.severity_range().numeric_range().get().lower_bound();
      temp.upper_bound = injury.severity_range().numeric_range().get().upper_bound();
      std::string citations;
      for (auto citation : injury.citations().citation_ref()) {
        std::string citation_id = find_citation(citation, scenario_schema, _db);
        citations += citation_id + ";";
      }
      if (!citations.empty()) {
        citations.pop_back();
      }
      temp.citations = QString::fromStdString(citations);
      if (!_db.update_injury(&temp)) {
        wasSuccessful = false;
        return scenario_schema;
      }
    }
    wasSuccessful = true;
    return scenario_schema;
  }
  //-----------------------------------------------------------------------------
  auto PFC::load_injury_sets(std::unique_ptr<::pfc::schema::ScenarioSchema> scenario_schema, pfc::SQLite3Driver& _db, bool& wasSuccessful) -> std::unique_ptr<::pfc::schema::ScenarioSchema>
  {
    auto trauma_sets = scenario_schema->trauma_sets();
    auto trauma_profiles = trauma_sets.trauma_profile();
    for (auto trauma_profile : trauma_profiles) {
      InjurySet temp;
      temp.uuid = QString::fromStdString(trauma_profile.id());
      temp.name = QString::fromStdString(trauma_profile.name());
      std::string injuries;
      std::string locations;
      std::string severities;
      std::string descriptions;
      for (auto& injury : trauma_profile.injuries().trauma()) {
        std::string injury_id = find_injury(injury.id(), scenario_schema, _db);
        injuries += (injury_id + ";");
        std::string location = std::string(injury.location());
        locations += (location + ";");
        std::string severity = std::string(injury.severity());
        severities += (severity + ";");
        std::string description = std::string(injury.description());
        descriptions += (description + ";");
      }
      if (!injuries.empty()) { //Have to remove last ';' but pop_back() is undefined behavior on empty string
        injuries.pop_back();
      }
      if (!locations.empty()) {
        locations.pop_back();
      }
      if (!severities.empty()) {
        severities.pop_back();
      }
      if (!descriptions.empty()) {
        descriptions.pop_back();
      }
      temp.injuries = QString::fromStdString(injuries);
      temp.locations = QString::fromStdString(locations);
      temp.severities = QString::fromStdString(injuries);
      temp.description = QString::fromStdString(descriptions);
      if (!_db.update_injury_set(&temp)) {
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
    //TODO: Moved location to scenes due to current hiarchy in schema
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
      std::string citations;
      for (auto citation : objective.references().citations().citation_ref()) {
        std::string citation_id = find_citation(citation, scenario_schema, _db);
        citations += citation_id + ";";
      }
      if (!citations.empty()) {
        citations.pop_back();
      }
      temp.citations = QString::fromStdString(citations);
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
      temp.time_of_day = QString("%1").arg(scene.time_of_day());
      temp.time_in_simulation = scene.time_in_simulation();

      temp.details = scene.details().present() ? QString::fromStdString(scene.details().get()) : "";
      temp.weather = scene.weather().present() ? QString::fromStdString(scene.weather().get()) : "";

      if (!_db.update_scene(&temp)) {
        wasSuccessful = false;
        return scenario_schema;
      }

      Location temp_location;
      temp_location.id = -1;
      temp_location.name = generate_location_name(scene.name());

      if (!_db.select_location(&temp_location)) {
        wasSuccessful = false;
        return scenario_schema;
        return scenario_schema;
      }

      LocationMap dbLocationMap;
      dbLocationMap.id = -1;
      dbLocationMap.fk_scene = temp.id;
      dbLocationMap.fk_location = temp_location.id;
      if (!_db.select_location_map(&dbLocationMap)) {
        wasSuccessful = false;
        return scenario_schema;
      }

      auto& events = scene.events().event();
      for (auto& event : events) {
        Event dbEvent;
        dbEvent.id = -1;
        dbEvent.name = event.name().c_str();
        if (_db.select_event(&dbEvent)) {
          EventMap map;
          map.fk_event = dbEvent.id;
          map.fk_scene = temp.id;
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
              eqMap.scene->id = temp.id;
              eqMap.equipment->id = eq.id;
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
              rMap.fk_scene = temp.id;
              rMap.fk_role = dbPart.id;
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
      std::string citations;
      for (auto citation : treatment.references().citations().citation_ref()) {
        std::string citation_id = find_citation(citation, scenario_schema, _db);
        citations += citation_id + ";";
      }
      if (!citations.empty()) {
        citations.pop_back();
      }
      std::string equipments;
      for (auto equipment : treatment.required_equipment().equipment_refs()) {
        std::string equipment_id = find_equipment(equipment, scenario_schema, _db);
        equipments += equipment_id + ";";
      }
      if (!equipments.empty()) {
        equipments.pop_back();
      }
      temp.citations = QString::fromStdString(citations);
      temp.equipment = QString::fromStdString(equipments);
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