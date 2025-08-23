import QtQuick 2.15
import "."

Rectangle {
    id: dashboard
    width: 800
    height: 500
    color: "#0a0a0a"
    radius: 8

    // Main dashboard layout
    Row {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 30

        // Left side - Speedometer and main gauges
        Column {
            spacing: 20
            width: 250

            Speedometer {
                id: speedometer
                anchors.horizontalCenter: parent.horizontalCenter
            }

            // Fuel and temperature gauges
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 20

                CircularGauge {
                    id: fuelGauge
                    width: 100
                    height: 100
                    value: vehicleData.fuelLevel
                    maxValue: 100
                    title: "FUEL"
                    unit: "%"
                    color: value < 20 ? "#ff4444" : "#00aa44"
                    warningThreshold: 20
                }

                CircularGauge {
                    id: tempGauge
                    width: 100
                    height: 100
                    value: vehicleData.engineTemperature
                    maxValue: 120
                    title: "TEMP"
                    unit: "°C"
                    color: value > 100 ? "#ff4444" : "#00aaff"
                    warningThreshold: 105
                }
            }
        }

        // Center - Tachometer and gear display
        Column {
            spacing: 20
            width: 200

            TachometerGauge {
                id: tachometer
                anchors.horizontalCenter: parent.horizontalCenter
            }

            // Gear and status display
            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 120
                height: 80
                color: "#1a1a1a"
                radius: 8
                border.color: "#333"
                border.width: 1

                Column {
                    anchors.centerIn: parent
                    spacing: 5

                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "GEAR"
                        color: "#666"
                        font.pixelSize: 12
                    }

                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: vehicleData.gear
                        color: "#ffffff"
                        font.pixelSize: 32
                        font.bold: true
                    }
                }
            }
        }

        // Right side - Warning lights and vehicle status
        Column {
            spacing: 15
            width: 300

            // Warning lights grid
            Rectangle {
                width: 280
                height: 200
                color: "#1a1a1a"
                radius: 8
                border.color: "#333"
                border.width: 1

                Text {
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "TELLTALES"
                    color: "#666"
                    font.pixelSize: 14
                    font.bold: true
                }

                Grid {
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: 10
                    columns: 4
                    spacing: 15

                    // Turn signals
                    WarningLight {
                        active: vehicleData.leftTurnSignal
                        color: "#00aa00"
                        symbol: "◀"
                        blinking: true
                    }

                    WarningLight {
                        active: vehicleData.rightTurnSignal
                        color: "#00aa00"
                        symbol: "▶"
                        blinking: true
                    }

                    // Engine warning
                    WarningLight {
                        active: vehicleData.engineTemperature > 105
                        color: "#ff4444"
                        symbol: "🌡"
                        blinking: false
                    }

                    // Low fuel warning
                    WarningLight {
                        active: vehicleData.fuelLevel < 20
                        color: "#ffaa00"
                        symbol: "⛽"
                        blinking: vehicleData.fuelLevel < 10
                    }

                    // Battery warning
                    WarningLight {
                        active: vehicleData.batteryVoltage < 12
                        color: "#ff4444"
                        symbol: "🔋"
                        blinking: false
                    }

                    // Headlights
                    WarningLight {
                        active: vehicleData.headlights
                        color: "#00aaff"
                        symbol: "💡"
                        blinking: false
                    }

                    // Parking brake
                    WarningLight {
                        active: vehicleData.parkingBrake
                        color: "#ff4444"
                        symbol: "🅿"
                        blinking: false
                    }

                    // Seatbelt
                    WarningLight {
                        active: !vehicleData.seatbelt && vehicleData.engineRunning
                        color: "#ff4444"
                        symbol: "🔗"
                        blinking: true
                    }
                }
            }

            // Vehicle status information
            Rectangle {
                width: 280
                height: 120
                color: "#1a1a1a"
                radius: 8
                border.color: "#333"
                border.width: 1

                Column {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 8

                    Text {
                        text: "VEHICLE STATUS"
                        color: "#666"
                        font.pixelSize: 14
                        font.bold: true
                    }

                    Row {
                        spacing: 20
                        Text {
                            text: "Odometer:"
                            color: "#aaa"
                            font.pixelSize: 12
                        }
                        Text {
                            text: vehicleData.odometer.toFixed(1) + " km"
                            color: "#fff"
                            font.pixelSize: 12
                            font.family: "monospace"
                        }
                    }

                    Row {
                        spacing: 20
                        Text {
                            text: "Battery:"
                            color: "#aaa"
                            font.pixelSize: 12
                        }
                        Text {
                            text: vehicleData.batteryVoltage + "V"
                            color: vehicleData.batteryVoltage < 12 ? "#ff4444" : "#00aa44"
                            font.pixelSize: 12
                            font.family: "monospace"
                        }
                    }

                    Row {
                        spacing: 20
                        Text {
                            text: "Engine:"
                            color: "#aaa"
                            font.pixelSize: 12
                        }
                        Text {
                            text: vehicleData.engineRunning ? "RUNNING" : "OFF"
                            color: vehicleData.engineRunning ? "#00aa44" : "#666"
                            font.pixelSize: 12
                            font.bold: true
                        }
                    }
                }
            }

            // CAN Bus status
            Rectangle {
                width: 280
                height: 40
                color: "#1a1a1a"
                radius: 8
                border.color: "#333"
                border.width: 1

                Row {
                    anchors.centerIn: parent
                    spacing: 10

                    Rectangle {
                        width: 8
                        height: 8
                        radius: 4
                        color: canBusController.connected ? "#00aa44" : "#ff4444"
                        
                        SequentialAnimation {
                            running: canBusController.connected
                            loops: Animation.Infinite
                            
                            PropertyAnimation {
                                target: parent
                                property: "opacity"
                                to: 0.3
                                duration: 1000
                            }
                            PropertyAnimation {
                                target: parent
                                property: "opacity"
                                to: 1.0
                                duration: 1000
                            }
                        }
                    }

                    Text {
                        text: "CAN Bus: " + canBusController.status
                        color: canBusController.connected ? "#00aa44" : "#ff4444"
                        font.pixelSize: 11
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }
}