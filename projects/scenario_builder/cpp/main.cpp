#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include "Scenario.h"

int main(int argc, char* argv[])
{
  /* Commenti
  g the following line resolves the issue but then UI elements looks small */
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);
  QQuickStyle::setStyle("Material");
  QQmlApplicationEngine engine;

  qmlRegisterType<pfc::Scenario>("com.ara.pfc.ScenarioModel", 1,0, "ScenarioModel");
  
  engine.load(QUrl(QLatin1String("qrc:/Main.qml")));
  if (engine.rootObjects().isEmpty())
    return -1;

  return app.exec();
}
       