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
  Q_PROPERTY(int available_points MEMBER available_points)
  Q_PROPERTY(QString criteria MEMBER criteria)
public:
  int32_t id = -1;
  QString name = "";
  QString description = "";
  QString type = "";
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
    criteria = rhs.criteria;
  }
};
//----End Assessment
struct Event : public QObject {
  Q_OBJECT
  Q_PROPERTY(int prop_id MEMBER id)
public:
  int32_t id = -1;

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
    return id == rhs.id;
  }
  bool operator!=(const Event& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const Event& rhs)
  {
    id = rhs.id;
  }
};
//----End Event
struct Equipment : public QObject {
  Q_OBJECT
  Q_PROPERTY(int equipment_id MEMBER id)
  Q_PROPERTY(QString name MEMBER name)
  Q_PROPERTY(QString description MEMBER description)
  Q_PROPERTY(QList<QString> equipment_list MEMBER equipment_list)
  Q_PROPERTY(QList<int> citations MEMBER citations)
public:
  int32_t id = -1;
  QString name = "";
  QString description = "";
  QList<QString> equipment_list;
  QList<int> citations;
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
      && description == rhs.description
      && equipment_list == rhs.equipment_list
      && citations == rhs.citations;
  }
  bool operator!=(const Equipment& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const Equipment& rhs)
  {
    id = rhs.id;
    name = rhs.name;
    description = rhs.description;
    equipment_list = rhs.equipment_list;
    citations = rhs.citations;
  }
};
//----End Equipment
struct Injury : public QObject {
  Q_OBJECT
  Q_PROPERTY(int injury_id MEMBER id)
  Q_PROPERTY(QString medical_name MEMBER medical_name)
  Q_PROPERTY(QString common_name MEMBER common_name)
  Q_PROPERTY(QString description MEMBER description)
  Q_PROPERTY(QList<QString> equipment_list MEMBER equipment_list)
  Q_PROPERTY(QList<int> citations MEMBER citations)
public:
  int32_t id = -1;
  QString medical_name = "";
  QString common_name = "";
  QString description = "";
  QList<QString> equipment_list;
  QList<int> citations;
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
      && equipment_list == rhs.equipment_list
      && citations == rhs.citations;
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
    equipment_list = rhs.equipment_list;
    citations = rhs.citations;
  }
};
//----End Injury
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
  Q_PROPERTY(QList<int> citations MEMBER citations)
public:
  int32_t id = -1;
  QString name = "";
  QString description = "";
  QList<int> citations;

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
      && citations == rhs.citations;
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
struct Prop : public QObject {
  Q_OBJECT
  Q_PROPERTY(int prop_id MEMBER id)
  Q_PROPERTY(QString equipment MEMBER equipment)
public:
  int32_t id = -1;
  QString equipment = "";

  Prop(QObject* parent = nullptr)
    : QObject(parent)
  {
  }
  Prop(const Prop&) = delete;
  Prop(Prop&&) = delete;
  Prop& operator=(const Prop&) = delete;
  Prop& operator=(Prop&&) = delete;
  virtual ~Prop() = default;

  bool operator==(const Prop& rhs) const
  {
    return id == rhs.id
      && equipment == rhs.equipment;
  }
  bool operator!=(const Prop& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const Prop& rhs)
  {
    id = rhs.id;
    equipment = rhs.equipment;
  }
};

//----End Prop
struct Citation : public QObject {
  Q_OBJECT
  Q_PROPERTY(int citation_id MEMBER id)
  Q_PROPERTY(QString key MEMBER key)
  Q_PROPERTY(QString title MEMBER title)
  Q_PROPERTY(QList<QString> authors MEMBER authors)
  Q_PROPERTY(QString year MEMBER year)
  Q_PROPERTY(QString publisher MEMBER publisher)
public:
  int32_t id = -1;
  QString key = "";
  QString title = "";
  QList<QString> authors;
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
  Q_PROPERTY(QString description MEMBER description)
public:
  int32_t id = -1;
  QString description = "";

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
      && description == rhs.description;
  }
  bool operator!=(const Role& rhs) const
  {
    return !(*this == rhs);
  }
  void assign(const Role& rhs)
  {
    id = rhs.id;
    description = rhs.description;
  }
};
//----End Role
struct Treatment : public QObject {
  Q_OBJECT
  Q_PROPERTY(int treatment_id MEMBER id)
  Q_PROPERTY(QString medical_name MEMBER medical_name)
  Q_PROPERTY(QString common_name MEMBER common_name)
  Q_PROPERTY(QString description MEMBER description)
  Q_PROPERTY(QList<QString> equipment_list MEMBER equipment_list)
  Q_PROPERTY(QList<int> citations MEMBER citations)
public:
  int32_t id = -1;
  QString medical_name = "";
  QString common_name = "";
  QString description = "";
  QList<QString> equipment_list;
  QList<int> citations;
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
      && equipment_list == rhs.equipment_list
      && citations == rhs.citations;
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
    equipment_list = rhs.equipment_list;
    citations = rhs.citations;
  }
};
//----End Treatment
}

#endif