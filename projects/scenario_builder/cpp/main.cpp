
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

#include "PlatformHelper.h"
#include "sql/SqlLite3Driver.h"
#include "xml/Serializer.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileSelector>
#include <QList>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QSettings>
#include <QSplashScreen>

int main(int argc, char* argv[])
{
  /* Commenting the following line resolves the issue but then UI elements looks small */
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication app(argc, argv);
  app.setOrganizationName("Applied Research Associates");
  app.setOrganizationDomain("https://biogearsengine.com");

  QQuickStyle::setStyle("Material");
  QQmlApplicationEngine engine;

  //UI Types
  qmlRegisterType<pfc::SQLite3Driver>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "SQLBackend");
  qmlRegisterType<Author>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Author");
  qmlRegisterType<Assessment>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Assessment");
  qmlRegisterType<Citation>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Citation");
  qmlRegisterType<Equipment>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Equipment");
  qmlRegisterType<EquipmentParameter>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "EquipmentParameter");
  qmlRegisterType<ParameterField>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "ParameterField");
  qmlRegisterType<Sustain>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Sustain");
  qmlRegisterType<EquipmentMap>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "EquipmentMap");
  qmlRegisterType<Event>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Event");
  qmlRegisterType<EventMap>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "EventMap");
  qmlRegisterType<Image>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Image");
  qmlRegisterType<Trauma>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Trauma");
  qmlRegisterType<TraumaProfile>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "TraumaProfile");
  qmlRegisterType<TraumaOccurence>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "TraumaOccurence");
  qmlRegisterType<Location>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Location");
  qmlRegisterType<LocationMap>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "LocationMap");
  qmlRegisterType<Objective>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Objective");
  qmlRegisterType<Property>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Property");
  qmlRegisterType<Role>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Role");
  qmlRegisterType<RoleMap>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "RoleMap");
  qmlRegisterType<Scene>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Scene");
  qmlRegisterType<Treatment>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Treatment");

  qmlRegisterType<pfc::Serializer>("com.ara.pfc.ScenarioModel.XML", 1, 0, "XMLSeralizer");
  qmlRegisterType<pfc::PlatformHelper>("com.ara.pfc.ScenarioModel", 1, 0, "SustainUtils");

  QPixmap pixmap("qrc://img/sustain_splash.png");
  QSplashScreen splash(pixmap);
  splash.show();
  engine.load(QUrl(QLatin1String("qrc:/ScenarioBuilder.qml")));
  if (engine.rootObjects().isEmpty())
    return -1;
  splash.close();

  QFile file("pfc_sb_working.sqlite");
  file.remove();

  QFileSelector selector;
  //QFile defaultsFile(selector.select("SustainScenarioBuilder.conf"));

  return app.exec();
}
