#include "Location.h"

Location::Location(QObject* parent)
  : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------
bool Location::operator==(const Location& rhs) const
{
  return name == rhs.name
    && scene_name == rhs.scene_name
    && time_of_day == rhs.time_of_day
    && environment == rhs.environment;
}
//--------------------------------------------------------------------------------------------
bool Location::operator!=(const Location& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
  Location* Location::make()
{
  return new Location();
}
//--------------------------------------------------------------------------------------------
 void Location::assign(Location* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//--------------------------------------------------------------------------------------------
void Location::assign(const Location& rhs)
{
  id = rhs.id;
  uuid = rhs.uuid;
  name = rhs.name;
  scene_name = rhs.scene_name;
  time_of_day = rhs.time_of_day;
  environment = rhs.environment;
}
//--------------------------------------------------------------------------------------------
void Location::clear()
{
  id = -1;
  uuid.clear();
  name.clear();
  scene_name.clear();
  time_of_day.clear();
  environment.clear();
}
//--------------------------------------------------------------------------------------------