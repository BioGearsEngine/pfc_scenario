#include "CitationMap.h"

#include "Scene.h"
#include "Citation.h"

CitationMap::CitationMap(QObject* parent)
  : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------
bool CitationMap::operator==(const CitationMap& rhs) const
{
  return fk_scene == rhs.fk_scene
    && fk_citation == rhs.fk_citation;
}
//--------------------------------------------------------------------------------------------
bool CitationMap::operator!=(const CitationMap& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
CitationMap* CitationMap::make()
{
  return new CitationMap();
}
//--------------------------------------------------------------------------------------------
void CitationMap::assign(CitationMap* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//--------------------------------------------------------------------------------------------
void CitationMap::assign(const CitationMap& rhs)
{
  id = rhs.id;
  fk_scene = rhs.fk_scene;
  fk_citation = rhs.fk_citation;
}
//--------------------------------------------------------------------------------------------
void CitationMap::clear()
{
  id = -1;
  fk_scene->clear();
  fk_citation->clear();
}