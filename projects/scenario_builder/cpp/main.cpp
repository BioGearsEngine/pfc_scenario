
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

#include <QApplication>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QSplashScreen>
#include <QList>
#include "sql/SqlLite3Driver.h"

int main(int argc, char* argv[])
{
  /* Commenti 
  g the following line resolves the issue but then UI elements looks small */
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication app(argc, argv);
  QQuickStyle::setStyle("Material");
  QQmlApplicationEngine engine;


  //UI 0.1 Types
  qmlRegisterType<pfc::SQLite3Driver>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "SQLBackend");
  qmlRegisterType<pfc::Author>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Author");
  qmlRegisterType<pfc::Property>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Property");
  qmlRegisterType<pfc::Restriction>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Restriction");
  qmlRegisterType<pfc::Objective>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Objective");

  QPixmap pixmap("D:/biogears/pfc/scenario/build/img/sustain_splash.png");
  qDebug() << QString("%1,%2").arg(pixmap.height()).arg(pixmap.width()); //1280x768
  QSplashScreen splash(pixmap);
  splash.show();
  engine.load(QUrl(QLatin1String("qrc:/ScenarioBuilder.qml")));
  if (engine.rootObjects().isEmpty())
    return -1;
  splash.close();

  return app.exec();
}
 