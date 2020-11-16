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
    && code_name == rhs.code_name
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
  code_name = rhs.code_name;
  trauma_profile = rhs.trauma_profile;
}
//--------------------------------------------------------------------------------------------
void Role::clear()
{
  id = -1;
  uuid.clear();
  name.clear();
  code_name.clear();
  description.clear();
  trauma_profile->clear();
}
//--------------------------------------------------------------------------------------------
void Role::clear(int index)
{
  id = -1;
  uuid.clear();
  name = QString("Role %1").arg(index);
  code_name = QString("Role %1").arg(index);     
  description = QString("Undefined role %1").arg(index);   ;
  trauma_profile->clear();
}
//--------------------------------------------------------------------------------------------