#include "Property.h"

Property::Property(QObject* parent)
  : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------
bool Property::operator==(const Property& rhs) const
{
  return id == rhs.id

    && name == rhs.name
    && value == rhs.value;
}
//--------------------------------------------------------------------------------------------
bool Property::operator!=(const Property& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
Property* Property::make()
{
  return new Property();
}
//--------------------------------------------------------------------------------------------
 void Property::assign(Property* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//--------------------------------------------------------------------------------------------
void Property::assign(const Property& rhs)
{
  id = rhs.id;
  uuid = rhs.uuid;
  name = rhs.name;
  value = rhs.value;
}   
//--------------------------------------------------------------------------------------------
void Property::clear()
{
  id = -1;
  uuid.clear();
  name.clear();
  value.clear();
}
//--------------------------------------------------------------------------------------------
