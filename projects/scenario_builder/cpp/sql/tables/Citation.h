#ifndef PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_CITATION_H
#define PFC_SCENARIO_BUILDER_SQL_SQLLITE3_TABLES_CITATION_H

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

struct Citation : public QObject {
  Q_OBJECT
  Q_PROPERTY(int citation_id MEMBER id)
  Q_PROPERTY(QString uuid MEMBER uuid)
  Q_PROPERTY(QString key MEMBER key)
  Q_PROPERTY(QString title MEMBER title)
  Q_PROPERTY(QString authors MEMBER authors)
  Q_PROPERTY(QString year MEMBER year)
  Q_PROPERTY(QString publisher MEMBER publisher)

public:
  int32_t id = -1;
  QString uuid = "";
  QString key = "";
  QString title = "";
  QString authors;
  QString year = "";
  QString publisher = "";

  Citation(QObject* parent = nullptr);
  Citation(const Citation&) = delete;
  Citation(Citation&&) = delete;
  Citation& operator=(const Citation&) = delete;
  Citation& operator=(Citation&&) = delete;
  virtual ~Citation() = default;

  bool operator==(const Citation& rhs) const;
  bool operator!=(const Citation& rhs) const;

  static Q_INVOKABLE Citation* make();

  Q_INVOKABLE void assign(Citation* rhs);
  void assign(const Citation& rhs);

  void clear();
};
//----End Citation

#endif