#include "RoleMap.h"

#include "Scene.h"
#include "Role.h"

RoleMap::RoleMap(QObject* parent)
  : QObject(parent)
{
}
//-------------------------------------------------------------------------------
bool RoleMap::operator==(const RoleMap& rhs) const
{
  return fk_scene == rhs.fk_scene
    && fk_role == rhs.fk_role;
}
//-------------------------------------------------------------------------------
bool RoleMap::operator!=(const RoleMap& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
RoleMap* RoleMap::make()
{
  return new RoleMap();
}
//-------------------------------------------------------------------------------
void RoleMap::assign(RoleMap* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//-------------------------------------------------------------------------------
void RoleMap::assign(const RoleMap& rhs)
{
  id = rhs.id;
  fk_scene = rhs.fk_scene;
  fk_role = rhs.fk_role;
}
//-------------------------------------------------------------------------------