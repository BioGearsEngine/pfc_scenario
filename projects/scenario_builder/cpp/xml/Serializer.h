#ifndef PFC_VISUALIZER_XML_SERALIZER_H
#define PFC_VISUALIZER_XML_SERALIZER_H

#include <QObject>
#include <sstream>

#include "../sql/SqlLite3Driver.h"

namespace pfc {
class Serializer : public QObject {
public:
  Q_OBJECT

  Q_PROPERTY(SQLite3Driver* db MEMBER _db)
  Q_PROPERTY(QString file MEMBER _sourceFile)
public:
  explicit Serializer(QObject* parent = nullptr);
  Serializer(const Serializer&) = delete;
  Serializer(Serializer&&) = delete;
  Serializer& operator=(const Serializer&) = delete;
  Serializer& operator=(Serializer&&) = delete;
  ~Serializer();

  Q_INVOKABLE bool save() const;
  Q_INVOKABLE bool save_as(const QString& Save) const;
  Q_INVOKABLE bool load(const QString& filename);

signals:
  void dbChanged();

protected:
  QString get_property(SQLite3Driver* db, const QString& name) const;
  auto generate_msdl_stream() const -> std::stringstream;
  auto generate_pfc_stream() const -> std::stringstream;
  bool update_property(SQLite3Driver* db, const std::string& name, const std::string& value) const;

private:
  SQLite3Driver* _db = nullptr;
  QString _sourceFile = "PFC_SCENARIO.pfc.zip";
  std::map<std::string, std::string> _images; //Map of Image Keys,Paths
  QList<QString> _known_images;
  QList<QString> _known_schemas;
};

}
#endif //PFC_VISUALIZER_XML_SERALIZER_H
