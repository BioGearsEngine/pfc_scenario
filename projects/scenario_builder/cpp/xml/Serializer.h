#ifndef PFC_VISUALIZER_XML_SERALIZER_H
#define PFC_VISUALIZER_XML_SERALIZER_H

#include <sstream>
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
protected:
  QString get_property(const QString& name);
  void generate_msdl_stream();
  void generate_pfc_stream();
private:
  SQLite3Driver* _db = nullptr;
  std::stringstream _msdl_content;
  std::stringstream _pfc_content;
  std::map<std::string, std::string> _images;  //Map of Image Keys,Paths
};
}
#endif //PFC_VISUALIZER_XML_SERALIZER_H
