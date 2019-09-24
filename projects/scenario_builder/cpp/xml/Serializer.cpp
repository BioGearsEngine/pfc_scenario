#include "Serializer.h"

#include <string>

#include "../zip/ZipFile.h"
#include "../zip/streams/memstream.h"
#include "../zip/methods/Bzip2Method.h"

namespace pfc {

Serializer::Serializer() {}
Serializer::~Serializer() {}

void Serializer::Save(SQLite3Driver* db)
{
}

bool Serializer::Load(std::string& filename, SQLite3Driver* db)
{
  ZipArchive::Ptr archive = ZipFile::Open("filename");
  if (archive) {
    return true;
  } else {
    return false;
  }
}

}// namespace pfc
