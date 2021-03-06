#include "EquipmentMap.h"

#include "Equipment.h"
#include "Scene.h"

EquipmentMap::EquipmentMap(QObject* parent)
  : QObject(parent)
  , scene(new Scene(this))
  , equipment(new Equipment(this))
{
}
//--------------------------------------------------------------------------------------------
EquipmentMap::EquipmentMap(Scene* s, Equipment* e, QObject* parent)
  : QObject(parent)
  , scene(s)
  , equipment(e)
{
}
//--------------------------------------------------------------------------------------------
bool EquipmentMap::operator==(const EquipmentMap& rhs) const
{
  return name == rhs.name
    && *scene == *rhs.scene
    && *equipment == *rhs.equipment
    && property_values == rhs.property_values
    && notes == rhs.notes;
}
//--------------------------------------------------------------------------------------------
bool EquipmentMap::operator!=(const EquipmentMap& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
 EquipmentMap* EquipmentMap::make()
{
  return new EquipmentMap();
}
//--------------------------------------------------------------------------------------------
void EquipmentMap::assign(EquipmentMap* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//--------------------------------------------------------------------------------------------
void EquipmentMap::assign(const EquipmentMap& rhs)
{
  //!
  //!  TODO: Clone Scene and Equipment if non null to avoid
  //!        Garbage Collection

  id = rhs.id;
  name = rhs.name;
  scene->assign(*rhs.scene);
  equipment->assign(*rhs.equipment);
  property_values = rhs.property_values;
  notes = rhs.notes;
  emit idChanged(id);
  emit nameChanged(name);
  emit sceneChanged(scene);
  emit equipmentChanged(equipment);
  emit valuesChanged(property_values);
  emit notesChanged(notes);
}
//--------------------------------------------------------------------------------------------
void EquipmentMap::clear()
{
  id = -1;

  name.clear();
  scene->clear();
  equipment->clear();
  property_values.clear();
  notes.clear();
}
//--------------------------------------------------------------------------------------------