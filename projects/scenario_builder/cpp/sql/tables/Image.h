#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_IMAGES_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_IMAGES_H

/**************************************************************************************
Copyright 2019 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#include <QList>
#include <QObject>
#include <QString>

struct Objective;

struct Image : public QObject {
  Q_OBJECT
  Q_PROPERTY(int assessment_id MEMBER id NOTIFY idChanged)
  Q_PROPERTY(QString uuid MEMBER uuid NOTIFY uuidChanged)
  Q_PROPERTY(QString uri READ URI WRITE URI NOTIFY uriChanged)
  Q_PROPERTY(int width MEMBER width NOTIFY widthChanged)
  Q_PROPERTY(int height MEMBER height NOTIFY heightChanged)
  Q_PROPERTY(QString format MEMBER format NOTIFY formatChanged)
signals:
  void idChanged();
  void uuidChanged();
  void uriChanged();
  void widthChanged();
  void heightChanged();
  void formatChanged();

public:
  int32_t id = -1;
  QString uuid = "";
  QString uri = "";
  int width= 0;
  int height= 0;
  QString format = "jpg";

  Image(QObject* parent = nullptr);
  Image(const Image&) = delete;
  Image(Image&&) = delete;
  Image& operator=(const Image&) = delete;
  Image& operator=(Image&&) = delete;
  virtual ~Image() = default;

  bool operator==(const Image& rhs) const;
  bool operator!=(const Image& rhs) const;

  void URI(QString uri);
  QString URI();

  static Q_INVOKABLE Image* make();
  Q_INVOKABLE bool cache(QString cache_name, QString placeholder_uri = "");

  Q_INVOKABLE void assign(Image* rhs);
  void assign(const Image& rhs);

  Q_INVOKABLE void clear();
  Q_INVOKABLE void clear(int index);
};
//----End Image

#endif
