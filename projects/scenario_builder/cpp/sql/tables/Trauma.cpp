#include "Trauma.h"

#include "Citation.h"

Trauma::Trauma(QObject* parent )
  : QObject(parent)
, fk_treatment(new Treatment())
{
}
//-------------------------------------------------------------------------------
bool Trauma::operator==(const Trauma& rhs) const
{
  return medical_name == rhs.medical_name
    && common_name == rhs.common_name
    && description == rhs.description
    && citations == rhs.citations
    && lower_bound == rhs.lower_bound
    && upper_bound == rhs.upper_bound
    && fk_treatment == rhs.fk_treatment;
}
//-------------------------------------------------------------------------------
bool Trauma::operator!=(const Trauma& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
  Trauma* Trauma::make()
{
    return new Trauma();
  }
//-------------------------------------------------------------------------------
 void Trauma::assign(Trauma* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//-------------------------------------------------------------------------------
void Trauma::assign(const Trauma& rhs)
{
  id = rhs.id;
  uuid = rhs.uuid;
  medical_name = rhs.medical_name;
  common_name = rhs.common_name;
  description = rhs.description;
  lower_bound = rhs.lower_bound;
  upper_bound = rhs.upper_bound;
  qDeleteAll(citations);
  citations.clear();
  for ( auto citation : rhs.citations) {
    citations.push_back(new Citation(this));
    citations.back()->assign(citation);
  }
  fk_treatment->assign(rhs.fk_treatment);

}
//-------------------------------------------------------------------------------
void Trauma::clear()
{
  id = -1;
  uuid.clear();
  medical_name.clear();
  common_name.clear();
  description.clear();
  qDeleteAll(citations);
  citations.clear();
  lower_bound = 0;
  upper_bound = 1;
  fk_treatment->clear();
}
//-------------------------------------------------------------------------------
void Trauma::clear(int index)
{
  id = -1;
  uuid.clear();
  common_name = QString("New Trauma:%1").arg(index);
  medical_name= QString("New Trauma:%1").arg(index);
  description = QString("Undefined trauma %1.").arg(index);
  qDeleteAll(citations);
  citations.clear();
  lower_bound = 0;
  upper_bound = 1;
  fk_treatment->clear();
}
//-------------------------------------------------------------------------------
QQmlListProperty<Citation> Trauma::getCitations()
{
  return QQmlListProperty<Citation>(this, this,
                                    &Trauma::AppendCitation,
                                    &Trauma::CountCitations,
                                    &Trauma::GetCitation,
                                    &Trauma::ClearCitations);
}
//-------------------------------------------------------------------------------
//! Helper functions for Traumas
void Trauma::AppendCitation(QQmlListProperty<Citation>* list, Citation* value)
{
  Trauma* TraumaOccurance = qobject_cast<Trauma*>(list->object);
  if (TraumaOccurance) {
    TraumaOccurance->citations.append(value);
  }
}
//-------------------------------------------------------------------------------
Citation* Trauma::GetCitation(QQmlListProperty<Citation>* list, int index)
{
  Trauma* TraumaOccurance = qobject_cast<Trauma*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->citations[index];
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
void Trauma::ClearCitations(QQmlListProperty<Citation>* list)
{
  Trauma* TraumaOccurance = qobject_cast<Trauma*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->citations.clear();
  }
}
//-------------------------------------------------------------------------------
int Trauma::CountCitations(QQmlListProperty<Citation>* list)
{
  Trauma* TraumaOccurance = qobject_cast<Trauma*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->citations.count();
  }
  return 0;
}
//-------------------------------------------------------------------------------
void Trauma::RemoveLastCitation(QQmlListProperty<Citation>* list)
{
  Trauma* TraumaOccurance = qobject_cast<Trauma*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->citations.removeLast();
  }
}
//-------------------------------------------------------------------------------
void Trauma::ReplaceCitation(QQmlListProperty<Citation>* list, int index, Citation* value)
{
  Trauma* TraumaOccurance = qobject_cast<Trauma*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->citations.replace(index, value);
  }
}
//