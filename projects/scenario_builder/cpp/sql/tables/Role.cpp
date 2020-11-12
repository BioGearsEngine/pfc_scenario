#include "Role.h"

#include "TraumaProfile.h"

Role::Role(QObject* parent)
  : QObject(parent)
  , trauma_profile(new TraumaProfile(this))
{
}
//--------------------------------------------------------------------------------------------
Role::Role(TraumaProfile* profile, QObject* parent)
  : QObject(parent)
  , trauma_profile(profile)
{
}
//--------------------------------------------------------------------------------------------
bool Role::operator==(const Role& rhs) const
{
  return name == rhs.name
    && description == rhs.description
    && short_name == rhs.short_name
    && trauma_profile == rhs.trauma_profile;
}
//--------------------------------------------------------------------------------------------
bool Role::operator!=(const Role& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
Role* Role::make()
{
  return new Role();
}
//--------------------------------------------------------------------------------------------
void Role::assign(Role* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//--------------------------------------------------------------------------------------------
void Role::assign(const Role& rhs)
{
  id = rhs.id;
  uuid = rhs.uuid;
  name = rhs.name;
  description = rhs.description;
  short_name = rhs.short_name;
  trauma_profile = rhs.trauma_profile;
}
//--------------------------------------------------------------------------------------------
void Role::clear()
{
  id = -1;
  uuid.clear();
  name.clear();
  short_name.clear();
  description.clear();
  trauma_profile->clear();
}
//--------------------------------------------------------------------------------------------