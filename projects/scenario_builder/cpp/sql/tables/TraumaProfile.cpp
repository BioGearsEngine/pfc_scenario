#include "TraumaProfile.h"

#include "Trauma.h"
#include "Treatment.h"

///
/// Trauma Instance Functions
///

TraumaOccurence::TraumaOccurence(QObject* parent)
  : QObject(parent)
  , fk_trauma(new Trauma(this))

{
}
//-------------------------------------------------------------------------------
TraumaOccurence::TraumaOccurence(Trauma* trauma, Treatment* treatment, QObject* parent)
  : QObject(parent)
  , fk_trauma(trauma)
{
}
//-------------------------------------------------------------------------------
bool TraumaOccurence::operator==(const TraumaOccurence& rhs) const
{
  return *fk_trauma == *rhs.fk_trauma
    && description == rhs.description
    && location == rhs.location
    && severity == rhs.severity;
}
//-------------------------------------------------------------------------------
bool TraumaOccurence::operator!=(const TraumaOccurence& rhs) const
{
  return !(*this == rhs);
}
//-------------------------------------------------------------------------------
TraumaOccurence* TraumaOccurence::make()
{
  return new TraumaOccurence();
}
//-------------------------------------------------------------------------------
void TraumaOccurence::assign(TraumaOccurence* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//-------------------------------------------------------------------------------
void TraumaOccurence::assign(const TraumaOccurence& rhs)
{

  fk_trauma->assign(rhs.fk_trauma);
  description = rhs.description;
  location = rhs.location;
  severity = rhs.severity;
}
//-------------------------------------------------------------------------------
void TraumaOccurence::clear()
{

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
  name = QString(" New Trauma Profile:%1").arg(index);
  description = QString("Undefined trauma profile %1.").arg(index);
  qDeleteAll(traumas);
  traumas.clear();
  physiologyTree.clear();
}
//-------------------------------------------------------------------------------
QQmlListProperty<TraumaOccurence> TraumaProfile::get_traumas()
{
  return QQmlListProperty<TraumaOccurence>(this, this,
                                           &TraumaProfile::AppendTrauma,
                                           &TraumaProfile::CountTraumas,
                                           &TraumaProfile::GetTrauma,
                                           &TraumaProfile::ClearTraumas);
}
//-------------------------------------------------------------------------------

//! Helper functions for Traumas
void TraumaProfile::AppendTrauma(QQmlListProperty<TraumaOccurence>* list, TraumaOccurence* value)
{
  auto profile = qobject_cast<TraumaProfile*>(list->object);
  if (profile) {
    profile->traumas.append(value);
  }
}
//-------------------------------------------------------------------------------
auto TraumaProfile::GetTrauma(QQmlListProperty<TraumaOccurence>* list, int index) -> TraumaOccurence*
{
  auto profile = qobject_cast<TraumaProfile*>(list->object);
  if (profile) {
    return profile->traumas[index];
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
void TraumaProfile::ClearTraumas(QQmlListProperty<TraumaOccurence>* list)
{
  auto profile = qobject_cast<TraumaProfile*>(list->object);
  if (profile) {
    profile->traumas.clear();
  }
}
//-------------------------------------------------------------------------------
auto TraumaProfile::CountTraumas(QQmlListProperty<TraumaOccurence>* list) -> int
{
  auto profile = qobject_cast<TraumaProfile*>(list->object);
  if (profile) {
    return profile->traumas.count();
  }
  return 0;
}
//-------------------------------------------------------------------------------
void TraumaProfile::RemoveTrauma(int index)
{
  traumas.removeAt(index);
}
//-------------------------------------------------------------------------------
void TraumaProfile::ReplaceTrauma(int index, TraumaOccurence* value)
{
  traumas.replace(index, value);
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
//   TraumaProfile* TraumaOccurence = qobject_cast<TraumaProfile*>(list->object);
//   if (TraumaOccurence) {
//     TraumaOccurence->physiologyTree.append(*value);
//   }
// }
// //-------------------------------------------------------------------------------
// QString* TraumaProfile::GetPhysiologyState(QQmlListProperty<QString>* list, int index)
// {
//   TraumaProfile* TraumaOccurence = qobject_cast<TraumaProfile*>(list->object);
//   if (TraumaOccurence) {
//     return &(TraumaOccurence->physiologyTree[index]);
//   }
//   return nullptr;
// }
// //-------------------------------------------------------------------------------
// void TraumaProfile::ClearPhysiologyStates(QQmlListProperty<QString>* list)
// {
//   TraumaProfile* TraumaOccurence = qobject_cast<TraumaProfile*>(list->object);
//   if (TraumaOccurence) {
//     return TraumaOccurence->physiologyTree.clear();
//   }
// }
// //-------------------------------------------------------------------------------
// int TraumaProfile::CountPhysiologyStates(QQmlListProperty<QString>* list)
// {
//   TraumaProfile* TraumaOccurence = qobject_cast<TraumaProfile*>(list->object);
//   if (TraumaOccurence) {
//     return TraumaOccurence->physiologyTree.count();
//   }
//   return 0;
// }
// //-------------------------------------------------------------------------------
// void TraumaProfile::RemoveLastPhysiologyState(int index)
// {
//     >physiologyTree.removeAt(index);
// }
// //-------------------------------------------------------------------------------
// void TraumaProfile::ReplacePhysiologyState( int index, QString* value)
// {
//     return physiologyTree.replace(index, *value);
// }
// //-------------------------------------------------------------------------------
;