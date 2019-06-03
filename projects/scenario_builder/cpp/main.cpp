
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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include "Scenario.h"
#include "Locations.h"
#include "Objects.h"
#include "Scenes.h"
#include "Actors.h"

int main(int argc, char* argv[])
{
  /* Commenti
  g the following line resolves the issue but then UI elements looks small */
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);
  QQuickStyle::setStyle("Material");
  QQmlApplicationEngine engine;

  qmlRegisterType<pfc::Scenario>("com.ara.pfc.ScenarioModel", 1,0, "ScenarioModel");
  qmlRegisterType<pfc::LocationSequence>("com.ara.pfc.ScenarioModel.Locations", 1,0, "Locations");
  qmlRegisterType<pfc::ObjectSequence>("com.ara.pfc.ScenarioModel.Objects"  , 1,0, "Objects");
  qmlRegisterType<pfc::ActorSequence>("com.ara.pfc.ScenarioModel.Actors"   , 1,0, "Actors");
  qmlRegisterType<pfc::NarativeSequence>("com.ara.pfc.ScenarioModel.Narative"   , 1,0, "Naratives");

  engine.load(QUrl(QLatin1String("qrc:/Main.qml")));
  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}
       
