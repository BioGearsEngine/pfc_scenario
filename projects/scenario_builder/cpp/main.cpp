
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

#include "sql/SqlLite3Driver.h"
#include "xml/Serializer.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
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

  //UI 0.1 Types
  qmlRegisterType<pfc::SQLite3Driver>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "SQLBackend");
  qmlRegisterType<Author>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Author");
  qmlRegisterType<Assessment>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Assessment");
  qmlRegisterType<Citation>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Citation");
  qmlRegisterType<Equipment>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Equipment");
  qmlRegisterType<EquipmentMap>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "EquipmentMap");
  qmlRegisterType<Event>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Event");
  qmlRegisterType<Trauma>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Injury");
  qmlRegisterType<TraumaProfile>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "InjurySet");
  qmlRegisterType<Location>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Location");
  qmlRegisterType<RoleMap>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "RoleMap");
  qmlRegisterType<Objective>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Objective");
  qmlRegisterType<Property>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Property");
  qmlRegisterType<Role>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Role");
  qmlRegisterType<Treatment>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Treatment");
  qmlRegisterType<Scene>("com.ara.pfc.ScenarioModel.SQL", 1, 0, "Scene");

  qmlRegisterType<pfc::Serializer>("com.ara.pfc.ScenarioModel.XML", 1, 0, "XMLSeralizer");

  qmlRegisterInterface<Assessment>("const Assessment*");
  qmlRegisterInterface<Author>("const Author*");
  qmlRegisterInterface<Citation>("const Citation*");
  qmlRegisterInterface<CitationMap>("const CitationMap*");

  qmlRegisterInterface<Equipment>("const Equipment*");
  
  qmlRegisterInterface<EquipmentMap>("const EquipmentMap*");
  qmlRegisterInterface<Event>("const Event*");
  qmlRegisterInterface<EventMap>("const EventMap*");
  qmlRegisterInterface<Location>("const Location*");
  qmlRegisterInterface<LocationMap>("const LocationMap*");
  qmlRegisterInterface<Objective>("const Objective*");
  qmlRegisterInterface<Property>("const Property*");
  qmlRegisterInterface<Role>("const Role*");
  qmlRegisterInterface<RoleMap>("const RoleMap*");
  qmlRegisterInterface<Scene>("const Scene*");
  qmlRegisterInterface<Trauma>("const Trauma*");
  qmlRegisterInterface<TraumaProfile>("const TraumaProfile*");
  qmlRegisterInterface<Treatment>("const Treatment*");

  QPixmap pixmap("D:/biogears/pfc/scenario/build/img/sustain_splash.png");
  qDebug() << QString("%1,%2").arg(pixmap.height()).arg(pixmap.width()); //1280x768
  QSplashScreen splash(pixmap);
  splash.show();
  engine.load(QUrl(QLatin1String("qrc:/ScenarioBuilder.qml")));
  if (engine.rootObjects().isEmpty())
    return -1;
  splash.close();

  QFile file("pfc_sb_working.sqlite");
  file.remove();

  return app.exec();
}
