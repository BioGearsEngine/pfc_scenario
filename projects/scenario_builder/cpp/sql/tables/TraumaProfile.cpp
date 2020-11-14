#include "TraumaProfile.h"

#include "Trauma.h"
#include "Treatment.h"

///
/// Trauma Instance Functions
///

TraumaOccurance::TraumaOccurance(QObject* parent)
  : QObject(parent)
  , fk_trauma(new Trauma(this))

{
}
//-------------------------------------------------------------------------------
TraumaOccurance::TraumaOccurance(Trauma* trauma, Treatment* treatment, QObject* parent)
  : QObject(parent)
  , fk_trauma(trauma)
{
}
//-------------------------------------------------------------------------------
bool TraumaOccurance::operator==(const TraumaOccurance& rhs) const
{
  return id == rhs.id
    && fk_trauma == rhs.fk_trauma
    && description == rhs.description
    && location == rhs.location
    && severity == rhs.severity;
}
//-------------------------------------------------------------------------------
bool TraumaOccurance::operator!=(const TraumaOccurance& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
TraumaOccurance* TraumaOccurance::make()
{
  return new TraumaOccurance();
}
//-------------------------------------------------------------------------------
void TraumaOccurance::assign(TraumaOccurance* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//-------------------------------------------------------------------------------
void TraumaOccurance::assign(const TraumaOccurance& rhs)
{
  id = rhs.id;
  fk_trauma = rhs.fk_trauma;
  description = rhs.description;
  location = rhs.location;
  severity = rhs.severity;
}
//-------------------------------------------------------------------------------
void TraumaOccurance::clear()
{
  id = -1;
  fk_trauma->clear();
  description.clear();
  location.clear();
  severity.clear();
}
//-------------------------------------------------------------------------------

///
///  Trauma Profile Functions
///
TraumaProfile::TraumaProfile(QObject* parent)
  : QObject(parent)
{
}
//-------------------------------------------------------------------------------
bool TraumaProfile::operator==(const TraumaProfile& rhs) const
{
  return name == rhs.name
    && description == rhs.description
    && traumas == rhs.traumas
    && physiologyTree == rhs.physiologyTree;
}
//-------------------------------------------------------------------------------
bool TraumaProfile::operator!=(const TraumaProfile& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
TraumaProfile* TraumaProfile::make()
{
  return new TraumaProfile();
}
//-------------------------------------------------------------------------------
void TraumaProfile::assign(TraumaProfile* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//-------------------------------------------------------------------------------
void TraumaProfile::assign(const TraumaProfile& rhs)
{
  id = rhs.id;
  uuid = rhs.uuid;
  name = rhs.name;
  description = rhs.description;
  traumas = rhs.traumas;
  physiologyTree = rhs.physiologyTree;
}
//-------------------------------------------------------------------------------
void TraumaProfile::clear()
{
  id = -1;
  uuid.clear();
  name.clear();
  description.clear();
  qDeleteAll(traumas);
  traumas.clear();
  physiologyTree.clear();
}
//-------------------------------------------------------------------------------
void TraumaProfile::clear(int index)
{
  id = -1;
  uuid.clear();
  name = QString("New Trauma Profile:%1").arg(index);
  description = QString("Undefined trauma profile %1.").arg(index);
  qDeleteAll(traumas);
  traumas.clear();
  physiologyTree.clear();
}
//-------------------------------------------------------------------------------
QQmlListProperty<TraumaOccurance> TraumaProfile::get_traumas()
{
  return QQmlListProperty<TraumaOccurance> ( nullptr, this,
                                             &TraumaProfile::AppendTrauma,
                                             &TraumaProfile::CountTraumas,
                                             &TraumaProfile::GetTrauma,
                                             &TraumaProfile::ClearTraumas );
}
//-------------------------------------------------------------------------------

//! Helper functions for Traumas
void TraumaProfile::AppendTrauma(QQmlListProperty<TraumaOccurance>* list, TraumaOccurance* value)
{
  auto profile = qobject_cast<TraumaProfile*>(list->object);
  if (profile) {
    profile->traumas.append(value);
  }
}
//-------------------------------------------------------------------------------
auto TraumaProfile::GetTrauma(QQmlListProperty<TraumaOccurance>* list, int index) -> TraumaOccurance*
{
  auto profile = qobject_cast<TraumaProfile*>(list->object);
  if (profile) {
    return profile->traumas[index];
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
void TraumaProfile::ClearTraumas(QQmlListProperty<TraumaOccurance>* list)
{
  auto profile = qobject_cast<TraumaProfile*>(list->object);
  if (profile) {
    profile->traumas.clear();
  }
}
//-------------------------------------------------------------------------------
auto TraumaProfile::CountTraumas(QQmlListProperty<TraumaOccurance>* list) -> int
{
  auto profile = qobject_cast<TraumaProfile*>(list->object);
  if (profile) {
    return profile->traumas.count();
  }
  return 0;
}
//-------------------------------------------------------------------------------
void TraumaProfile::RemoveLastTrauma(QQmlListProperty<TraumaOccurance>* list)
{
  auto profile = qobject_cast<TraumaProfile*>(list->object);
  if (profile) {
    profile->traumas.removeLast();
  }
}
//-------------------------------------------------------------------------------
void TraumaProfile::ReplaceTrauma(QQmlListProperty<TraumaOccurance>* list, int index, TraumaOccurance* value)
{
  auto profile = qobject_cast<TraumaProfile*>(list->object);
  if (profile) {
    profile->traumas.replace(index, value);
  }
}
// //-------------------------------------------------------------------------------
// //! Helper functions for Traumas
// //
// QQmlListProperty<QString> TraumaProfile::get_physiology_tree()
// {
//   return QQmlListProperty<QString>(this, this,
//                                    &TraumaProfile::AppendPhysiologyState,
//                                    &TraumaProfile::CountPhysiologyStates,
//                                    &TraumaProfile::GetPhysiologyState,
//                                    &TraumaProfile::ClearPhysiologyStates);
// }
//
// //-------------------------------------------------------------------------------
// void TraumaProfile::AppendPhysiologyState(QQmlListProperty<QString>* list, QString* value)
// {
//   TraumaProfile* TraumaOccurance = qobject_cast<TraumaProfile*>(list->object);
//   if (TraumaOccurance) {
//     TraumaOccurance->physiologyTree.append(*value);
//   }
// }
// //-------------------------------------------------------------------------------
// QString* TraumaProfile::GetPhysiologyState(QQmlListProperty<QString>* list, int index)
// {
//   TraumaProfile* TraumaOccurance = qobject_cast<TraumaProfile*>(list->object);
//   if (TraumaOccurance) {
//     return &(TraumaOccurance->physiologyTree[index]);
//   }
//   return nullptr;
// }
// //-------------------------------------------------------------------------------
// void TraumaProfile::ClearPhysiologyStates(QQmlListProperty<QString>* list)
// {
//   TraumaProfile* TraumaOccurance = qobject_cast<TraumaProfile*>(list->object);
//   if (TraumaOccurance) {
//     return TraumaOccurance->physiologyTree.clear();
//   }
// }
// //-------------------------------------------------------------------------------
// int TraumaProfile::CountPhysiologyStates(QQmlListProperty<QString>* list)
// {
//   TraumaProfile* TraumaOccurance = qobject_cast<TraumaProfile*>(list->object);
//   if (TraumaOccurance) {
//     return TraumaOccurance->physiologyTree.count();
//   }
//   return 0;
// }
// //-------------------------------------------------------------------------------
// void TraumaProfile::RemoveLastPhysiologyState(QQmlListProperty<QString>* list)
// {
//   TraumaProfile* TraumaOccurance = qobject_cast<TraumaProfile*>(list->object);
//   if (TraumaOccurance) {
//     return TraumaOccurance->physiologyTree.removeLast();
//   }
// }
// //-------------------------------------------------------------------------------
// void TraumaProfile::ReplacePhysiologyState(QQmlListProperty<QString>* list, int index, QString* value)
// {
//   TraumaProfile* TraumaOccurance = qobject_cast<TraumaProfile*>(list->object);
//   if (TraumaOccurance) {
//     return TraumaOccurance->physiologyTree.replace(index, *value);
//   }
// }
// //-------------------------------------------------------------------------------
;