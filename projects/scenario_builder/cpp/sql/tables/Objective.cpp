#include "Objective.h"

#include "Citation.h"
#include "TraumaProfile.h"
#include "Treatment.h"

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
void Objective::clear(int index)
{
  id = -1;
  uuid.clear();
  _citations.clear();
  _citations.clear();
  _cpgs.clear();
  _treatment_plans.clear();
  _trauma_profiles.clear();

  name = QString(" New Objective:%1").arg(index);
  description = QString("Undefined learning objective %1.").arg(index);
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
    TraumaOccurance->citationsChanged();
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
    TraumaOccurance->_citations.clear();
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
void Objective::removeCitation(int index)
{
  _citations.removeAt(index);
  citationsChanged();
}
//-------------------------------------------------------------------------------
void Objective::replaceCitation(int index, Citation* value)
{
  _citations.replace(index, value);
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
void Objective::removeCPG(int index)
{
  _cpgs.removeAt(index);
}
//-------------------------------------------------------------------------------
void Objective::replaceCPG(int index, Citation* value)
{
  return _cpgs.replace(index, value);
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
void Objective::removeTreatment(int index)
{
  _treatment_plans.removeAt(index);
}
//-------------------------------------------------------------------------------
void Objective::replaceTreatment(int index, Treatment* value)
{
  _treatment_plans.replace(index, value);
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
void Objective::removeTraumaProfile(int index)
{
  _trauma_profiles.removeAt(index);
}
//-------------------------------------------------------------------------------
void Objective::replaceTraumaProfile(int index, TraumaProfile* value)
{
  _trauma_profiles.replace(index, value);
}
