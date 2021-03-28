#include "Image.h"

#include <QDebug>
#include <QDir>
#include <QImage>
#include <QImageWriter>
#include <QUrl>

#include <iostream>

Image::Image(QObject* parent)
  : QObject(parent)
{
}

//--------------------------------------------------------------------------------------------
bool Image::operator==(const Image& rhs) const
{
  return id == rhs.id
    && uuid == rhs.uuid
    && uri == rhs.uri
    && height == rhs.height
    && width == rhs.width
    && format == rhs.format;
}
//--------------------------------------------------------------------------------------------
bool Image::operator!=(const Image& rhs) const
{
  return !(*this == rhs);
}
//--------------------------------------------------------------------------------------------
Image* Image::make()
{
  return new Image();
}
//--------------------------------------------------------------------------------------------
void Image::URI(QString u)
{
  uri = u;
}
//--------------------------------------------------------------------------------------------
QString Image::URI()
{
  return uri;
}
//--------------------------------------------------------------------------------------------
bool Image::cache(QString cache_uri, QString placeholder_uri)
{
  QUrl uri_resource { uri };
  QImage source;

  if (source.load(uri_resource.toLocalFile()) && !source.isNull()) {
    id = -1;
    uuid.clear();
    uri = "tmp/images/" + cache_uri + ".jpeg";
    width = source.width();
    height = source.height();

    QDir temporary_dir(".");
    temporary_dir.mkdir("tmp");
    temporary_dir.mkdir("tmp/images");

    QImageWriter writer { uri };
    writer.setFormat("JPEG"); // same as writer.setFormat("PNG");
    if (writer.canWrite()) {
      if (writer.write(source)) {
        QFileInfo cache{ writer.fileName() };
        uri = QUrl::fromLocalFile(cache.absoluteFilePath()).toString();
        return true;
      } else {
        qDebug() << "Failed to cache "
                 << uri << "Error: " << writer.error();
        return false;
      }
    } else {
      qDebug() << "System is unable to cache to "
               << uri;
      return false;
    }
  } else {
    id = -1;
    uri = placeholder_uri;
    width = -1;
    height = -1;
    return false;
  }
  return true;
}
//--------------------------------------------------------------------------------------------
void Image::assign(Image* rhs)
{
  if (rhs) {
    assign(*rhs);
  }
}
//--------------------------------------------------------------------------------------------
void Image::assign(const Image& rhs)
{
  id = rhs.id;
  uuid = rhs.uuid;
  uri = rhs.uri;
  width = rhs.width;
  height = rhs.height;
  format = rhs.format;
}
//--------------------------------------------------------------------------------------------
void Image::clear()
{
  id = -1;
  uuid.clear();
  uri.clear();
  width = 0;
  height = 0;
  format = "Unknown";
}
//--------------------------------------------------------------------------------------------
void Image::clear(int index)
{
  id = -1;
  uuid.clear();
  uri = QString(" New Image:%1").arg(index);
  width = 0;
  height = 0;
  format = "Unknown";
}