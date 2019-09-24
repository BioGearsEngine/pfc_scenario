#ifndef PFC_VISUALIZER_XML_SERALIZER_H
#define PFC_VISUALIZER_XML_SERALIZER_H

#include <QObject>

#include "../sql/SqlLite3Driver.h"

namespace pfc {
class Serializer: public QObject{
public:
  Serializer();
 ~Serializer();

 void Save(SQLite3Driver *db);
 bool Load(std::string& filename, SQLite3Driver* db);

 private:

};
}
#endif //PFC_VISUALIZER_XML_SERALIZER_H

