#ifndef PFC_VISUALIZER_XML_SERALIZER_H
#define PFC_VISUALIZER_XML_SERALIZER_H

#include <QObject>

#include "../sql/SqlLite3Driver.h"

namespace pfc {
class Serializer : public QObject {
public:
  Q_OBJECT

  Q_PROPERTY(SQLite3Driver* db MEMBER _db)
public:
  explicit Serializer(QObject* parent = nullptr);
  Serializer(const Serializer&) = delete;
  Serializer(Serializer&&) = delete;
  Serializer& operator=(const Serializer&) = delete;
  Serializer& operator=(Serializer&&) = delete;
  ~Serializer();

  Q_INVOKABLE bool save();
  Q_INVOKABLE bool load(const QString& filename);

signals:
  void dbChanged();

private:
  SQLite3Driver* _db = nullptr;
  
};
}
#endif //PFC_VISUALIZER_XML_SERALIZER_H
