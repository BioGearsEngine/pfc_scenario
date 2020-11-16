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
    && publisher == rhs.publisher
    && url == rhs.url
    && address == rhs.address;
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
  url = rhs.url;
  address = rhs.address;
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
  url.clear();
  address.clear();
}
//--------------------------------------------------------------------------------------------
void Citation::clear(int index, bool isCPG)
{
  id = -1;
  uuid.clear();
  authors = "Unknown Author";
  year = "Unknown Year";
  publisher = "Unknown publisher";
  url.clear();
  address.clear();
  if (isCPG) {
    key = QString("CPG:%1").arg(index);
    title = QString("CPG %1").arg(index);
  } else {
    key = QString("AuthorYear_%1").arg(index);
    title = QString("Reference %1").arg(index);
  }
}