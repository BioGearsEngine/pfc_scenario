#include "Serializer.h"

#include <string>

#include <QDebug>

//#include "../zip/ZipFile.h"
//#include "../zip/methods/Bzip2Method.h"
//#include "../zip/streams/memstream.h"
#include <regex>

namespace pfc {

Serializer::Serializer(QObject* parent)
  : QObject(parent)
{
}

Serializer::~Serializer() {}

bool Serializer::save()
{
  if (!_db) {
    return false;
  }
}

bool Serializer::load(const QString& filename)
{
  if (!_db) {
    return false;
  }
  //try {
  //  ZipArchive::Ptr archive = ZipFile::Open(filename.toStdString());
  //  qInfo() << "OPENED!";
  //  qInfo() << QString("%1 constains %2 entries").arg(filename).arg(archive->GetEntriesCount());
  //  for (auto i = 0; i < archive->GetEntriesCount(); ++i) {
  //    qInfo() << QString("%1").arg(archive->GetEntry(i)->GetName().c_str());
  //  }
  //  return true;
  //} catch (std::runtime_error e){
  //  qCritical() << "FAILED! "  << e.what();
    return false;
  //}
}

} // namespace pfc
