#pragma once

#include <QObject>

namespace pfc {
class PlatformHelper : public QObject {
Q_OBJECT
public:
  PlatformHelper(QObject* parent = nullptr);
  virtual ~PlatformHelper() = default;

  Q_INVOKABLE bool make_directory(QString path);
};
}