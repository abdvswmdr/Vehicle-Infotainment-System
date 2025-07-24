#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "controllers/headers/system.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
	
  QGuiApplication app(argc, argv);

	System m_systemHandler;
	
  QQmlApplicationEngine engine;
  
  // Set context property BEFORE loading QML
	QQmlContext * context( engine.rootContext() );
	context->setContextProperty( "systemHandler", &m_systemHandler );
	
  engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));
  if (engine.rootObjects().isEmpty())
    exit(-1);
	
  return app.exec();
}
