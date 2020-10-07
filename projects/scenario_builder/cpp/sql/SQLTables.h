#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_H

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

#include <QList>
#include <QObject>
#include <QString>

namespace pfc {
struct Author : public QObject {
  Q_OBJECT
public:
  int32_t id = -1;
  QString first = "";
  QString middle = "";
  QString last = "";
  QString email = "";
  QString zip = "";
  QString plus_4 = "";
  QString state = "";
  QString country = "";
  QString phone = "";
  QString organization = "";
  Q_PROPERTY(int author_id MEMBER id)
  Q_PROPERTY(QString first MEMBER first)
  Q_PROPERTY(QString middle MEMBER middle)
  Q_PROPERTY(QString last MEMBER last)
  Q_PROPERTY(QString email MEMBER email)
  Q_PROPERTY(QString zip MEMBER zip)
  Q_PROPERTY(QString plus_4 MEMBER plus_4)
  Q_PROPERTY(QString state MEMBER state)
  Q_PROPERTY(QString country MEMBER country)
  Q_PROPERTY(QString phone MEMBER phone)
  Q_PROPERTY(QString organization MEMBER organization)

  Author(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  Author(const Author&) = delete;
  Author(Author&&) = delete;
  Author& operator=(const Author&) = delete;
  Author& operator=(Author&&) = delete;
  virtual ~Author() = default;

  bool operator==(const Author& rhs) const
  {
    return id == rhs.id
      && first == rhs.first
      && middle == rhs.middle
      && last == rhs.last
      && email == rhs.email
      && zip == rhs.zip
      && plus_4 == rhs.plus_4
      && state == rhs.state
      && country == rhs.country
      && phone == rhs.phone
      && organization == rhs.organization;
  }
  bool operator!=(const Author& rhs) const
  {
    return !(*this == rhs);
  }

  void assign(const Author& rhs)
  {
    id = rhs.id;
    first = rhs.first;
    middle = rhs.middle;
    last = rhs.last;
    email = rhs.email;
    zip = rhs.zip;
    plus_4 = rhs.plus_4;
    state = rhs.state;
    country = rhs.country;
    phone = rhs.phone;
    organization = rhs.organization;
  }
};
//----End Author
struct Assessment : public QObject {
  Q_OBJECT
  Q_PROPERTY(int assessment_id MEMBER id)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(QString description MEMBER description)
  Q_PROPERTY(QString type MEMBER type)
  Q_PROPERTY(QString objective MEMBER objective)
  Q_PROPERTY(int available_points MEMBER available_points)
  Q_PROPERTY(QString criteria MEMBER criteria)
public:
  int32_t id = -1;
  QString name = "";
  QString description = "";
  QString type = "";
  QString objective = ""; // Forign Key to Objective this Assesses
  int32_t available_points = -1;
  QString criteria = "";
  Assessment(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  Assessment(const Assessment&) = delete;
  Assessment(Assessment&&) = delete;
  Assessment& operator=(const Assessment&) = delete;
  Assessment& operator=(Assessment&&) = delete;
  virtual ~Assessment() = default;

  bool operator==(const Assessment& rhs) const
  {
    return id == rhs.id
      && name == rhs.name
      && description == rhs.description
      && type == rhs.type
      && objective == rhs.objective
      && available_points == rhs.available_points
      && criteria == rhs.criteria;
  }
  bool operator!=(const Assessment& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const Assessment& rhs)
  {
    id = rhs.id;
    name = rhs.name;
    description = rhs.description;
    type = rhs.type;
    available_points = rhs.available_points;
    objective = rhs.objective;
    criteria = rhs.criteria;
  }
};
//----End Assessment
struct Citation : public QObject {
  Q_OBJECT
  Q_PROPERTY(int citation_id MEMBER id)
  Q_PROPERTY(QString key MEMBER key)
  Q_PROPERTY(QString title MEMBER title)
  Q_PROPERTY(QString authors MEMBER authors)
  Q_PROPERTY(QString year MEMBER year)
  Q_PROPERTY(QString publisher MEMBER publisher)
public:
  int32_t id = -1;
  QString key = "";
  QString title = "";
  QString authors;
  QString year = "";
  QString publisher = "";
  Citation(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  Citation(const Citation&) = delete;
  Citation(Citation&&) = delete;
  Citation& operator=(const Citation&) = delete;
  Citation& operator=(Citation&&) = delete;
  virtual ~Citation() = default;

  bool operator==(const Citation& rhs) const
  {
    return id == rhs.id
      && key == rhs.key
      && title == rhs.title
      && authors == rhs.authors
      && year == rhs.year
      && publisher == rhs.publisher;
  }
  bool operator!=(const Citation& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const Citation& rhs)
  {
    id = rhs.id;
    key = rhs.key;
    title = rhs.title;
    authors = rhs.authors;
    year = rhs.year;
    publisher = rhs.publisher;
  }
};
//----End Citation

struct Event : public QObject {
  Q_OBJECT
  Q_PROPERTY(int event_id MEMBER id)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(QString description MEMBER description)
  Q_PROPERTY(QString category MEMBER category)
  Q_PROPERTY(QString fidelity MEMBER fidelity)
  Q_PROPERTY(QString actor_1 MEMBER fk_actor_1)
  Q_PROPERTY(QString actor_2 MEMBER fk_actor_2)
  Q_PROPERTY(QString equipmet MEMBER equipment)
  Q_PROPERTY(QString details MEMBER details)
public:
  int32_t id = -1;
  QString name = "";
  QString description = "";
  QString category = "";
  QString fidelity = "";
  QString fk_actor_1 = "";
  QString fk_actor_2 = "";
  QString equipment;
  QString details = "";

  Event(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  Event(const Event&) = delete;
  Event(Event&&) = delete;
  Event& operator=(const Event&) = delete;
  Event& operator=(Event&&) = delete;
  virtual ~Event() = default;

  bool operator==(const Event& rhs) const
  {
    return id == rhs.id
      && name == rhs.name
      && description == rhs.description
      && category == rhs.category
      && fidelity == rhs.fidelity
      && fk_actor_1 == rhs.fk_actor_1
      && fk_actor_2 == rhs.fk_actor_2
      && equipment == rhs.equipment
      && details == rhs.details;
  }
  bool operator!=(const Event& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const Event& rhs)
  {
    id = rhs.id;
    name = rhs.name;
    description = rhs.description;
    category = rhs.category;
    fidelity = rhs.fidelity;
    fk_actor_1 = rhs.fk_actor_1;
    fk_actor_2 = rhs.fk_actor_2;
    equipment = rhs.equipment;
    details = rhs.details;
  }
};
//----End Event
struct Equipment : public QObject {
  Q_OBJECT
  Q_PROPERTY(int equipment_id MEMBER id)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(int type MEMBER type)
  Q_PROPERTY(QString description MEMBER description)
  Q_PROPERTY(QString citations MEMBER citations)
  Q_PROPERTY(QString image MEMBER image)
  Q_PROPERTY(QString properties MEMBER properties)
public:
  int32_t id = -1;
  int32_t type = 1;
  QString name = "";
  QString description = "";
  QString citations;
  QString image = "";
  QString properties = "";
  Equipment(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  Equipment(const Equipment&) = delete;
  Equipment(Equipment&&) = delete;
  Equipment& operator=(const Equipment&) = delete;
  Equipment& operator=(Equipment&&) = delete;
  virtual ~Equipment() = default;

  bool operator==(const Equipment& rhs) const
  {
    return id == rhs.id
      && name == rhs.name
      && type == rhs.type
      && description == rhs.description
      && citations == rhs.citations
      && image == rhs.image
      && properties == rhs.properties;
  }
  bool operator!=(const Equipment& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const Equipment& rhs)
  {
    id = rhs.id;
    name = rhs.name;
    type = rhs.type;
    description = rhs.description;
    citations = rhs.citations;
    image = rhs.image;
    properties = rhs.properties;
  }
  void clear()
  {
    id = -1;
    name.clear();
    type = -1;
    description.clear();
    citations.clear();
    image.clear();
    properties.clear();
  }
};
//----End Equipment

struct Injury : public QObject {
  Q_OBJECT
  Q_PROPERTY(int injury_id MEMBER id)
  Q_PROPERTY(QString medical_name MEMBER medical_name)
  Q_PROPERTY(QString common_name MEMBER common_name)
  Q_PROPERTY(QString description MEMBER description)
  Q_PROPERTY(QString citations MEMBER citations)
  Q_PROPERTY(float min MEMBER lower_bound)
  Q_PROPERTY(float max MEMBER upper_bound)
public:
  int32_t id = -1;
  QString medical_name = "";
  QString common_name = "";
  QString description = "";
  QString citations;
  float lower_bound = 0;
  float upper_bound = 1;
  Injury(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  Injury(const Injury&) = delete;
  Injury(Injury&&) = delete;
  Injury& operator=(const Injury&) = delete;
  Injury& operator=(Injury&&) = delete;
  virtual ~Injury() = default;

  bool operator==(const Injury& rhs) const
  {
    return id == rhs.id
      && medical_name == rhs.medical_name
      && common_name == rhs.common_name
      && description == rhs.description
      && citations == rhs.citations
      && lower_bound == rhs.lower_bound
      && upper_bound == rhs.upper_bound;
  }
  bool operator!=(const Injury& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const Injury& rhs)
  {
    id = rhs.id;
    medical_name = rhs.medical_name;
    common_name = rhs.common_name;
    description = rhs.description;
    lower_bound = rhs.lower_bound;
    upper_bound = rhs.upper_bound;
    citations = rhs.citations;
  }
};
//----End Injury
struct InjurySet : public QObject {
  Q_OBJECT
  Q_PROPERTY(int injury_set_id MEMBER id)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(QString description MEMBER description)
  Q_PROPERTY(QString injuries MEMBER injuries)
  Q_PROPERTY(QString locations MEMBER locations)
  Q_PROPERTY(QString severities MEMBER severities)
  Q_PROPERTY(QString physiology_file MEMBER physiology_file)
  Q_PROPERTY(QString treatments MEMBER treatments)

public:
  int32_t id = -1;
  QString name = "";
  QString description = "";
  QString injuries = "";
  QString locations = "";
  QString severities = "";
  QString physiology_file = "";
  QString treatments = "";

  InjurySet(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  InjurySet(const InjurySet&) = delete;
  InjurySet(InjurySet&&) = delete;
  InjurySet& operator=(const InjurySet&) = delete;
  InjurySet& operator=(InjurySet&&) = delete;
  virtual ~InjurySet() = default;

  bool operator==(const InjurySet& rhs) const
  {
    return id == rhs.id
      && name == rhs.name
      && description == rhs.description
      && injuries == rhs.injuries
      && locations == rhs.locations
      && severities == rhs.severities
      && physiology_file == rhs.physiology_file
      && treatments == rhs.treatments;
  }
  bool operator!=(const InjurySet& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const InjurySet& rhs)
  {
    id = rhs.id;
    name = rhs.name;
    description = rhs.description;
    injuries = rhs.injuries;
    locations = rhs.locations;
    severities = rhs.severities;
    physiology_file = rhs.physiology_file;
    treatments = rhs.treatments;
  }
};
//----End Injury Set
struct Location : public QObject {
  Q_OBJECT
  Q_PROPERTY(int location_id MEMBER id)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(QString scene_name MEMBER scene_name)
  Q_PROPERTY(QString time_of_day MEMBER time_of_day)
  Q_PROPERTY(QString environment MEMBER environment)

public:
  int32_t id = -1;
  QString name = "";
  QString scene_name = "";
  QString time_of_day = "";
  QString environment = "";

  Location(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  Location(const Location&) = delete;
  Location(Location&&) = delete;
  Location& operator=(const Location&) = delete;
  Location& operator=(Location&&) = delete;
  virtual ~Location() = default;

  bool operator==(const Location& rhs) const
  {
    return id == rhs.id
      && name == rhs.name
      && scene_name == rhs.scene_name
      && time_of_day == rhs.time_of_day
      && environment == rhs.environment;
  }
  bool operator!=(const Location& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const Location& rhs)
  {
    id = rhs.id;
    name = rhs.name;
    scene_name = rhs.scene_name;
    time_of_day = rhs.time_of_day;
    environment = rhs.environment;
  }
};
//----End Location
struct Objective : public QObject {
  Q_OBJECT
  Q_PROPERTY(int objective_id MEMBER id)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(QString description MEMBER description)
  Q_PROPERTY(QString citations MEMBER citations)
  Q_PROPERTY(QString cpgs MEMBER cpgs)
  Q_PROPERTY(QString treatment_plans MEMBER treatment_plans)
  Q_PROPERTY(QString injury_profiles MEMBER injury_profiles)
public:
  int32_t id = -1;
  QString name = "";
  QString description = "";
  QString citations;
  QString cpgs;
  QString treatment_plans;
  QString injury_profiles;

  Objective(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  Objective(const Objective&) = delete;
  Objective(Objective&&) = delete;
  Objective& operator=(const Objective&) = delete;
  Objective& operator=(Objective&&) = delete;
  virtual ~Objective() = default;

  bool operator==(const Objective& rhs) const
  {
    return id == rhs.id
      && name == rhs.name
      && description == rhs.description
      && citations == rhs.citations
      && cpgs == rhs.cpgs
      && treatment_plans == rhs.treatment_plans
      && injury_profiles == rhs.injury_profiles;
  }
  bool operator!=(const Objective& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const Objective& rhs)
  {
    id = rhs.id;
    name = rhs.name;
    description = rhs.description;
    citations = rhs.citations;
    cpgs = rhs.cpgs;
    treatment_plans = rhs.treatment_plans;
    injury_profiles = rhs.injury_profiles;
  }
};
//----End Objective
struct Property : public QObject {
  Q_OBJECT
  Q_PROPERTY(int property_id MEMBER id)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(QString value MEMBER value)
public:
  int32_t id = -1;
  QString name = "";
  QString value = "";

  Property(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  Property(const Property&) = delete;
  Property(Property&&) = delete;
  Property& operator=(const Property&) = delete;
  Property& operator=(Property&&) = delete;
  virtual ~Property() = default;

  bool operator==(const Property& rhs) const
  {
    return id == rhs.id
      && name == rhs.name
      && value == rhs.value;
  }
  bool operator!=(const Property& rhs) const
  {
    return !(*this == rhs);
  }

  void assign(const Property& rhs)
  {
    id = rhs.id;
    name = rhs.name;
    value = rhs.value;
  }
};
//----End Property
struct Restriction : public QObject {
  Q_OBJECT
  Q_PROPERTY(int restriction_id MEMBER id)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(QString value MEMBER value)
public:
  int32_t id = -1;
  QString name = "";
  QString value = "";

  Restriction(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  Restriction(const Restriction&) = delete;
  Restriction(Restriction&&) = delete;
  Restriction& operator=(const Restriction&) = delete;
  Restriction& operator=(Restriction&&) = delete;
  virtual ~Restriction() = default;

  bool operator==(const Restriction& rhs) const
  {
    return id == rhs.id
      && name == rhs.name
      && value == rhs.value;
  }
  bool operator!=(const Restriction& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const Restriction& rhs)
  {
    id = rhs.id;
    name = rhs.name;
    value = rhs.value;
  }
};
//----End Restriction
struct Role : public QObject {
  Q_OBJECT
  Q_PROPERTY(int role_id MEMBER id)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(QString description MEMBER description)
  Q_PROPERTY(QString short_name MEMBER description)
  Q_PROPERTY(QString trauma_profile MEMBER trauma_profile)
public:
  int32_t id = -1;
  QString name = "";
  QString short_name = "";
  QString description = "";
  QString trauma_profile = "";
  Role(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  Role(const Role&) = delete;
  Role(Role&&) = delete;
  Role& operator=(const Role&) = delete;
  Role& operator=(Role&&) = delete;
  virtual ~Role() = default;

  bool operator==(const Role& rhs) const
  {
    return id == rhs.id
      && name == rhs.name
      && description == rhs.description
      && short_name == rhs.short_name
      && trauma_profile == rhs.trauma_profile;
  }
  bool operator!=(const Role& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const Role& rhs)
  {
    id = rhs.id;
    name = rhs.name;
    description = rhs.description;
    short_name = rhs.short_name;
    trauma_profile = rhs.trauma_profile;
  }
};
//----End Role
struct Scene : public QObject {
  Q_OBJECT
  Q_PROPERTY(int scene_id MEMBER id)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(QString description MEMBER description)
  Q_PROPERTY(QString time_of_day MEMBER time_of_day)
  Q_PROPERTY(int time_in_simulation MEMBER time_in_simulation)
  Q_PROPERTY(QString weather MEMBER weather)
  Q_PROPERTY(QString events MEMBER events)
  Q_PROPERTY(QString items MEMBER items)
  Q_PROPERTY(QString roles MEMBER roles)
  Q_PROPERTY(QString details MEMBER details)
public:
  int32_t id = -1;
  QString name = "";
  QString description = "";
  QString time_of_day = "";
  int32_t time_in_simulation = 0;
  QString weather = "";
  QString events = "";
  QString items = "";
  QString roles = "";
  QString details = "";

  Scene(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  Scene(const Scene&) = delete;
  Scene(Scene&&) = delete;
  Scene& operator=(const Scene&) = delete;
  Scene& operator=(Scene&&) = delete;
  virtual ~Scene() = default;

  bool operator==(const Scene& rhs) const
  {
    return id == rhs.id
      && name == rhs.name
      && description == rhs.description
      && time_of_day == rhs.time_of_day
      && time_in_simulation == rhs.time_in_simulation
      && weather == rhs.weather
      && events == rhs.events
      && items == rhs.items
      && roles == rhs.roles
      && details == rhs.details;
  }
  bool operator!=(const Scene& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const Scene& rhs)
  {
    id = rhs.id;
    name = rhs.name;
    description = rhs.description;
    time_of_day = rhs.time_of_day;
    time_in_simulation = rhs.time_in_simulation;
    weather = rhs.weather;
    events = rhs.events;
    items = rhs.items;
    roles = rhs.roles;
    details = rhs.details;
  }
  void clear()
  {
    id = -1;
    name.clear();
    description.clear();
    time_of_day.clear();
    time_in_simulation = 0;
    weather.clear();
    events.clear();
    items.clear();
    roles.clear();
    details.clear();
  }
};
//----End Scene
struct Treatment : public QObject {
  Q_OBJECT
  Q_PROPERTY(int treatment_id MEMBER id)
  Q_PROPERTY(QString medical_name MEMBER medical_name)
  Q_PROPERTY(QString common_name MEMBER common_name)
  Q_PROPERTY(QString description MEMBER description)
  Q_PROPERTY(QString equipment MEMBER equipment)
  Q_PROPERTY(QString citations MEMBER citations)
  Q_PROPERTY(QString cpgs MEMBER citations)
public:
  int32_t id = -1;
  QString medical_name = "";
  QString common_name = "";
  QString description = "";
  QString equipment;
  QString citations;
  QString cpgs;

  Treatment(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  Treatment(const Treatment&) = delete;
  Treatment(Treatment&&) = delete;
  Treatment& operator=(const Treatment&) = delete;
  Treatment& operator=(Treatment&&) = delete;
  virtual ~Treatment() = default;

  bool operator==(const Treatment& rhs) const
  {
    return id == rhs.id
      && medical_name == rhs.medical_name
      && common_name == rhs.common_name
      && description == rhs.description
      && equipment == rhs.equipment
      && citations == rhs.citations
      && cpgs == rhs.cpgs;
  }
  bool operator!=(const Treatment& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const Treatment& rhs)
  {
    id = rhs.id;
    medical_name = rhs.medical_name;
    common_name = rhs.common_name;
    description = rhs.description;
    equipment = rhs.equipment;
    citations = rhs.citations;
    cpgs = rhs.cpgs;
  }
  void clear()
  {
    id = -1;
    medical_name.clear();
    common_name.clear();
    description.clear();
    equipment.clear();
    citations.clear();
    cpgs.clear();
  }
};
//----End Treatment

//!
//!  Map Types.
//!  Version 2.0 Will contain Pointers to the
//!  Classes they refer to
//!  Allowing them to be used like join statments

struct CitationMap : public QObject {
  Q_OBJECT
  Q_PROPERTY(int map_id MEMBER id)
  Q_PROPERTY(int fk_scene MEMBER fk_scene)
  Q_PROPERTY(int fk_citation MEMBER fk_citation)

public:
  int32_t id = -1;
  int32_t fk_scene = -1;
  int32_t fk_citation = -1;

  CitationMap(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  CitationMap(const CitationMap&) = delete;
  CitationMap(CitationMap&&) = delete;
  CitationMap& operator=(const CitationMap&) = delete;
  CitationMap& operator=(CitationMap&&) = delete;
  virtual ~CitationMap() = default;

  bool operator==(const CitationMap& rhs) const
  {
    return id == rhs.id
      && fk_scene == rhs.fk_scene
      && fk_citation == rhs.fk_citation;
  }
  bool operator!=(const CitationMap& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const CitationMap& rhs)
  {
    id = rhs.id;
    fk_scene = rhs.fk_scene;
    fk_citation = rhs.fk_citation;
  }
};
//----End Citation_Map
struct EventMap : public QObject {
  Q_OBJECT
  Q_PROPERTY(int map_id MEMBER id)
  Q_PROPERTY(int fk_scene MEMBER fk_scene)
  Q_PROPERTY(int fk_event MEMBER fk_event)

public:
  int32_t id = -1;
  int32_t fk_scene = -1;
  int32_t fk_event = -1;

  EventMap(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  EventMap(const EventMap&) = delete;
  EventMap(EventMap&&) = delete;
  EventMap& operator=(const EventMap&) = delete;
  EventMap& operator=(EventMap&&) = delete;
  virtual ~EventMap() = default;

  bool operator==(const EventMap& rhs) const
  {
    return id == rhs.id
      && fk_scene == rhs.fk_scene
      && fk_event == rhs.fk_event;
  }
  bool operator!=(const EventMap& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const EventMap& rhs)
  {
    id = rhs.id;
    fk_scene = rhs.fk_scene;
    fk_event = rhs.fk_event;
  }
};
//----End Event_Map
struct EquipmentMap : public QObject {

  Q_OBJECT
  Q_PROPERTY(int equipment_id MEMBER id)
  Q_PROPERTY(Scene* scene MEMBER scene)
  Q_PROPERTY(Equipment* equipment MEMBER equipment)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(QString values MEMBER property_values)
  Q_PROPERTY(QString notes MEMBER notes)
public:
  int32_t id = -1;
  Scene* scene = nullptr;
  Equipment* equipment = nullptr;
  QString name = "";
  QString property_values = "";
  QString notes;
  ;
  EquipmentMap(QObject* parent = nullptr)
    : QObject(parent)
    , scene(new Scene(this))
    , equipment(new Equipment(this))
  {
  }
  EquipmentMap(Scene* s, Equipment* e, QObject* parent = nullptr)
    : QObject(parent)
    , scene(s)
    , equipment(e)
  {
  }

  EquipmentMap(const EquipmentMap&) = delete;
  EquipmentMap(EquipmentMap&&) = delete;
  EquipmentMap& operator=(const EquipmentMap&) = delete;
  EquipmentMap& operator=(EquipmentMap&&) = delete;
  virtual ~EquipmentMap() = default;

  bool operator==(const EquipmentMap& rhs) const
  {
    return id == rhs.id
      && name == rhs.name
      && *scene == *rhs.scene
      && *equipment == *rhs.equipment
      && property_values == rhs.property_values
      && notes == rhs.notes;
  }
  bool operator!=(const EquipmentMap& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const EquipmentMap& rhs)
  {
    //!
    //!  TODO: Clone Scene and Equipment if non null to avoid
    //!        Garbage Collection

    id = rhs.id;
    name = rhs.name;
    scene->assign(*rhs.scene);
    equipment->assign(*rhs.equipment);
    property_values = rhs.property_values;
    notes = rhs.notes;
  }
  void clear()
  {
    id = -1;
    name.clear();
    scene->clear();
    equipment->clear();
    property_values.clear();
    notes.clear();
  }
};
//----End EquipmentMap
struct LocationMap : public QObject {
  Q_OBJECT
  Q_PROPERTY(int map_id MEMBER id)
  Q_PROPERTY(int fk_scene MEMBER fk_scene)
  Q_PROPERTY(int fk_location MEMBER fk_location)

public:
  int32_t id = -1;
  int32_t fk_scene = -1;
  int32_t fk_location = -1;

  LocationMap(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  LocationMap(const LocationMap&) = delete;
  LocationMap(LocationMap&&) = delete;
  LocationMap& operator=(const LocationMap&) = delete;
  LocationMap& operator=(LocationMap&&) = delete;
  virtual ~LocationMap() = default;

  bool operator==(const LocationMap& rhs) const
  {
    return id == rhs.id
      && fk_scene == rhs.fk_scene
      && fk_location == rhs.fk_location;
  }
  bool operator!=(const LocationMap& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const LocationMap& rhs)
  {
    id = rhs.id;
    fk_scene = rhs.fk_scene;
    fk_location = rhs.fk_location;
  }
};
//----End Location_Map
struct RoleMap : public QObject {
  Q_OBJECT
  Q_PROPERTY(int map_id MEMBER id)
  Q_PROPERTY(int fk_scene MEMBER fk_scene)
  Q_PROPERTY(int fk_role MEMBER fk_role)

public:
  int32_t id = -1;
  int32_t fk_scene = -1;
  int32_t fk_role = -1;

  RoleMap(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  RoleMap(const RoleMap&) = delete;
  RoleMap(RoleMap&&) = delete;
  RoleMap& operator=(const RoleMap&) = delete;
  RoleMap& operator=(RoleMap&&) = delete;
  virtual ~RoleMap() = default;

  bool operator==(const RoleMap& rhs) const
  {
    return id == rhs.id
      && fk_scene == rhs.fk_scene
      && fk_role == rhs.fk_role;
  }
  bool operator!=(const RoleMap& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const RoleMap& rhs)
  {
    id = rhs.id;
    fk_scene = rhs.fk_scene;
    fk_role = rhs.fk_role;
  }
};
//----End Role Map
struct PropMap : public QObject {
  Q_OBJECT
  Q_PROPERTY(int map_id MEMBER id)
  Q_PROPERTY(int fk_scene MEMBER fk_scene)
  Q_PROPERTY(int fk_prop MEMBER fk_prop)

public:
  int32_t id = -1;
  int32_t fk_scene = -1;
  int32_t fk_prop = -1;

  PropMap(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  PropMap(const PropMap&) = delete;
  PropMap(PropMap&&) = delete;
  PropMap& operator=(const PropMap&) = delete;
  PropMap& operator=(PropMap&&) = delete;
  virtual ~PropMap() = default;

  bool operator==(const PropMap& rhs) const
  {
    return id == rhs.id
      && fk_scene == rhs.fk_scene
      && fk_prop == rhs.fk_prop;
  }
  bool operator!=(const PropMap& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const PropMap& rhs)
  {
    id = rhs.id;
    fk_scene = rhs.fk_scene;
    fk_prop = rhs.fk_prop;
  }
};
//----End Prop_Map
struct RestrictionMap : public QObject {
  Q_OBJECT
  Q_PROPERTY(int map_id MEMBER id)
  Q_PROPERTY(int fk_scene MEMBER fk_scene)
  Q_PROPERTY(int fk_restriction MEMBER fk_restriction)

public:
  int32_t id = -1;
  int32_t fk_scene = -1;
  int32_t fk_restriction = -1;

  RestrictionMap(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  RestrictionMap(const RestrictionMap&) = delete;
  RestrictionMap(RestrictionMap&&) = delete;
  RestrictionMap& operator=(const RestrictionMap&) = delete;
  RestrictionMap& operator=(RestrictionMap&&) = delete;
  virtual ~RestrictionMap() = default;

  bool operator==(const RestrictionMap& rhs) const
  {
    return id == rhs.id
      && fk_scene == rhs.fk_scene
      && fk_restriction == rhs.fk_restriction;
  }
  bool operator!=(const RestrictionMap& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const RestrictionMap& rhs)
  {
    id = rhs.id;
    fk_scene = rhs.fk_scene;
    fk_restriction = rhs.fk_restriction;
  }
};
//----End Restriction_Map
}

#endif
