#include "Treatment.h"

  Treatment::Treatment(QObject* parent)
  : QObject(parent)
{
}
//-------------------------------------------------------------------------------
bool Treatment::operator==(const Treatment& rhs) const
{
  return medical_name == rhs.medical_name
    && common_name == rhs.common_name
    && description == rhs.description
    && equipment == rhs.equipment
    && citations == rhs.citations
    && cpgs == rhs.cpgs;
}
//-------------------------------------------------------------------------------
bool Treatment::operator!=(const Treatment& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
Treatment* Treatment::make()
{
  return new Treatment();
}
//-------------------------------------------------------------------------------
void Treatment::assign(Treatment* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//-------------------------------------------------------------------------------
void Treatment::assign(const Treatment& rhs)
{
  id = rhs.id;
  uuid = rhs.uuid;
  medical_name = rhs.medical_name;
  common_name = rhs.common_name;
  description = rhs.description;
  equipment = rhs.equipment;
  citations = rhs.citations;
  cpgs = rhs.cpgs;
}
//-------------------------------------------------------------------------------
void Treatment::clear()
{
  id = -1;
  uuid = "";
  medical_name.clear();
  common_name.clear();
  description.clear();
  equipment.clear();
  citations.clear();
  cpgs.clear();
}
//-------------------------------------------------------------------------------
void Treatment::clear(int index)
{
  id = -1;
  uuid = "";
  medical_name = QString("New Treatment:%1").arg(index);
  common_name = QString("New Treatment:%1").arg(index);
  description = QString("Undefined treatment %1.").arg(index);
  equipment.clear();
  citations.clear();
  cpgs.clear();
}
//--------------------------------------------------------------------------------------------
QQmlListProperty<Citation> Treatment::getCitations()
{
  return QQmlListProperty<Citation>(this, this,
                                    &Treatment::AppendCitation,
                                    &Treatment::CountCitations,
                                    &Treatment::GetCitation,
                                    &Treatment::ClearCitations);
}
//-------------------------------------------------------------------------------
//! Helper functions for Traumas
void Treatment::AppendCitation(QQmlListProperty<Citation>* list, Citation* value)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    TraumaOccurance->citations.append(value);
  }
}
//-------------------------------------------------------------------------------
Citation* Treatment::GetCitation(QQmlListProperty<Citation>* list, int index)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->citations[index];
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
void Treatment::ClearCitations(QQmlListProperty<Citation>* list)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->citations.clear();
  }
}
//-------------------------------------------------------------------------------
int Treatment::CountCitations(QQmlListProperty<Citation>* list)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->citations.count();
  }
  return 0;
}
//-------------------------------------------------------------------------------
void Treatment::RemoveLastCitation(QQmlListProperty<Citation>* list)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->citations.removeLast();
  }
}
//-------------------------------------------------------------------------------
void Treatment::ReplaceCitation(QQmlListProperty<Citation>* list, int index, Citation* value)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->citations.replace(index, value);
  }
}
//-------------------------------------------------------------------------------

QQmlListProperty<Citation> Treatment::getCPGS()
{
  return QQmlListProperty<Citation>(this, this,
                                    &Treatment::AppendCPG,
                                    &Treatment::CountCPGs,
                                    &Treatment::GetCPG,
                                    &Treatment::ClearCPGs);
}
//-------------------------------------------------------------------------------
//! Helper functions for Traumas
void Treatment::AppendCPG(QQmlListProperty<Citation>* list, Citation* value)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    TraumaOccurance->cpgs.append(value);
  }
}
//-------------------------------------------------------------------------------
Citation* Treatment::GetCPG(QQmlListProperty<Citation>* list, int index)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->cpgs[index];
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
void Treatment::ClearCPGs(QQmlListProperty<Citation>* list)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->cpgs.clear();
  }
}
//-------------------------------------------------------------------------------
int Treatment::CountCPGs(QQmlListProperty<Citation>* list)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->cpgs.count();
  }
  return 0;
}
//-------------------------------------------------------------------------------
void Treatment::RemoveLastCPG(QQmlListProperty<Citation>* list)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->cpgs.removeLast();
  }
}
//-------------------------------------------------------------------------------
void Treatment::ReplaceCPG(QQmlListProperty<Citation>* list, int index, Citation* value)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->cpgs.replace(index, value);
  }
}
//-------------------------------------------------------------------------------
QQmlListProperty<Equipment> Treatment::getEquipment()
{
  return QQmlListProperty<Equipment>(this, this,
                                    &Treatment::AppendEquipment,
                                    &Treatment::CountEquipments,
                                    &Treatment::GetEquipment,
                                    &Treatment::ClearEquipments);
}
//-------------------------------------------------------------------------------
//! Helper functions for Traumas
void Treatment::AppendEquipment(QQmlListProperty<Equipment>* list, Equipment* value)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    TraumaOccurance->equipment.append(value);
  }
}
//-------------------------------------------------------------------------------
Equipment* Treatment::GetEquipment(QQmlListProperty<Equipment>* list, int index)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->equipment[index];
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
void Treatment::ClearEquipments(QQmlListProperty<Equipment>* list)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->equipment.clear();
  }
}
//-------------------------------------------------------------------------------
int Treatment::CountEquipments(QQmlListProperty<Equipment>* list)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->equipment.count();
  }
  return 0;
}
//-------------------------------------------------------------------------------
void Treatment::RemoveLastEquipment(QQmlListProperty<Equipment>* list)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->equipment.removeLast();
  }
}
//-------------------------------------------------------------------------------
void Treatment::ReplaceEquipment(QQmlListProperty<Equipment>* list, int index, Equipment* value)
{
  Treatment* TraumaOccurance = qobject_cast<Treatment*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->equipment.replace(index, value);
  }
}
//-------------------------------------------------------------------------------