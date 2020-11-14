#include "Assessment.h"

#include "Objective.h"
#include "Citation.h"

Assessment::Assessment(QObject* parent)
  : QObject(parent)
  , fk_objective(new Objective)
{
}
//--------------------------------------------------------------------------------------------
Assessment::Assessment(Objective* objective, QObject* parent)
  : QObject(parent)
  , fk_objective(objective)
{
}
//--------------------------------------------------------------------------------------------
bool Assessment::operator==(const Assessment& rhs) const
{
  return id == rhs.id

    && name == rhs.name
    && description == rhs.description
    && type == rhs.type
    && fk_objective == rhs.fk_objective
    && available_points == rhs.available_points
    && criteria == rhs.criteria;
}
//--------------------------------------------------------------------------------------------
bool Assessment::operator!=(const Assessment& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
Assessment* Assessment::make()
{
  return new Assessment();
}
//--------------------------------------------------------------------------------------------
void Assessment::assign(Assessment* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//--------------------------------------------------------------------------------------------
void Assessment::assign(const Assessment& rhs)
{
  id = rhs.id;
  uuid = rhs.uuid;
  name = rhs.name;
  description = rhs.description;
  type = rhs.type;
  available_points = rhs.available_points;
  fk_objective = rhs.fk_objective;
  criteria = rhs.criteria;
}
//--------------------------------------------------------------------------------------------
void Assessment::clear()
{
  id = -1;
  uuid.clear();
  name.clear();
  description.clear();
  type.clear();
  fk_objective->clear();
  available_points = -1;
  criteria.clear();
}
//--------------------------------------------------------------------------------------------
void Assessment::clear(int index)
{
  id = -1;
  uuid.clear();
  name = QString("New Assessment:%1").arg(index);
  description = QString("Undefined assessment criteria %1.").arg(index);
  type.clear();
  fk_objective->clear();
  available_points = -1;
  criteria.clear();
}