#include "Objective.h"

#include "TraumaProfile.h"
#include "Treatment.h"
#include "Citation.h"

Objective::Objective(QObject* parent)
  : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------
bool Objective::operator==(const Objective& rhs) const
{
  return name == rhs.name
    && description == rhs.description
    && _citations == rhs._citations
    && _cpgs == rhs._cpgs
    && _treatment_plans == rhs._treatment_plans
    && _trauma_profiles == rhs._trauma_profiles;
}
//--------------------------------------------------------------------------------------------
bool Objective::operator!=(const Objective& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
Objective* Objective::make()
{
  return new Objective();
}
//--------------------------------------------------------------------------------------------
void Objective::assign(Objective* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//--------------------------------------------------------------------------------------------
void Objective::assign(const Objective& rhs)
{
  id = rhs.id;
  uuid = rhs.uuid;
  name = rhs.name;
  description = rhs.description;
  _citations = rhs._citations;
  _cpgs = rhs._cpgs;
  _treatment_plans = rhs._treatment_plans;
  _trauma_profiles = rhs._trauma_profiles;
}
//--------------------------------------------------------------------------------------------
void Objective::clear()
{
  id = -1;
  uuid.clear();
  name.clear();
  description.clear();
  _citations.clear();
  _cpgs.clear();
  _treatment_plans.clear();
  _trauma_profiles.clear();
}
//--------------------------------------------------------------------------------------------
QQmlListProperty<Citation> Objective::getCitations()
{
  return QQmlListProperty<Citation>(this, this,
                                    &Objective::AppendCitation,
                                    &Objective::CountCitations,
                                    &Objective::GetCitation,
                                    &Objective::ClearCitations);
}
//-------------------------------------------------------------------------------
//! Helper functions for Traumas
void Objective::AppendCitation(QQmlListProperty<Citation>* list, Citation* value)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    TraumaOccurance->_citations.append(value);
  }
}
//-------------------------------------------------------------------------------
Citation* Objective::GetCitation(QQmlListProperty<Citation>* list, int index)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_citations[index];
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
void Objective::ClearCitations(QQmlListProperty<Citation>* list)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_citations.clear();
  }
}
//-------------------------------------------------------------------------------
int Objective::CountCitations(QQmlListProperty<Citation>* list)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_citations.count();
  }
  return 0;
}
//-------------------------------------------------------------------------------
void Objective::RemoveLastCitation(QQmlListProperty<Citation>* list)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_citations.removeLast();
  }
}
//-------------------------------------------------------------------------------
void Objective::ReplaceCitation(QQmlListProperty<Citation>* list, int index, Citation* value)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_citations.replace(index, value);
  }
}
//-------------------------------------------------------------------------------
//! Helper functions for Traumas

QQmlListProperty<Citation> Objective::getCPGS()
{
  return QQmlListProperty<Citation>(this, this,
                                    &Objective::AppendCPG,
                                    &Objective::CountCPGs,
                                    &Objective::GetCPG,
                                    &Objective::ClearCPGs);
}
//--------------------------------------------------------------------------------------------
void Objective::AppendCPG(QQmlListProperty<Citation>* list, Citation* value)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    TraumaOccurance->_cpgs.append(value);
  }
}
//-------------------------------------------------------------------------------
Citation* Objective::GetCPG(QQmlListProperty<Citation>* list, int index)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_cpgs[index];
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
void Objective::ClearCPGs(QQmlListProperty<Citation>* list)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_cpgs.clear();
  }
}
//-------------------------------------------------------------------------------
int Objective::CountCPGs(QQmlListProperty<Citation>* list)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_cpgs.count();
  }
  return 0;
}
//-------------------------------------------------------------------------------
void Objective::RemoveLastCPG(QQmlListProperty<Citation>* list)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_cpgs.removeLast();
  }
}
//-------------------------------------------------------------------------------
void Objective::ReplaceCPG(QQmlListProperty<Citation>* list, int index, Citation* value)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_cpgs.replace(index, value);
  }
}
//-------------------------------------------------------------------------------
//! Helper functions for Traumas
QQmlListProperty<Treatment> Objective::getTreatments()
{
  return QQmlListProperty<Treatment>(this, this,
                                    &Objective::AppendTreatment,
                                    &Objective::CountTreatments,
                                    &Objective::GetTreatment,
                                    &Objective::ClearTreatments);
}
//-------------------------------------------------------------------------------
void Objective::AppendTreatment(QQmlListProperty<Treatment>* list, Treatment* value)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    TraumaOccurance->_treatment_plans.append(value);
  }
}
//-------------------------------------------------------------------------------
Treatment* Objective::GetTreatment(QQmlListProperty<Treatment>* list, int index)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_treatment_plans[index];
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
void Objective::ClearTreatments(QQmlListProperty<Treatment>* list)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_treatment_plans.clear();
  }
}
//-------------------------------------------------------------------------------
int Objective::CountTreatments(QQmlListProperty<Treatment>* list)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_treatment_plans.count();
  }
  return 0;
}
//-------------------------------------------------------------------------------
void Objective::RemoveLastTreatment(QQmlListProperty<Treatment>* list)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_treatment_plans.removeLast();
  }
}
//-------------------------------------------------------------------------------
void Objective::ReplaceTreatment(QQmlListProperty<Treatment>* list, int index, Treatment* value)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_treatment_plans.replace(index, value);
  }
}
//-------------------------------------------------------------------------------
//! Helper functions for Traumas
QQmlListProperty<TraumaProfile> Objective::getTraumaProfiles()
{
  return QQmlListProperty<TraumaProfile>(this, this,
                                     &Objective::AppendTraumaProfile,
                                     &Objective::CountTraumaProfiles,
                                     &Objective::GetTraumaProfile,
                                     &Objective::ClearTraumaProfiles);
}
//-------------------------------------------------------------------------------
void Objective::AppendTraumaProfile(QQmlListProperty<TraumaProfile>* list, TraumaProfile* value)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    TraumaOccurance->_trauma_profiles.append(value);
  }
}
//-------------------------------------------------------------------------------
TraumaProfile* Objective::GetTraumaProfile(QQmlListProperty<TraumaProfile>* list, int index)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_trauma_profiles[index];
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
void Objective::ClearTraumaProfiles(QQmlListProperty<TraumaProfile>* list)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_trauma_profiles.clear();
  }
}
//-------------------------------------------------------------------------------
int Objective::CountTraumaProfiles(QQmlListProperty<TraumaProfile>* list)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_trauma_profiles.count();
  }
  return 0;
}
//-------------------------------------------------------------------------------
void Objective::RemoveLastTraumaProfile(QQmlListProperty<TraumaProfile>* list)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_trauma_profiles.removeLast();
  }
}
//-------------------------------------------------------------------------------
void Objective::ReplaceTraumaProfile(QQmlListProperty<TraumaProfile>* list, int index, TraumaProfile* value)
{
  Objective* TraumaOccurance = qobject_cast<Objective*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->_trauma_profiles.replace(index, value);
  }
}
