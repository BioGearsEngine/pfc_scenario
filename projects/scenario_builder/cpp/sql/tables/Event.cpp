#include "Event.h"

#include "Equipment.h"
#include "Role.h"

Event::Event(QObject* parent)
  : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------
bool Event::operator==(const Event& rhs) const
{
  return name == rhs.name
    && description == rhs.description
    && category == rhs.category
    && fidelity == rhs.fidelity
    && fk_actor_1 == rhs.fk_actor_1
    && fk_actor_2 == rhs.fk_actor_2
    && fk_equipment == rhs.fk_equipment
    && details == rhs.details;
}
//--------------------------------------------------------------------------------------------
bool Event::operator!=(const Event& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
Event* Event::make()
{
  return new Event();
}
//--------------------------------------------------------------------------------------------
void Event::assign(Event* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//--------------------------------------------------------------------------------------------
void Event::assign(const Event& rhs)
{
  uuid = rhs.uuid;
  name = rhs.name;
  description = rhs.description;
  category = rhs.category;
  fidelity = rhs.fidelity;
  fk_actor_1 = rhs.fk_actor_1;
  fk_actor_2 = rhs.fk_actor_2;
  fk_equipment = rhs.fk_equipment;
  details = rhs.details;
}
//--------------------------------------------------------------------------------------------
void Event::clear()
{
  id = -1;
  uuid.clear();
  name.clear();
  description.clear();
  category.clear();
  fidelity.clear();
  fk_actor_1->clear();
  fk_actor_2->clear();
  fk_equipment->clear();
  details.clear();
}
//--------------------------------------------------------------------------------------------