#include "PlatformHelper.h"

#include <QDir>
#include <QFile>
#include <QUrl>

namespace pfc {

PlatformHelper::PlatformHelper(QObject* parent)
  : QObject(parent)
{
}

bool PlatformHelper::make_directory(QString path)
{
  QUrl url_path { path };

  if (!QDir(url_path.toLocalFile()).exists()) {
    return QDir(url_path.toLocalFile()).mkpath(".");
  }
  return true;
}

}