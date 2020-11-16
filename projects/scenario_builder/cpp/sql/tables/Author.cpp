#include "Author.h"

  Author::Author(QObject* parent)
  : QObject(parent)
{
}
//--------------------------------------------------------------------------------------------
bool Author::operator==(const Author& rhs) const
{
  return id == rhs.id

    && first == rhs.first
    && middle == rhs.middle
    && last == rhs.last
    && email == rhs.email
    && zip == rhs.zip
    && plus_4 == rhs.plus_4
    && state == rhs.state
    && country == rhs.country
    && phone == rhs.phone
    && organization == rhs.organization;
}
//--------------------------------------------------------------------------------------------
bool Author::operator!=(const Author& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
//! \brief Designed to be passed to QML and have ownership assumed.
Author* Author::make()
{
  return new Author();
}
//--------------------------------------------------------------------------------------------
 void Author::assign(Author* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//--------------------------------------------------------------------------------------------
void Author::assign(const Author& rhs)
{
  id = rhs.id;
  uuid = rhs.uuid;
  first = rhs.first;
  middle = rhs.middle;
  last = rhs.last;
  email = rhs.email;
  zip = rhs.zip;
  plus_4 = rhs.plus_4;
  state = rhs.state;
  country = rhs.country;
  phone = rhs.phone;
  organization = rhs.organization;
}
//--------------------------------------------------------------------------------------------
void Author::clear()
{
  id = -1;
  uuid.clear();
  first.clear();
  middle.clear();
  last.clear();
  email.clear();
  zip.clear();
  plus_4.clear();
  state.clear();
  country.clear();
  phone.clear();
  organization.clear();
}
void Author::clear(int index)
{
  id = -1;
  uuid.clear();
  first = QString("Author %1").arg(index);
  middle.clear();
  last = QString("Author %1").arg(index);
  email = QString("Email@Author%1.com").arg(index);
  zip.clear();
  plus_4.clear();
  state.clear();
  country.clear();
  phone.clear();
  organization.clear();
}
//--------------------------------------------------------------------------------------------