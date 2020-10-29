/**************************************************************************************
/**************************************************************************************
Copyright 2019 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/
#include "SqlLite3Driver.h"

#include <memory>

#include <QDebug>
#include <QMessageLogger>
#include <QRegularExpression>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QUuid>
#include <QtGlobal>

#include <fstream>
#include <iostream>

#include "SqlLite3_Statments.h"
#include "sqlite3ext.h"
#include <regex>

namespace pfc {
void assign_equipment_map(const QSqlRecord& record, EquipmentMap& map, Scene const* const scene = nullptr, Equipment const* const equipment = nullptr);
void assign_location_map(const QSqlRecord& record, LocationMap& map);
void assign_role_map(const QSqlRecord& record, RoleMap& map);

//------------------------------------------------------------------------------
//                      SQLiteDriver Functions
//------------------------------------------------------------------------------
SQLite3Driver::SQLite3Driver(QObject* parent)
  : QObject(parent)
{
  establish_settings();
}
//------------------------------------------------------------------------------
SQLite3Driver::SQLite3Driver(const std::string& dbName, const std::string& path, QObject* parent)
  : QObject(parent)
  , _db_name(dbName.c_str())
  , _db_path(path.c_str())
{
  if (!open()) {
    throw std::runtime_error("Database Creation Failure");
  }
  //TODO: Add Logic for detecting if this is required
  if (!initialize_db()) {
    QSqlError("Unable to properly initialize db!");
  }
}
//------------------------------------------------------------------------------
SQLite3Driver::~SQLite3Driver()
{
  close();
}
//------------------------------------------------------------------------------
void SQLite3Driver::establish_settings()
{
  QSettings settings;
  recent_files.push_back(settings.value("firstFile").toString().toStdString());
  recent_files.push_back(settings.value("secondFile").toString().toStdString());
  recent_files.push_back(settings.value("thirdFile").toString().toStdString());
  recent_files.push_back(settings.value("fourthFile").toString().toStdString());
  recent_files.push_back(settings.value("fifthFile").toString().toStdString());
  recent_files.push_back(settings.value("sixthFile").toString().toStdString());
  recent_files.push_back(settings.value("seventhFile").toString().toStdString());
  recent_files.push_back(settings.value("eighthFile").toString().toStdString());
  recent_files.push_back(settings.value("ninthFile").toString().toStdString());
  recent_files.push_back(settings.value("tenthFile").toString().toStdString());
}
//------------------------------------------------------------------------------
void SQLite3Driver::log_scenario_file(QString file)
{
  log_scenario_file(file.toStdString());
}
//------------------------------------------------------------------------------
void SQLite3Driver::log_scenario_file(std::string file)
{
  QSettings settings;
  int i = 0;
  for (; i < 9; i++) {
    if (recent_files[i] == file) {
      break;
    }
  }
  for (; i > 0; i--) {
    recent_files[i] = recent_files[i - 1];
  }
  recent_files[0] = file;
  settings.setValue("firstFile", QString::fromStdString(recent_files[0]));
  settings.setValue("secondFile", QString::fromStdString(recent_files[1]));
  settings.setValue("thirdFile", QString::fromStdString(recent_files[2]));
  settings.setValue("fourthFile", QString::fromStdString(recent_files[3]));
  settings.setValue("fifthFile", QString::fromStdString(recent_files[4]));
  settings.setValue("sixthFile", QString::fromStdString(recent_files[5]));
  settings.setValue("seventhFile", QString::fromStdString(recent_files[6]));
  settings.setValue("eighthFile", QString::fromStdString(recent_files[7]));
  settings.setValue("ninthFile", QString::fromStdString(recent_files[8]));
  settings.setValue("tenthFile", QString::fromStdString(recent_files[9]));
}
//------------------------------------------------------------------------------
QList<QString> SQLite3Driver::get_recent_scenario_files()
{
  QList<QString> q_recent_files;
  QSettings settings;
  q_recent_files.push_back(settings.value("firstFile").toString());
  q_recent_files.push_back(settings.value("secondFile").toString());
  q_recent_files.push_back(settings.value("thirdFile").toString());
  q_recent_files.push_back(settings.value("fourthFile").toString());
  q_recent_files.push_back(settings.value("fifthFile").toString());
  q_recent_files.push_back(settings.value("sixthFile").toString());
  q_recent_files.push_back(settings.value("seventhFile").toString());
  q_recent_files.push_back(settings.value("eighthFile").toString());
  q_recent_files.push_back(settings.value("ninthFile").toString());
  q_recent_files.push_back(settings.value("tenthFile").toString());
  return q_recent_files;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::ready()
{
  return QSqlDatabase::database(_db_name).isOpen();
}
//------------------------------------------------------------------------------
bool SQLite3Driver::open(QString name)
{
  QSqlDatabase::database(_db_name).close();
  _db_name = name;
  return open();
}
//------------------------------------------------------------------------------
bool SQLite3Driver::open()
{
  if (!QSqlDatabase::database(_db_name).isValid()) {
    QSqlDatabase::addDatabase("QSQLITE", _db_name);
  }
  QSqlDatabase::database(_db_name).setDatabaseName(_db_path + "/" + _db_name);

  if (!QSqlDatabase::database(_db_name).open()) {
    qDebug() << QSqlDatabase::database(_db_name).lastError();
    return false;
  }
  return true;
}
//------------------------------------------------------------------------------
void SQLite3Driver::close()
{
  QSqlDatabase::database(_db_name).commit();
  QSqlDatabase::database(_db_name).close();
  QSqlDatabase::removeDatabase(_db_name);
}
//------------------------------------------------------------------------------
bool SQLite3Driver::initialize_db()
{
  bool creation_failure = false;
  std::map<char const*, char const*> create_statments = {
    { tables[AUTHORS], sqlite3::create_authors_table },
    { tables[ASSESSMENTS], sqlite3::create_assessments_table },
    { tables[CITATIONS], sqlite3::create_citations_table },
    { tables[CITATION_MAPS], sqlite3::create_citation_maps_table },
    { tables[EVENTS], sqlite3::create_events_table },
    { tables[EVENT_MAPS], sqlite3::create_event_maps_table },
    { tables[EQUIPMENTS], sqlite3::create_equipment_table },
    { tables[EQUIPMENT_MAPS], sqlite3::create_equipment_map_table },
    { tables[INJURIES], sqlite3::create_injuries_table },
    { tables[INJURY_SETS], sqlite3::create_injury_sets_table },
    { tables[LOCATIONS], sqlite3::create_locations_table },
    { tables[LOCATION_MAPS], sqlite3::create_location_maps_table },
    { tables[ROLE_MAPS], sqlite3::create_role_maps_table },
    { tables[OBJECTIVES], sqlite3::create_objectives_table },
    { tables[PROPERTIES], sqlite3::create_properties_table },
    { tables[ROLES], sqlite3::create_roles_table },
    { tables[SCENES], sqlite3::create_scenes_table },
    { tables[TREATMENTS], sqlite3::create_treatments_table }
  };

  if (open()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    for (auto pair : create_statments) {
      query.prepare(pair.second);
      if (!query.exec()) {
        qCritical() << QString("Could not create table %1 \n\t --").arg(pair.first) << query.lastError();
        creation_failure = true;
        continue;
      }
    }
    return !creation_failure;
  }
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::populate_db()
{
  Property default_property;
  Author default_author;
  Assessment default_assessment;
  Citation default_citation;
  Event default_event;
  Equipment default_equipment;
  Injury default_injury;
  InjurySet default_injury_set;
  Location default_location;
  Objective default_objective;
  Role default_role;
  Scene default_scene;
  Treatment default_treatment;
  //---Author---

  std::map<char const*, char const*> default_prop_values = {
    { "scenario_title", "PFC Scenario" },
    { "scenario_version", "0.1" },
    { "scenario_security", "UnClassified" },
    { "scenario_description", "" },
    { "scenario_keywords", "" },
    { "scenario_domain", "Medical Prolonged Field Care" },
    { "scenario_limitations", "" }
  };
  for (auto& prop : default_prop_values) {
    default_property.id = -1;
    default_property.name = prop.first;
    if (!select_property(&default_property)) {
      default_property.value = prop.second;
      update_property(&default_property);
    }
  }
  if (author_count() == 0) {
    default_author.first = "First_Name";
    default_author.last = "Last_Name";
    default_author.email = "example@email.com";
    default_author.zip = "00000";
    default_author.state = "VA";
    default_author.country = "United States";
    default_author.phone = "(000)000-0000";
    default_author.organization = "Organization";
    if (!update_first_author(&default_author)) {
      return false;
    }
  }
  //---Assessment---
  if (assessment_count() == 0) {
    default_assessment.name = "Assessment_1";
    default_assessment.description = "Description of Assessment_1";
    default_assessment.type = "binary";
    default_assessment.available_points = 1;
    default_assessment.objective = ""; // This isn't actually a db field
    default_assessment.criteria = "Criteria for Assessment_1";
    if (!update_assessment(&default_assessment)) {
      return false;
    }
  }
  //---Citation---
  //if (citation_count() == 0) {
  //  default_citation.title = "Reference 1";
  //  default_citation.key = "AuthorYear_1";
  //  default_citation.authors = "Reference Authors";
  //  default_citation.year = "Reference Year";
  //  default_citation.publisher = "";
  if (!populate_citations()) {
    return false;
  }
  //}
  //---Event---
  if (event_count() == 0) {
    default_event.name = "New Event 1";
    default_event.description = "Description of Event 1";
    default_event.category = "ACTION";
    default_event.fidelity = "LOW";
    default_event.fk_actor_1 = "";
    default_event.fk_actor_2 = "";
    default_event.equipment = "";
    default_event.details = "";
    if (!update_event(&default_event)) {
      return false;
    }
  }
  //---Equipment---
  if (!populate_equipment()) {
    return false;
  }
  //---Injury---
  if (!populate_injuries()) {
    return false;
  }
  //---InjurySet---
  if (injury_set_count() == 0) {
    default_injury_set.name = "Trauma Set 1";
    default_injury_set.description = "";
    default_injury_set.injuries = "";
    default_injury_set.locations = "";
    default_injury_set.severities = "";
    default_injury_set.physiology_file = "";
    default_injury_set.treatments = "";
    if (!update_injury_set(&default_injury_set)) {
      return false;
    }
  }
  //---Objective---
  if (objective_count() == 0) {
    default_objective.name = "Objective_1";
    default_objective.description = "Description of Objective_1";
    default_objective.citations = "";
    default_objective.cpgs = "";
    default_objective.treatment_plans = "";
    default_objective.injury_profiles = "";
    if (!update_objective(&default_objective)) {
      return false;
    }
  }
  //---Role---
  if (role_count() == 0) {
    default_role.name = "Role_1";
    default_role.description = "Description of Role_1";
    if (!update_role(&default_role)) {
      return false;
    }
  }
  //---Scene---
  if (scene_count() == 0) {
    default_scene.name = "Scene_1";
    default_scene.description = "Description of Scene_1";
    default_scene.time_of_day = "00:00:00";
    default_scene.time_in_simulation = 0;
    default_scene.weather = "";
    default_scene.events = "";
    default_scene.items = "";
    default_scene.roles = "";
    default_scene.details = "";
    if (!update_scene(&default_scene)) {
      return false;
    }
    if (!update_event_in_scene(&default_scene, &default_event)) {
      return false;
    }
    if (!update_role_in_scene(&default_scene, &default_role)) {
      return false;
    }
  }
  //---Treatment---
  if (!populate_treatments()) {
    return false;
  }
  return true;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::populate_citations()
{
  struct sCitation {
    std::string key;
    std::string title;
    std::string authors;
    std::string year;
    std::string publisher;
  };

  //New additions must be added to the bottom in order to keep id numbering unchanged for existing citations
  std::vector<sCitation> default_citations = {
    { "Lawnick2013Combat", "Combat Injury Coding: A Review and Reconfiguration", "Mary M Lawnick, Howard R Champion, Thomas Gennarelli, Michael R Galarneau, Edwin D'Souza, Ross R Vickers, Vern Wing, Brian J Eastridge, Lee Ann Young, Judy Dye, and others", "2013", "Journal of Trauma and Acute Care" }, //hemorrhage paper
    { "Rhoades2003Medical", "Medical Physiology", "Rodney Rhoades", "2003", "Lippincott-Raven Publishers" }, // physiology textbook
    { "Legome2011Trauma", "Trauma: A Comprehensive Emergency Medicine Textbook", "Eric Legome", "2011", "Cambridge" }, //trauma textbook
    { "Roy2005Unusual", "Unusual Foreign Body Airway Obstruction After Laryngeal Mask Insertion", "Ravishankar M Roy", "2005", "Anesth Analg." }, //airway obstruction paper
    { "Army2017Official", "Official US Army Combat Medic Manual & Trainer's Guide", "US Army", "2017", "CreateSpace Independent Publishing Platform" }, //Combat medic handbook
    { "Guyton2006Textbook", "Textbook of Medical Physiology", "Guyton and Hall", "2006", "Elsevier" }, //BioGears medical textbook hard copy
    { "Boron2012Medical", "Medical Physiology", "Walter F Boron and Emile L Boulapep", "2012", "Elsevier" } // Secondary BioGears med textbook hard cover
  };

  Citation temp;
  for (auto& citationDef : default_citations) {
    temp.clear();
    temp.key = citationDef.key.c_str();
    temp.title = citationDef.title.c_str();
    temp.authors = citationDef.authors.c_str();
    temp.year = citationDef.year.c_str();
    temp.publisher = citationDef.publisher.c_str();
    if (!select_citation(&temp)) {
      update_citation(&temp);
    }
  }
  return true;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::populate_equipment()
{

  struct sEquipment {
    std::string name;
    int type;
      // Type Key
      // 1 = Equipment
      // 2 = Attachment
      // 3 = Substance
      // 4 = Consumable Item
    std::string summary;
    std::string description;
    std::string citations;
    std::string properties;
  };

  std::vector<sEquipment> default_equipment = {
    { "Tempus Pro with Peripherals", 2, "A small, lightweight vital signs monitor.", "Light enough to carry and small enough to hold in one hand, it can easily be deployed in numerous clinical scenarios. While it does support additional attachments to allow for a wide range of measurements, some important included parameters the tempus pro measures includes pulse rate, impedance respiration, contact temperature, pulse oximetry, and noninvasive blood pressure.", "2;5;6;7", "Available,BOOLEAN" },
    { "Tourniquet", 2, "A device which applies pressure to a limb or extremity in order to control the flow of blood.", "A tourniquet uses compression, typically on a limb, to constrict a vein or artery and limit blood flow as much as possible. This is typically done during hemorrhaging when limiting blood flow to the extremity also limits the blood lost. In a pinch, a cord or tie can be used.", "2;5;6;7", "Available,BOOLEAN" },
    { "Nasal Cannula", 2, "A device used to deliver increased airflow to a patient in need of respiratory help.", "Tubing with two prongs that get inserted into a patient's nose, this equipment is attached to an oxygen source and mainly used to control oxygen flow to the patient. The advantages to this delivery method is that is less invasive than other oxygen masks and allows a patient to eat and speak normally.", "2;5;6;7", "Available,BOOLEAN" },
    { "Blood Collection Bag", 4, "Clear plastic bag used to collect and store blood.", "Bags are usually 500 mL in volume (referred to as one unit). Blood type is dependent on antigen presence (A/B) or absence (O) in addition to the presence or absence of Rh factor (+/-).", "2;5;6;7", "Available,BOOLEAN; Count, INTEGER; Kind, ENUM{APos, ANeg, BPos, BNeg, ABPos, ABNeg, OPos, ONeg}" },
    { "Blood Transfusion Kit", 1, "Kit used for in field blood transfusion.", "Designed for field application, a transfusion kit contains supplies necessary to collect and transfuse fresh whole blood (FWB). The kit typically contains everything from safety supplies and equipment (gloves, blood type test kits, and swabsticks) to collection materials (tubing, needles, collection bag, etc.). Additionally, two sets of most safety materials are supplied to prevent multiple uses on both donor and recipient (i.e. two needles).", "2;5;6;7", "Available,BOOLEAN" },
    { "Blanket", 2, "Low-weight, low-bulk blanket made of heat-reflective thin plastic sheeting.", "Also referred to as mylar, these are commonly used to prevent heat loss from a human body.", "2;5;6;7", "Available, BOOLEAN" },
    { "Syringe", 4, "Simple reciprocating pump using plunger or piston to expel liquid or gas through a hypodermic needle.", "In medicine, these can be used to withdraw liquids, such as when cleaning wounds or body cavities, or to inject fluids, as is the case with some drug injections.", "2;5;6;7", "Available,BOOLEAN; Count, INTEGER" },
    { "Antibiotics", 3, "Antimicrobial substance active against bacteria, used in the treatment and prevention of bacterial infections.", "This medication usually comes in the form of a pill. There are different types of antibiotics for different applications.", "2;5;6;7", "Available, BOOLEAN; Volume, Integer" },
    { "Epinephrine", 3, "A chemical that narrows blood vessels and opens airways in the lungs.", "Also referred to as adrenaline, this drug is used to treat life-threatening allergic reactions. Epinephrine acts quickly and works to improve breathing, stimulate the heart, raise a dropping blood pressure, reverse hives, and reduce swelling.", "2;5;6;7", "Available, BOOLEAN; Volume, Integer" },
    { "Fentanyl", 3, "A synthetic opioid pain reliever, approved for treating severe pain.", "Fentanyl is a prescription drug that is also made and used illegally. Like morphine, it is a medicine that is typically used to treat patients with severe pain, especially after surgery. It is also sometimes used to treat patients with chronic pain who are physically tolerant to other opioids.", "2;5;6;7", "Available, BOOLEAN; Volume, Integer" },
    { "Ketamine", 3, "A dissociative anesthetic used pain relief.", "Ketamine can provide pain relief and short-term memory loss (for example, amnesia of a medical procedure). In surgery, it is used an induction and maintenance agent for sedation and to provide general anesthesia. It has also been used for pain control in burn therapy, battlefield injuries, and in children who cannot use other anesthetics due to side effects or allergies. At normal doses, it is often preferred as an anesthetic in patients at risk of bronchospasm and respiratory depression.", "2;5;6;7", "Available, BOOLEAN; Volume, Integer" },
    { "Midazolam", 3, "A short action sedative used for anesthesia, procedural sedation, trouble sleeping, and severe agitation.", "Midazolam injection is used before medical procedures and surgery to cause drowsiness, relieve anxiety, and prevent any memory of the event. It is also sometimes given as part of the anesthesia during surgery to produce a loss of consciousness.", "2;5;6;7", "Available, BOOLEAN; Volume, Integer" },
    { "Morphine", 3, "A narcotic pain reliever used to treat moderate to severe pain.", "Morphine is an opioid medication used to treat severe chronic pain. It has a high rate of addiction, overdose, and death.", "2;5;6;7", "Available, BOOLEAN; Volume, Integer" },
    { "Narcan", 3, "Naloxone, a medication used to block the effects of opioids, commonly used for decreased breathing in opioid overdose.", "Naloxone is a medicine that rapidly reverses an opioid overdose. It attaches to opioid receptors and reverses and blocks the effects of other opioids. Naloxone is a safe medicine. It only reverses overdoses in people with opioids in their systems.", "2;5;6;7", "Available, BOOLEAN; Volume, Integer" },
    { "Urine Foley", 4, "Catheter used to drain urine.", "A soft plastic or rubber tube that is inserted through the urethra into the bladder. These are typically used when a patient has difficulties urinating on their own.", "2;5;6;7", "" },
    { "Urine Bottle", 4, "Bag used to catch and store drained urine.", "", "2;5;6;7", "" },
    { "Water", 4, "An inorganic, transparent, tasteless, odorless, and nearly colorless chemical substance.", "It is also essential to and the primary component of living organic beings.", "2;5;6;7", "Volume, INTEGER" },
    { "Energy Gel", 4, "Carbohydrate gel used to provide energy and promote recovery.", "", "2;5;6;7", "Count, INTEGER" },
    { "Splint", 2, "a rigid or flexible device that maintains in position a displaced or injured part.", "Mostly used for broken bones, a splint is fastened along a injured length of a person's body in order to limit movement and further injury.", "2;5;6;7", "Available, BOOLEAN" },
    { "Peripheral IV", 2, "A peripheral intravenous line is a small, short plastic catheter that is placed through the skin into a vein.", "A peripheral intravenous line is used to give fluids and/or medications directly into the blood stream.", "2;5;6;7", "Available, BOOLEAN" },
    { "Wound Pack", 2, "Non-adherent and absorbent material used to control bleeding of an open wound.", "Wound fillers, such as non-adherent gauze, pads, ointments, sponges, and other materials designed to manage exudate.", "2;5;6;7", "Available, BOOLEAN" },
    { "Wound Wrap", 2, "A strip of fabric used especially to cover, dress, and bind up wounds.", "It can also be used to provide pressure to the bleeding.", "2;5;6;7", "Available, BOOLEAN" },
    { "Oxygen Tank", 1, "A tank of oxygen.", "In medicine, an oxygen tank provide controlled and therapeutic oxygen delivery to a patient.", "2;5;6;7", "Available, BOOLEAN; Volume, INTEGER" },
    { "IV Pole", 1, "The pole use to hang IV bags on.", "It provides patient mobility to move around a facility while keeping IV lines in.", "2;5;6;7", "Available, BOOLEAN" },
    { "IV Bag", 4, "Bag used to store and deliver fluids to a patient.", "", "2;5;6;7", "Available, BOOLEAN; Count, INTEGER; Volume, INTEGER; Kind, ENUM{Saline, Lactated Ringers}" },
    { "Catheter Supplies", 4, "Catheters and the products that are used to insert or remove a catheter, and maintain catheter function.", "", "2;5;6;7", "" }
  };

  Equipment temp;
  for (auto& equipmentDef : default_equipment) {
    temp.clear();
    temp.name = equipmentDef.name.c_str();
    temp.type = equipmentDef.type;
    temp.summary = equipmentDef.summary.c_str();
    temp.description = equipmentDef.description.c_str();
    temp.citations = equipmentDef.citations.c_str();
    temp.properties = equipmentDef.properties.c_str();
    if (!select_equipment(&temp)) {
      update_equipment(&temp);
    }
  }
  return true;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::populate_injuries()
{
  struct sInjury {
    std::string medical_name;
    std::string common_name;
    std::string description;
    std::string citations;
    float lower_bound;
    float upper_bound;
  };

  std::vector<sInjury> default_injuries = {
    { "Contusion", "Bruising", "Bleeding under the skin due to trauma to the blood capillaries can cause injured tissues. It usually darkens the skin to a black/blue color.", "1;2;3", 0.0, 225 }, // linked to hemorrhage in mL/min
    { "Laceration", "External Bleeding", "External bleeding of the body produced by the tearing of soft body tissue.", "1;2;3", 0.0, 225 }, // linked to hemorrhage in mL/min
    { "Puncture", "Puncture Wound", "An injury to the body cause by a piercing or penetrating object. Depth and severity can vary.", "1;2;3", 0.0, 225 }, // linked to hemorrhage in mL/min
    { "Airway Trauma", "Trauma to airway", "Airway injuries are life threatening and often occur as a result of blunt and penetrating force to the neck and/or chest. Diagnosis can be challenging due to symptoms and signs that are nonspecific to this injury type.", "2;3;4", 0.0, 1.0 }, //airway obstruction in BG (severity, unitless)
    { "Infection", "Infection", "The invasion and growth of bacteria, viruses, yeast, or fungi in the body. Infections can begin at any point in the body and spread, causing fever and other health issues.", "2;3", 0.0, 0.0 }, //Infection (numerous inputs)
    { "First Degree Burn", "Superficial Burn", "A heat injury that affects the first layer of skin. Most commonly, it causes redness, pain, and swelling and will heal on its own, though larger burns can be uncomfortable and may require a doctor's opinion.", "2;3", 0.0, 0.33 }, //Burn Wound, first third (% surface area)
    { "Second Degree Burn", "Partial Thickness Burn", "A burn that affects the epidermis and dermis (lower layers of skin). In addition to the symptoms of a first degree, a second degree burn can also cause blistering.", "2;3", 0.34, 0.66 }, //Burn Wound, second third (% surface area)
    { "Third Degree Burn", "Full Thickness Burn", "A burn that goes through the dermis and affects deep skin tissue layers. The result is a white or blackened, charred skin that may lose some feeling.", "2;3", 0.67, 1.0 }, //Burn Wound, last third (% surface area)
    { "Hemorrhage", "Bleeding", "Hemorrhaging is the release of blood from any blood vessel, either inside or outside of the body. When discussing hemorrhage, it is important to identify both severity (typically as a measure of blood loss rate) and location.", "1;2;3", 0.0, 225 } //Hemorrhage in mL/min (150 considered massive blood loss, increase by 50% for extreme condition)
  };

  Injury temp;
  for (auto& injuryDef : default_injuries) {
    temp.clear();
    temp.medical_name = injuryDef.medical_name.c_str();
    temp.common_name = injuryDef.common_name.c_str();
    temp.description = injuryDef.description.c_str();
    temp.citations = injuryDef.citations.c_str();
    temp.lower_bound = (100 * injuryDef.lower_bound); //multiply by 100 so decimals do not get destroyed by int on qml side
    temp.upper_bound = (100 * injuryDef.upper_bound); //multiply by 100 so decimals do not get destroyed by int on qml side
    if (!select_injury(&temp)) {
      update_injury(&temp);
    }
  }
  return true;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::populate_treatments()
{

  struct sTreatment {
    std::string medical_name;
    std::string common_name;
    std::string description;
    std::string citations;
  };

  std::vector<sTreatment> default_treatments = {
    { "Chest Tube", "Chest Tube", "A chest tube is a hollow, flexible tube placed into the chest. It acts as a drain for blood, fluid, or air from around the heart and lungs. ", "2;5;6;7" },
    { "Needle Thoracostomy", "Needle Decompression", "A procedure performed to stabilize deteriorating patients in the life-threatening situation of a tension pneumothorax. The procedure involves inserting a large needle through the chest wall into the pleural cavity to allow air to escape. ", "2;5;6;7" },
    { "Burn Ointment", "Burn Ointment", "Used to help in the prevention and treatment of infections during burn healing, burn ointment kills infectious microorganisms and reduces inflammation.", "2;5;6;7" },
    { "Bandage", "Bandage/Wrap", "A strip of fabric used especially to cover, dress, and bind up wounds.", "2;5;6;7" },
    { "Suture", "Stitches", "A stitch or row of stitches holding together the edges of a wound or incision. They typically help hold tissue together during healing.", "2;5;6;7" },
    { "Antibiotics", "Antibiotics", "Antimicrobial substance active against bacteria, used in the treatment and prevention of bacterial infections. This medication usually comes in the form of a pill. There are different types of antibiotics for different applications.", "2;5;6;7" },
    { "Fentanyl", "Fentanyl", "Fentanyl is a prescription drug that is also made and used illegally. Like morphine, it is a medicine that is typically used to treat patients with severe pain, especially after surgery. It is also sometimes used to treat patients with chronic pain who are physically tolerant to other opioids.", "2;5;6;7" },
    { "Morphine", "Morphine", "Morphine is an opioid medication used to treat severe chronic pain. It has a high rate of addiction, overdose, and death.", "2;5;6;7" },
    { "Pack Wound", "Pack Wound", "When a wound is deep, or when it tunnels under the skin, packing the wound can help it heal. The packing material soaks up any drainage from the wound, which helps the tissues heal from the inside out.", "2;5;6;7" },
    { "Escharotomy", "Escharotomy", "During treatment of a hazardous burn, an escharotomy is the surgical incision through the eschar into the subcutaneous tissues to allow the extremity to continue to swell without compressing underlying blood vessels.", "2;5;6;7" }
  };
  Treatment temp;
  for (auto& treatmentDef : default_treatments) {
    temp.clear();
    temp.medical_name = treatmentDef.medical_name.c_str();
    temp.common_name = treatmentDef.common_name.c_str();
    temp.description = treatmentDef.description.c_str();
    temp.citations = treatmentDef.citations.c_str();
    if (!select_treatment(&temp)) {
      update_treatment(&temp);
    }
  }
  return true;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::clear_db()
{
  QSqlQuery query(QSqlDatabase::database(_db_name));
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS authors;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS assessments;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS citations;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS citation_maps;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS events;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS event_maps;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS equipments;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS equipment_map;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS injuries;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS injury_sets;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS locations;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS location_maps;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS objectives;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS properties;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS roles;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS role_maps;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS restrictions;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS restriction_maps;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS scenes;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS treatments;");

    return true;
  }
  return false;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::clear_table(enum SQLite3Driver::Sqlite3Table t)
{

  QSqlQuery query{ QSqlDatabase::database(_db_name) };
  query.prepare(sqlite3::drop_table);
  if (QSqlDatabase::database(_db_name).isOpen()) {
    switch (t) {
    case ASSESSMENTS:
      query.bindValue(":table", tables[ASSESSMENTS]);
      break;
    case AUTHORS:
      query.bindValue(":table", tables[AUTHORS]);
      break;
    case CITATIONS:
      query.bindValue(":table", tables[CITATIONS]);
      break;
    case EQUIPMENTS:
      query.bindValue(":table", tables[EQUIPMENTS]);
      break;
    case EVENTS:
      query.bindValue(":table", tables[EVENTS]);
      break;
    case INJURIES:
      query.bindValue(":table", tables[INJURIES]);
      break;
    case LOCATIONS:
      query.bindValue(":table", tables[LOCATIONS]);
      break;
    case OBJECTIVES:
      query.bindValue(":table", tables[OBJECTIVES]);
      break;
    case PROPERTIES:
      query.bindValue(":table", tables[PROPERTIES]);
      break;
    case RESTRICTIONS:
      query.bindValue(":table", tables[RESTRICTIONS]);
      break;
    case ROLES:
      query.bindValue(":table", tables[ROLES]);
      break;
    case TREATMENTS:
      query.bindValue(":table", tables[TREATMENTS]);
      break;
    default:
      return false;
    }
    return query.exec();
  }
  return false;
}
//-----------------------------------------------------------------------------
int SQLite3Driver::nextID(Sqlite3Table table) const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    QString stmt = "SELECT MAX(%1_id) FROM %1s";
    switch (table) {
    case AUTHORS:
      stmt = stmt.arg("author");
      query.prepare(stmt);
      break;
    case ASSESSMENTS:
      stmt = stmt.arg("assessment");
      query.prepare(stmt);
      break;
    case EVENTS:
      query.prepare(stmt.arg("event"));
      break;
    case EQUIPMENTS:
      query.prepare(stmt.arg("equipment"));
      break;
    case INJURIES:
      query.prepare("SELECT MAX(injury_id) FROM injuries");
      break;
    case INJURY_SETS:
      query.prepare(stmt.arg("injury_set"));
      break;
    case LOCATIONS:
      query.prepare(stmt.arg("location"));
      break;
    case OBJECTIVES:
      query.prepare(stmt.arg("objective"));
      break;
    case PROPERTIES:
      query.prepare("SELECT MAX(property_id) FROM properties");
      break;
    case CITATIONS:
      query.prepare(stmt.arg("citation"));
      break;
    case RESTRICTIONS:
      query.prepare(stmt.arg("restriction"));
      break;
    case ROLES:
      query.prepare(stmt.arg("role"));
      break;
    case TREATMENTS:
      query.prepare(stmt.arg("treatment"));
      break;
    default:
      return -1;
    }
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt() + 1;
    }
  }
  return -1;
}
//----------------------------ASSESSMENT-----------------------------------------
inline void assign_assessment(const QSqlRecord& record, Assessment& assessment)
{
  assessment.id = record.value(ASSESSMENT_ID).toInt();
  assessment.uuid = record.value(ASSESSMENT_UUID).toString();
  assessment.name = record.value(ASSESSMENT_NAME).toString();
  assessment.description = record.value(ASSESSMENT_DESCRIPTION).toString();
  assessment.type = record.value(ASSESSMENT_TYPE).toString();
  assessment.available_points = record.value(ASSESSMENT_AVAILABLE_POINTS).toInt();
  assessment.criteria = record.value(ASSESSMENT_CRITERIA).toString();
}
int SQLite3Driver::assessment_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_assessments);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::assessments()
{
  qDeleteAll(_assessments);
  _assessments.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_assessments);
    query.exec();
    while (query.next()) {
      auto assessment = std::make_unique<Assessment>();
      auto record = query.record();

      assign_assessment(record, *assessment);
      _assessments.push_back(assessment.release());
    }
    _current_assessment = _assessments.begin();
    emit assessmentsChanged();
  }
}
bool SQLite3Driver::next_assessment(Assessment* assessment)
{
  if (_current_assessment == _assessments.end() || _assessments.empty()) {
    return false;
  }
  assessment->assign(*(*_current_assessment));
  ++_current_assessment;

  return true;
}
bool SQLite3Driver::select_assessment(Assessment* assessment) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (assessment->id != -1) {
      query.prepare(sqlite3::select_assessment_by_id);
      query.bindValue(":id", assessment->id);
    } else if (!assessment->name.isEmpty()) {
      query.prepare(sqlite3::select_assessment_by_name);
      query.bindValue(":name", assessment->name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_assessment(record, *assessment);
        return true;
      }
    } else {
      qWarning() << "select_assessment" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_assessment(Assessment* assessment)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (-1 != assessment->id) {
      query.prepare(sqlite3::update_assessment_by_id);
      query.bindValue(":id", assessment->id);
    } else if (!assessment->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_assessments);
    }
    if (assessment->uuid == "") {
      assessment->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
    query.bindValue(":uuid", assessment->uuid);
    query.bindValue(":name", assessment->name);
    query.bindValue(":description", assessment->description);
    query.bindValue(":type", assessment->type);
    query.bindValue(":available_points", assessment->available_points);
    query.bindValue(":criteria", assessment->criteria);

    if (!query.exec()) {
      qWarning() << "updaate_assessment" << query.lastError();
      return false;
    }
    if (-1 == assessment->id) {
      const auto r = select_assessment(assessment);
      assessmentUpdated(assessment->id);
      return r;
    }
    assessmentUpdated(assessment->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_assessment(Assessment* assessment)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_assessment(assessment)) {
      query.prepare(sqlite3::delete_assessment_by_id);
      query.bindValue(":id", assessment->id);
      if (!query.exec()) {
        qWarning() << "remove_assessment" << query.lastError();
        return false;
      }
      assessmentRemoved(assessment->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//----------------------------AUTHOR---------------------------------------------
inline void assign_author(const QSqlRecord& record, Author& author)
{
  author.id = record.value(AUTHOR_ID).toInt();
  author.uuid = record.value(AUTHOR_UUID).toString();
  author.first = record.value(AUTHOR_FIRST_NAME).toString();
  author.last = record.value(AUTHOR_LAST_NAME).toString();
  author.email = record.value(AUTHOR_EMAIL).toString();

  auto zip_qstr = record.value(AUTHOR_ZIPCODE).toString();

  QRegularExpression zip(R"((\d{5}))");
  QRegularExpression zip_plus4(R"((\d{5})-(\d{4}))");
  QRegularExpressionMatch match = zip_plus4.match(zip_qstr);

  if (match.hasMatch()) {
    author.zip = match.captured(1);
    author.plus_4 = match.captured(2);
  } else {
    match = zip.match(zip_qstr);
    if (match.hasMatch()) {
      author.zip = match.captured(1);
    }
  }
  author.state = record.value(AUTHOR_STATE).toString();
  author.country = record.value(AUTHOR_COUNTRY).toString();
  author.phone = record.value(AUTHOR_PHONE).toString();
  author.organization = record.value(AUTHOR_ORGANIZATION).toString();
}
int SQLite3Driver::author_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_authors);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::authors()
{
  qDeleteAll(_authors);
  _authors.clear();
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_authors);
    query.exec();
    while (query.next()) {
      auto author = std::make_unique<Author>();
      auto record = query.record();

      assign_author(record, *author);
      _authors.push_back(author.release());
    }
    _current_author = _authors.begin();
    emit authorsChanged();
  }
}
bool SQLite3Driver::next_author(Author* author)
{
  if (_current_author == _authors.end() || _authors.empty()) {
    return false;
  }
  author->assign(*(*_current_author));
  ++_current_author;
  return true;
}
bool SQLite3Driver::select_author(Author* author) const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (author->id != -1) {
      query.prepare(sqlite3::select_author_by_id);
      query.bindValue(":id", author->id);
    } else if (!author->email.isEmpty()) {
      query.prepare(sqlite3::select_author_by_email);
      query.bindValue(":email", author->email);
    } else {
      qWarning() << "Provided Author has no id or email one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_author(record, *author);
        return true;
      }
    } else {
      qWarning() << "select_author" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_author(Author* author)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    if (!author->email.isEmpty()) {
      QSqlQuery query{ QSqlDatabase::database(_db_name) };

      query.prepare(sqlite3::insert_or_update_authors);
      if (author->uuid == "") {
        author->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
      }
      query.bindValue(":uuid", author->uuid);
      query.bindValue(":name_first", author->first);
      query.bindValue(":name_last", author->last);
      query.bindValue(":email", author->email);

      const auto zipcode = (author->plus_4.isEmpty()) ? QString("%1").arg(author->zip) : QString("%1-%2").arg(author->zip).arg(author->plus_4);
      query.bindValue(":zipcode", zipcode);

      query.bindValue(":state", author->state);
      query.bindValue(":country", author->country);
      query.bindValue(":phone", author->phone);
      query.bindValue(":organization", author->organization);
      if (!query.exec()) {
        qWarning() << "update_author" << query.lastError();
        return false;
      }
      if (-1 == author->id) {
        const auto r = select_author(author);
        authorUpdated(author->id);
        return r;
      }
      authorUpdated(author->id);
      return true;
    }
    qWarning() << "Author Requires an Email Value";
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_first_author(Author* author)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    if (!author->email.isEmpty()) {
      QSqlQuery query{ QSqlDatabase::database(_db_name) };
      query.prepare(sqlite3::insert_or_update_first_author);
      if (author->uuid == "") {
        author->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
      }
      query.bindValue(":uuid", author->uuid);
      query.bindValue(":name_first", author->first);
      query.bindValue(":name_last", author->last);
      query.bindValue(":email", author->email);

      const auto zipcode = (author->plus_4.isEmpty()) ? QString("%1").arg(author->zip) : QString("%1-%2").arg(author->zip).arg(author->plus_4);
      query.bindValue(":zipcode", zipcode);

      query.bindValue(":state", author->state);
      query.bindValue(":country", author->country);
      query.bindValue(":phone", author->phone);
      query.bindValue(":organization", author->organization);
      if (!query.exec()) {
        qWarning() << "update_first_author" << query.lastError();
        return false;
      }
      authorUpdated(1);
      return true;
    }
    qWarning() << "Author Requires an Email Value";
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_author(Author* author)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_author(author)) {
      query.prepare(sqlite3::delete_author_by_id);
      query.bindValue(":id", author->id);
      if (!query.exec()) {
        qWarning() << "remove_author" << query.lastError();
        return false;
      }
      authorRemoved(author->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//----------------------------CITATION--------------------------------------------
inline void assign_citation(const QSqlRecord& record, Citation& citation)
{
  citation.id = record.value(CITATION_ID).toInt();
  citation.uuid = record.value(CITATION_UUID).toString();
  citation.key = record.value(CITATION_KEY).toString();
  citation.title = record.value(CITATION_TITLE).toString();
  citation.authors = record.value(CITATION_AUTHORS).toString();
  citation.year = record.value(CITATION_YEAR).toString();
  citation.publisher = record.value(CITATION_PUBLISHER).toString();
}
int SQLite3Driver::citation_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_citations);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
int SQLite3Driver::citation_count(Scene* scene) const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_citations_in_scene);
    query.bindValue(":id", scene->id);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::citations()
{
  qDeleteAll(_citations);
  _citations.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_citations);
    query.exec();
    while (query.next()) {
      auto citation = std::make_unique<Citation>();
      auto record = query.record();
      assign_citation(record, *citation);
      _citations.push_back(citation.release());
    }
    _current_citation = _citations.begin();
    emit citationsChanged();
  }
}
bool SQLite3Driver::next_citation(Citation* citation)
{
  if (_current_citation == _citations.end() || _citations.empty()) {
    return false;
  }
  citation->assign(*(*_current_citation));
  ++_current_citation;

  return true;
}
bool SQLite3Driver::select_citation(Citation* citation) const
{
  //TODO: Hitting the exact title is going to be pretty hard
  //TODO: Might want to add an alias or transform for the select where you remove all whitespace and downcase the title

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (citation->id != -1) {
      query.prepare(sqlite3::select_citation_by_id);
      query.bindValue(":id", citation->id);
    } else if (!citation->key.isEmpty()) {
      query.prepare(sqlite3::select_citation_by_key);
      query.bindValue(":key", citation->key);
    } else if (!citation->title.isEmpty()) {
      query.prepare(sqlite3::select_citation_by_title);
      query.bindValue(":title", citation->title);
    } else {
      qWarning() << "Provided Property has no id, key, or title one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_citation(record, *citation);
        return true;
      }
    } else {
      qWarning() << "select_citation" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_citation(Citation* citation)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (-1 != citation->id) {
      query.prepare(sqlite3::update_citation_by_id);
      query.bindValue(":id", citation->id);
    } else if (!citation->key.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_citations);
    } else if (!citation->title.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_citations);
      auto first_author = citation->authors.split(";", QString::SkipEmptyParts);
      citation->key = (first_author.empty()) ? citation->title.toLower().simplified().remove(' ')
                                             : QString("%1%2").arg(first_author[0]).arg(citation->year);
    }
    if (citation->uuid == "") {
      citation->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
    query.bindValue(":uuid", citation->uuid);
    query.bindValue(":key", citation->key);
    query.bindValue(":title", citation->title);
    query.bindValue(":authors", citation->authors);
    query.bindValue(":year", citation->year);
    query.bindValue(":publisher", citation->publisher);

    if (!query.exec()) {
      qWarning() << "update_citation" << query.lastError();
      return false;
    }
    if (-1 == citation->id) {
      auto r = select_citation(citation);
      citationUpdated(citation->id);
      return r;
    }
    citationUpdated(citation->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_citation_in_scene(Scene* scene, Citation* citation)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (select_scene(scene)) {
      if (!select_citation(citation)) {
        update_citation(citation);
      }
      CitationMap map;
      map.fk_citation = citation->id;
      map.fk_scene = scene->id;
      if (!select_citation_map(&map)) {
        update_citation_map(&map);
      }
      return true;
    }
    qWarning() << "Scene not found";
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_citation(Citation* citation)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_citation(citation)) {
      query.prepare(sqlite3::delete_citation_by_id);
      query.bindValue(":id", citation->id);
      if (!query.exec()) {
        qWarning() << "remove_citation" << query.lastError();
        return false;
      }
      citationRemoved(citation->id);
      QSqlQuery query_map(QSqlDatabase::database(_db_name));
      query_map.prepare(sqlite3::delete_citation_map_by_fk_citation);
      query_map.bindValue(":fk_citation", citation->id);
      if (!query_map.exec()) {
        qWarning() << query_map.lastError();
        return false;
      }
      return (remove_citation_from_equipment(citation->id)
              && remove_citation_from_treatments(citation->id)
              && remove_citation_from_injuries(citation->id)
              && remove_citation_from_objectives(citation->id));
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_citation_from_scene(Citation* citation, Scene* scene)
{
  CitationMap map;
  map.fk_scene = scene->id;
  map.fk_citation = citation->id;
  return remove_citation_map_by_fk(&map);
}
//----------------------------INJURY-------------------------------------------------
inline void assign_injury(const QSqlRecord& record, Injury& injury)
{
  injury.id = record.value(INJURY_ID).toInt();
  injury.uuid = record.value(INJURY_UUID).toString();
  injury.medical_name = record.value(INJURY_MEDICAL_NAME).toString();
  injury.common_name = record.value(INJURY_COMMON_NAME).toString();
  injury.description = record.value(INJURY_DESCRIPTION).toString();
  injury.lower_bound = record.value(INJURY_LOWER_BOUND).toFloat();
  injury.upper_bound = record.value(INJURY_UPPER_BOUND).toFloat();
  injury.citations = record.value(INJURY_CITATIONS).toString();
}
int SQLite3Driver::injury_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_injuries);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::injuries()
{
  qDeleteAll(_injuries);
  _injuries.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_injuries);
    query.exec();
    while (query.next()) {
      auto injury = std::make_unique<Injury>();
      auto record = query.record();

      assign_injury(record, *injury);
      _injuries.push_back(injury.release());
    }
    _current_injury = _injuries.begin();
    emit injuriesChanged();
  }
}
bool SQLite3Driver::next_injury(Injury* injury)
{
  if (_current_injury == _injuries.end() || _injuries.empty()) {
    return false;
  }
  injury->assign(*(*_current_injury));
  ++_current_injury;

  return true;
}
bool SQLite3Driver::select_injury(Injury* injury) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (injury->id != -1) {
      query.prepare(sqlite3::select_injury_by_id);
      query.bindValue(":id", injury->id);
    } else if (!injury->medical_name.isEmpty()) {
      query.prepare(sqlite3::select_injury_by_medical_name);
      query.bindValue(":medical_name", injury->medical_name);
    } else if (!injury->common_name.isEmpty()) {
      query.prepare(sqlite3::select_injury_by_common_name);
      query.bindValue(":common_name", injury->common_name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_injury(record, *injury);
        return true;
      }
    } else {
      qWarning() << "select_injury" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_injury(Injury* injury)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (-1 != injury->id) {
      query.prepare(sqlite3::update_injury_by_id);
      query.bindValue(":id", injury->id);
    } else if (!injury->medical_name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_injuries);
    } else if (!injury->common_name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_injuries);
    }
    if (injury->uuid == "") {
      injury->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
    query.bindValue(":uuid", injury->uuid);
    query.bindValue(":citations", injury->citations);
    query.bindValue(":medical_name", injury->medical_name);
    query.bindValue(":common_name", injury->common_name);
    query.bindValue(":description", injury->description);
    query.bindValue(":lower_bound", injury->lower_bound);
    query.bindValue(":upper_bound", injury->upper_bound);

    if (!query.exec()) {
      qWarning() << "update_injury" << query.lastError();
      return false;
    }
    if (-1 == injury->id) {
      const auto r = select_injury(injury);
      injuryUpdated(injury->id);
      return r;
    }
    injuryUpdated(injury->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_injury(Injury* injury)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_injury(injury)) {
      query.prepare(sqlite3::delete_injury_by_id);
      query.bindValue(":id", injury->id);
      if (!query.exec()) {
        qWarning() << "remove_injury" << query.lastError();
        return false;
      }
      injuryRemoved(injury->id);
      return remove_injury_from_injury_sets(injury->id);
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------INJURY_SET----------------------------------------
inline void assign_injury_set(const QSqlRecord& record, InjurySet& injury)
{
  injury.id = record.value(INJURY_SET_ID).toInt();
  injury.uuid = record.value(INJURY_SET_UUID).toString();
  injury.name = record.value(INJURY_SET_NAME).toString();
  injury.description = record.value(INJURY_SET_DESCRIPTION).toString();
  injury.injuries = record.value(INJURY_SET_INJURIES).toString();
  injury.locations = record.value(INJURY_SET_LOCATIONS).toString();
  injury.severities = record.value(INJURY_SET_SEVERITIES).toString();
}
int SQLite3Driver::injury_set_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_injury_sets);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::injury_sets()
{
  qDeleteAll(_injury_sets);
  _injury_sets.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_injury_sets);
    query.exec();
    while (query.next()) {
      auto set = std::make_unique<InjurySet>();
      auto record = query.record();

      assign_injury_set(record, *set);
      _injury_sets.push_back(set.release());
    }
    _current_injury_set = _injury_sets.begin();
    emit injuriesChanged();
  }
}
bool SQLite3Driver::next_injury_set(InjurySet* injury_set)
{
  if (_current_injury_set == _injury_sets.end() || _injury_sets.empty()) {
    return false;
  }
  injury_set->assign(*(*_current_injury_set));
  ++_current_injury_set;

  return true;
}
bool SQLite3Driver::select_injury_set(InjurySet* set) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (set->id != -1) {
      query.prepare(sqlite3::select_injury_set_by_id);
      query.bindValue(":id", set->id);
    } else if (!set->name.isEmpty()) {
      query.prepare(sqlite3::select_injury_set_by_name);
      query.bindValue(":name", set->name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_injury_set(record, *set);
        return true;
      }
    } else {
      qWarning() << "select_injury_set" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_injury_set(InjurySet* injury_set)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (-1 != injury_set->id) {
      query.prepare(sqlite3::update_injury_set_by_id);
      query.bindValue(":id", injury_set->id);
    } else if (!injury_set->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_injury_sets);
    }
    if (injury_set->uuid == "") {
      injury_set->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
    query.bindValue(":uuid", injury_set->uuid);
    query.bindValue(":name", injury_set->name);
    query.bindValue(":description", injury_set->description);
    query.bindValue(":injuries", injury_set->injuries);
    query.bindValue(":severities", injury_set->severities);
    query.bindValue(":locations", injury_set->locations);

    if (!query.exec()) {
      qWarning() << "update_injury_set" << query.lastError();
      return false;
    }
    if (-1 == injury_set->id) {
      const auto r = select_injury_set(injury_set);
      injurySetUpdated(injury_set->id);
      return r;
    }
    injurySetUpdated(injury_set->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_injury_set(InjurySet* injury_set)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_injury_set(injury_set)) {
      query.prepare(sqlite3::delete_injury_set_by_id);
      query.bindValue(":id", injury_set->id);
      if (!query.exec()) {
        qWarning() << "remove_injury_set" << query.lastError();
        return false;
      }
      injurySetRemoved(injury_set->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//----------------------------EVENT-------------------------------------------------
inline void assign_event(QSqlRecord& record, Event& event)
{
  event.id = record.value(EVENT_ID).toInt();
  event.uuid = record.value(EVENT_UUID).toString();
  event.name = record.value(EVENT_NAME).toString();
  event.description = record.value(EVENT_DESCRIPTION).toString();
  event.fidelity = record.value(EVENT_FIDELITY).toString();
  event.category = record.value(EVENT_CATEGORY).toString();
  event.fk_actor_1 = record.value(EVENT_ACTOR_1).toString();
  event.fk_actor_2 = record.value(EVENT_ACTOR_2).toString();
  event.equipment = record.value(EVENT_EQUIPMENT).toString();
  event.details = record.value(EVENT_DETAILS).toString();
}
int SQLite3Driver::event_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_events);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
int SQLite3Driver::event_count(Scene* scene) const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_events_in_scene);
    query.bindValue(":id", scene->id);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::events()
{
  qDeleteAll(_events);
  _events.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_events);
    query.exec();
    while (query.next()) {
      auto event = std::make_unique<Event>();
      auto record = query.record();

      assign_event(record, *event);
      _events.push_back(event.release());
    }
    _current_event = _events.begin();
    emit eventsChanged();
  }
}
void SQLite3Driver::events_in_scene(Scene* scene)
{
  qDeleteAll(_events);
  _events.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_events_in_scene);
    query.bindValue(":scene_id", scene->id);
    query.exec();
    while (query.next()) {
      auto event = std::make_unique<Event>();
      auto record = query.record();

      assign_event(record, *event);
      _events.push_back(event.release());
    }
    _current_event = _events.begin();
    emit eventsChanged();
  }
}
bool SQLite3Driver::next_event(Event* event)
{
  if (_current_event == _events.end() || _events.empty()) {
    return false;
  }
  event->assign(*(*_current_event));
  ++_current_event;

  return true;
}
bool SQLite3Driver::select_event(Event* event) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (event->id != -1) {
      query.prepare(sqlite3::select_event_by_id);
      query.bindValue(":id", event->id);
    } else if (!event->name.isEmpty()) {
      query.prepare(sqlite3::select_event_by_name);
      query.bindValue(":name", event->name);
    } else {
      qWarning() << "Provided Event has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_event(record, *event);
        return true;
      }
    } else {
      qWarning() << "select_event" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_event(Event* event)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (-1 != event->id) {
      query.prepare(sqlite3::update_event_by_id);
      query.bindValue(":id", event->id);
    } else if (!event->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_events);
    }
    if (event->uuid == "") {
      event->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
    query.bindValue(":uuid", event->uuid);
    query.bindValue(":equipment", event->equipment);
    query.bindValue(":name", event->name);
    query.bindValue(":description", event->description);
    query.bindValue(":category", event->category);
    query.bindValue(":fidelity", event->fidelity);
    query.bindValue(":actor_1", event->fk_actor_1);
    query.bindValue(":actor_2", event->fk_actor_2);
    query.bindValue(":details", event->details);
    if (!query.exec()) {
      qWarning() << "update_event" << query.lastError();
      return false;
    }
    if (-1 == event->id) {
      const auto r = select_event(event);
      eventUpdated(event->id);
      return r;
    }
    eventUpdated(event->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_event_in_scene(Scene* scene, Event* event)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (select_scene(scene)) {
      if (!select_event(event)) {
        update_event(event);
      }
      EventMap map;
      map.fk_event = event->id;
      map.fk_scene = scene->id;
      if (!select_event_map(&map)) {
        update_event_map(&map);
      }
      return true;
    }
    qWarning() << "Scene not found";
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_event(Event* event)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_event(event)) {
      query.prepare(sqlite3::delete_event_by_id);
      query.bindValue(":id", event->id);
      if (!query.exec()) {
        qWarning() << "remove_event" << query.lastError();
        return false;
      }
      eventRemoved(event->id);
      QSqlQuery query_map(QSqlDatabase::database(_db_name));
      query_map.prepare(sqlite3::delete_event_map_by_fk_event);
      query_map.bindValue(":fk_event", event->id);
      if (!query_map.exec()) {
        qWarning() << query_map.lastError();
        return false;
      }
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_event_from_scene(Event* event, Scene* scene)
{
  EventMap map;
  map.fk_scene = scene->id;
  map.fk_event = event->id;
  return remove_event_map_by_fk(&map);
}
//-----------------------------LOCATION------------------------------------------
inline void assign_location(const QSqlRecord& record, Location& location)
{
  location.id = record.value(LOCATION_ID).toInt();
  location.uuid = record.value(LOCATION_UUID).toString();
  location.name = record.value(LOCATION_NAME).toString();
  location.environment = record.value(LOCATION_ENVIRONMENT).toString();
}
int SQLite3Driver::location_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_locations);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::locations()
{
  qDeleteAll(_locations);
  _locations.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_locations);
    query.exec();
    while (query.next()) {
      auto location = std::make_unique<Location>();
      auto record = query.record();

      assign_location(record, *location);
      _locations.push_back(location.release());
    }
    _current_location = _locations.begin();
    emit locationsChanged();
  }
}
bool SQLite3Driver::next_location(Location* location)
{
  if (_current_location == _locations.end() || _locations.empty()) {
    return false;
  }
  location->assign(*(*_current_location));
  ++_current_location;

  return true;
}
bool SQLite3Driver::select_location(Location* location) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (location->id != -1) {
      query.prepare(sqlite3::select_location_by_id);
      query.bindValue(":id", location->id);
    } else if (!location->name.isEmpty()) {
      query.prepare(sqlite3::select_location_by_name);
      query.bindValue(":name", location->name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_location(record, *location);
        return true;
      }
    } else {
      qWarning() << "select_locaton" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_location(Location* location)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (-1 != location->id) {
      query.prepare(sqlite3::update_location_by_id);
      query.bindValue(":id", location->id);
    } else if (!location->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_locations);
    }
    if (location->uuid == "") {
      location->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
    query.bindValue(":uuid", location->uuid);
    query.bindValue(":name", location->name);
    query.bindValue(":environment", location->environment);
    if (!query.exec()) {
      qWarning() << "update_location" << query.lastError();
      return false;
    }
    if (-1 == location->id) {
      const auto r = select_location(location);
      locationUpdated(location->id);
      return r;
    }
    locationUpdated(location->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
int SQLite3Driver::location_count(Scene* scene) const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_locations_in_scene);
    query.bindValue(":id", scene->id);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::locations_in_scene(Scene* scene)
{
  qDeleteAll(_locations);
  _locations.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<int32_t> fk_location;
    QSqlQuery location_map_query{ QSqlDatabase::database(_db_name) };
    location_map_query.prepare(sqlite3::select_location_map_by_fk_scene);
    location_map_query.bindValue(":fk_scene", scene->id);
    location_map_query.exec();
    while (location_map_query.next()) {
      auto location_map = std::make_unique<LocationMap>();
      auto location_map_record = location_map_query.record();
      assign_location_map(location_map_record, *location_map);
      fk_location.push_back(location_map->fk_location);
    }
    emit locationMapsChanged();

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_location_by_id);
    while (!fk_location.empty()) {
      query.bindValue(":id", fk_location.back());
      bool huh = query.exec();
      fk_location.pop_back();
      if (query.next()) {
        auto location = std::make_unique<Location>();
        auto record = query.record();
        assign_location(record, *location);
        _locations.push_back(location.release());
      }
    }
    _current_location = _locations.begin();
    emit locationsChanged();
  }
}
bool SQLite3Driver::update_location_in_scene(Scene* scene, Location* location)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (select_scene(scene)) {
      if (!select_location(location)) {
        update_location(location);
      }
      LocationMap map;
      map.fk_location = location->id;
      map.fk_scene = scene->id;
      if (location_map_count(scene) > 0) {
        QSqlQuery query_location_map(QSqlDatabase::database(_db_name));
        query_location_map.prepare(sqlite3::delete_location_map_by_fk_scene);
        query_location_map.bindValue(":fk_scene", scene->id);
        if (!query_location_map.exec()) {
          qWarning() << query_location_map.lastError();
          return false;
        }
      }
      if (!select_location_map(&map)) {
        update_location_map(&map);
      } else {
        LocationMap prev_map;
        prev_map.fk_location = scene->id;
        remove_location_map(&prev_map);
        update_location_map(&map);
      }
      return true;
    }
    qWarning() << "Scene not found";
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_location(Location* location)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_location(location)) {
      query.prepare(sqlite3::delete_location_by_id);
      query.bindValue(":id", location->id);
      if (!query.exec()) {
        qWarning() << "remove_location" << query.lastError();
        return false;
      }
      locationRemoved(location->id);
      QSqlQuery query_map(QSqlDatabase::database(_db_name));
      query_map.prepare(sqlite3::delete_location_map_by_fk_location);
      query_map.bindValue(":fk_location", location->id);
      if (!query_map.exec()) {
        qWarning() << query_map.lastError();
        return false;
      }
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_location_from_scene(Location* location, Scene* scene)
{
  LocationMap map;
  map.fk_scene = scene->id;
  map.fk_location = location->id;
  return remove_location_map_by_fk(&map);
}
//----------------------------EQUIPMENT------------------------------------------
inline void assign_equipment(const QSqlRecord& record, Equipment& equipment)
{
  equipment.id = record.value(EQUIPMENT_ID).toInt();
  equipment.uuid = record.value(EQUIPMENT_UUID).toString();
  equipment.name = record.value(EQUIPMENT_NAME).toString();
  equipment.type = record.value(EQUIPMENT_TYPE).toInt();
  equipment.summary = record.value(EQUIPMENT_SUMMARY).toString();
  equipment.description = record.value(EQUIPMENT_DESCRIPTION).toString();
  equipment.citations = record.value(EQUIPMENT_CITATIONS).toString();
  equipment.image = record.value(EQUIPMENT_IMAGE).toString();
  equipment.properties = record.value(EQUIPMENT_PROPERTIES).toString();
}
int SQLite3Driver::equipment_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_equipments);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
int SQLite3Driver::equipment_count(Scene* scene) const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_equipments_in_scene);
    query.bindValue(":id", scene->id);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::equipments()
{
  qDeleteAll(_equipments);
  _equipments.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_equipments);
    query.exec();
    while (query.next()) {
      auto equipment = std::make_unique<Equipment>();
      auto record = query.record();

      assign_equipment(record, *equipment);
      _equipments.push_back(equipment.release());
    }
    _current_equipment = _equipments.begin();
    emit equipmentsChanged();
  }
}
void SQLite3Driver::equipment_in_scene(Scene* scene)
{

  qDeleteAll(_equipment_maps);
  _equipment_maps.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery equipment_map_query{ QSqlDatabase::database(_db_name) };
    equipment_map_query.prepare(sqlite3::select_all_equipment_in_a_scene);
    equipment_map_query.bindValue(":fk_scene", scene->id);
    equipment_map_query.exec();
    while (equipment_map_query.next()) {
      auto equipment_map = std::make_unique<EquipmentMap>();
      auto equipment_map_record = equipment_map_query.record();
      assign_equipment_map(equipment_map_record, *equipment_map, scene);
      _equipment_maps.push_back(equipment_map.release());
    }
    _current_equipment_map = _equipment_maps.begin();
    emit equipmentMapsChanged();
  }
}
bool SQLite3Driver::next_equipment(Equipment* equipment)
{
  if (_current_equipment == _equipments.end() || _equipments.empty()) {
    return false;
  }
  equipment->assign(*(*_current_equipment));
  ++_current_equipment;

  return true;
}
bool SQLite3Driver::select_equipment(Equipment* equipment) const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (equipment->id != -1) {
      query.prepare(sqlite3::select_equipment_by_id);
      query.bindValue(":id", equipment->id);
    } else if (!equipment->name.isEmpty()) {
      query.prepare(sqlite3::select_equipment_by_name);
      query.bindValue(":name", equipment->name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_equipment(record, *equipment);
        return true;
      }
    } else {
      qWarning() << "select_equipment" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_equipment(Equipment* equipment)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (-1 != equipment->id) {
      query.prepare(sqlite3::update_equipment_by_id);
      query.bindValue(":id", equipment->id);
    } else if (!equipment->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_equipments);
    }
    if (equipment->uuid == "") {
      equipment->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
    query.bindValue(":uuid", equipment->uuid);
    query.bindValue(":name", equipment->name);
    query.bindValue(":type", equipment->type);
    query.bindValue(":summary", equipment->summary);
    query.bindValue(":description", equipment->description);
    query.bindValue(":citations", equipment->citations);
    query.bindValue(":image", equipment->image);
    query.bindValue(":properties", equipment->properties);

    if (!query.exec()) {
      qWarning() << "update_equipment" << query.lastError();
      return false;
    }
    if (-1 == equipment->id) {
      const auto r = select_equipment(equipment);
      equipmentUpdated(equipment->id);
      return r;
    }
    equipmentUpdated(equipment->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_equipment_in_scene(EquipmentMap* map)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (map->id == -1) {
      if (map->scene && map->equipment) {
        if (select_scene(map->scene)) {
          if (!select_equipment(map->equipment) && !update_equipment(map->equipment)) {
            qWarning() << QString("Unable to add New Equipment %1 to equipment table.").arg(map->equipment->name);
            return false;
          }
          return update_equipment_map(map);
        }
      }
      qWarning() << "Error adding Equipment to Scene with out valid scene and equipment.";
      return false;
    } else {
      return update_equipment_map(map);
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_equipment(Equipment* equipment)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_equipment(equipment)) {
      query.prepare(sqlite3::delete_equipment_by_id);
      query.bindValue(":id", equipment->id);
      if (!query.exec()) {
        qWarning() << "remove_equipment" << query.lastError();
        return false;
      }
      equipmentRemoved(equipment->id);
      QSqlQuery query_map(QSqlDatabase::database(_db_name));
      query_map.prepare(sqlite3::delete_this_equipment_across_all_scenes);
      query_map.bindValue(":fk_equipment", equipment->id);
      if (!query_map.exec()) {
        qWarning() << query_map.lastError();
        return false;
      }
      return remove_equipment_from_treatments(equipment->id);
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_equipment_from_scene(Equipment* equipment, Scene* scene)
{
  EquipmentMap map;
  map.scene = scene;
  map.equipment = equipment;
  return remove_equipment_map_by_fk(&map);
}
//-----------------------------OBJECTIVE-----------------------------------------
inline void assign_objective(const QSqlRecord& record, Objective& objective)
{
  objective.id = record.value(OBJECTIVE_ID).toInt();
  objective.uuid = record.value(OBJECTIVE_UUID).toString();
  objective.name = record.value(OBJECTIVE_NAME).toString();
  objective.description = record.value(OBJECTIVE_DESCRIPTION).toString();
  objective.citations = record.value(OBJECTIVE_CITATIONS).toString();
}
int SQLite3Driver::objective_count() const

{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_objectives);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::objectives()
{
  qDeleteAll(_objectives);
  _objectives.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_objectives);
    query.exec();
    while (query.next()) {
      auto objective = std::make_unique<Objective>();
      auto record = query.record();

      assign_objective(record, *objective);
      _objectives.push_back(objective.release());
    }
    _current_objective = _objectives.begin();
    emit objectivesChanged();
  }
}
bool SQLite3Driver::next_objective(Objective* objective)
{
  if (_current_objective == _objectives.end() || _objectives.empty()) {
    return false;
  }
  objective->assign(*(*_current_objective));
  ++_current_objective;

  return true;
}
bool SQLite3Driver::select_objective(Objective* objective) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (objective->id != -1) {
      query.prepare(sqlite3::select_objective_by_id);
      query.bindValue(":id", objective->id);
    } else if (!objective->name.isEmpty()) {
      query.prepare(sqlite3::select_objective_by_name);
      query.bindValue(":name", objective->name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_objective(record, *objective);
        return true;
      }
    } else {
      qWarning() << "select_objective" << query.lastError();
    }
  } else {
    qWarning() << "No Database connection";
  }
  return false;
}
bool SQLite3Driver::update_objective(Objective* objective)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (-1 != objective->id) {
      query.prepare(sqlite3::update_objective_by_id);
      query.bindValue(":id", objective->id);
    } else if (!objective->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_objective);
    }
    if (objective->uuid == "") {
      objective->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
    query.bindValue(":uuid", objective->uuid);
    query.bindValue(":name", objective->name);
    query.bindValue(":description", objective->description);
    query.bindValue(":citations", objective->citations);

    if (!query.exec()) {
      qWarning() << "update_objective" << query.lastError();
      return false;
    }
    if (-1 == objective->id) {
      auto r = select_objective(objective);
      objectiveUpdated(objective->id);
      return r;
    }
    objectiveUpdated(objective->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_objective(Objective* objective)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_objective(objective)) {
      query.prepare(sqlite3::delete_objective_by_id);
      query.bindValue(":id", objective->id);
      if (!query.exec()) {
        qWarning() << "remove_objective" << query.lastError();
        return false;
      }
      objectiveRemoved(objective->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------PROPERTY------------------------------------------
inline void assign_property(const QSqlRecord& record, Property& property)
{
  property.id = record.value(PROPERTY_ID).toInt();
  property.name = record.value(PROPERTY_NAME).toString();
  property.value = record.value(PROPERTY_VALUE).toString();
}
int SQLite3Driver::property_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_properties);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::properties()
{
  qDeleteAll(_properties);
  _properties.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_properties);
    query.exec();
    while (query.next()) {
      auto property = std::make_unique<Property>();
      auto record = query.record();
      int ct = record.count();

      assign_property(record, *property);
      _properties.push_back(property.release());
    }
    _current_property = _properties.begin();
    emit propertiesChanged();
  }
}
bool SQLite3Driver::next_property(Property* property)
{
  if (_current_property == _properties.end() || _properties.empty()) {
    return false;
  }
  property->assign(*(*_current_property));
  ++_current_property;
  return true;
}
bool SQLite3Driver::select_property(Property* property) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (property->id != -1) {
      query.prepare(sqlite3::select_property_by_id);
      query.bindValue(":id", property->id);
    } else if (!property->name.isEmpty()) {
      query.prepare(sqlite3::select_property_by_name);
      query.bindValue(":name", property->name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {

      while (query.next()) {
        record = query.record();
        assign_property(record, *property);
        return true;
      }
    } else {
      qWarning() << "select_property" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_property(Property* property)
{

  if (ready()) {
    if (!property->name.isEmpty()) {
      QSqlQuery query{ QSqlDatabase::database(_db_name) };
      query.prepare(sqlite3::insert_or_update_properties);
      query.bindValue(":name", property->name);
      query.bindValue(":value", property->value);
      if (!query.exec()) {
        qWarning() << "update_property" << query.lastError();
        return false;
      }
      if (-1 == property->id) {
        const auto r = select_property(property);
        propertyUpdated(property->id);
        return r;
      }
      propertyUpdated(property->id);
      return true;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_property(Property* property)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_property(property)) {
      query.prepare(sqlite3::delete_property_by_id);
      query.bindValue(":id", property->id);
      if (!query.exec()) {
        qWarning() << "remove_property" << query.lastError();
        return false;
      }
      propertyRemoved(property->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------ROLE----------------------------------------------
inline void assign_role(QSqlRecord& record, Role& role)
{
  role.id = record.value(ROLE_ID).toInt();
  role.uuid = record.value(ROLE_UUID).toString();
  role.name = record.value(ROLE_NAME).toString();
  role.description = record.value(ROLE_DESCRIPTION).toString();
}
int SQLite3Driver::role_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_roles);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
int SQLite3Driver::role_count(Scene* scene) const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_roles_in_scene);
    query.bindValue(":id", scene->id);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::roles()
{
  qDeleteAll(_roles);
  _roles.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_roles);
    query.exec();
    while (query.next()) {
      auto role = std::make_unique<Role>();
      auto record = query.record();

      assign_role(record, *role);
      _roles.push_back(role.release());
    }
    _current_role = _roles.begin();
    emit rolesChanged();
  }
}
void SQLite3Driver::roles_in_scene(Scene* scene)
{
  qDeleteAll(_roles);
  _roles.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<int32_t> fk_role;
    QSqlQuery map_query{ QSqlDatabase::database(_db_name) };
    map_query.prepare(sqlite3::select_role_map_by_fk_scene);
    map_query.bindValue(":fk_scene", scene->id);
    map_query.exec();
    while (map_query.next()) {
      auto map = std::make_unique<RoleMap>();
      auto map_record = map_query.record();
      assign_role_map(map_record, *map);
      fk_role.push_back(map->fk_role);
    }
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_role_by_id);
    while (!fk_role.empty()) {
      query.bindValue(":id", fk_role.back());
      bool huh = query.exec();
      fk_role.pop_back();
      if (query.next()) {
        auto role = std::make_unique<Role>();
        auto record = query.record();
        assign_role(record, *role);
        int32_t iid = role->id;
        QString nuum = role->name;
        QString desc = role->description;
        _roles.push_back(role.release());
      }
    }
    _current_role = _roles.begin();
    emit rolesChanged();
  }
}
bool SQLite3Driver::next_role(Role* role)
{
  if (_current_role == _roles.end() || _roles.empty()) {
    return false;
  }
  role->assign(*(*_current_role));
  ++_current_role;

  return true;
}
bool SQLite3Driver::select_role(Role* role) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (role->id != -1) {
      query.prepare(sqlite3::select_role_by_id);
      query.bindValue(":id", role->id);
    } else if (!role->name.isEmpty()) {
      query.prepare(sqlite3::select_role_by_name);
      query.bindValue(":name", role->name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_role(record, *role);
        return true;
      }
    } else {

      qWarning() << "select_role" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_role(Role* role)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (-1 != role->id) {
      query.prepare(sqlite3::update_role_by_id);
      query.bindValue(":id", role->id);
    } else if (!role->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_roles);
    }
    if (role->uuid == "") {
      role->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
    query.bindValue(":uuid", role->uuid);
    query.bindValue(":description", role->description);
    query.bindValue(":name", role->name);
    if (!query.exec()) {
      qWarning() << "update_role" << query.lastError();
      return false;
    }
    if (-1 == role->id) {
      const auto r = select_role(role);
      roleUpdated(role->id);
      return r;
    }
    roleUpdated(role->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_role_in_scene(Scene* scene, Role* role)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (select_scene(scene)) {
      if (!select_role(role)) {
        update_role(role);
      }
      RoleMap map;
      map.fk_role = role->id;
      map.fk_scene = scene->id;
      if (!select_role_map(&map)) {
        update_role_map(&map);
      }
      return true;
    }
    qWarning() << "Scene not found";
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_role(Role* role) // This deletes a role completely from the database
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_role(role)) {
      query.prepare(sqlite3::delete_role_by_id);
      query.bindValue(":id", role->id);
      if (!query.exec()) {
        qWarning() << "remove_role" << query.lastError();
        return false;
      }
      roleRemoved(role->id);
      QSqlQuery query_map(QSqlDatabase::database(_db_name));
      query_map.prepare(sqlite3::delete_role_map_by_fk_role);
      query_map.bindValue(":fk_role", role->id);
      if (!query_map.exec()) {
        qWarning() << query_map.lastError();
        return false;
      }
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_role_from_scene(Role* role, Scene* scene)
{
  RoleMap map;
  map.fk_scene = scene->id;
  map.fk_role = role->id;
  return remove_role_map_by_fk(&map);
}
//-----------------------------SCENE---------------------------------------------
inline void assign_scene(QSqlRecord& record, Scene& scene)
{
  scene.id = record.value(SCENE_ID).toInt();
  scene.uuid = record.value(SCENE_UUID).toString();
  scene.name = record.value(SCENE_NAME).toString();
  scene.description = record.value(SCENE_DESCRIPTION).toString();
  scene.time_of_day = record.value(SCENE_TIME_OF_DAY).toString();
  scene.time_in_simulation = record.value(SCENE_TIME_IN_SIMULATION).toInt();
  scene.weather = record.value(SCENE_WEATHER).toString();
  scene.events = record.value(SCENE_EVENTS).toString();
  scene.items = record.value(SCENE_ITEMS).toString();
  scene.roles = record.value(SCENE_ROLES).toString();
  scene.details = record.value(SCENE_DETAILS).toString();
}
int SQLite3Driver::scene_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_scenes);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::scenes()
{
  qDeleteAll(_scenes);
  _scenes.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_scenes);
    query.exec();
    while (query.next()) {
      auto scene = std::make_unique<Scene>();
      auto record = query.record();

      assign_scene(record, *scene);
      _scenes.push_back(scene.release());
    }
    _current_scene = _scenes.begin();
    emit scenesChanged();
  }
}
bool SQLite3Driver::next_scene(Scene* scene)
{
  if (_current_scene == _scenes.end() || _scenes.empty()) {
    return false;
  }
  scene->assign(*(*_current_scene));
  ++_current_scene;

  return true;
}
bool SQLite3Driver::select_scene(Scene* scene) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (scene->id != -1) {
      query.prepare(sqlite3::select_scene_by_id);
      query.bindValue(":id", scene->id);
    } else if (!scene->name.isEmpty()) {
      query.prepare(sqlite3::select_scene_by_name);
      query.bindValue(":name", scene->name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_scene(record, *scene);
        return true;
      }
    } else {

      qWarning() << "select_scene" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_scene(Scene* scene)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (-1 != scene->id) {
      query.prepare(sqlite3::update_scene_by_id);
      query.bindValue(":id", scene->id);
    } else if (!scene->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_scenes);
    }
    if (scene->uuid == "") {
      scene->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
    query.bindValue(":uuid", scene->uuid);
    query.bindValue(":name", scene->name);
    query.bindValue(":description", scene->description);
    query.bindValue(":time_of_day", scene->time_of_day);
    query.bindValue(":time_in_simulation", scene->time_in_simulation);
    query.bindValue(":weather", scene->weather);
    query.bindValue(":events", scene->events);
    query.bindValue(":items", scene->items);
    query.bindValue(":roles", scene->roles);
    query.bindValue(":details", scene->details);
    if (!query.exec()) {
      qWarning() << "update_scene" << query.lastError();
      return false;
    }
    if (-1 == scene->id) {
      //!
      //!  Logic may need to be maintained if
      //!  we ever allow a location to be reused in multiple scenes.
      //!
      if (select_scene(scene)) {
        sceneUpdated(scene->id);
        Location location;
        location.id = -1;
        location.name = scene->name + " Location";
        if (update_location_in_scene(scene, &location)) {
          locationUpdated(location.id);
          return true;
        }
      }
      qWarning() << "Unable to update Location associated with Scene";
      return false;
    }
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_scene(Scene* scene)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_scene(scene)) {
      query.prepare(sqlite3::delete_scene_by_id);
      query.bindValue(":id", scene->id);
      if (!query.exec()) {
        qWarning() << "remove_scene" << query.lastError();
        return false;
      }
      sceneRemoved(scene->id);
      QSqlQuery query_map(QSqlDatabase::database(_db_name));
      query_map.prepare(sqlite3::delete_role_map_by_fk_scene);
      query_map.bindValue(":fk_scene", scene->id);
      if (!query_map.exec()) {
        qWarning() << query_map.lastError();
        return false;
      }
      QSqlQuery query_event_map(QSqlDatabase::database(_db_name));
      query_event_map.prepare(sqlite3::delete_event_map_by_fk_scene);
      query_event_map.bindValue(":fk_scene", scene->id);
      if (!query_event_map.exec()) {
        qWarning() << query_event_map.lastError();
        return false;
      }
      QSqlQuery query_equipment_map(QSqlDatabase::database(_db_name));
      query_equipment_map.prepare(sqlite3::delete_all_equipment_from_a_scene);
      query_equipment_map.bindValue(":fk_scene", scene->id);
      if (!query_equipment_map.exec()) {
        qWarning() << query_equipment_map.lastError();
        return false;
      }
      QSqlQuery query_citation_map(QSqlDatabase::database(_db_name));
      query_citation_map.prepare(sqlite3::delete_citation_map_by_fk_scene);
      query_citation_map.bindValue(":fk_scene", scene->id);
      if (!query_citation_map.exec()) {
        qWarning() << query_citation_map.lastError();
        return false;
      }
      LocationMap location_map;
      QSqlQuery query_location_map(QSqlDatabase::database(_db_name));
      query_location_map.prepare(sqlite3::select_location_map_by_fk_scene);
      query_location_map.bindValue(":fk_scene", scene->id);
      if (query_location_map.exec()) {
        if (query_location_map.next()) {
          QSqlRecord record = query_location_map.record();
          assign_location_map(record, location_map);
          QSqlQuery query_delete_location_map(QSqlDatabase::database(_db_name));
          query_delete_location_map.prepare(sqlite3::delete_location_map_by_fk_scene);
          query_delete_location_map.bindValue(":fk_scene", location_map.fk_scene);
          if (!query_delete_location_map.exec()) {
            qWarning() << query_delete_location_map.lastError();
            return false;
          }
          QSqlQuery query_location(QSqlDatabase::database(_db_name));
          query_location.prepare(sqlite3::delete_location_by_id);
          query_location.bindValue(":id", location_map.fk_location);
          if (!query_location.exec()) {
            qWarning() << query_location.lastError();
            return false;
          }
        }
      } else {
        qWarning() << query_location_map.lastError();
        return false;
      }
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------TREATMENT-----------------------------------------
inline void assign_treatment(const QSqlRecord& record, Treatment& treatment)
{
  treatment.id = record.value(TREATMENT_ID).toInt();
  treatment.uuid = record.value(TREATMENT_UUID).toString();
  treatment.medical_name = record.value(TREATMENT_MEDICAL_NAME).toString();
  treatment.common_name = record.value(TREATMENT_COMMON_NAME).toString();
  treatment.description = record.value(TREATMENT_DESCRIPTION).toString();
  treatment.equipment = record.value(TREATMENT_EQUIPMENT).toString();
  treatment.citations = record.value(TREATMENT_CITATIONS).toString();
}
int SQLite3Driver::treatment_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_treatments);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::treatments()
{
  qDeleteAll(_treatments);
  _treatments.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_treatments);
    query.exec();
    while (query.next()) {
      auto treatment = std::make_unique<Treatment>();
      auto record = query.record();

      assign_treatment(record, *treatment);
      _treatments.push_back(treatment.release());
    }
    _current_treatment = _treatments.begin();
    emit treatmentsChanged();
  }
}
bool SQLite3Driver::next_treatment(Treatment* treatment)
{
  if (_current_treatment == _treatments.end() || _treatments.empty()) {
    return false;
  }
  treatment->assign(*(*_current_treatment));
  ++_current_treatment;

  return true;
}
bool SQLite3Driver::select_treatment(Treatment* treatment) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (treatment->id != -1) {
      query.prepare(sqlite3::select_treatment_by_id);
      query.bindValue(":id", treatment->id);
    } else if (!treatment->medical_name.isEmpty()) {
      query.prepare(sqlite3::select_treatment_by_medical_name);
      query.bindValue(":medical_name", treatment->medical_name);
    } else if (!treatment->common_name.isEmpty()) {
      query.prepare(sqlite3::select_treatment_by_common_name);
      query.bindValue(":common_name", treatment->common_name);
    } else {
      qWarning() << "Provided Property has no id or name one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_treatment(record, *treatment);
        return true;
      }
    } else {
      qWarning() << "select_treatment" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_treatment(Treatment* treatment)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (-1 != treatment->id) {
      query.prepare(sqlite3::update_treatment_by_id);
      query.bindValue(":id", treatment->id);
    } else if (!treatment->medical_name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_treatments);
    } else if (!treatment->common_name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_treatments);
    }
    if (treatment->uuid == "") {
      treatment->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
    query.bindValue(":uuid", treatment->uuid);
    query.bindValue(":citations", treatment->citations);
    query.bindValue(":equipment", treatment->equipment);
    query.bindValue(":medical_name", treatment->medical_name);
    query.bindValue(":common_name", treatment->common_name);
    query.bindValue(":description", treatment->description);

    if (!query.exec()) {
      qWarning() << "update_treatment" << query.lastError();
      return false;
    }
    if (-1 == treatment->id) {
      const auto r = select_treatment(treatment);
      treatmentUpdated(treatment->id);
      return r;
    }
    treatmentUpdated(treatment->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_treatment(Treatment* treatment)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_treatment(treatment)) {
      query.prepare(sqlite3::delete_treatment_by_id);
      query.bindValue(":id", treatment->id);
      if (!query.exec()) {
        qWarning() << "remove_treatment" << query.lastError();
        return false;
      }
      treatmentRemoved(treatment->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------MAP------------------------------------------
inline void assign_role_map(const QSqlRecord& record, RoleMap& map)
{
  map.id = record.value(ROLE_MAP_ID).toInt();
  map.fk_scene = record.value(ROLE_MAP_FK_SCENE).toInt();
  map.fk_role = record.value(ROLE_MAP_FK_ROLE).toInt();
}
int SQLite3Driver::role_map_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_role_maps);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::role_maps()
{
  qDeleteAll(_role_maps);
  _role_maps.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_role_maps);
    query.exec();
    while (query.next()) {
      auto map = std::make_unique<RoleMap>();
      auto record = query.record();

      assign_role_map(record, *map);
      _role_maps.push_back(map.release());
    }
    _current_role_map = _role_maps.begin();
    emit mapsChanged();
  }
}
bool SQLite3Driver::next_role_map(RoleMap* map)
{
  if (_current_role_map == _role_maps.end() || _role_maps.empty()) {
    return false;
  }
  map->assign(*(*_current_role_map));
  ++_current_role_map;

  return true;
}
bool SQLite3Driver::select_role_map(RoleMap* map) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (map->id != -1) {
      query.prepare(sqlite3::select_role_map_by_id);
      query.bindValue(":id", map->id);
    } else if (map->fk_scene != -1) {
      query.prepare(sqlite3::select_role_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_role", map->fk_role);
    } else {
      qWarning() << "Provided Property has no id, fk_scene, or fk_role one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_role_map(record, *map);
        return true;
      }
    } else {
      qWarning() << "select_role_map" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_role_map(RoleMap* map)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (-1 != map->id) {
      query.prepare(sqlite3::update_role_map_by_id);
      query.bindValue(":id", map->id);
    } else {
      query.prepare(sqlite3::insert_or_update_role_maps);
    }
    query.bindValue(":fk_scene", map->fk_scene);
    query.bindValue(":fk_role", map->fk_role);
    if (!query.exec()) {
      qWarning() << "update_role_map" << query.lastError();
      return false;
    }
    if (-1 == map->id) {
      const auto r = select_role_map(map);
      mapUpdated(map->id);
      return r;
    }
    mapUpdated(map->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_role_map(RoleMap* map)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_role_map(map)) {
      query.prepare(sqlite3::delete_role_map_by_id);
      query.bindValue(":id", map->id);
      if (!query.exec()) {
        qWarning() << "remove_role_map" << query.lastError();
        return false;
      }
      mapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_role_map_by_fk(RoleMap* map)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_role_map(map)) {
      query.prepare(sqlite3::delete_role_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_role", map->fk_role);
      if (!query.exec()) {
        qWarning() << "remove_role_map_by_fk" << query.lastError();
        return false;
      }
      mapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------EVENT MAP------------------------------------------
inline void assign_event_map(const QSqlRecord& record, EventMap& map)
{
  map.id = record.value(EVENT_MAP_ID).toInt();
  map.fk_scene = record.value(EVENT_MAP_FK_SCENE).toInt();
  map.fk_event = record.value(EVENT_MAP_FK_EVENT).toInt();
}
int SQLite3Driver::event_map_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_event_maps);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::event_maps()
{
  qDeleteAll(_event_maps);
  _event_maps.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_event_maps);
    query.exec();
    while (query.next()) {
      auto map = std::make_unique<EventMap>();
      auto record = query.record();

      assign_event_map(record, *map);
      _event_maps.push_back(map.release());
    }
    _current_event_map = _event_maps.begin();
    emit eventMapsChanged();
  }
}
bool SQLite3Driver::next_event_map(EventMap* map)
{
  if (_current_event_map == _event_maps.end() || _event_maps.empty()) {
    return false;
  }
  map->assign(*(*_current_event_map));
  ++_current_event_map;

  return true;
}
bool SQLite3Driver::select_event_map(EventMap* map) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (map->id != -1) {
      query.prepare(sqlite3::select_event_map_by_id);
      query.bindValue(":id", map->id);
    } else if (map->fk_scene != -1) {
      query.prepare(sqlite3::select_event_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_event", map->fk_event);
    } else {
      qWarning() << "Provided Property has no id, fk_scene, or fk_event one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_event_map(record, *map);
        return true;
      }
    } else {
      qWarning() << "select_event_map" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_event_map(EventMap* map)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (-1 != map->id) {
      query.prepare(sqlite3::update_event_map_by_id);
      query.bindValue(":id", map->id);
    } else {
      query.prepare(sqlite3::insert_or_update_event_maps);
    }
    query.bindValue(":fk_scene", map->fk_scene);
    query.bindValue(":fk_event", map->fk_event);
    if (!query.exec()) {
      qWarning() << "update_event_map" << query.lastError();
      return false;
    }
    if (-1 == map->id) {
      const auto r = select_event_map(map);
      eventMapUpdated(map->id);
      return r;
    }
    eventMapUpdated(map->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_event_map(EventMap* map)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_event_map(map)) {
      query.prepare(sqlite3::delete_event_map_by_id);
      query.bindValue(":id", map->id);
      if (!query.exec()) {
        qWarning() << "remove_event_map" << query.lastError();
        return false;
      }
      eventMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_event_map_by_fk(EventMap* map)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_event_map(map)) {
      query.prepare(sqlite3::delete_event_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_event", map->fk_event);
      if (!query.exec()) {
        qWarning() << "remove_event_map_by_fk" << query.lastError();
        return false;
      }
      eventMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------LOCATION MAP------------------------------------------
inline void assign_location_map(const QSqlRecord& record, LocationMap& map)
{
  map.id = record.value(LOCATION_MAP_ID).toInt();
  map.fk_scene = record.value(LOCATION_MAP_FK_SCENE).toInt();
  map.fk_location = record.value(LOCATION_MAP_FK_LOCATION).toInt();
}
int SQLite3Driver::location_map_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_location_maps);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
int SQLite3Driver::location_map_count(Scene* scene) const //we currently only support a scene having one location, but this might change in the future
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_location_maps_in_scene);
    query.bindValue(":fk_scene", scene->id);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::location_maps()
{
  qDeleteAll(_location_maps);
  _location_maps.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_location_maps);
    query.exec();
    while (query.next()) {
      auto map = std::make_unique<LocationMap>();
      auto record = query.record();

      assign_location_map(record, *map);
      _location_maps.push_back(map.release());
    }
    _current_location_map = _location_maps.begin();
    emit locationMapsChanged();
  }
}
bool SQLite3Driver::next_location_map(LocationMap* map)
{
  if (_current_location_map == _location_maps.end() || _location_maps.empty()) {
    return false;
  }
  map->assign(*(*_current_location_map));
  ++_current_location_map;

  return true;
}
bool SQLite3Driver::select_location_map(LocationMap* map) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (map->id != -1) {
      query.prepare(sqlite3::select_location_map_by_id);
      query.bindValue(":id", map->id);
    } else if (map->fk_scene != -1) {
      query.prepare(sqlite3::select_location_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_location", map->fk_location);
    } else {
      qWarning() << "Provided Property has no id, fk_scene, or fk_location one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_location_map(record, *map);
        return true;
      }
    } else {
      qWarning() << "select_location_map" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_location_map(LocationMap* map)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (-1 != map->id) {
      query.prepare(sqlite3::update_location_map_by_id);
      query.bindValue(":id", map->id);
    } else {
      query.prepare(sqlite3::insert_or_update_location_maps);
    }
    query.bindValue(":fk_scene", map->fk_scene);
    query.bindValue(":fk_location", map->fk_location);
    if (!query.exec()) {
      qWarning() << "update_location_map" << query.lastError();
      return false;
    }
    if (-1 == map->id) {
      const auto r = select_location_map(map);
      locationMapUpdated(map->id);
      return r;
    }
    locationMapUpdated(map->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_location_map(LocationMap* map)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_location_map(map)) {
      query.prepare(sqlite3::delete_location_map_by_id);
      query.bindValue(":id", map->id);
      if (!query.exec()) {
        qWarning() << "remove_location_map" << query.lastError();
        return false;
      }
      locationMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_location_map_by_fk(LocationMap* map)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_location_map(map)) {
      query.prepare(sqlite3::delete_location_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_location", map->fk_location);
      if (!query.exec()) {
        qWarning() << "remove_location_map_by_fk" << query.lastError();
        return false;
      }
      locationMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------CITATION MAP-----------------------------------------
inline void assign_citation_map(const QSqlRecord& record, CitationMap& map)
{
  map.id = record.value(CITATION_MAP_ID).toInt();
  map.fk_scene = record.value(CITATION_MAP_FK_SCENE).toInt();
  map.fk_citation = record.value(CITATION_MAP_FK_CITATION).toInt();
}
int SQLite3Driver::citation_map_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_citation_maps);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::citation_maps()
{
  qDeleteAll(_citation_maps);
  _citation_maps.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_citation_maps);
    query.exec();
    while (query.next()) {
      auto map = std::make_unique<CitationMap>();
      auto record = query.record();

      assign_citation_map(record, *map);
      _citation_maps.push_back(map.release());
    }
    _current_citation_map = _citation_maps.begin();
    emit citationMapsChanged();
  }
}
bool SQLite3Driver::next_citation_map(CitationMap* map)
{
  if (_current_citation_map == _citation_maps.end() || _citation_maps.empty()) {
    return false;
  }
  map->assign(*(*_current_citation_map));
  ++_current_citation_map;

  return true;
}
bool SQLite3Driver::select_citation_map(CitationMap* map) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (map->id != -1) {
      query.prepare(sqlite3::select_citation_map_by_id);
      query.bindValue(":id", map->id);
    } else if (map->fk_scene != -1) {
      query.prepare(sqlite3::select_citation_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_citation", map->fk_citation);
    } else {
      qWarning() << "Provided Property has no id, fk_scene, or fk_citation one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_citation_map(record, *map);
        return true;
      }
    } else {
      qWarning() << "select_citation_map" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_citation_map(CitationMap* map)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (-1 != map->id) {
      query.prepare(sqlite3::update_citation_map_by_id);
      query.bindValue(":id", map->id);
    } else {
      query.prepare(sqlite3::insert_or_update_citation_maps);
    }
    query.bindValue(":fk_scene", map->fk_scene);
    query.bindValue(":fk_citation", map->fk_citation);
    if (!query.exec()) {
      qWarning() << "update_citation_map" << query.lastError();
      return false;
    }
    if (-1 == map->id) {
      const auto r = select_citation_map(map);
      citationMapUpdated(map->id);
      return r;
    }
    citationMapUpdated(map->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_citation_map(CitationMap* map)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_citation_map(map)) {
      query.prepare(sqlite3::delete_citation_map_by_id);
      query.bindValue(":id", map->id);
      if (!query.exec()) {
        qWarning() << "remove_citation_map" << query.lastError();
        return false;
      }
      citationMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_citation_map_by_fk(CitationMap* map)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_citation_map(map)) {
      query.prepare(sqlite3::delete_citation_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene);
      query.bindValue(":fk_citation", map->fk_citation);
      if (!query.exec()) {
        qWarning() << "remove_citation_map_by_fk" << query.lastError();
        return false;
      }
      citationMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//------------------------------EQUIPMENT MAP----------------------------------------
inline void assign_equipment_map(const QSqlRecord& record, EquipmentMap& map, Scene const* const scene, Equipment const* const equipment)
{
  map.id = record.value(EQUIPMENT_MAP_ID).toInt();
  emit map.idChanged(map.id);
  if (scene) {
    map.scene->assign(*scene);
  } else {
    map.scene->id = record.value(EQUIPMENT_MAP_FK_SCENE).toInt();
  }
  emit map.sceneChanged(map.scene);

  if (equipment) {
    map.equipment->assign(*equipment);

  } else {
    map.equipment->id = record.value(EQUIPMENT_MAP_FK_EQUIPMENT).toInt();
  }
  emit map.equipmentChanged(map.equipment);
  map.name = record.value(EQUIPMENT_MAP_NAME).toString();
  emit map.nameChanged(map.name);
  map.property_values = record.value(EQUIPMENT_MAP_VALUES).toString();
  emit map.valuesChanged(map.property_values);
  map.notes = record.value(EQUIPMENT_MAP_NOTES).toString();
  emit map.notesChanged(map.notes);
}

int SQLite3Driver::equipment_map_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_equipment_map);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
void SQLite3Driver::equipment_maps()
{
  qDeleteAll(_equipment_maps);
  _equipment_maps.clear();

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_equipment_map);
    query.exec();
    while (query.next()) {
      auto map = std::make_unique<EquipmentMap>();
      auto record = query.record();

      assign_equipment_map(record, *map);
      _equipment_maps.push_back(map.release());
    }
    _current_equipment_map = _equipment_maps.begin();
    emit equipmentMapsChanged();
  }
}
bool SQLite3Driver::next_equipment_map(EquipmentMap* map)
{
  if (_current_equipment_map == _equipment_maps.end() || _equipment_maps.empty()) {
    return false;
  }
  map->assign(**_current_equipment_map);
  ++_current_equipment_map;

  return true;
}

//!
//!  Select
//!
//!
bool SQLite3Driver::select_equipment_map(EquipmentMap* map) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (map->id != -1) {
      query.prepare(sqlite3::select_equipment_map_by_id);
      query.bindValue(":id", map->id);
    } else if (map->scene && map->equipment) {
      if (!select_scene(map->scene)) {
        qWarning() << QString("Unable to find scene based on given name %1").arg(map->scene->name);
        return false;
      }

      if (!select_equipment(map->equipment)) {
        qWarning() << QString("Unable to find equipment based on given name %1").arg(map->equipment->name);
      }

      if (map->name == "") {
        map->name = map->equipment->name;
        emit map->nameChanged(map->name);
      }

      query.prepare(sqlite3::select_equipment_from_scene_with_name);
      query.bindValue(":fk_scene", map->scene->id);
      query.bindValue(":fk_equipment", map->equipment->id);
      query.bindValue(":name", map->name);

    } else {
      qWarning() << "Provided Property has no id, fk_scene, or fk_equipment one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_equipment_map(record, *map);
        return true;
      }
    } else {
      qWarning() << "select_equipment_map" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_equipment_map(EquipmentMap* map)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    if (-1 != map->id) {
      query.prepare(sqlite3::update_equipment_map_by_id);
      query.bindValue(":id", map->id);
    } else {
      query.prepare(sqlite3::insert_or_update_equipment_map);
    }
    if (!select_scene(map->scene)) {
      qWarning() << QString("Unable to find Scene (%1,%2) in database").arg(map->scene->id).arg(map->scene->name);
      return false;
    }

    if (!select_equipment(map->equipment)) {
      qWarning() << QString("Unable to find Equipment (%1,%2) in database").arg(map->equipment->id).arg(map->equipment->name);
      return false;
    }

    if (map->name == "") {
      map->name = map->equipment->name;
    }
    query.bindValue(":fk_scene", map->scene->id);
    query.bindValue(":fk_equipment", map->equipment->id);
    query.bindValue(":name", map->name);
    query.bindValue(":values", map->property_values);
    query.bindValue(":notes", map->notes);
    if (!query.exec()) {
      qWarning() << "update_equipment_map" << query.lastError();
      return false;
    }
    if (-1 == map->id) {
      const auto r = select_equipment_map(map);
      equipmentMapUpdated(map->id);
      return r;
    }
    equipmentMapUpdated(map->id);
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_equipment_map(EquipmentMap* map)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_equipment_map(map)) {
      query.prepare(sqlite3::delete_equipment_map_by_id);
      query.bindValue(":id", map->id);
      if (!query.exec()) {
        qWarning() << "remove_equipment_map" << query.lastError();
        return false;
      }
      equipmentMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_equipment_map_by_fk(EquipmentMap* map)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_equipment_map(map)) {
      query.prepare(sqlite3::delete_an_equipment_from_a_scene);
      if (!map->scene) {
        qWarning() << "A valid Scene must be provided to remove equipment from it.";
        return false;
      } else if (map->scene->id == -1) {
        if (!select_scene(map->scene)) {
          qWarning() << QString("Unable to retrieve details for Scene %1").arg(map->scene->name);
          return false;
        }
        return false;
      }
      if (!map->equipment) {
        qWarning() << "A valid Equipment must be provided to remove occurrences from a scene.";
        return false;
      } else if (map->equipment->id == -1) {
        if (!select_equipment(map->equipment)) {
          qWarning() << QString("Unable to retrieve details for Equipment %1").arg(map->equipment->name);
          return false;
        }
        return false;
      }

      query.bindValue(":fk_scene", map->scene->id);
      query.bindValue(":fk_equipment", map->equipment->id);
      query.bindValue(":name", map->name);

      if (!query.exec()) {
        qWarning() << "remove_equipment_map_by_fk" << query.lastError();
        return false;
      }
      //!
      //! This function does not properly call equipmentMapremoved for removed maps
      //! It would be slower but we should likely call SELECT on fk
      //! Then iterate over results and call delete on each one to properly call this signal
      //! Or we can create a bulk delete signal

      equipmentMapRemoved(map->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}

//-----------------------------BACK DOORS FOR NOW----------------------------------
std::vector<std::unique_ptr<Author>> SQLite3Driver::get_authors() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<Author>> author_list;
    QSqlQuery author_query{ QSqlDatabase::database(_db_name) };
    author_query.prepare(select_all_authors);
    author_query.exec();
    while (author_query.next()) {
      auto temp = std::make_unique<Author>();
      auto record = author_query.record();
      assign_author(record, *temp);
      author_list.push_back(std::move(temp));
    }
    return author_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Assessment>> SQLite3Driver::get_assessments() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<Assessment>> assessment_list;
    QSqlQuery assessment_query{ QSqlDatabase::database(_db_name) };
    assessment_query.prepare(select_all_assessments);
    assessment_query.exec();
    while (assessment_query.next()) {
      auto temp = std::make_unique<Assessment>();
      auto record = assessment_query.record();
      assign_assessment(record, *temp);
      assessment_list.push_back(std::move(temp));
    }
    return assessment_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Citation>> SQLite3Driver::get_citations() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<Citation>> citation_list;
    QSqlQuery citation_query{ QSqlDatabase::database(_db_name) };
    citation_query.prepare(select_all_citations);
    citation_query.exec();
    while (citation_query.next()) {
      auto temp = std::make_unique<Citation>();
      auto record = citation_query.record();
      assign_citation(record, *temp);
      citation_list.push_back(std::move(temp));
    }
    return citation_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Event>> SQLite3Driver::get_events() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<Event>> event_list;
    QSqlQuery event_query{ QSqlDatabase::database(_db_name) };
    event_query.prepare(select_all_events);
    event_query.exec();
    while (event_query.next()) {
      auto temp = std::make_unique<Event>();
      auto record = event_query.record();
      assign_event(record, *temp);
      event_list.push_back(std::move(temp));
    }
    return event_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Event>> SQLite3Driver::get_events_in_scene(Scene const* const scene) const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<Event>> event_list;
    QSqlQuery event_query{ QSqlDatabase::database(_db_name) };
    event_query.prepare(select_events_in_scene);
    event_query.bindValue(":scene_id", scene->id);
    event_query.exec();
    while (event_query.next()) {
      auto temp = std::make_unique<Event>();
      auto record = event_query.record();
      assign_event(record, *temp);
      event_list.push_back(std::move(temp));
    }
    return event_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Equipment>> SQLite3Driver::get_equipments() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<Equipment>> equipment_list;
    QSqlQuery equipment_query{ QSqlDatabase::database(_db_name) };
    equipment_query.prepare(select_all_equipments);
    equipment_query.exec();
    while (equipment_query.next()) {
      auto temp = std::make_unique<Equipment>();
      auto record = equipment_query.record();
      assign_equipment(record, *temp);
      equipment_list.push_back(std::move(temp));
    }
    return equipment_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<EquipmentMap>> SQLite3Driver::get_equipment_in_scene(Scene const* const scene) const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<EquipmentMap>> equipment_list;
    QSqlQuery scene_query{ QSqlDatabase::database(_db_name) };
    scene_query.prepare(select_equipment_in_scene);
    scene_query.bindValue(":scene_id", scene->id);
    scene_query.exec();
    while (scene_query.next()) {
      auto temp = std::make_unique<EquipmentMap>();
      auto record = scene_query.record();
      assign_equipment_map(record, *temp, scene);
      select_equipment(temp->equipment);
      equipment_list.push_back(std::move(temp));
    }
    return equipment_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Injury>> SQLite3Driver::get_injuries() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<Injury>> injury_list;
    QSqlQuery injury_query{ QSqlDatabase::database(_db_name) };
    injury_query.prepare(select_all_injuries);
    injury_query.exec();
    while (injury_query.next()) {
      auto temp = std::make_unique<Injury>();
      auto record = injury_query.record();
      assign_injury(record, *temp);
      injury_list.push_back(std::move(temp));
    }
    return injury_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<InjurySet>> SQLite3Driver::get_injury_sets() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<InjurySet>> injury_set_list;
    QSqlQuery injury_set_query{ QSqlDatabase::database(_db_name) };
    injury_set_query.prepare(select_all_injury_sets);
    injury_set_query.exec();
    while (injury_set_query.next()) {
      auto temp = std::make_unique<InjurySet>();
      auto record = injury_set_query.record();
      assign_injury_set(record, *temp);
      injury_set_list.push_back(std::move(temp));
    }
    return injury_set_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<RoleMap>> SQLite3Driver::get_role_maps() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<RoleMap>> role_map_list;
    QSqlQuery role_map_query{ QSqlDatabase::database(_db_name) };
    role_map_query.prepare(select_all_role_maps);
    role_map_query.exec();
    while (role_map_query.next()) {
      auto temp = std::make_unique<RoleMap>();
      auto record = role_map_query.record();
      assign_role_map(record, *temp);
      role_map_list.push_back(std::move(temp));
    }
    return role_map_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<EventMap>> SQLite3Driver::get_event_maps() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<EventMap>> event_map_list;
    QSqlQuery event_map_query{ QSqlDatabase::database(_db_name) };
    event_map_query.prepare(select_all_event_maps);
    event_map_query.exec();
    while (event_map_query.next()) {
      auto temp = std::make_unique<EventMap>();
      auto record = event_map_query.record();
      assign_event_map(record, *temp);
      event_map_list.push_back(std::move(temp));
    }
    return event_map_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<LocationMap>> SQLite3Driver::get_location_maps() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<LocationMap>> location_map_list;
    QSqlQuery location_map_query{ QSqlDatabase::database(_db_name) };
    location_map_query.prepare(select_all_location_maps);
    location_map_query.exec();
    while (location_map_query.next()) {
      auto temp = std::make_unique<LocationMap>();
      auto record = location_map_query.record();
      assign_location_map(record, *temp);
      location_map_list.push_back(std::move(temp));
    }
    return location_map_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<CitationMap>> SQLite3Driver::get_citation_maps() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<CitationMap>> citation_map_list;
    QSqlQuery citation_map_query{ QSqlDatabase::database(_db_name) };
    citation_map_query.prepare(select_all_citation_maps);
    citation_map_query.exec();
    while (citation_map_query.next()) {
      auto temp = std::make_unique<CitationMap>();
      auto record = citation_map_query.record();
      assign_citation_map(record, *temp);
      citation_map_list.push_back(std::move(temp));
    }
    return citation_map_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<EquipmentMap>> SQLite3Driver::get_equipment_maps() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<EquipmentMap>> equipment_map_list;
    QSqlQuery equipment_map_query{ QSqlDatabase::database(_db_name) };
    equipment_map_query.prepare(select_all_equipment_map);
    equipment_map_query.exec();
    while (equipment_map_query.next()) {
      auto temp = std::make_unique<EquipmentMap>();
      auto record = equipment_map_query.record();
      assign_equipment_map(record, *temp);
      equipment_map_list.push_back(std::move(temp));
    }
    return equipment_map_list;
  }
  throw std::runtime_error("No db connection");
}

std::vector<std::unique_ptr<Objective>> SQLite3Driver::get_objectives() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<Objective>> objective_list;
    QSqlQuery objective_query{ QSqlDatabase::database(_db_name) };
    objective_query.prepare(select_all_objectives);
    objective_query.exec();
    while (objective_query.next()) {
      auto temp = std::make_unique<Objective>();
      auto record = objective_query.record();
      assign_objective(record, *temp);
      objective_list.push_back(std::move(temp));
    }
    return objective_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Location>> SQLite3Driver::get_locations() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<Location>> location_list;
    QSqlQuery location_query{ QSqlDatabase::database(_db_name) };
    location_query.prepare(select_all_locations);
    location_query.exec();
    while (location_query.next()) {
      auto temp = std::make_unique<Location>();
      auto record = location_query.record();
      assign_location(record, *temp);
      location_list.push_back(std::move(temp));
    }
    return location_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Location>> SQLite3Driver::get_locations_in_scene(Scene* scene) const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<Location>> location_list;
    std::vector<int32_t> fk_locations;
    QSqlQuery map_query{ QSqlDatabase::database(_db_name) };
    map_query.prepare(sqlite3::select_location_map_by_fk_scene);
    map_query.bindValue(":fk_scene", scene->id);
    map_query.exec();
    while (map_query.next()) {
      auto map = std::make_unique<LocationMap>();
      auto map_record = map_query.record();
      assign_location_map(map_record, *map);
      fk_locations.push_back(map->fk_location);
    }
    QSqlQuery query{ QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_location_by_id);
    for (auto& id : fk_locations) {
      query.bindValue(":id", id);
      if (query.next()) {
        auto temp = std::make_unique<Location>();
        auto record = query.record();
        assign_location(record, *temp);
        location_list.push_back(std::move(temp));
      }
    }
    return location_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Property>> SQLite3Driver::get_properties() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<Property>> property_list;
    QSqlQuery property_query{ QSqlDatabase::database(_db_name) };
    property_query.prepare(select_all_properties);
    property_query.exec();
    while (property_query.next()) {
      auto temp = std::make_unique<Property>();
      auto record = property_query.record();
      assign_property(record, *temp);
      property_list.push_back(std::move(temp));
    }
    return property_list;
  }
  throw std::runtime_error("No db connection");
}

std::vector<std::unique_ptr<Role>> SQLite3Driver::get_roles() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<Role>> role_list;
    QSqlQuery role_query{ QSqlDatabase::database(_db_name) };
    role_query.prepare(select_all_roles);
    role_query.exec();
    while (role_query.next()) {
      auto temp = std::make_unique<Role>();
      auto record = role_query.record();
      assign_role(record, *temp);
      role_list.push_back(std::move(temp));
    }
    return role_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Role>> SQLite3Driver::get_roles_in_scene(Scene* scene) const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<Role>> role_list;
    QSqlQuery role_query{ QSqlDatabase::database(_db_name) };
    role_query.prepare(sqlite3::select_roles_in_scene);
    role_query.bindValue(":scene_id", scene->id);
    role_query.exec();
    while (role_query.next()) {
      auto temp = std::make_unique<Role>();
      auto record = role_query.record();
      assign_role(record, *temp);
      role_list.push_back(std::move(temp));
    }
    return role_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Treatment>> SQLite3Driver::get_treatments() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<Treatment>> treatment_list;
    QSqlQuery treatment_query{ QSqlDatabase::database(_db_name) };
    treatment_query.prepare(select_all_treatments);
    treatment_query.exec();
    while (treatment_query.next()) {
      auto temp = std::make_unique<Treatment>();
      auto record = treatment_query.record();
      assign_treatment(record, *temp);
      treatment_list.push_back(std::move(temp));
    }
    return treatment_list;
  }
  throw std::runtime_error("No db connection");
}
std::vector<std::unique_ptr<Scene>> SQLite3Driver::get_scenes() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    std::vector<std::unique_ptr<Scene>> scene_list;
    QSqlQuery scene_query{ QSqlDatabase::database(_db_name) };
    scene_query.prepare(select_all_scenes);
    scene_query.exec();
    while (scene_query.next()) {
      auto temp = std::make_unique<Scene>();
      auto record = scene_query.record();
      assign_scene(record, *temp);
      scene_list.push_back(std::move(temp));
    }
    return scene_list;
  }
  throw std::runtime_error("No db connection");
}
//----------------------------------------------------------------------------
bool SQLite3Driver::remove_equipment_from_treatments(int equipment_id)
{
  return remove_equipment_from_treatments(std::to_string(equipment_id));
}
bool SQLite3Driver::remove_equipment_from_treatments(std::string equipment_id)
{
  auto treatments = get_treatments();
  if (QSqlDatabase::database(_db_name).isOpen()) {
    for (auto& treatment : treatments) {
      std::string equipment = list_remove(treatment->equipment.toStdString(), equipment_id);
      QSqlQuery query{ QSqlDatabase::database(_db_name) };
      query.prepare(update_treatment_by_id);
      query.bindValue(":id", treatment->id);
      query.bindValue(":citations", treatment->citations);
      query.bindValue(":equipment", equipment.c_str());
      query.bindValue(":medical_name", treatment->medical_name);
      query.bindValue(":common_name", treatment->common_name);
      query.bindValue(":description", treatment->description);
      if (!query.exec()) {
        qWarning() << "remove_equipment_from_treatment" << query.lastError();
        return false;
      }
    }
    return true;
  }
  return false;
}
bool SQLite3Driver::remove_citation_from_treatments(int citation_id)
{
  return remove_citation_from_treatments(std::to_string(citation_id));
}
bool SQLite3Driver::remove_citation_from_treatments(std::string citation_id)
{
  auto treatments = get_treatments();
  if (QSqlDatabase::database(_db_name).isOpen()) {
    for (auto& treatment : treatments) {
      std::string citations = list_remove(treatment->citations.toStdString(), citation_id);
      QSqlQuery query{ QSqlDatabase::database(_db_name) };
      query.prepare(update_treatment_by_id);
      query.bindValue(":id", treatment->id);
      query.bindValue(":citations", citations.c_str());
      query.bindValue(":equipment", treatment->equipment);
      query.bindValue(":medical_name", treatment->medical_name);
      query.bindValue(":common_name", treatment->common_name);
      query.bindValue(":description", treatment->description);
      if (!query.exec()) {
        qWarning() << "remove_citation_from_treatments" << query.lastError();
        return false;
      }
    }
    return true;
  }
  return false;
}
bool SQLite3Driver::remove_citation_from_injuries(int citation_id)
{
  return remove_citation_from_injuries(std::to_string(citation_id));
}
bool SQLite3Driver::remove_citation_from_injuries(std::string citation_id)
{
  auto injuries = get_injuries();
  if (QSqlDatabase::database(_db_name).isOpen()) {
    for (auto& injury : injuries) {
      std::string citations = list_remove(injury->citations.toStdString(), citation_id);
      QSqlQuery query{ QSqlDatabase::database(_db_name) };
      query.prepare(update_injury_by_id);
      query.bindValue(":id", injury->id);
      query.bindValue(":citations", citations.c_str());
      query.bindValue(":medical_name", injury->medical_name);
      query.bindValue(":common_name", injury->common_name);
      query.bindValue(":description", injury->description);
      query.bindValue(":min", injury->lower_bound);
      query.bindValue(":max", injury->upper_bound);
      if (!query.exec()) {
        qWarning() << "remove_citation_from_injuries" << query.lastError();
        return false;
      }
    }
    return true;
  }
  return false;
}
bool SQLite3Driver::remove_citation_from_equipment(int citation_id)
{
  return remove_citation_from_equipment(std::to_string(citation_id));
}
bool SQLite3Driver::remove_citation_from_equipment(std::string citation_id)
{
  auto equipments = get_equipments();
  if (QSqlDatabase::database(_db_name).isOpen()) {
    for (auto& equipment : equipments) {
      std::string citations = list_remove(equipment->citations.toStdString(), citation_id);
      QSqlQuery query{ QSqlDatabase::database(_db_name) };
      query.prepare(update_equipment_by_id);
      query.bindValue(":id", equipment->id);
      query.bindValue(":name", equipment->name);
      query.bindValue(":type", equipment->type);
      query.bindValue(":description", equipment->description);
      query.bindValue(":citations", citations.c_str());
      query.bindValue(":image", equipment->image);
      if (!query.exec()) {
        qWarning() << "remove_citation_from_equipment" << query.lastError();
        return false;
      }
    }
    return true;
  }
  return false;
}
bool SQLite3Driver::remove_citation_from_objectives(int citation_id)
{
  return remove_citation_from_objectives(std::to_string(citation_id));
}
bool SQLite3Driver::remove_citation_from_objectives(std::string citation_id)
{
  auto objectives = get_objectives();
  if (QSqlDatabase::database(_db_name).isOpen()) {
    for (auto& objective : objectives) {
      std::string citations = list_remove(objective->citations.toStdString(), citation_id);
      QSqlQuery query{ QSqlDatabase::database(_db_name) };
      query.prepare(update_objective_by_id);
      query.bindValue(":id", objective->id);
      query.bindValue(":name", objective->name);
      query.bindValue(":description", objective->description);
      query.bindValue(":citations", citations.c_str());
      if (!query.exec()) {
        qWarning() << "remove_citation_from_objectives" << query.lastError();
        return false;
      }
    }
    return true;
  }
  return false;
}
bool SQLite3Driver::remove_injury_from_injury_sets(int injury_id)
{
  return remove_injury_from_injury_sets(std::to_string(injury_id));
}
bool SQLite3Driver::remove_injury_from_injury_sets(std::string injury_id)
{
  auto injury_sets = get_injury_sets();
  if (QSqlDatabase::database(_db_name).isOpen()) {
    for (auto& injury_set : injury_sets) {
      std::string descriptions = list_remove_index(injury_set->description.toStdString(), list_find(injury_set->injuries.toStdString(), injury_id));
      std::string severities = list_remove_index(injury_set->severities.toStdString(), list_find(injury_set->injuries.toStdString(), injury_id));
      std::string locations = list_remove_index(injury_set->locations.toStdString(), list_find(injury_set->injuries.toStdString(), injury_id));
      std::string injuries = list_remove(injury_set->injuries.toStdString(), injury_id);
      QSqlQuery query{ QSqlDatabase::database(_db_name) };
      query.prepare(update_injury_set_by_id);
      query.bindValue(":id", injury_set->id);
      query.bindValue(":name", injury_set->name);
      query.bindValue(":description", descriptions.c_str());
      query.bindValue(":locations", locations.c_str());
      query.bindValue(":severities", severities.c_str());
      query.bindValue(":injuries", injuries.c_str());
      //query.bindValue(":physiology_file", injury_set->physiology_file);
      //query.bindValue(":treatment", injury_set->treatment);
      if (!query.exec()) {
        qWarning() << "remove_injury_from_injury_sets" << query.lastError();
        return false;
      }
    }
    return true;
  }
  return false;
}

std::string SQLite3Driver::list_remove(std::string list, std::string id) const
{
  std::string result;
  std::string temp_id;
  for (int i = 0; i < list.length(); ++i) {
    if (list[i] != ';') {
      temp_id += list[i];
    } else {
      if (temp_id != id && !temp_id.empty()) {
        result += (temp_id + ";");
      }
      temp_id.clear();
    }
  }
  if (temp_id != id) {
    result += temp_id;
  }
  if (!result.empty() && result[result.length() - 1] == ';') {
    result.pop_back();
  }
  return result;
}

std::string SQLite3Driver::list_remove_index(std::string list, int index) const
{
  std::vector<std::string> tokenized_list;
  std::string temp;
  int current_index = 0;
  for (int i = 0; i <= list.length(); ++i) {
    if (i != list.length() && list[i] != ';') {
      temp += list[i];
    } else {
      if (current_index != index) {
        tokenized_list.push_back(temp);
      }
      ++current_index;
      temp.clear();
    }
  }
  std::string result;
  for (int i = 0; i < tokenized_list.size(); ++i) {
    result += (tokenized_list[i] + ';');
  }
  if (!result.empty()) {
    result.pop_back();
  }
  return result;
}

int SQLite3Driver::list_find(std::string list, std::string id) const
{
  int result = 0;
  std::string temp_id;
  for (int i = 0; i <= list.length(); ++i) {
    if (i != list.length() && list[i] != ';') {
      temp_id += list[i];
    } else {
      if (temp_id == id) {
        return result;
      }
      ++result;
      temp_id.clear();
    }
  }
  return -1;
}
}
