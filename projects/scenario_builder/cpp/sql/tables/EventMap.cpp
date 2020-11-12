#include "EventMap.h"

#include "Scene.h"
#include "Event.h"

EventMap::EventMap(QObject* parent)
  : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------
bool EventMap::operator==(const EventMap& rhs) const
{
  return fk_scene == rhs.fk_scene
    && fk_event == rhs.fk_event;
}
//--------------------------------------------------------------------------------------------
bool EventMap::operator!=(const EventMap& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
EventMap* EventMap::make()
{
  return new EventMap();
}
//--------------------------------------------------------------------------------------------
 void EventMap::assign(EventMap* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//--------------------------------------------------------------------------------------------
void EventMap::assign(const EventMap& rhs)
{
  id = rhs.id;
  fk_scene = rhs.fk_scene;
  fk_event = rhs.fk_event;
}
//--------------------------------------------------------------------------------------------
void EventMap::clear()
{
  id = -1;
  fk_scene->clear();
  fk_event->clear();
}