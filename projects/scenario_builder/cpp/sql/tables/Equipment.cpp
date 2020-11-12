#include "Equipment.h"

Equipment::Equipment(QObject* parent)
  : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------
bool Equipment::operator==(const Equipment& rhs) const
{
  return name == rhs.name
    && type == rhs.type
    && summary == rhs.summary
    && description == rhs.description
    && citations == rhs.citations
    && image == rhs.image
    && properties == rhs.properties;
}
//--------------------------------------------------------------------------------------------
bool Equipment::operator!=(const Equipment& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
Equipment* Equipment::make()
{
  return new Equipment();
}
//--------------------------------------------------------------------------------------------
void Equipment::assign(Equipment* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//--------------------------------------------------------------------------------------------
void Equipment::assign(const Equipment& rhs)
{
  id = rhs.id;
  uuid = rhs.uuid;
  name = rhs.name;
  type = rhs.type;
  summary = rhs.summary;
  description = rhs.description;
  citations = rhs.citations;
  image = rhs.image;
  properties = rhs.properties;
}
//--------------------------------------------------------------------------------------------
void Equipment::clear()
{
  id = -1;
  uuid = "";
  name.clear();
  type = -1;
  summary.clear();
  description.clear();
  citations.clear();
  image.clear();
  properties.clear();
}
//--------------------------------------------------------------------------------------------
QQmlListProperty<Citation> Equipment::getCitations()
{
  return QQmlListProperty<Citation> ( this, this,
           &Equipment::AppendCitation,
           &Equipment::CountCitations,
           &Equipment::GetCitation,
           &Equipment::ClearCitations
   );
}
//-------------------------------------------------------------------------------
//! Helper functions for Equipments
void Equipment::AppendCitation(QQmlListProperty<Citation>* list, Citation* value)
{
  Equipment* EquipmentOccurance = qobject_cast<Equipment*>(list->object);
  if (EquipmentOccurance) {
    EquipmentOccurance->citations.append(value);
  }
}
//-------------------------------------------------------------------------------
Citation* Equipment::GetCitation(QQmlListProperty<Citation>* list, int index)
{
  Equipment* EquipmentOccurance = qobject_cast<Equipment*>(list->object);
  if (EquipmentOccurance) {
    return EquipmentOccurance->citations[index];
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
void Equipment::ClearCitations(QQmlListProperty<Citation>* list)
{
  Equipment* EquipmentOccurance = qobject_cast<Equipment*>(list->object);
  if (EquipmentOccurance) {
    return EquipmentOccurance->citations.clear();
  }
}
//-------------------------------------------------------------------------------
int Equipment::CountCitations(QQmlListProperty<Citation>* list)
{
  Equipment* EquipmentOccurance = qobject_cast<Equipment*>(list->object);
  if (EquipmentOccurance) {
    return EquipmentOccurance->citations.count();
  }
  return 0;
}
//-------------------------------------------------------------------------------
void Equipment::RemoveLastCitation(QQmlListProperty<Citation>* list)
{
  Equipment* EquipmentOccurance = qobject_cast<Equipment*>(list->object);
  if (EquipmentOccurance) {
    return EquipmentOccurance->citations.removeLast();
  }
}
//-------------------------------------------------------------------------------
void Equipment::ReplaceCitation(QQmlListProperty<Citation>* list, int index, Citation* value)
{
  Equipment* EquipmentOccurance = qobject_cast<Equipment*>(list->object);
  if (EquipmentOccurance) {
    return EquipmentOccurance->citations.replace(index, value);
  }
}
//-------------------------------------------------------------------------------