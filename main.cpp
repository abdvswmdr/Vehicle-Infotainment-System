#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "controllers/headers/system.h"
#include "controllers/headers/hvachandler.h"
#include "controllers/headers/audiocontroller.h"
#include "controllers/headers/canbuscontroller.h"
#include "controllers/headers/vehicledatacontroller.h"
#include "controllers/headers/mediacontroller.h"


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
	CanBusController m_canBusController;
	VehicleDataController m_vehicleDataController;
	MediaController m_mediaController;
	
  QQmlApplicationEngine engine;
  
	// Connect CAN bus to vehicle data controller
	QObject::connect(&m_canBusController, &CanBusController::frameReceived,
					 &m_vehicleDataController, &VehicleDataController::processCanFrame);
	
	// Connect audio controller to media controller for volume sync
	QObject::connect(&m_audioController, &AudioController::volumeLevelChanged,
					 &m_mediaController, &MediaController::setVolume);
	QObject::connect(&m_mediaController, &MediaController::volumeChanged,
					 &m_audioController, &AudioController::setVolumeLevel);
	
	// Start CAN bus simulation
	m_canBusController.connectToSimulator();
	
  // Set context property BEFORE loading QML
	QQmlContext * context( engine.rootContext() );
	context->setContextProperty( "systemHandler", &m_systemHandler );
	context->setContextProperty( "driverHVAC", &m_driverHvacHandler );
	context->setContextProperty( "passengerHVAC", &m_passengerHvacHandler );
	context->setContextProperty( "audioController", &m_audioController );
	context->setContextProperty( "canBusController", &m_canBusController );
	context->setContextProperty( "vehicleData", &m_vehicleDataController );
	context->setContextProperty( "mediaController", &m_mediaController );
	
  engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));
  if (engine.rootObjects().isEmpty())
    exit(-1);
	
  return app.exec();
}
