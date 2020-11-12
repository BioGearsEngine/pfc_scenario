#include "Citation.h"

Citation::Citation(QObject* parent)
  : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------
bool Citation::operator==(const Citation& rhs) const
{
  return id == rhs.id

    && key == rhs.key
    && title == rhs.title
    && authors == rhs.authors
    && year == rhs.year
    && publisher == rhs.publisher;
}
//--------------------------------------------------------------------------------------------
bool Citation::operator!=(const Citation& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
Citation* Citation::make()
{
  return new Citation();
}
//--------------------------------------------------------------------------------------------
Q_INVOKABLE void Citation::assign(Citation* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//--------------------------------------------------------------------------------------------
void Citation::assign(const Citation& rhs)
{
  id = rhs.id;
  uuid = rhs.uuid;
  key = rhs.key;
  title = rhs.title;
  authors = rhs.authors;
  year = rhs.year;
  publisher = rhs.publisher;
}
//--------------------------------------------------------------------------------------------
void Citation::clear()
{
  id = -1;
  uuid.clear();
  key.clear();
  title.clear();
  authors.clear();
  year.clear();
  publisher.clear();
}
//--------------------------------------------------------------------------------------------