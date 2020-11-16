#include "Scene.h"

Scene::Scene(QObject* parent)
  : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------
bool Scene::operator==(const Scene& rhs) const
{
  return name == rhs.name
    && description == rhs.description
    && time_of_day == rhs.time_of_day
    && time_in_simulation == rhs.time_in_simulation
    && weather == rhs.weather
    && events == rhs.events
    && roles == rhs.roles;

}
//--------------------------------------------------------------------------------------------
bool Scene::operator!=(const Scene& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
Scene* Scene::make()
{
  return new Scene();
}
//--------------------------------------------------------------------------------------------
Q_INVOKABLE void Scene::assign(Scene* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//--------------------------------------------------------------------------------------------
void Scene::assign(const Scene& rhs)
{
  id = rhs.id;
  uuid = rhs.uuid;
  name = rhs.name;
  description = rhs.description;
  time_of_day = rhs.time_of_day;
  time_in_simulation = rhs.time_in_simulation;
  weather = rhs.weather;
  events = rhs.events;
  roles = rhs.roles;

}
//--------------------------------------------------------------------------------------------
void Scene::clear()
{
  id = -1;
  uuid = "";
  name.clear();
  description.clear();
  time_of_day = 0;
  time_in_simulation = 0;
  weather.clear();
  events.clear();
  roles.clear();

}
//--------------------------------------------------------------------------------------------
void Scene::clear(int index)
{
  id = -1;
  uuid = "";
  name = QString("Scene %1").arg(index);
  description = QString("Undefined Scene %1").arg(index);
  time_of_day = 0;
  time_in_simulation = 0;
  weather.clear();
  events.clear();
  roles.clear();

}
//--------------------------------------------------------------------------------------------
 QQmlListProperty<Event> Scene::getEvents()
{
  return QQmlListProperty<Event>(this, this,
                                    &Scene::AppendEvent,
                                    &Scene::CountEvents,
                                    &Scene::GetEvent,
                                    &Scene::ClearEvents);
}
//-------------------------------------------------------------------------------
//! Helper functions for Traumas
void Scene::AppendEvent(QQmlListProperty<Event>* list, Event* value)
{
  Scene* TraumaOccurance = qobject_cast<Scene*>(list->object);
  if (TraumaOccurance) {
    TraumaOccurance->events.append(value);
  }
}
//-------------------------------------------------------------------------------
Event* Scene::GetEvent(QQmlListProperty<Event>* list, int index)
{
  Scene* TraumaOccurance = qobject_cast<Scene*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->events[index];
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
void Scene::ClearEvents(QQmlListProperty<Event>* list)
{
  Scene* TraumaOccurance = qobject_cast<Scene*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->events.clear();
  }
}
//-------------------------------------------------------------------------------
int Scene::CountEvents(QQmlListProperty<Event>* list)
{
  Scene* TraumaOccurance = qobject_cast<Scene*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->events.count();
  }
  return 0;
}
//-------------------------------------------------------------------------------
void Scene::RemoveEvent(int index)
{
  events.removeAt(index);
}
//-------------------------------------------------------------------------------
void Scene::ReplaceEvent(int index, Event* value)
{
  events.replace(index, value);
}
//-------------------------------------------------------------------------------

QQmlListProperty<Role> Scene::getRoles()
{
  return QQmlListProperty<Role>(this, this,
                                 &Scene::AppendRole,
                                 &Scene::CountRoles,
                                 &Scene::GetRole,
                                 &Scene::ClearRoles);
}
//-------------------------------------------------------------------------------
//! Helper functions for Traumas
void Scene::AppendRole(QQmlListProperty<Role>* list, Role* value)
{
  Scene* TraumaOccurance = qobject_cast<Scene*>(list->object);
  if (TraumaOccurance) {
    TraumaOccurance->roles.append(value);
  }
}
//-------------------------------------------------------------------------------
Role* Scene::GetRole(QQmlListProperty<Role>* list, int index)
{
  Scene* TraumaOccurance = qobject_cast<Scene*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->roles[index];
  }
  return nullptr;
}
//-------------------------------------------------------------------------------
void Scene::ClearRoles(QQmlListProperty<Role>* list)
{
  Scene* TraumaOccurance = qobject_cast<Scene*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->roles.clear();
  }
}
//-------------------------------------------------------------------------------
int Scene::CountRoles(QQmlListProperty<Role>* list)
{
  Scene* TraumaOccurance = qobject_cast<Scene*>(list->object);
  if (TraumaOccurance) {
    return TraumaOccurance->roles.count();
  }
  return 0;
}
//-------------------------------------------------------------------------------
void Scene::RemoveRole(int index)
{
   roles.removeAt(index);
 }
//-------------------------------------------------------------------------------
void Scene::ReplaceRole( int index, Role* value)
{
   roles.replace(index, value);
}
//-------------------------------------------------------------------------------