#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "controllers/headers/system.h"
#include "controllers/headers/hvachandler.h"
#include "controllers/headers/audiocontroller.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
	
  QGuiApplication app(argc, argv);

	System m_systemHandler;
	HvacHandler m_driverHvacHandler;
	HvacHandler m_passengerHvacHandler;
	AudioController m_audioController;
	
  QQmlApplicationEngine engine;
  
  // Set context property BEFORE loading QML
	QQmlContext * context( engine.rootContext() );
	context->setContextProperty( "systemHandler", &m_systemHandler );
	context->setContextProperty( "driverHVAC", &m_driverHvacHandler );
	context->setContextProperty( "passengerHVAC", &m_passengerHvacHandler );
	context->setContextProperty( "audioController", &m_audioController );
	
  engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));
  if (engine.rootObjects().isEmpty())
    exit(-1);
	
  return app.exec();
}
