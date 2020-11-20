#include "Location.h"

Location::Location(QObject* parent)
  : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------
bool Location::operator==(const Location& rhs) const
{
  return name == rhs.name
    && description == rhs.description
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
  description = rhs.description;
  environment = rhs.environment;
}
//--------------------------------------------------------------------------------------------
void Location::clear()
{
  id = -1;
  uuid.clear();
  name.clear();
  environment = "Exterior:Terrestrial:Desert";
}
//--------------------------------------------------------------------------------------------
void Location::clear(int index)
{
  id = -1;
  uuid.clear();
  name = QString(" Location %1").arg(index);
  description = QString("Undefined location %1").arg(index);
  environment = "Exterior:Terrestrial:Desert";
}
  //--------------------------------------------------------------------------------------------