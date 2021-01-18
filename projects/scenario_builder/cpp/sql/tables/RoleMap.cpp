#include "RoleMap.h"

#include "Role.h"
#include "Scene.h"

RoleMap::RoleMap(QObject* parent)
  : QObject(parent)
  , fk_scene(new Scene(this))
  , fk_role(new Role(this))
  , category("")
{
}
//-------------------------------------------------------------------------------
RoleMap::RoleMap(Scene* scene, Role* role, QString c, QObject* parent)
  : QObject(parent)
  , fk_scene(scene)
  , fk_role(role)
  , category(c)
{
}
//-------------------------------------------------------------------------------
bool RoleMap::operator==(const RoleMap& rhs) const
{
  return fk_scene == rhs.fk_scene
    && fk_role == rhs.fk_role
    && category == rhs.category;
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
  category = rhs.category;
}
//--------------------------------------------------------------------------------------------
void RoleMap::clear()
{
  id = -1;
  fk_role->clear();
  fk_scene->clear();
  category.clear();
}
//--------------------------------------------------------------------------------------------
void RoleMap::clear(int index)
{
  id = -1;
  fk_role->clear();
  fk_scene->clear();
  category.clear();
}
//--------------------------------------------------------------------------------------------