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
  qmlRegisterType<pfc::Scenario>("com.ara.pfc.ScenarioModel.Locations", 1,0, "ScenarioLocations");
  qmlRegisterType<pfc::Scenario>("com.ara.pfc.ScenarioModel.Objects"  , 1,0, "ScenarioObjects");
  qmlRegisterType<pfc::Scenario>("com.ara.pfc.ScenarioModel.Scenes"   , 1,0, "ScenarioScenes");
  qmlRegisterType<pfc::Scenario>("com.ara.pfc.ScenarioModel.Actors"   , 1,0, "ScenarioActors");

  engine.load(QUrl(QLatin1String("qrc:/Main.qml")));
  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}
       