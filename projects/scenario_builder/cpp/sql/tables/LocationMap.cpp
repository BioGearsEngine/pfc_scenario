#include "LocationMap.h"

#include "Scene.h"
#include "Location.h"

LocationMap::LocationMap(QObject* parent)
  : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------
bool LocationMap::operator==(const LocationMap& rhs) const
{
  return fk_scene == rhs.fk_scene
    && fk_location == rhs.fk_location;
}
//--------------------------------------------------------------------------------------------
bool LocationMap::operator!=(const LocationMap& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
LocationMap* LocationMap::make()
{
  return new LocationMap();
}
//--------------------------------------------------------------------------------------------
void LocationMap::assign(LocationMap* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//--------------------------------------------------------------------------------------------
void LocationMap::assign(const LocationMap& rhs)
{
  id = rhs.id;
  fk_scene = rhs.fk_scene;
  fk_location = rhs.fk_location;
}
//--------------------------------------------------------------------------------------------
void LocationMap::clear()
{
  id = -1;
  fk_scene->clear();
  fk_location->clear();
}