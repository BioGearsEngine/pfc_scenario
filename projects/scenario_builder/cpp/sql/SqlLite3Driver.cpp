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

#include "SQLTables.h"

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

//----------------------------------------------------------------------------
std::vector<std::string> string_split(const std::string& src, const std::string& delimiter)
{
  std::vector<std::string> pieces;
  size_t i = 0;
  while (i < src.size()) {
    const auto n = std::min(src.size(), src.find(delimiter, i) - i);
    pieces.emplace_back(src, i, n);
    i += n + 1;
  }
  return pieces;
}

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
//------------------------------------------------  ------------------------------
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
    { tables[IMAGES], sqlite3::create_images_table},
    { tables[TRAUMAS], sqlite3::create_traumas_table },
    { tables[TRAUMA_PROFILES], sqlite3::create_trauma_profiles_table },
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
  Trauma default_trauma;
  TraumaProfile default_trauma_profile;
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
    default_event.details = "";
    if (!update_event(&default_event)) {
      return false;
    }
  }
  //---Equipment---
  if (!populate_equipment()) {
    return false;
  }
  //---Trauma---
  if (!populate_traumas()) {
    return false;
  }
  //---TraumaProfile---
  if (trauma_profile_count() == 0) {
    default_trauma_profile.name = "Trauma Profile 1";
    default_trauma_profile.description = "Simple Multitrauma Profile";
    if (!update_trauma_profile(&default_trauma_profile)) {
      return false;
    }
  }
  //---Objective---
  if (objective_count() == 0) {
    default_objective.name = "Objective_1";
    default_objective.description = "Description of Objective_1";
    if (!update_objective(&default_objective)) {
      return false;
    }
  }
  //---Role---
  if (role_count() == 0) {
    default_role.name = "Role_1";
    default_role.description = "Description of Role_1";
    default_role.trauma_profile->assign(default_trauma_profile);
    if (!update_role(&default_role)) {
      return false;
    }
  }
  // //---Locations---
  // if (location_count() == 0) {
  //   default_location.name = "Location_1";
  //   default_location.description = "Description of Location_1";
  //   default_location.environment = "External:Tesstrial:Dessert";
  //   if (!update_location(&default_location)) {
  //     return false;
  //   }
  // }
  //---Scene---
  if (scene_count() == 0) {
    default_scene.name = "Scene_1";
    default_scene.description = "Description of Scene_1";
    default_scene.time_of_day = 0;
    default_scene.time_in_simulation = 0;
    default_scene.weather = "";
    if (!update_scene(&default_scene)) {
      return false;
    }
    if (!update_event_in_scene(&default_scene, &default_event)) {
      return false;
    }
    if (!update_role_in_scene(&default_scene, &default_role)) {
      return false;
    }
    // if (!update_location_in_scene(&default_scene, &default_location)) {
    //   return false;
    // }
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
    { "Lawnick2013Combat", "Combat Trauma Coding: A Review and Reconfiguration", "Mary M Lawnick, Howard R Champion, Thomas Gennarelli, Michael R Galarneau, Edwin D'Souza, Ross R Vickers, Vern Wing, Brian J Eastridge, Lee Ann Young, Judy Dye, and others", "2013", "Journal of Trauma and Acute Care" }, //hemorrhage paper
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
    QString name;
    int type;
    // Type Key
    // 1 = Equipment
    // 2 = Attachment
    // 3 = Substance
    // 4 = Consumable Item
    QString summary;
    QString description;
    QString citations;
    QString parameters;
  };

  std::vector<sEquipment> default_equipment = {
    { "Antibiotics", 3, "Antimicrobial substance active against bacteria, used in the treatment and prevention of bacterial infections.", "This medication usually comes in the form of a pill. There are different types of antibiotics for different applications.", "2;5;6;7", "Available:BOOLEAN;Volume:INTEGER" },
    { "Blanket", 2, "Low-weight, low-bulk blanket made of heat-reflective thin plastic sheeting.", "Also referred to as mylar, these are commonly used to prevent heat loss from a human body.", "2;5;6;7", "Available:BOOLEAN" },
    { "Blood Bag", 4, "A Unit of collected human blood.", "Bags are usually 500 mL in volume (referred to as one unit). Blood type is dependent on antigen presence (A/B) or absence (O) in addition to the presence or absence of Rh factor (+/-).", "2;5;6;7", "Available:BOOLEAN;Count:INTEGER;Kind:ENUM,APos,ANeg,BPos,BNeg,ABPos,ABNeg,OPos,ONeg" },
    { "Blood Collection Bag", 4, "Clear plastic bag used to collect and store blood.", "Bags are usually 500 mL in volume (referred to as one unit). Blood type is dependent on antigen presence (A/B) or absence (O) in addition to the presence or absence of Rh factor (+/-).", "2;5;6;7", "Available:BOOLEAN;Count:INTEGER;" },
    { "Blood Transfusion Kit", 1, "Kit used for in field blood transfusion.", "Designed for field application, a transfusion kit contains supplies necessary to collect and transfuse fresh whole blood (FWB). The kit typically contains everything from safety supplies and equipment (gloves, blood type test kits, and swabsticks) to collection materials (tubing, needles, collection bag, etc.). Additionally, two sets of most safety materials are supplied to prevent multiple uses on both donor and recipient (i.e. two needles).", "2;5;6;7", "Available:BOOLEAN" },
    { "Burn Ointments", 4, "Neosporin Burn Relief & First-Aid Antibiotic Ointment, .5 OZ", "NEOSPORIN + Burn Relief Dual Action Ointment is an antibiotic ointment that provides infection protection and helps soothe minor burn pain. Formulated for first aid wound treatment, it contains bacitracin zinc, neomycin sulfate, and polymyxin B sulfate for antibiotic care of minor burns and wounds. The topical analgesic ointment is also formulated with pramoxine hydrochloride to help soothe and reduce burn pain for maximum-strength relief. From the #1 doctor-recommended brand, this antibiotic and pain relief ointment provides maximum strength relief without any sting forburn treatment, including cooking burns. Neosporin + Burn Relief Dual Action Ointment is a wound care essential to include in any burn care first-aid kit. ", "2;5;6;7", "Available:BOOLEAN;Count:INTEGER" },
    { "Catheter Supplies", 4, "Catheters and the products that are used to insert or remove a catheter, and maintain catheter function.", "", "2;5;6;7", "Available:BOOLEAN;Count:INTEGER" },
    { "Chest Tube", 4, "A chest tube is a hollow, flexible tube placed into the chest.", "Acts as a drain. Chest tubes drain blood, fluid, or air from around your lungs, heart, or esophagus. The tube around your lung is placed between your ribs and into the space between the inner lining and the outer lining of your chest cavity", "2;5;6;7", "Available:BOOLEAN;Count:INTEGER" },
    { "Decompression Needle", 2, "The physical needle used for treating tension pneumothorax", "Typically a 14 gauge by 3.25 inch needle, this needle is a component of the needle decompression kit and is considered an attachment. This needle is used to reestablish the pleural space. ", "2;5;6;7", "Available:BOOLEAN;Count:INTEGER" },
    { "Energy Gel", 4, "Carbohydrate gel used to provide energy and promote recovery.", "", "2;5;6;7", "Count:INTEGER" },
    { "Epinephrine", 3, "A chemical that narrows blood vessels and opens airways in the lungs.", "Also referred to as adrenaline, this drug is used to treat life-threatening allergic reactions. Epinephrine acts quickly and works to improve breathing, stimulate the heart, raise a dropping blood pressure, reverse hives, and reduce swelling.", "2;5;6;7", "Available:BOOLEAN;Volume:INTEGER" },
    { "Fentanyl", 3, "A synthetic opioid pain reliever, approved for treating severe pain.", "Fentanyl is a prescription drug that is also made and used illegally. Like morphine, it is a medicine that is typically used to treat patients with severe pain, especially after surgery. It is also sometimes used to treat patients with chronic pain who are physically tolerant to other opioids.", "2;5;6;7", "Available:BOOLEAN;Volume:INTEGER" },
    { "IV Pole", 1, "The pole use to hang IV bags on.", "It provides patient mobility to move around a facility while keeping IV lines in.", "2;5;6;7", "Available:BOOLEAN" },
    { "IV Bag", 4, "Bag used to store and deliver fluids to a patient.", "", "2;5;6;7", "Available:BOOLEAN;Count:INTEGER;Volume:INTEGER;Kind:ENUM,Saline,Lactated Ringers" },
    { "Ketamine", 3, "A dissociative anesthetic used pain relief.", "Ketamine can provide pain relief and short-term memory loss (for example, amnesia of a medical procedure). In surgery, it is used an induction and maintenance agent for sedation and to provide general anesthesia. It has also been used for pain control in burn therapy, battlefield traumas, and in children who cannot use other anesthetics due to side effects or allergies. At normal doses, it is often preferred as an anesthetic in patients at risk of bronchospasm and respiratory depression.", "2;5;6;7", "Available:BOOLEAN;Volume:INTEGER" },
    { "Midazolam", 3, "A short action sedative used for anesthesia, procedural sedation, trouble sleeping, and severe agitation.", "Midazolam injection is used before medical procedures and surgery to cause drowsiness, relieve anxiety, and prevent any memory of the event. It is also sometimes given as part of the anesthesia during surgery to produce a loss of consciousness.", "2;5;6;7", "Available:BOOLEAN;Volume:INTEGER" },
    { "Morphine", 3, "A narcotic pain reliever used to treat moderate to severe pain.", "Morphine is an opioid medication used to treat severe chronic pain. It has a high rate of addiction, overdose, and death.", "2;5;6;7", "Available:BOOLEAN;Volume:INTEGER" },
    { "Narcan", 3, "Naloxone, a medication used to block the effects of opioids, commonly used for decreased breathing in opioid overdose.", "Naloxone is a medicine that rapidly reverses an opioid overdose. It attaches to opioid receptors and reverses and blocks the effects of other opioids. Naloxone is a safe medicine. It only reverses overdoses in people with opioids in their systems.", "2;5;6;7", "Available:BOOLEAN;Volume:INTEGER" },
    { "Nasal Cannula", 2, "A device used to deliver increased airflow to a patient in need of respiratory help.", "Tubing with two prongs that get inserted into a patient's nose, this equipment is attached to an oxygen source and mainly used to control oxygen flow to the patient. The advantages to this delivery method is that is less invasive than other oxygen masks and allows a patient to eat and speak normally.", "2;5;6;7", "Available:BOOLEAN" },
    { "Needle Decompression Kits", 4, "14 guage by 3.25 inch needle and catheter", "This is a 14 gauge by 3.25 inch needle and catheter for use in the management of combat casualties who present with the signs and symptoms of a tension pneumothorax. This device meets 100% of the requirements for fit, form and function that is listed for NSN 6515-01-541-0635. The hard plastic case protects the needle and catheter from bending or damage and harsh environmental conditions while in your kit. ", "2;5;6;7", "Available:BOOLEAN;Count:INTEGER" },
    { "Oxygen Tank", 1, "A tank of oxygen.", "In medicine, an oxygen tank provide controlled and therapeutic oxygen delivery to a patient.", "2;5;6;7", "Available:BOOLEAN;Volume:INTEGER" },
    { "Peripheral IV", 2, "A peripheral intravenous line is a small, short plastic catheter that is placed through the skin into a vein.", "A peripheral intravenous line is used to give fluids and/or medications directly into the blood stream.", "2;5;6;7", "Available:BOOLEAN" },
    { "Splint", 2, "a rigid or flexible device that maintains in position a displaced or injured part.", "Mostly used for broken bones, a splint is fastened along a injured length of a person's body in order to limit movement and further trauma.", "2;5;6;7", "Available:BOOLEAN" },
    { "Syringe", 4, "Simple reciprocating pump using plunger or piston to expel liquid or gas through a hypodermic needle.", "In medicine, these can be used to withdraw liquids, such as when cleaning wounds or body cavities, or to inject fluids, as is the case with some drug injections.", "2;5;6;7", "Available:BOOLEAN;Count:INTEGER" },
    { "Tempus Pro Kit", 2, "Attatchment kit for the vitals sign monitor.", "Light enough to carry and small enough to hold in one hand, it allows the vitals monitor to easily be deployed in numerous clinical scenarios. While it does support many attachments to allow for a wide range of measurements, some important parameters the tempus pro kit measures includes pulse rate, impedance respiration, contact temperature, pulse oximetry, and noninvasive blood pressure.", "2;5;6;7", "Available:BOOLEAN" },
    { "Tourniquet", 2, "A device which applies pressure to a limb or extremity in order to control the flow of blood.", "A tourniquet uses compression, typically on a limb, to constrict a vein or artery and limit blood flow as much as possible. This is typically done during hemorrhaging when limiting blood flow to the extremity also limits the blood lost. In a pinch, a cord or tie can be used.", "2;5;6;7", "Available:BOOLEAN" },
    { "Water", 4, "An inorganic, transparent, tasteless, odorless, and nearly colorless chemical substance.", "It is also essential to and the primary component of living organic beings.", "2;5;6;7", "Volume:INTEGER" },
    { "Vitals Monitor", 1, "A small, lightweight vital signs monitor.", "A basic level vitals monitor with many uses depending on available attacthments.", "2;5;6;7", "Available:BOOLEAN" },
    { "Urine Foley", 4, "Catheter used to drain urine.", "A soft plastic or rubber tube that is inserted through the urethra into the bladder. These are typically used when a patient has difficulties urinating on their own.", "2;5;6;7", "Available:BOOLEAN" },
    { "Urine Bottle", 4, "Bag used to catch and store drained urine.", "", "2;5;6;7", "Available:BOOLEAN" },
    { "Wound Pack", 2, "Non-adherent and absorbent material used to control bleeding of an open wound.", "Wound fillers, such as non-adherent gauze, pads, ointments, sponges, and other materials designed to manage exudate.", "2;5;6;7", "Available:BOOLEAN" },
    { "Wound Wrap", 2, "A strip of fabric used especially to cover, dress, and bind up wounds.", "It can also be used to provide pressure to the bleeding.", "2;5;6;7", "Available:BOOLEAN" }
  };

  Equipment temp;
  Citation* citation;

  for (auto& equipmentDef : default_equipment) {
    temp.clear();
    temp.name = equipmentDef.name;
    temp.type = equipmentDef.type;
    temp.summary = equipmentDef.summary;
    temp.description = equipmentDef.description;

    for (auto citation_id : equipmentDef.citations.split(";")) {
      if (!citation_id.isEmpty()) {
        citation = new Citation(&temp);
        citation->id = citation_id.toInt();
        if (select_citation(citation)) {
          temp.citations.push_back(citation);
        }
      }
    }

    temp.parametersFromString(equipmentDef.parameters);

    if (!select_equipment(&temp)) {
      update_equipment(&temp);
    }
  }
  return true;
}
//------------------------------------------------------------------------------
bool SQLite3Driver::populate_traumas()
{
  struct sTrauma {
    std::string medical_name;
    std::string common_name;
    std::string description;
    std::string citations;
    double lower_bound;
    double upper_bound;
  };

  std::vector<sTrauma> default_traumas = {
    { "Contusion", "Bruising", "Bleeding under the skin due to trauma to the blood capillaries can cause injured tissues. It usually darkens the skin to a black/blue color.", "1;2;3", 0.0, 225 }, // linked to hemorrhage in mL/min
    { "Laceration", "External Bleeding", "External bleeding of the body produced by the tearing of soft body tissue.", "1;2;3", 0.0, 225 }, // linked to hemorrhage in mL/min
    { "Puncture", "Puncture Wound", "An trauma to the body cause by a piercing or penetrating object. Depth and severity can vary.", "1;2;3", 0.0, 225 }, // linked to hemorrhage in mL/min
    { "Airway Trauma", "Trauma to airway", "Airway traumas are life threatening and often occur as a result of blunt and penetrating force to the neck and/or chest. Diagnosis can be challenging due to symptoms and signs that are nonspecific to this trauma type.", "2;3;4", 0.0, 1.0 }, //airway obstruction in BG (severity, unitless)
    { "Infection", "Infection", "The invasion and growth of bacteria, viruses, yeast, or fungi in the body. Infections can begin at any point in the body and spread, causing fever and other health issues.", "2;3", 0.0, 0.0 }, //Infection (numerous inputs)
    { "First Degree Burn", "Superficial Burn", "A heat trauma that affects the first layer of skin. Most commonly, it causes redness, pain, and swelling and will heal on its own, though larger burns can be uncomfortable and may require a doctor's opinion.", "2;3", 0.0, 0.33 }, //Burn Wound, first third (% surface area)
    { "Second Degree Burn", "Partial Thickness Burn", "A burn that affects the epidermis and dermis (lower layers of skin). In addition to the symptoms of a first degree, a second degree burn can also cause blistering.", "2;3", 0.34, 0.66 }, //Burn Wound, second third (% surface area)
    { "Third Degree Burn", "Full Thickness Burn", "A burn that goes through the dermis and affects deep skin tissue layers. The result is a white or blackened, charred skin that may lose some feeling.", "2;3", 0.67, 1.0 }, //Burn Wound, last third (% surface area)
    { "Hemorrhage", "Bleeding", "Hemorrhaging is the release of blood from any blood vessel, either inside or outside of the body. When discussing hemorrhage, it is important to identify both severity (typically as a measure of blood loss rate) and location.", "1;2;3", 0.0, 225 }, //Hemorrhage in mL/min (150 considered massive blood loss, increase by 50% for extreme condition)
    { "Tension Pneumothorax", "Collapsed Lung", "A tension pneumothorax is when air leaks into the pleural space between the lungs and chest wall. It typically occurs as a result of blunt or penetrating trauma and can be classified as either open or closed.", "2;3", 0.0, 1.0 } //Tension Pneumothorax is a
  };

  Trauma temp;
  Citation* citation;

  for (auto& traumaDef : default_traumas) {
    temp.clear();
    temp.medical_name = traumaDef.medical_name.c_str();
    temp.common_name = traumaDef.common_name.c_str();
    temp.description = traumaDef.description.c_str();

    for (auto citation_id : string_split(traumaDef.citations, ";")) {
      if (!citation_id.empty()) {
        citation = new Citation(&temp);
        citation->id = std::stoi(citation_id);
        if (select_citation(citation)) {
          temp.citations.push_back(citation);
        }
      }
    }

    temp.lower_bound = (100 * traumaDef.lower_bound); //multiply by 100 so decimals do not get destroyed by int on qml side
    temp.upper_bound = (100 * traumaDef.upper_bound); //multiply by 100 so decimals do not get destroyed by int on qml side
    if (!select_trauma(&temp)) {
      update_trauma(&temp);
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
    std::string equipment;
    std::string citations;
  };

  std::vector<sTreatment> default_treatments = {
    { "Chest Tube", "Chest Tube",
      "A chest tube is a hollow, flexible tube placed into the chest."
      " It acts as a drain for blood, fluid, or air from around the heart and lungs. ",
      "26", "2;5;6;7" },
    { "Needle Thoracostomy", "Needle Decompression",
      "A procedure performed to stabilize deteriorating patients in the life-threatening situation of a tension pneumothorax."
      " The procedure involves inserting a large needle through the chest wall into the pleural cavity to allow air to escape. ",
      "27", "2;5;6;7" },
    { "Burn Ointment", "Burn Ointment",
      "Used to help in the prevention and treatment of infections during burn healing, burn ointment kills infectious microorganisms and reduces inflammation.",
      "28", "2;5;6;7" },
    { "Bandage", "Bandage/Wrap",
      "A strip of fabric used especially to cover, dress, and bind up wounds.",
      "22", "2;5;6;7" },
    { "Suture", "Stitches",
      "A stitch or row of stitches holding together the edges of a wound or incision. They typically help hold tissue together during healing.",
      "17;22", "2;5;6;7" },
    { "Antibiotics", "Antibiotics",
      "Antimicrobial substance active against bacteria, used in the treatment and prevention of bacterial infections."
      " This medication usually comes in the form of a pill."
      " There are different types of antibiotics for different applications.",
      "8;25;20", "2;5;6;7" },
    { "Fentanyl", "Fentanyl",
      "Fentanyl is a prescription drug that is also made and used illegally."
      " Like morphine, it is a medicine that is typically used to treat patients with severe pain, especially after surgery."
      " It is also sometimes used to treat patients with chronic pain who are physically tolerant to other opioids.",
      "10;25;20", "2;5;6;7" },
    { "Morphine", "Morphine",
      "Morphine is an opioid medication used to treat severe chronic pain. It has a high rate of addiction, overdose, and death.",
      "13;25;20", "2;5;6;7" },
    { "Pack Wound", "Pack Wound",
      "When a wound is deep, or when it tunnels under the skin, packing the wound can help it heal."
      " The packing material soaks up any drainage from the wound, which helps the tissues heal from the inside out.",
      "17;22;23", "2;5;6;7" },
    { "Escharotomy", "Escharotomy",
      "During treatment of a hazardous burn, an escharotomy is the surgical incision through the eschar into the subcutaneous tissues"
      " to allow the extremity to continue to swell without compressing underlying blood vessels.",
      "2;3;", "2;5;6;7" }
  };

  Treatment temp;
  Citation* citation;
  Equipment* equipment;

  for (auto& treatmentDef : default_treatments) {
    temp.clear();
    temp.medical_name = treatmentDef.medical_name.c_str();
    temp.common_name = treatmentDef.common_name.c_str();
    temp.description = treatmentDef.description.c_str();

    for (auto citation_id : string_split(treatmentDef.citations, ";")) {
      if (!citation_id.empty()) {
        citation = new Citation(&temp);
        citation->id = std::stoi(citation_id);
        if (select_citation(citation)) {
          temp.citations.push_back(citation);
        }
      }
    }

    for (auto equipment_id : string_split(treatmentDef.equipment, ";")) {
      if (!equipment_id.empty()) {
        equipment = new Equipment(&temp);
        equipment->id = std::stoi(equipment_id);
        if (select_equipment(equipment)) {
          temp.equipment.push_back(equipment);
        }
      }
    }
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
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS traumas;");
    QSqlDatabase::database(_db_name).exec("DROP TABLE IF EXISTS trauma_profiles;");
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

  QSqlQuery query { QSqlDatabase::database(_db_name) };
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
    case TRAUMAS:
      query.bindValue(":table", tables[TRAUMAS]);
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

    QSqlQuery query { QSqlDatabase::database(_db_name) };
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
    case TRAUMAS:
      query.prepare("SELECT MAX(trauma_id) FROM traumas");
      break;
    case TRAUMA_PROFILES:
      query.prepare(stmt.arg("trauma_profile"));
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
    case SCENES:
      query.prepare(stmt.arg("scene"));
      break;
    default:
      return -1;
    }
    query.exec();
    if (query.next()) {
      auto record = query.record();

      auto nextID = record.value(0).toInt() + 1;
      return nextID;
    }
  }
  return -1;
}
//----------------------------ASSESSMENT-----------------------------------------
inline void SQLite3Driver::assign_assessment(const QSqlRecord& record, Assessment& assessment) const
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

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_assessments);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
QList<Assessment*> SQLite3Driver::assessments() const
{
  QList<Assessment*> _assessments;
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_assessments);
    query.exec();
    while (query.next()) {
      auto assessment = std::make_unique<Assessment>();
      auto record = query.record();

      assign_assessment(record, *assessment);
      _assessments.push_back(assessment.release());
    }
  }
  return _assessments;
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
      qWarning() << "Provided Asseessement has no id or name one is required";
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
    QSqlQuery query { QSqlDatabase::database(_db_name) };
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
      emit assessmentUpdated(assessment->id);
      emit assessmentsChanged();
      return r;
    }
    emit assessmentUpdated(assessment->id);
    emit assessmentsChanged();
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
inline void SQLite3Driver::assign_author(const QSqlRecord& record, Author& author) const
{
  author.id = record.value(AUTHOR_ID).toInt();
  author.uuid = record.value(AUTHOR_UUID).toString();
  author.first = record.value(AUTHOR_FIRST_NAME).toString();
  author.middle = record.value(AUTHOR_OTHER_NAMES).toString();
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

  author.firstChanged();
  author.middleChanged();
  author.lastChanged();
  author.emailChanged();
  author.zipChanged();
  author.stateChanged();
  author.countryChanged();
  author.phoneChanged();
  author.organizationChanged();
  author.refreshed();
}

int SQLite3Driver::author_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_authors);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
QList<Author*> SQLite3Driver::authors() const
{
  QList<Author*> _authors;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_authors);
    query.exec();
    while (query.next()) {
      auto author = std::make_unique<Author>();
      auto record = query.record();

      assign_author(record, *author);
      _authors.push_back(author.release());
    }
  }
  return _authors;
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
      QSqlQuery query { QSqlDatabase::database(_db_name) };

      query.prepare(sqlite3::insert_or_update_authors);
      if (author->uuid == "") {
        author->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
      }
      query.bindValue(":uuid", author->uuid);
      query.bindValue(":name_first", author->first);
      query.bindValue(":name_others", author->middle);
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
        emit authorUpdated(author->id);
        emit authorsChanged();
        return r;
      }
      emit authorUpdated(author->id);
      emit authorsChanged();
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
      QSqlQuery query { QSqlDatabase::database(_db_name) };
      query.prepare(sqlite3::insert_or_update_first_author);
      if (author->uuid == "") {
        author->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
      }
      query.bindValue(":uuid", author->uuid);
      query.bindValue(":name_first", author->first);
      query.bindValue(":name_others", author->middle);
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
      emit authorUpdated(1);
      emit authorsChanged();
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
inline void SQLite3Driver::assign_citation(const QSqlRecord& record, Citation& citation) const
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

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_citations);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
int SQLite3Driver::citation_count(Scene const* scene) const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
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
QList<Citation*> SQLite3Driver::citations() const
{
  QList<Citation*> _citations;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_citations);
    query.exec();
    while (query.next()) {
      auto citation = std::make_unique<Citation>();
      auto record = query.record();
      assign_citation(record, *citation);
      _citations.push_back(citation.release());
    }
  }
  return _citations;
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
    } else if (!citation->uuid.isEmpty()) {
      query.prepare(sqlite3::select_citation_by_uuid);
      query.bindValue(":uuid", citation->uuid);
    } else {
      qWarning() << "Provided Citaion has no id, key, title, or uuid one is required";
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
    QSqlQuery query { QSqlDatabase::database(_db_name) };
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
      emit citationUpdated(citation->id);
      emit emit citationsChanged();
      return r;
    }
    citationUpdated(citation->id);
    emit emit citationsChanged();
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_citation_in_scene(Scene* scene, Citation* citation)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query { QSqlDatabase::database(_db_name) };
    if (select_scene(scene)) {
      if (!select_citation(citation)) {
        update_citation(citation);
      }
      CitationMap map;
      map.fk_citation->id = citation->id;
      map.fk_scene->id = scene->id;
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
      emit citationsChanged();
      return (remove_citation_from_equipment(citation->id)
              && remove_citation_from_treatments(citation->id)
              && remove_citation_from_traumas(citation->id)
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
  map.fk_scene->id = scene->id;
  map.fk_citation->id = citation->id;
  return remove_citation_map_by_fk(&map);
}
//----------------------------IMAGE-------------------------------------------------
inline void SQLite3Driver::assign_image(const QSqlRecord& record, Image& image) const
{
  image.id = record.value(IMAGES_ID).toInt();
  image.uuid = record.value(IMAGES_UUID).toString();
  image.uri = record.value(IMAGES_URI).toString();
  image.width = record.value(IMAGES_WIDTH).toInt();
  image.height = record.value(IMAGES_HEIGHT).toInt();
  image.format = record.value(IMAGES_FORMAT).toString();
}

int SQLite3Driver::image_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_images);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
QList<Image*> SQLite3Driver::images() const
{
  QList<Image*> _images;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_images);
    query.exec();
    while (query.next()) {
      auto image = std::make_unique<Image>();
      auto record = query.record();

      assign_image(record, *image);
      _images.push_back(image.release());
    }
  }
  return _images;
}

bool SQLite3Driver::select_image(Image* image) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (image->id != -1) {
      query.prepare(sqlite3::select_image_by_id);
      query.bindValue(":id", image->id);
    } else if (!image->uri.isEmpty()) {
      query.prepare(sqlite3::select_image_by_uri);
      query.bindValue(":uri", image->uri);
    } else {
      qWarning() << "Provided Image has no id, name, or uuid and one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_image(record, *image);
        return true;
      }
    } else {
      qWarning() << "select_image" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_image(Image* image)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query { QSqlDatabase::database(_db_name) };
    if (-1 != image->id) {
      query.prepare(sqlite3::update_image_by_id);
      query.bindValue(":id", image->id);
    } else if (!image->uri.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_images);
    }
    if (image->uuid == "") {
      image->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
    query.bindValue(":uuid", image->uuid);
    query.bindValue(":uri", image->uri);
    query.bindValue(":width", image->width);
    query.bindValue(":height", image->height);
    query.bindValue(":format", image->format);

    if (!query.exec()) {
      qWarning() << "update_image" << query.lastError();
      return false;
    }
    if (-1 == image->id) {
      const auto r = select_image(image);
      emit imageUpdated(image->id);
      emit emit imagesChanged();
      return r;
    }
    emit imageUpdated(image->id);
    emit emit imagesChanged();
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_image(Image* image)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_image(image)) {
      query.prepare(sqlite3::delete_image_by_id);
      query.bindValue(":id", image->id);
      if (!query.exec()) {
        qWarning() << "remove_image" << query.lastError();
        return false;
      }
      imageRemoved(image->id);
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//----------------------------INJURY-------------------------------------------------
inline void SQLite3Driver::assign_trauma(const QSqlRecord& record, Trauma& trauma) const
{
  trauma.id = record.value(INJURY_ID).toInt();
  trauma.uuid = record.value(INJURY_UUID).toString();
  trauma.medical_name = record.value(INJURY_MEDICAL_NAME).toString();
  trauma.common_name = record.value(INJURY_COMMON_NAME).toString();
  trauma.description = record.value(INJURY_DESCRIPTION).toString();
  trauma.lower_bound = record.value(INJURY_LOWER_BOUND).toFloat();
  trauma.upper_bound = record.value(INJURY_UPPER_BOUND).toFloat();

  Citation* citation;
  for (auto citation_id : record.value(INJURY_CITATIONS).toString().split(';')) {
    if (!citation_id.isEmpty()) {
      citation = new Citation(&trauma);
      citation->id = citation_id.toInt();
      if (select_citation(citation)) {
        trauma.citations.push_back(citation);
      }
    }
  }
}
int SQLite3Driver::trauma_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_traumas);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
QList<Trauma*> SQLite3Driver::traumas() const
{
  QList<Trauma*> _traumas;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_traumas);
    query.exec();
    while (query.next()) {
      auto trauma = std::make_unique<Trauma>();
      auto record = query.record();

      assign_trauma(record, *trauma);
      _traumas.push_back(trauma.release());
    }
  }
  return _traumas;
}

bool SQLite3Driver::select_trauma(Trauma* trauma) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (trauma->id != -1) {
      query.prepare(sqlite3::select_trauma_by_id);
      query.bindValue(":id", trauma->id);
    } else if (!trauma->medical_name.isEmpty()) {
      query.prepare(sqlite3::select_trauma_by_medical_name);
      query.bindValue(":medical_name", trauma->medical_name);
    } else if (!trauma->common_name.isEmpty()) {
      query.prepare(sqlite3::select_trauma_by_common_name);
      query.bindValue(":common_name", trauma->common_name);
    } else if (!trauma->uuid.isEmpty()) {
      query.prepare(sqlite3::select_trauma_by_uuid);
      query.bindValue(":uuid", trauma->uuid);
    } else {
      qWarning() << "Provided Trauma has no id, name, or uuid and one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_trauma(record, *trauma);
        return true;
      }
    } else {
      qWarning() << "select_trauma" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_trauma(Trauma* trauma)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query { QSqlDatabase::database(_db_name) };
    if (-1 != trauma->id) {
      query.prepare(sqlite3::update_trauma_by_id);
      query.bindValue(":id", trauma->id);
    } else if (!trauma->medical_name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_traumas);
    } else if (!trauma->common_name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_traumas);
    }
    if (trauma->uuid == "") {
      trauma->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
    query.bindValue(":uuid", trauma->uuid);

    QString citations;
    for (auto citation : trauma->citations) {
      citations += QString("%1;").arg(citation->id);
    }
    query.bindValue(":citations", citations);
    query.bindValue(":medical_name", trauma->medical_name);
    query.bindValue(":common_name", trauma->common_name);
    query.bindValue(":description", trauma->description);
    query.bindValue(":lower_bound", trauma->lower_bound);
    query.bindValue(":upper_bound", trauma->upper_bound);

    if (!query.exec()) {
      qWarning() << "update_trauma" << query.lastError();
      return false;
    }
    if (-1 == trauma->id) {
      const auto r = select_trauma(trauma);
      emit traumaUpdated(trauma->id);
      emit emit traumasChanged();
      return r;
    }
    emit traumaUpdated(trauma->id);
    emit emit traumasChanged();
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_trauma(Trauma* trauma)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_trauma(trauma)) {
      query.prepare(sqlite3::delete_trauma_by_id);
      query.bindValue(":id", trauma->id);
      if (!query.exec()) {
        qWarning() << "remove_trauma" << query.lastError();
        return false;
      }
      traumaRemoved(trauma->id);
      return remove_trauma_from_trauma_profiles(trauma->id);
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//-----------------------------INJURY_SET----------------------------------------
inline void SQLite3Driver::assign_trauma_profile(const QSqlRecord& record, TraumaProfile& profile) const
{
  profile.id = record.value(INJURY_SET_ID).toInt();
  profile.uuid = record.value(INJURY_SET_UUID).toString();
  profile.name = record.value(INJURY_SET_NAME).toString();
  profile.description = record.value(INJURY_SET_DESCRIPTION).toString();

  TraumaOccurence* instance;
  auto injury_ids = record.value(INJURY_SET_TRAUMAS).toString().split(';');
  auto severity_values = record.value(INJURY_SET_SEVERITIES).toString().split(';');
  auto location_values = record.value(INJURY_SET_LOCATIONS).toString().split(';');
  for (auto i = 0; i < injury_ids.size(); ++i) {
    instance = new TraumaOccurence();
    instance->fk_trauma->id = injury_ids[i].toInt();
    if (select_trauma(instance->fk_trauma)) {
      instance->severity = severity_values[i];
      instance->location = location_values[i];
      profile.traumas.push_back(instance);
    }
  }
}

int SQLite3Driver::trauma_profile_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_trauma_profiles);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
QList<TraumaProfile*> SQLite3Driver::trauma_profiles() const
{
  QList<TraumaProfile*> _trauma_profiles;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_trauma_profiles);
    query.exec();
    while (query.next()) {
      auto set = std::make_unique<TraumaProfile>();
      auto record = query.record();

      assign_trauma_profile(record, *set);
      _trauma_profiles.push_back(set.release());
    }
  }
  return _trauma_profiles;
}

bool SQLite3Driver::select_trauma_profile(TraumaProfile* set) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (set->id != -1) {
      query.prepare(sqlite3::select_trauma_profile_by_id);
      query.bindValue(":id", set->id);
    } else if (!set->name.isEmpty()) {
      query.prepare(sqlite3::select_trauma_profile_by_name);
      query.bindValue(":name", set->name);
    } else if (!set->uuid.isEmpty()) {
      query.prepare(sqlite3::select_trauma_profile_by_uuid);
      query.bindValue(":uuid", set->uuid);
    } else {
      qWarning() << "Provided Trauma Profile has no id, name, or uuid and one is required";
      return false;
    }
    if (query.exec()) {
      while (query.next()) {
        record = query.record();
        assign_trauma_profile(record, *set);
        return true;
      }
    } else {
      qWarning() << "select_trauma_profile" << query.lastError();
    }
    return false;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_trauma_profile(TraumaProfile* trauma_profile)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query { QSqlDatabase::database(_db_name) };
    if (-1 != trauma_profile->id) {
      query.prepare(sqlite3::update_trauma_profile_by_id);
      query.bindValue(":id", trauma_profile->id);
    } else if (!trauma_profile->name.isEmpty()) {
      query.prepare(sqlite3::insert_or_update_trauma_profiles);
    }
    if (trauma_profile->uuid == "") {
      trauma_profile->uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }
    query.bindValue(":uuid", trauma_profile->uuid);
    query.bindValue(":name", trauma_profile->name);
    query.bindValue(":description", trauma_profile->description);

    QString traumas;
    QString severities;
    QString locations;
    for (auto occurence : trauma_profile->traumas) {
      traumas += QString("%1;").arg(occurence->fk_trauma->id);
      severities += QString("%1;").arg(occurence->severity);
      locations += QString("%1;").arg(occurence->location);
    }
    query.bindValue(":traumas", traumas);
    query.bindValue(":severities", severities);
    query.bindValue(":locations", locations);

    if (!query.exec()) {
      qWarning() << "update_trauma_profile" << query.lastError();
      return false;
    }
    if (-1 == trauma_profile->id) {
      const auto r = select_trauma_profile(trauma_profile);
      emit traumaProfileUpdated(trauma_profile->id);
      emit traumaProfilesChanged();
      return r;
    }
    emit traumaProfileUpdated(trauma_profile->id);
    emit traumaProfilesChanged();
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::remove_trauma_profile(TraumaProfile* trauma_profile)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    if (select_trauma_profile(trauma_profile)) {
      query.prepare(sqlite3::delete_trauma_profile_by_id);
      query.bindValue(":id", trauma_profile->id);
      if (!query.exec()) {
        qWarning() << "remove_trauma_profile" << query.lastError();
        return false;
      }
      traumaProfileRemoved(trauma_profile->id);
      return true;
    } else {
      return false;
    }
  }
  qWarning() << "No Database connection";
  return false;
}
//----------------------------EVENT-------------------------------------------------
inline void SQLite3Driver::assign_event(QSqlRecord& record, Event& event) const
{
  event.id = record.value(EVENT_ID).toInt();
  event.uuid = record.value(EVENT_UUID).toString();
  event.name = record.value(EVENT_NAME).toString();
  event.description = record.value(EVENT_DESCRIPTION).toString();
  event.fidelity = record.value(EVENT_FIDELITY).toString();
  event.category = record.value(EVENT_CATEGORY).toString();
  event.fk_actor_1->clear();
  event.fk_actor_1->id = record.value(EVENT_ACTOR_1).toInt();
  event.fk_actor_2->clear();
  event.fk_actor_2->id = record.value(EVENT_ACTOR_2).toInt();
  event.fk_equipment->clear();
  event.fk_equipment->id = record.value(EVENT_EQUIPMENT).toInt();
  event.details = record.value(EVENT_DETAILS).toString();
}
int SQLite3Driver::event_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_events);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
int SQLite3Driver::event_count(Scene const* scene) const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
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
QList<Event*> SQLite3Driver::events() const
{
  QList<Event*> _events;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_events);
    query.exec();
    while (query.next()) {
      auto event = std::make_unique<Event>();
      auto record = query.record();

      assign_event(record, *event);
      _events.push_back(event.release());
    }
  }
  return _events;
}
QList<Event*> SQLite3Driver::events_in_scene(Scene const* scene) const
{
  QList<Event*> _events;
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_events_in_scene);
    query.bindValue(":scene_id", scene->id);
    query.exec();
    while (query.next()) {
      auto event = std::make_unique<Event>();
      auto record = query.record();

      assign_event(record, *event);
      _events.push_back(event.release());
    }
  }
  return _events;
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
    } else if (!event->uuid.isEmpty()) {
      query.prepare(sqlite3::select_event_by_uuid);
      query.bindValue(":uuid", event->uuid);
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
    QSqlQuery query { QSqlDatabase::database(_db_name) };
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
    query.bindValue(":equipment", event->fk_equipment->id);
    query.bindValue(":name", event->name);
    query.bindValue(":description", event->description);
    query.bindValue(":category", event->category);
    query.bindValue(":fidelity", event->fidelity);
    query.bindValue(":actor_1", event->fk_actor_1->id);
    query.bindValue(":actor_2", event->fk_actor_2->id);
    query.bindValue(":details", event->details);
    if (!query.exec()) {
      qWarning() << "update_event" << query.lastError();
      return false;
    }
    if (-1 == event->id) {
      const auto r = select_event(event);
      emit eventUpdated(event->id);
      emit eventsChanged();
      return r;
    }
    emit eventUpdated(event->id);
    emit eventsChanged();
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_event_in_scene(Scene* scene, Event* event)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query { QSqlDatabase::database(_db_name) };
    if (select_scene(scene)) {
      if (!select_event(event)) {
        update_event(event);
      }
      EventMap map;
      map.fk_event->assign(event);
      map.fk_scene->assign(scene);
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
  map.fk_scene->assign(scene);
  map.fk_event->assign(event);
  return remove_event_map_by_fk(&map);
}
//-----------------------------LOCATION------------------------------------------
inline void SQLite3Driver::assign_location(const QSqlRecord& record, Location& location) const
{
  location.id = record.value(LOCATION_ID).toInt();
  location.uuid = record.value(LOCATION_UUID).toString();
  location.name = record.value(LOCATION_NAME).toString();
  location.description = record.value(LOCATION_DESCRIPTION).toString();
  location.environment = record.value(LOCATION_ENVIRONMENT).toString();
}
int SQLite3Driver::location_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_locations);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
QList<Location*> SQLite3Driver::locations() const
{
  QList<Location*> _locations;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_locations);
    query.exec();
    while (query.next()) {
      auto location = std::make_unique<Location>();
      auto record = query.record();

      assign_location(record, *location);
      _locations.push_back(location.release());
    }
  }
  return _locations;
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
    } else if (!location->uuid.isEmpty()) {
      query.prepare(sqlite3::select_location_by_uuid);
      query.bindValue(":uuid", location->uuid);
    } else {
      qWarning() << "Provided Location has no id, name, or uuid one is required";
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
    QSqlQuery query { QSqlDatabase::database(_db_name) };
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
    query.bindValue(":description", location->description);
    query.bindValue(":environment", location->environment);
    if (!query.exec()) {
      qWarning() << "update_location" << query.lastError();
      return false;
    }
    if (-1 == location->id) {
      const auto r = select_location(location);
      emit locationUpdated(location->id);
      emit locationsChanged();

      return r;
    }
    emit locationUpdated(location->id);
    emit locationsChanged();
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
int SQLite3Driver::location_count(Scene const* scene) const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
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
Location* SQLite3Driver::getLocationOfScene(Scene* scene) const
{
  //TODO: Remove second loop as location_map should include an inflated fk_lcoation

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery location_map_query { QSqlDatabase::database(_db_name) };
    location_map_query.prepare(sqlite3::select_location_map_by_fk_scene);
    location_map_query.bindValue(":fk_scene", scene->id);
    location_map_query.exec();
    while (location_map_query.next()) {
      auto location = Location::make();
      auto location_map = std::make_unique<LocationMap>();
      auto location_map_record = location_map_query.record();
      assign_location_map(location_map_record, *location_map);
      location->assign(location_map->fk_location);
      return location;
    };
  }
  return nullptr;
}
bool SQLite3Driver::update_location_in_scene(Scene* scene, Location* location)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query { QSqlDatabase::database(_db_name) };
    if (select_scene(scene)) {
      if (!select_location(location)) {
        update_location(location);
      }
      LocationMap map;
      map.fk_location->assign(location);
      map.fk_scene->assign(scene);
      if (location_map_count(scene) > 0) {
        QSqlQuery query_location_map(QSqlDatabase::database(_db_name));
        query_location_map.prepare(sqlite3::delete_location_map_by_fk_scene);
        query_location_map.bindValue(":fk_scene", scene->id);
        if (!query_location_map.exec()) {
          qWarning() << query_location_map.lastError();
          return false;
        }
      }
      //! We may need to change how this works
      //! Trying to avoid duplicate location entries due to
      //! Missing unique constraint on (fk_scene,fk_location)
      select_location_map(&map);
      update_location_map(&map);

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
  map.fk_scene->assign(scene);
  map.fk_location->assign(location);
  return remove_location_map_by_fk(&map);
}
//----------------------------EQUIPMENT------------------------------------------
inline void SQLite3Driver::assign_equipment(const QSqlRecord& record, Equipment& equipment) const
{
  equipment.id = record.value(EQUIPMENT_ID).toInt();
  equipment.uuid = record.value(EQUIPMENT_UUID).toString();
  equipment.name = record.value(EQUIPMENT_NAME).toString();
  equipment.type = record.value(EQUIPMENT_TYPE).toInt();
  equipment.summary = record.value(EQUIPMENT_SUMMARY).toString();
  equipment.description = record.value(EQUIPMENT_DESCRIPTION).toString();

  Citation* citation;
  for (auto citation_id : record.value(EQUIPMENT_CITATIONS).toString().split(';')) {
    if (!citation_id.isEmpty()) {
      citation = new Citation(&equipment);
      citation->id = citation_id.toInt();
      if (select_citation(citation)) {
        equipment.citations.push_back(citation);
      }
    }
  }

  if (!record.value(EQUIPMENT_IMAGE).toString().isEmpty()) {
    auto image = new Image(&equipment);
    image->id = record.value(EQUIPMENT_IMAGE).toInt();
    if (select_image(image)) {
      equipment.fk_image->assign(image);
    }
  }

  equipment.parametersFromString(record.value(EQUIPMENT_PROPERTIES).toString());

  equipment.idChanged(equipment.id);
  equipment.uuidChanged(equipment.uuid);
  equipment.nameChanged(equipment.name);
  equipment.typeChanged(equipment.type);
  equipment.summaryChanged(equipment.summary);
  equipment.descriptionChanged(equipment.description);
  equipment.citationsChanged();
  equipment.imageChanged();
  equipment.propertiesChanged();
  equipment.refreshed();
}
int SQLite3Driver::equipment_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_equipments);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
int SQLite3Driver::equipment_count(Scene const* scene) const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
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
QList<Equipment*> SQLite3Driver::equipments() const
{
  QList<Equipment*> _equipments;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_equipments);
    query.exec();
    while (query.next()) {
      auto equipment = std::make_unique<Equipment>();
      auto record = query.record();

      assign_equipment(record, *equipment);
      _equipments.push_back(equipment.release());
    }
  }
  return _equipments;
}
QList<EquipmentMap*> SQLite3Driver::equipment_in_scene(Scene const* scene) const
{
  QList<EquipmentMap*> _equipment_maps;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery equipment_map_query { QSqlDatabase::database(_db_name) };
    equipment_map_query.prepare(sqlite3::select_all_equipment_in_a_scene);
    equipment_map_query.bindValue(":fk_scene", scene->id);
    equipment_map_query.exec();
    while (equipment_map_query.next()) {
      auto equipment_map = std::make_unique<EquipmentMap>();
      auto equipment_map_record = equipment_map_query.record();
      assign_equipment_map(equipment_map_record, *equipment_map, scene);
      _equipment_maps.push_back(equipment_map.release());
    }
  }
  return _equipment_maps;
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
    } else if (!equipment->uuid.isEmpty()) {
      query.prepare(sqlite3::select_equipment_by_uuid);
      query.bindValue(":uuid", equipment->uuid);
    } else {
      qWarning() << "Provided Equipment has no id, name, or uuid one is required";
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
    QSqlQuery query { QSqlDatabase::database(_db_name) };
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

    QString citations;
    for (auto citation : equipment->citations) {
      citations += QString("%1;").arg(citation->id);
    }

    query.bindValue(":citations", citations);
    query.bindValue(":image", equipment->fk_image->id);
    query.bindValue(":properties", equipment->parametersToString());

    if (!query.exec()) {
      qWarning() << "update_equipment" << query.lastError();
      return false;
    }
    if (-1 == equipment->id) {
      const auto r = select_equipment(equipment);
      emit equipmentUpdated(equipment->id);
      emit equipmentChanged();
      return r;
    }
    emit equipmentUpdated(equipment->id);

    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_equipment_in_scene(EquipmentMap* map)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query { QSqlDatabase::database(_db_name) };
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
inline void SQLite3Driver::assign_objective(const QSqlRecord& record, Objective& objective) const
{
  objective.id = record.value(OBJECTIVE_ID).toInt();
  objective.uuid = record.value(OBJECTIVE_UUID).toString();
  objective.name = record.value(OBJECTIVE_NAME).toString();
  objective.description = record.value(OBJECTIVE_DESCRIPTION).toString();

  Citation* citation;
  for (auto citation_id : record.value(OBJECTIVE_CITATIONS).toString().split(';')) {
    if (!citation_id.isEmpty()) {
      citation = new Citation(&objective);
      citation->id = citation_id.toInt();
      if (select_citation(citation)) {
        objective._citations.push_back(citation);
      }
    }
  }
}
int SQLite3Driver::objective_count() const

{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_objectives);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
QList<Objective*> SQLite3Driver::objectives() const
{
  QList<Objective*> _objectives;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_objectives);
    query.exec();
    while (query.next()) {
      auto objective = std::make_unique<Objective>();
      auto record = query.record();

      assign_objective(record, *objective);
      _objectives.push_back(objective.release());
    }
  }
  return _objectives;
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
    } else if (!objective->uuid.isEmpty()) {
      query.prepare(sqlite3::select_objective_by_uuid);
      query.bindValue(":uuid", objective->uuid);
    } else {
      qWarning() << "Provided Objective has no id, name or uuid one is required";
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
    QSqlQuery query { QSqlDatabase::database(_db_name) };
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

    QString citations;
    for (auto citation : objective->_citations) {
      citations += QString("%1;").arg(citation->id);
    }
    query.bindValue(":citations", citations);

    if (!query.exec()) {
      qWarning() << "update_objective" << query.lastError();
      return false;
    }
    if (-1 == objective->id) {
      auto r = select_objective(objective);
      emit objectiveUpdated(objective->id);
      emit objectivesChanged();
      return r;
    }
    emit objectiveUpdated(objective->id);
    emit objectivesChanged();
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
inline void SQLite3Driver::assign_property(const QSqlRecord& record, Property& property) const
{
  property.id = record.value(PROPERTY_ID).toInt();
  property.name = record.value(PROPERTY_NAME).toString();
  property.value = record.value(PROPERTY_VALUE).toString();
}
int SQLite3Driver::property_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_properties);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
QList<Property*> SQLite3Driver::properties() const
{
  QList<Property*> _properties;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_properties);
    query.exec();
    while (query.next()) {
      auto property = std::make_unique<Property>();
      auto record = query.record();
      int ct = record.count();

      assign_property(record, *property);
      _properties.push_back(property.release());
    }
  }
  return _properties;
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
      QSqlQuery query { QSqlDatabase::database(_db_name) };
      query.prepare(sqlite3::insert_or_update_properties);
      query.bindValue(":name", property->name);
      query.bindValue(":value", property->value);
      if (!query.exec()) {
        qWarning() << "update_property" << query.lastError();
        return false;
      }
      if (-1 == property->id) {
        const auto r = select_property(property);
        emit propertyUpdated(property->id);
        emit propertiesChanged();
        return r;
      }
      emit propertyUpdated(property->id);
      emit propertiesChanged();
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
inline void SQLite3Driver::assign_role(QSqlRecord& record, Role& role) const
{
  role.id = record.value(ROLE_ID).toInt();
  role.uuid = record.value(ROLE_UUID).toString();
  role.name = record.value(ROLE_NAME).toString();
  role.description = record.value(ROLE_DESCRIPTION).toString();

  role.trauma_profile->id = record.value(ROLE_TRAUMA_PROFILE).toInt();
  select_trauma_profile(role.trauma_profile);

  role.category = record.value(ROLE_CATEGORY).toString();
}
int SQLite3Driver::role_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_roles);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
int SQLite3Driver::role_count(Scene const* scene) const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
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
QList<Role*> SQLite3Driver::roles() const
{
  QList<Role*> _roles;

  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_roles);
    query.exec();
    while (query.next()) {
      auto role = std::make_unique<Role>();
      auto record = query.record();

      assign_role(record, *role);
      _roles.push_back(role.release());
    }
  }
  return _roles;
}
QList<RoleMap*> SQLite3Driver::roles_in_scene(Scene const* scene) const
{
  QList<RoleMap*> _roleMaps;
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery map_query { QSqlDatabase::database(_db_name) };
    map_query.prepare(sqlite3::select_role_map_by_fk_scene);
    map_query.bindValue(":fk_scene", scene->id);
    map_query.exec();
    while (map_query.next()) {
      auto map = std::make_unique<RoleMap>();
      auto map_record = map_query.record();

      assign_role_map(map_record, *map);
      if (select_role(map->fk_role) && select_scene(map->fk_scene)) {
        _roleMaps.push_back(RoleMap::make());
        _roleMaps.back()->assign(map.release());
      }
    }
  }
  return _roleMaps;
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
      qWarning() << "Provided Role has no id or name one is required";
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
    QSqlQuery query { QSqlDatabase::database(_db_name) };
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
    query.bindValue(":category", role->category);
    query.bindValue(":name", role->name);
    query.bindValue(":trauma_profile", role->trauma_profile->id);

    if (!query.exec()) {
      qWarning() << "update_role" << query.lastError();
      return false;
    }
    if (-1 == role->id) {
      const auto r = select_role(role);
      emit roleUpdated(role->id);
      emit rolesChanged();
      return r;
    }
    emit roleUpdated(role->id);
    emit rolesChanged();
    return true;
  }
  qWarning() << "No Database connection";
  return false;
}
bool SQLite3Driver::update_role_in_scene(Scene* scene, Role* role)
{
  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query { QSqlDatabase::database(_db_name) };
    if (select_scene(scene)) {
      if (!select_role(role)) {
        update_role(role);
      }
      RoleMap map;
      map.fk_role->assign(role);
      map.fk_scene->assign(scene);
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
  map.fk_scene->assign(scene);
  map.fk_role->assign(role);
  return remove_role_map_by_fk(&map);
}

//-----------------------------SCENE---------------------------------------------
inline void SQLite3Driver::assign_scene(QSqlRecord& record, Scene& scene) const
{
  scene.id = record.value(SCENE_ID).toInt();
  scene.uuid = record.value(SCENE_UUID).toString();
  scene.name = record.value(SCENE_NAME).toString();
  scene.description = record.value(SCENE_DESCRIPTION).toString();
  scene.time_of_day = record.value(SCENE_TIME_OF_DAY).toInt();
  scene.time_in_simulation = record.value(SCENE_TIME_IN_SIMULATION).toInt();
  scene.weather = record.value(SCENE_WEATHER).toString();

  Event* event;
  for (auto event_id : record.value(SCENE_EVENTS).toString().split(';')) {
    event = new Event(&scene);
    event->id = event_id.toInt();
    scene.events.push_back(event);
  }

  Role* role;
  for (auto role_id : record.value(SCENE_ROLES).toString().split(';')) {
    role = new Role(&scene);
    role->id = role_id.toInt();
    scene.roles.push_back(role);
  }
}
int SQLite3Driver::scene_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_scenes);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
QList<Scene*> SQLite3Driver::scenes() const
{
  QList<Scene*> _scenes;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_scenes);
    query.exec();
    while (query.next()) {
      auto scene = std::make_unique<Scene>();
      auto record = query.record();

      assign_scene(record, *scene);
      _scenes.push_back(scene.release());
    }
  }
  return _scenes;
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
      qWarning() << "Provided Scene has no id or name one is required";
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
bool SQLite3Driver::update_scene(Scene* scene, bool createLocationIfMissing)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query { QSqlDatabase::database(_db_name) };
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

    QString events;
    for (auto event : scene->events) {
      events += QString("%1;").arg(event->id);
    }
    query.bindValue(":events", events);

    QString roles;
    for (auto role : scene->roles) {
      roles += QString("%1;").arg(role->id);
    }
    query.bindValue(":roles", roles);

    if (!query.exec()) {
      qWarning() << "update_scene" << query.lastError();
      return false;
    }
    if (-1 == scene->id) {
      //!
      //!  This logic ensures there is always one location for each new scene
      //!  The current UI doesn't allow location selection for a scene so this must be true.
      //!  Future updates will allow us to remove this
      if (select_scene(scene)) {
        sceneUpdated(scene->id);
        if (createLocationIfMissing) {
          Location location;
          location.id = 1;

          QSqlQuery locations_in_scene_query { QSqlDatabase::database(_db_name) };
          locations_in_scene_query.prepare(select_scene_locations_by_fk_scene);
          locations_in_scene_query.bindValue(":scene_id", scene->id);
          if (!locations_in_scene_query.exec()) {
            qWarning() << "update_scene:locations_in_scene_query: " << locations_in_scene_query.lastError();
            return false;
          }
          if (query.next()) {
            QSqlRecord location_record = query.record();
            assign_location(location_record, location);
          } else {
            location.clear(nextID(LOCATIONS));
            update_location(&location);
          }
          if (update_location_in_scene(scene, &location)) {
            emit locationUpdated(location.id);
            emit locationsChanged();
            return true;
          }
        } else {
          return true;
        }
      }
      qWarning() << "Unable to update Location associated with Scene";
      return false;
    }
    emit sceneUpdated(scene->id);
    emit scenesChanged();
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
          query_delete_location_map.bindValue(":fk_scene", location_map.fk_scene->id);
          if (!query_delete_location_map.exec()) {
            qWarning() << query_delete_location_map.lastError();
            return false;
          }
          QSqlQuery query_location(QSqlDatabase::database(_db_name));
          query_location.prepare(sqlite3::delete_location_by_id);
          query_location.bindValue(":id", location_map.fk_location->id);
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
inline void SQLite3Driver::assign_treatment(const QSqlRecord& record, Treatment& treatment) const
{
  treatment.id = record.value(TREATMENT_ID).toInt();
  treatment.uuid = record.value(TREATMENT_UUID).toString();
  treatment.medical_name = record.value(TREATMENT_MEDICAL_NAME).toString();
  treatment.common_name = record.value(TREATMENT_COMMON_NAME).toString();
  treatment.description = record.value(TREATMENT_DESCRIPTION).toString();

  Equipment* equipment;
  for (auto equipment_id : record.value(TREATMENT_EQUIPMENT).toString().split(';')) {
    if (!equipment_id.isEmpty()) {
      equipment = new Equipment(&treatment);
      equipment->id = equipment_id.toInt();
      if (select_equipment(equipment)) {
        treatment.equipment.push_back(equipment);
      }
    }
  }

  Citation* citation;
  for (auto citation_id : record.value(TREATMENT_CITATIONS).toString().split(';')) {
    if (!citation_id.isEmpty()) {
      citation = new Citation(&treatment);
      citation->id = citation_id.toInt();
      if (select_citation(citation)) {
        treatment.citations.push_back(citation);
      }
    }
  }

  Citation* cpg;
  for (auto cpg_id : record.value(TREATMENT_CPGS).toString().split(';')) {
    cpg = new Citation(&treatment);
    cpg->id = cpg_id.toInt();
    treatment.cpgs.push_back(cpg);
  }
}
int SQLite3Driver::treatment_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_treatments);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
QList<Treatment*> SQLite3Driver::treatments() const
{
  QList<Treatment*> _treatments;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_treatments);
    query.exec();
    while (query.next()) {
      auto treatment = std::make_unique<Treatment>();
      auto record = query.record();

      assign_treatment(record, *treatment);
      _treatments.push_back(treatment.release());
    }
  }
  return _treatments;
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
      qWarning() << "Provided Teretment has no id or name one is required";
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
    QSqlQuery query { QSqlDatabase::database(_db_name) };
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

    QString citations;
    for (auto citation : treatment->citations) {
      citations += QString("%1;").arg(citation->id);
    }
    query.bindValue(":citations", citations);

    QString equipments;
    for (auto equipment : treatment->equipment) {
      equipments += QString("%1;").arg(equipment->id);
    }
    query.bindValue(":equipment", equipments);

    query.bindValue(":medical_name", treatment->medical_name);
    query.bindValue(":common_name", treatment->common_name);
    query.bindValue(":description", treatment->description);

    if (!query.exec()) {
      qWarning() << "update_treatment" << query.lastError();
      return false;
    }
    if (-1 == treatment->id) {
      const auto r = select_treatment(treatment);
      emit treatmentUpdated(treatment->id);
      emit treatmentsChanged();
      return r;
    }
    emit treatmentUpdated(treatment->id);
    emit treatmentsChanged();
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
inline void SQLite3Driver::assign_role_map(const QSqlRecord& record, RoleMap& map) const
{
  map.id = record.value(ROLE_MAP_ID).toInt();
  map.fk_scene->id = record.value(ROLE_MAP_FK_SCENE).toInt();
  select_scene(map.fk_scene);
  map.fk_role->id = record.value(ROLE_MAP_FK_ROLE).toInt();
  select_role(map.fk_role);
  map.category = record.value(ROLE_MAP_CATEGORY).toString();
}
int SQLite3Driver::role_map_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_role_maps);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
QList<RoleMap*> SQLite3Driver::role_maps() const
{
  QList<RoleMap*> _role_maps;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_role_maps);
    query.exec();
    while (query.next()) {
      auto map = std::make_unique<RoleMap>();
      auto record = query.record();

      assign_role_map(record, *map);
      _role_maps.push_back(map.release());
    }
  }
  return _role_maps;
}

bool SQLite3Driver::select_role_map(RoleMap* map) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (map->id != -1) {
      query.prepare(sqlite3::select_role_map_by_id);
      query.bindValue(":id", map->id);
    } else if (map->fk_scene->id != -1) {
      query.prepare(sqlite3::select_role_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene->id);
      query.bindValue(":fk_role", map->fk_role->id);
    } else {
      qWarning() << "Provided RoleMap has no id, fk_scene, or fk_role one is required";
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
    QSqlQuery query { QSqlDatabase::database(_db_name) };
    if (-1 != map->id) {
      query.prepare(sqlite3::update_role_map_by_id);
      query.bindValue(":id", map->id);
    } else {
      query.prepare(sqlite3::insert_or_update_role_maps);
    }
    query.bindValue(":fk_scene", map->fk_scene->id);
    query.bindValue(":fk_role", map->fk_role->id);
    query.bindValue(":category", map->category);
    if (!query.exec()) {
      qWarning() << "update_role_map" << query.lastError();
      return false;
    }
    if (-1 == map->id) {
      const auto r = select_role_map(map);
      emit mapUpdated(map->id);
      emit roleMapsChanged();
      return r;
    }
    emit mapUpdated(map->id);
    emit roleMapsChanged();
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
      query.bindValue(":fk_scene", map->fk_scene->id);
      query.bindValue(":fk_role", map->fk_role->id);
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
inline void SQLite3Driver::assign_event_map(const QSqlRecord& record, EventMap& map) const
{
  map.id = record.value(EVENT_MAP_ID).toInt();
  map.fk_scene->id = record.value(EVENT_MAP_FK_SCENE).toInt();
  select_scene(map.fk_scene);
  map.fk_event->id = record.value(EVENT_MAP_FK_EVENT).toInt();
  select_event(map.fk_event);
}
int SQLite3Driver::event_map_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_event_maps);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
QList<EventMap*> SQLite3Driver::event_maps() const
{
  QList<EventMap*> _event_maps;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_event_maps);
    query.exec();
    while (query.next()) {
      auto map = std::make_unique<EventMap>();
      auto record = query.record();

      assign_event_map(record, *map);
      _event_maps.push_back(map.release());
    }
  }
  return _event_maps;
}

bool SQLite3Driver::select_event_map(EventMap* map) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (map->id != -1) {
      query.prepare(sqlite3::select_event_map_by_id);
      query.bindValue(":id", map->id);
    } else if (map->fk_scene->id != -1) {
      query.prepare(sqlite3::select_event_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene->id);
      query.bindValue(":fk_event", map->fk_event->id);
    } else {
      qWarning() << "Provided EventMap has no id, fk_scene, or fk_event one is required";
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
    QSqlQuery query { QSqlDatabase::database(_db_name) };
    if (-1 != map->id) {
      query.prepare(sqlite3::update_event_map_by_id);
      query.bindValue(":id", map->id);
    } else {
      query.prepare(sqlite3::insert_or_update_event_maps);
    }

    query.bindValue(":fk_scene", map->fk_scene->id);
    query.bindValue(":fk_event", map->fk_event->id);
    if (!query.exec()) {
      qWarning() << "update_event_map" << query.lastError();
      return false;
    }
    if (-1 == map->id) {
      const auto r = select_event_map(map);
      emit eventMapUpdated(map->id);
      emit eventMapsChanged();
      return r;
    }
    emit eventMapUpdated(map->id);
    emit eventMapsChanged();
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
      query.bindValue(":fk_scene", map->fk_scene->id);
      query.bindValue(":fk_event", map->fk_event->id);
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
inline void SQLite3Driver::assign_location_map(const QSqlRecord& record, LocationMap& map) const
{
  map.id = record.value(LOCATION_MAP_ID).toInt();
  map.fk_scene->id = record.value(LOCATION_MAP_FK_SCENE).toInt();
  select_scene(map.fk_scene);
  map.fk_location->id = record.value(LOCATION_MAP_FK_LOCATION).toInt();
  select_location(map.fk_location);
}
int SQLite3Driver::location_map_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_location_maps);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
int SQLite3Driver::location_map_count(Scene const* scene) const //we currently only support a scene having one location, but this might change in the future
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
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
QList<LocationMap*> SQLite3Driver::location_maps() const
{
  QList<LocationMap*> _location_maps;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_location_maps);
    query.exec();
    while (query.next()) {
      auto map = std::make_unique<LocationMap>();
      auto record = query.record();

      assign_location_map(record, *map);
      _location_maps.push_back(map.release());
    }
  }
  return _location_maps;
}

bool SQLite3Driver::select_location_map(LocationMap* map) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (map->id != -1) {
      query.prepare(sqlite3::select_location_map_by_id);
      query.bindValue(":id", map->id);
    } else if (map->fk_scene->id != -1) {
      query.prepare(sqlite3::select_location_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene->id);
      query.bindValue(":fk_location", map->fk_location->id);
    } else {
      qWarning() << "Provided LocationMap has no id, fk_scene, or fk_location one is required";
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
    QSqlQuery query { QSqlDatabase::database(_db_name) };
    if (-1 != map->id) {
      query.prepare(sqlite3::update_location_map_by_id);
      query.bindValue(":id", map->id);
    } else {
      query.prepare(sqlite3::insert_or_update_location_maps);
    }
    query.bindValue(":fk_scene", map->fk_scene->id);
    query.bindValue(":fk_location", map->fk_location->id);
    if (!query.exec()) {
      qWarning() << "update_location_map" << query.lastError();
      return false;
    }
    if (-1 == map->id) {
      const auto r = select_location_map(map);
      emit locationMapUpdated(map->id);
      emit locationMapsChanged();
      return r;
    }
    emit locationMapUpdated(map->id);
    emit locationMapsChanged();
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
      query.bindValue(":fk_scene", map->fk_scene->id);
      query.bindValue(":fk_location", map->fk_location->id);
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
inline void SQLite3Driver::assign_citation_map(const QSqlRecord& record, CitationMap& map) const
{
  map.id = record.value(CITATION_MAP_ID).toInt();
  map.fk_scene->id = record.value(CITATION_MAP_FK_SCENE).toInt();
  map.fk_citation->id = record.value(CITATION_MAP_FK_CITATION).toInt();
}
int SQLite3Driver::citation_map_count() const
{
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_citation_maps);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
QList<CitationMap*> SQLite3Driver::citation_maps() const
{
  QList<CitationMap*> _citation_maps;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_citation_maps);
    query.exec();
    while (query.next()) {
      auto map = std::make_unique<CitationMap>();
      auto record = query.record();

      assign_citation_map(record, *map);
      _citation_maps.push_back(map.release());
    }
  }
  return _citation_maps;
}

bool SQLite3Driver::select_citation_map(CitationMap* map) const
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    QSqlQuery query(QSqlDatabase::database(_db_name));
    QSqlRecord record;
    if (map->id != -1) {
      query.prepare(sqlite3::select_citation_map_by_id);
      query.bindValue(":id", map->id);
    } else if (map->fk_scene->id != -1) {
      query.prepare(sqlite3::select_citation_map_by_fk);
      query.bindValue(":fk_scene", map->fk_scene->id);
      query.bindValue(":fk_citation", map->fk_citation->id);
    } else {
      qWarning() << "Provided CitationMap has no id, fk_scene, or fk_citation one is required";
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
    QSqlQuery query { QSqlDatabase::database(_db_name) };
    if (-1 != map->id) {
      query.prepare(sqlite3::update_citation_map_by_id);
      query.bindValue(":id", map->id);
    } else {
      query.prepare(sqlite3::insert_or_update_citation_maps);
    }
    query.bindValue(":fk_scene", map->fk_scene->id);
    query.bindValue(":fk_citation", map->fk_citation->id);
    if (!query.exec()) {
      qWarning() << "update_citation_map" << query.lastError();
      return false;
    }
    if (-1 == map->id) {
      const auto r = select_citation_map(map);
      emit citationMapUpdated(map->id);
      emit citationMapsChanged();
      return r;
    }
    emit citationMapUpdated(map->id);
    emit citationMapsChanged();
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
      query.bindValue(":fk_scene", map->fk_scene->id);
      query.bindValue(":fk_citation", map->fk_citation->id);
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
inline void SQLite3Driver::assign_equipment_map(const QSqlRecord& record, EquipmentMap& map, Scene const* const scene, Equipment const* const equipment) const
{
  map.id = record.value(EQUIPMENT_MAP_ID).toInt();
  emit map.idChanged(map.id);
  if (scene) {
    map.scene->assign(*scene);
  } else {
    map.scene->id = record.value(EQUIPMENT_MAP_FK_SCENE).toInt();
    select_scene(map.scene);
  }
  emit map.sceneChanged(map.scene);

  if (equipment) {
    map.equipment->assign(*equipment);
  } else {
    map.equipment->id = record.value(EQUIPMENT_MAP_FK_EQUIPMENT).toInt();
    select_equipment(map.equipment);
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

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::count_equipment_map);
    query.exec();
    if (query.next()) {
      auto record = query.record();

      return record.value(0).toInt();
    }
  }
  return -1;
}
QList<EquipmentMap*> SQLite3Driver::equipment_maps() const
{
  QList<EquipmentMap*> _equipment_maps;
  if (QSqlDatabase::database(_db_name).isOpen()) {

    QSqlQuery query { QSqlDatabase::database(_db_name) };
    query.prepare(sqlite3::select_all_equipment_map);
    query.exec();
    while (query.next()) {
      auto map = std::make_unique<EquipmentMap>();
      auto record = query.record();

      assign_equipment_map(record, *map);
      _equipment_maps.push_back(map.release());
    }
  }
  return _equipment_maps;
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
      qWarning() << "Provided EquipmentMap has no id, fk_scene, or fk_equipment one is required";
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
    QSqlQuery query { QSqlDatabase::database(_db_name) };
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
      emit equipmentMapUpdated(map->id);
      emit equipmentMapsChanged();
      return r;
    }
    emit equipmentMapUpdated(map->id);
    //emit equipmentMapsChanged();
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
      emit equipmentMapRemoved(map->id);
      emit equipmentMapsChanged();
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

//----------------------------------------------------------------------------
bool SQLite3Driver::remove_equipment_from_treatments(int equipment_id)
{
  return remove_equipment_from_treatments(std::to_string(equipment_id));
}
bool SQLite3Driver::remove_equipment_from_treatments(std::string equipment_id)
{

  if (QSqlDatabase::database(_db_name).isOpen()) {
    for (auto& treatment : treatments()) {
      auto equipment = treatment->equipment.begin();
      while (equipment != treatment->equipment.end()) {
        if ((*equipment)->id == std::stoi(equipment_id)) {
          treatment->equipment.erase(equipment);
        }
        if (equipment != treatment->equipment.end()) {
          ++equipment;
        }
      }
      update_treatment(treatment);
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
  auto trs = treatments();
  if (QSqlDatabase::database(_db_name).isOpen()) {
    for (auto& treatment : trs) {
      auto citation = treatment->citations.begin();
      while (citation != treatment->citations.end()) {
        if ((*citation)->id == std::stoi(citation_id)) {
          treatment->citations.erase(citation);
        }
        if (citation != treatment->citations.end()) {
          ++citation;
        }
      }
      update_treatment(treatment);
    }
    return true;
  }
  return false;
}
bool SQLite3Driver::remove_citation_from_traumas(int citation_id)
{
  return remove_citation_from_traumas(std::to_string(citation_id));
}
bool SQLite3Driver::remove_citation_from_traumas(std::string citation_id)
{
  auto trs = traumas();
  if (QSqlDatabase::database(_db_name).isOpen()) {
    for (auto& trauma : trs) {
      auto citation = trauma->citations.begin();
      while (citation != trauma->citations.end()) {
        if ((*citation)->id == std::stoi(citation_id)) {
          trauma->citations.erase(citation);
        }
        if (citation != trauma->citations.end()) {
          ++citation;
        }
      }
      update_trauma(trauma);
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
  auto eqs = equipments();
  if (QSqlDatabase::database(_db_name).isOpen()) {
    for (auto& equipment : eqs) {
      auto citation = equipment->citations.begin();
      while (citation != equipment->citations.end()) {
        if ((*citation)->id == std::stoi(citation_id)) {
          equipment->citations.erase(citation);
        }
        if (citation != equipment->citations.end()) {
          ++citation;
        }
      }
      update_equipment(equipment);
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
  auto objs = objectives();
  if (QSqlDatabase::database(_db_name).isOpen()) {
    for (auto& objective : objs) {
      auto citation = objective->_citations.begin();
      while (citation != objective->_citations.end()) {
        if ((*citation)->id == std::stoi(citation_id)) {
          objective->_citations.erase(citation);
        }
        if (citation != objective->_citations.end()) {
          ++citation;
        }
      }
      update_objective(objective);
    }
    return true;
  }
  return false;
}
bool SQLite3Driver::remove_trauma_from_trauma_profiles(int trauma_id)
{
  return remove_trauma_from_trauma_profiles(std::to_string(trauma_id));
}
bool SQLite3Driver::remove_trauma_from_trauma_profiles(std::string trauma_id)
{
  auto tps = trauma_profiles();
  if (QSqlDatabase::database(_db_name).isOpen()) {
    for (auto& trauma_profile : tps) {
      auto trauma = trauma_profile->traumas.begin();
      while (trauma != trauma_profile->traumas.end()) {
        if ((*trauma)->fk_trauma->id == std::stoi(trauma_id)) {
          trauma_profile->traumas.erase(trauma);
        }
        if (trauma != trauma_profile->traumas.end()) {
          ++trauma;
        }
      }
      update_trauma_profile(trauma_profile);
    }
    return true;
  }
  return false;
}
//-------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------
//------------------------- QQmlListProperty Interfaces--------------------------
//! As of Qt5.12 it appears to be complicated to pass a QList<QObject*> to QML
//  And retain the ability to refer to  Q_INVOKABLE.  In reality this entire
//  interface should be rewritten using QSQLModel  and  QSQLQueryModels
//  That way each page of the UI can have its own custom code.
//-------------------------------------------------------------------------------
auto CountAuthor(QQmlListProperty<Author>* list) -> int
{
  auto authorList = reinterpret_cast<QList<Author*>*>(list->data);
  if (authorList) {
    return authorList->length();
  }
  return 0;
}
auto AtAuthor(QQmlListProperty<Author>* list, int index) -> Author*
{
  auto authorList = reinterpret_cast<QList<Author*>*>(list->data);
  if (authorList) {
    return authorList->at(index);
  }
  return nullptr;
}
QQmlListProperty<Author> SQLite3Driver::getAuthors() const
{

  return QQmlListProperty<Author>(nullptr, new QList<Author*>(authors()),
                                  nullptr,
                                  &CountAuthor,
                                  &AtAuthor,
                                  nullptr);
}
//-------------------------------------------------------------------------------
auto CountProperty(QQmlListProperty<Property>* list) -> int
{
  auto propertyList = reinterpret_cast<QList<Property*>*>(list->data);
  if (propertyList) {
    return propertyList->length();
  }
  return 0;
}
auto AtProperty(QQmlListProperty<Property>* list, int index) -> Property*
{
  auto propertyList = reinterpret_cast<QList<Property*>*>(list->data);
  if (propertyList) {
    return propertyList->at(index);
  }
  return nullptr;
}
QString SQLite3Driver::getProperty(QString name) const
{
  Property property;
  property.name = name;
  select_property(&property);
  return property.value;
}
QQmlListProperty<Property> SQLite3Driver::getProperties() const
{
  return QQmlListProperty<Property>(nullptr, new QList<Property*>(properties()),
                                    nullptr,
                                    &CountProperty,
                                    &AtProperty,
                                    nullptr);
}
//-------------------------------------------------------------------------------
auto CountObjective(QQmlListProperty<Objective>* list) -> int
{
  auto objectiveList = reinterpret_cast<QList<Objective*>*>(list->data);
  if (objectiveList) {
    return objectiveList->length();
  }
  return 0;
}
auto AtObjective(QQmlListProperty<Objective>* list, int index) -> Objective*
{
  auto objectiveList = reinterpret_cast<QList<Objective*>*>(list->data);
  if (objectiveList) {
    return objectiveList->at(index);
  }
  return nullptr;
}
QQmlListProperty<Objective> SQLite3Driver::getObjectives() const
{

  return QQmlListProperty<Objective>(nullptr, new QList<Objective*>(objectives()),
                                     nullptr,
                                     &CountObjective,
                                     &AtObjective,
                                     nullptr);
}
//-------------------------------------------------------------------------------
auto CountCitation(QQmlListProperty<Citation>* list) -> int
{
  auto citationList = reinterpret_cast<QList<Citation*>*>(list->data);
  if (citationList) {
    return citationList->length();
  }
  return 0;
}
auto AtCitation(QQmlListProperty<Citation>* list, int index) -> Citation*
{
  auto citationList = reinterpret_cast<QList<Citation*>*>(list->data);
  if (citationList) {
    return citationList->at(index);
  }
  return nullptr;
}
QQmlListProperty<Citation> SQLite3Driver::getCitations() const
{

  return QQmlListProperty<Citation>(nullptr, new QList<Citation*>(citations()),
                                    nullptr,
                                    &CountCitation,
                                    &AtCitation,
                                    nullptr);
}
//-------------------------------------------------------------------------------
auto CountTreatment(QQmlListProperty<Treatment>* list) -> int
{
  auto treatmentList = reinterpret_cast<QList<Treatment*>*>(list->data);
  if (treatmentList) {
    return treatmentList->length();
  }
  return 0;
}
auto AtTreatment(QQmlListProperty<Treatment>* list, int index) -> Treatment*
{
  auto treatmentList = reinterpret_cast<QList<Treatment*>*>(list->data);
  if (treatmentList) {
    return treatmentList->at(index);
  }
  return nullptr;
}
QQmlListProperty<Treatment> SQLite3Driver::getTreatments() const
{

  return QQmlListProperty<Treatment>(nullptr, new QList<Treatment*>(treatments()),
                                     nullptr,
                                     &CountTreatment,
                                     &AtTreatment,
                                     nullptr);
}
//-------------------------------------------------------------------------------
auto CountEquipment(QQmlListProperty<Equipment>* list) -> int
{
  auto equipmentList = reinterpret_cast<QList<Equipment*>*>(list->data);
  if (equipmentList) {
    return equipmentList->length();
  }
  return 0;
}
auto AtEquipment(QQmlListProperty<Equipment>* list, int index) -> Equipment*
{
  auto equipmentList = reinterpret_cast<QList<Equipment*>*>(list->data);
  if (equipmentList) {
    return equipmentList->at(index);
  }
  return nullptr;
}
QQmlListProperty<Equipment> SQLite3Driver::getEquipment() const
{

  return QQmlListProperty<Equipment>(nullptr, new QList<Equipment*>(equipments()),
                                     nullptr,
                                     &CountEquipment,
                                     &AtEquipment,
                                     nullptr);
}
//-------------------------------------------------------------------------------
auto CountTrauma(QQmlListProperty<Trauma>* list) -> int
{
  auto traumaList = reinterpret_cast<QList<Trauma*>*>(list->data);
  if (traumaList) {
    return traumaList->length();
  }
  return 0;
}
auto AtTrauma(QQmlListProperty<Trauma>* list, int index) -> Trauma*
{
  auto traumaList = reinterpret_cast<QList<Trauma*>*>(list->data);
  if (traumaList) {
    return traumaList->at(index);
  }
  return nullptr;
}
QQmlListProperty<Trauma> SQLite3Driver::getTraumas() const
{

  return QQmlListProperty<Trauma>(nullptr, new QList<Trauma*>(traumas()),
                                  nullptr,
                                  &CountTrauma,
                                  &AtTrauma,
                                  nullptr);
}
//-------------------------------------------------------------------------------
auto CountTraumaProfiles(QQmlListProperty<TraumaProfile>* list) -> int
{
  auto traumaProfilesList = reinterpret_cast<QList<TraumaProfile*>*>(list->data);
  if (traumaProfilesList) {
    return traumaProfilesList->length();
  }
  return 0;
}
auto AtTraumaProfiles(QQmlListProperty<TraumaProfile>* list, int index) -> TraumaProfile*
{
  auto traumaProfilesList = reinterpret_cast<QList<TraumaProfile*>*>(list->data);
  if (traumaProfilesList) {
    return traumaProfilesList->at(index);
  }
  return nullptr;
}
QQmlListProperty<TraumaProfile> SQLite3Driver::getTraumaProfiles() const
{

  return QQmlListProperty<TraumaProfile>(nullptr, new QList<TraumaProfile*>(trauma_profiles()),
                                         nullptr,
                                         &CountTraumaProfiles,
                                         &AtTraumaProfiles,
                                         nullptr);
}
//-------------------------------------------------------------------------------
auto CountAssessment(QQmlListProperty<Assessment>* list) -> int
{
  auto assessmentList = reinterpret_cast<QList<Assessment*>*>(list->data);
  if (assessmentList) {
    return assessmentList->length();
  }
  return 0;
}
auto AtAssessment(QQmlListProperty<Assessment>* list, int index) -> Assessment*
{
  auto assessmentList = reinterpret_cast<QList<Assessment*>*>(list->data);
  if (assessmentList) {
    return assessmentList->at(index);
  }
  return nullptr;
}
QQmlListProperty<Assessment> SQLite3Driver::getAssessments() const
{

  return QQmlListProperty<Assessment>(nullptr, new QList<Assessment*>(assessments()),
                                      nullptr,
                                      &CountAssessment,
                                      &AtAssessment,
                                      nullptr);
}
//-------------------------------------------------------------------------------
auto CountLocation(QQmlListProperty<Location>* list) -> int
{
  auto locationList = reinterpret_cast<QList<Location*>*>(list->data);
  if (locationList) {
    return locationList->length();
  }
  return 0;
}
auto AtLocation(QQmlListProperty<Location>* list, int index) -> Location*
{
  auto locationList = reinterpret_cast<QList<Location*>*>(list->data);
  if (locationList) {
    return locationList->at(index);
  }
  return nullptr;
}
QQmlListProperty<Location> SQLite3Driver::getLocations() const
{

  return QQmlListProperty<Location>(nullptr, new QList<Location*>(locations()),
                                    nullptr,
                                    &CountLocation,
                                    &AtLocation,
                                    nullptr);
}
//-------------------------------------------------------------------------------
auto CountRoleMap(QQmlListProperty<RoleMap>* list) -> int
{
  auto roleMapList = reinterpret_cast<QList<RoleMap*>*>(list->data);
  if (roleMapList) {
    return roleMapList->length();
  }
  return 0;
}
auto AtRoleMap(QQmlListProperty<RoleMap>* list, int index) -> RoleMap*
{
  auto roleMapList = reinterpret_cast<QList<RoleMap*>*>(list->data);
  if (roleMapList) {
    return roleMapList->at(index);
  }
  return nullptr;
}
QQmlListProperty<RoleMap> SQLite3Driver::getRoleMaps() const
{

  return QQmlListProperty<RoleMap>(nullptr, new QList<RoleMap*>(role_maps()),
                                   nullptr,
                                   &CountRoleMap,
                                   &AtRoleMap,
                                   nullptr);
}
//-------------------------------------------------------------------------------
auto CountEventMap(QQmlListProperty<EventMap>* list) -> int
{
  auto eventMapList = reinterpret_cast<QList<EventMap*>*>(list->data);
  if (eventMapList) {
    return eventMapList->length();
  }
  return 0;
}
auto AtEventMap(QQmlListProperty<EventMap>* list, int index) -> EventMap*
{
  auto eventMapList = reinterpret_cast<QList<EventMap*>*>(list->data);
  if (eventMapList) {
    return eventMapList->at(index);
  }
  return nullptr;
}
QQmlListProperty<EventMap> SQLite3Driver::getEventMaps() const
{

  return QQmlListProperty<EventMap>(nullptr, new QList<EventMap*>(event_maps()),
                                    nullptr,
                                    &CountEventMap,
                                    &AtEventMap,
                                    nullptr);
}
//-------------------------------------------------------------------------------
auto CountLocationMap(QQmlListProperty<LocationMap>* list) -> int
{
  auto locationMapList = reinterpret_cast<QList<LocationMap*>*>(list->data);
  if (locationMapList) {
    return locationMapList->length();
  }
  return 0;
}
auto AtLocationMap(QQmlListProperty<LocationMap>* list, int index) -> LocationMap*
{
  auto locationMapList = reinterpret_cast<QList<LocationMap*>*>(list->data);
  if (locationMapList) {
    return locationMapList->at(index);
  }
  return nullptr;
}
QQmlListProperty<LocationMap> SQLite3Driver::getLocationMaps() const
{

  return QQmlListProperty<LocationMap>(nullptr, new QList<LocationMap*>(location_maps()),
                                       nullptr,
                                       &CountLocationMap,
                                       &AtLocationMap,
                                       nullptr);
}
//-------------------------------------------------------------------------------
auto CountCitationMap(QQmlListProperty<CitationMap>* list) -> int
{
  auto citationMapList = reinterpret_cast<QList<CitationMap*>*>(list->data);
  if (citationMapList) {
    return citationMapList->length();
  }
  return 0;
}
auto AtCitationMap(QQmlListProperty<CitationMap>* list, int index) -> CitationMap*
{
  auto citationMapList = reinterpret_cast<QList<CitationMap*>*>(list->data);
  if (citationMapList) {
    return citationMapList->at(index);
  }
  return nullptr;
}
QQmlListProperty<CitationMap> SQLite3Driver::getCitationMaps() const
{

  return QQmlListProperty<CitationMap>(nullptr, new QList<CitationMap*>(citation_maps()),
                                       nullptr,
                                       &CountCitationMap,
                                       &AtCitationMap,
                                       nullptr);
}
//-------------------------------------------------------------------------------
auto CountEquipmentMap(QQmlListProperty<EquipmentMap>* list) -> int
{
  auto equipmentMapList = reinterpret_cast<QList<EquipmentMap*>*>(list->data);
  if (equipmentMapList) {
    return equipmentMapList->length();
  }
  return 0;
}
auto AtEquipmentMap(QQmlListProperty<EquipmentMap>* list, int index) -> EquipmentMap*
{
  auto equipmentMapList = reinterpret_cast<QList<EquipmentMap*>*>(list->data);
  if (equipmentMapList) {
    return equipmentMapList->at(index);
  }
  return nullptr;
}
QQmlListProperty<EquipmentMap> SQLite3Driver::getEquipmentMaps() const
{

  return QQmlListProperty<EquipmentMap>(nullptr, new QList<EquipmentMap*>(equipment_maps()),
                                        nullptr,
                                        &CountEquipmentMap,
                                        &AtEquipmentMap,
                                        nullptr);
}
//-------------------------------------------------------------------------------
auto CountRole(QQmlListProperty<Role>* list) -> int
{
  auto roleMapList = reinterpret_cast<QList<Role*>*>(list->data);
  if (roleMapList) {
    return roleMapList->length();
  }
  return 0;
}
auto AtRole(QQmlListProperty<Role>* list, int index) -> Role*
{
  auto roleMapList = reinterpret_cast<QList<Role*>*>(list->data);
  if (roleMapList) {
    return roleMapList->at(index);
  }
  return nullptr;
}
QQmlListProperty<Role> SQLite3Driver::getRoles() const
{

  return QQmlListProperty<Role>(nullptr, new QList<Role*>(roles()),
                                nullptr,
                                &CountRole,
                                &AtRole,
                                nullptr);
}
QQmlListProperty<RoleMap> SQLite3Driver::getRolesInScene(Scene* scene) const
{

  return QQmlListProperty<RoleMap>(nullptr, new QList<RoleMap*>(roles_in_scene(scene)),
                                   nullptr,
                                   &CountRoleMap,
                                   &AtRoleMap,
                                   nullptr);
}
//-------------------------------------------------------------------------------
auto CountEvent(QQmlListProperty<Event>* list) -> int
{
  auto eventList = reinterpret_cast<QList<Event*>*>(list->data);
  if (eventList) {
    return eventList->length();
  }
  return 0;
}
auto AtEvent(QQmlListProperty<Event>* list, int index) -> Event*
{
  auto eventList = reinterpret_cast<QList<Event*>*>(list->data);
  if (eventList) {
    return eventList->at(index);
  }
  return nullptr;
}
QQmlListProperty<Event> SQLite3Driver::getEvents() const
{

  return QQmlListProperty<Event>(nullptr, new QList<Event*>(events()),
                                 nullptr,
                                 &CountEvent,
                                 &AtEvent,
                                 nullptr);
}
//-------------------------------------------------------------------------------
auto CountScene(QQmlListProperty<Scene>* list) -> int
{
  auto sceneList = reinterpret_cast<QList<Scene*>*>(list->data);
  if (sceneList) {
    return sceneList->length();
  }
  return 0;
}
auto AtScene(QQmlListProperty<Scene>* list, int index) -> Scene*
{
  auto sceneList = reinterpret_cast<QList<Scene*>*>(list->data);
  if (sceneList) {
    return sceneList->at(index);
  }
  return nullptr;
}
QQmlListProperty<Scene> SQLite3Driver::getScenes() const
{

  return QQmlListProperty<Scene>(nullptr, new QList<Scene*>(scenes()),
                                 nullptr,
                                 &CountScene,
                                 &AtScene,
                                 nullptr);
}
//-------------------------------------------------------------------------------
auto CountImage(QQmlListProperty<Image>* list) -> int
{
  auto sceneList = reinterpret_cast<QList<Image*>*>(list->data);
  if (sceneList) {
    return sceneList->length();
  }
  return 0;
}
auto AtImage(QQmlListProperty<Image>* list, int index) -> Image*
{
  auto sceneList = reinterpret_cast<QList<Image*>*>(list->data);
  if (sceneList) {
    return sceneList->at(index);
  }
  return nullptr;
}
QQmlListProperty<Image> SQLite3Driver::getImages() const
{

  return QQmlListProperty<Image>(nullptr, new QList<Image*>(images()),
                                 nullptr,
                                 &CountImage,
                                 &AtImage,
                                 nullptr);
}
//-------------------------------------------------------------------------------
}
