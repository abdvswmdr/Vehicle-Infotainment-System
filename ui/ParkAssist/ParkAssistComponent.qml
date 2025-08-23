import QtQuick 2.15

Rectangle {
    id: parkAssist
    width: 400
    height: 350
    color: "#1a1a1a"
    radius: 8
    border.color: "#333"
    border.width: 1

    property bool parkAssistActive: false
    property var sensorDistances: [150, 120, 80, 200, 180, 90, 110, 140] // cm, front and rear sensors
    property int warningThreshold: 50 // cm

    Rectangle {
        id: header
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 50
        color: "#2a2a2a"
        radius: parent.radius
        
        Text {
            anchors.centerIn: parent
            text: "Park Assist"
            color: "#ffffff"
            font.pixelSize: 18
            font.bold: true
        }
        
        Rectangle {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 1
            color: "#444"
        }
    }

    // Vehicle representation
    Rectangle {
        id: vehicleBody
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 10
        width: 120
        height: 200
        color: "#4a4a4a"
        radius: 8
        border.color: "#666"
        border.width: 2

        // Front of car
        Rectangle {
            id: frontBumper
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 10
            height: 8
            color: "#666"
            radius: 2
        }

        // Rear of car
        Rectangle {
            id: rearBumper
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 10
            height: 8
            color: "#666"
            radius: 2
        }

        // Wheels
        Repeater {
            model: 4
            Rectangle {
                width: 15
                height: 25
                color: "#222"
                radius: 3
                x: index % 2 === 0 ? -7 : vehicleBody.width - 8
                y: index < 2 ? 20 : vehicleBody.height - 45
            }
        }
    }

    // Front sensors
    Row {
        anchors.bottom: vehicleBody.top
        anchors.bottomMargin: 20
        anchors.horizontalCenter: vehicleBody.horizontalCenter
        spacing: 15

        Repeater {
            model: 4
            Rectangle {
                width: 8
                height: 8
                radius: 4
                color: getSensorColor(sensorDistances[index])
                
                Rectangle {
                    anchors.centerIn: parent
                    width: 4
                    height: 4
                    radius: 2
                    color: "#fff"
                    opacity: 0.8
                }

                // Distance indicator
                Rectangle {
                    anchors.bottom: parent.top
                    anchors.bottomMargin: getSensorBeamLength(sensorDistances[index])
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 2
                    height: getSensorBeamLength(sensorDistances[index])
                    color: getSensorColor(sensorDistances[index])
                    opacity: 0.6
                    radius: 1
                }
            }
        }
    }

    // Rear sensors
    Row {
        anchors.top: vehicleBody.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: vehicleBody.horizontalCenter
        spacing: 15

        Repeater {
            model: 4
            Rectangle {
                width: 8
                height: 8
                radius: 4
                color: getSensorColor(sensorDistances[index + 4])
                
                Rectangle {
                    anchors.centerIn: parent
                    width: 4
                    height: 4
                    radius: 2
                    color: "#fff"
                    opacity: 0.8
                }

                // Distance indicator
                Rectangle {
                    anchors.top: parent.bottom
                    anchors.topMargin: 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 2
                    height: getSensorBeamLength(sensorDistances[index + 4])
                    color: getSensorColor(sensorDistances[index + 4])
                    opacity: 0.6
                    radius: 1
                }
            }
        }
    }

    // Side distance displays
    Rectangle {
        id: leftDistance
        anchors.right: vehicleBody.left
        anchors.rightMargin: 30
        anchors.verticalCenter: vehicleBody.verticalCenter
        width: 60
        height: 30
        color: "#2a2a2a"
        radius: 4
        border.color: "#444"
        border.width: 1

        Text {
            anchors.centerIn: parent
            text: Math.min(...sensorDistances.slice(0, 4)) + " cm"
            color: getSensorColor(Math.min(...sensorDistances.slice(0, 4)))
            font.pixelSize: 12
            font.bold: true
        }

        Text {
            anchors.bottom: parent.top
            anchors.bottomMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            text: "FRONT"
            color: "#aaa"
            font.pixelSize: 10
        }
    }

    Rectangle {
        id: rightDistance
        anchors.left: vehicleBody.right
        anchors.leftMargin: 30
        anchors.verticalCenter: vehicleBody.verticalCenter
        width: 60
        height: 30
        color: "#2a2a2a"
        radius: 4
        border.color: "#444"
        border.width: 1

        Text {
            anchors.centerIn: parent
            text: Math.min(...sensorDistances.slice(4, 8)) + " cm"
            color: getSensorColor(Math.min(...sensorDistances.slice(4, 8)))
            font.pixelSize: 12
            font.bold: true
        }

        Text {
            anchors.bottom: parent.top
            anchors.bottomMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            text: "REAR"
            color: "#aaa"
            font.pixelSize: 10
        }
    }

    // Warning indicator
    Rectangle {
        anchors.top: header.bottom
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 15
        width: 20
        height: 20
        radius: 10
        color: getMinDistance() < warningThreshold ? "#ff4444" : "#00aa00"
        
        Behavior on color {
            ColorAnimation { duration: 200 }
        }

        SequentialAnimation {
            running: getMinDistance() < warningThreshold
            loops: Animation.Infinite
            
            PropertyAnimation {
                target: parent
                property: "opacity"
                to: 0.3
                duration: 500
            }
            PropertyAnimation {
                target: parent
                property: "opacity"
                to: 1.0
                duration: 500
            }
        }
    }

    // Status text
    Text {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        text: {
            if (!parkAssistActive) {
                return "Park Assist Inactive"
            } else if (getMinDistance() < 30) {
                return "⚠️ STOP - Too Close!"
            } else if (getMinDistance() < warningThreshold) {
                return "⚠️ Warning - Obstacle Detected"
            } else {
                return "✓ Clear to maneuver"
            }
        }
        color: {
            if (!parkAssistActive) return "#666"
            else if (getMinDistance() < 30) return "#ff0000"
            else if (getMinDistance() < warningThreshold) return "#ffaa00"
            else return "#00aa00"
        }
        font.pixelSize: 14
        font.bold: true
        
        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }

    // Control toggle
    Rectangle {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15
        anchors.right: parent.right
        anchors.rightMargin: 15
        width: 80
        height: 25
        color: parkAssistActive ? "#00aa44" : "#666"
        radius: 12
        
        Text {
            anchors.centerIn: parent
            text: parkAssistActive ? "ON" : "OFF"
            color: "#fff"
            font.pixelSize: 11
            font.bold: true
        }
        
        MouseArea {
            anchors.fill: parent
            onClicked: {
                parkAssistActive = !parkAssistActive
                if (parkAssistActive) {
                    sensorUpdateTimer.start()
                } else {
                    sensorUpdateTimer.stop()
                }
            }
        }
        
        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }

    Timer {
        id: sensorUpdateTimer
        interval: 500
        running: parkAssistActive
        repeat: true
        onTriggered: updateSensorData()
    }

    function getSensorColor(distance) {
        if (distance < 30) return "#ff0000"      // Red - danger
        else if (distance < 50) return "#ff6600" // Orange - warning  
        else if (distance < 100) return "#ffaa00" // Yellow - caution
        else return "#00aa00"                     // Green - safe
    }

    function getSensorBeamLength(distance) {
        return Math.max(10, Math.min(40, 200 - distance) / 4)
    }

    function getMinDistance() {
        return Math.min(...sensorDistances)
    }

    function updateSensorData() {
        // Simulate realistic parking sensor behavior
        for (var i = 0; i < sensorDistances.length; i++) {
            var change = Math.random() * 20 - 10 // ±10 cm variation
            sensorDistances[i] = Math.max(20, Math.min(200, sensorDistances[i] + change))
        }
        
        // Trigger property change notifications
        sensorDistancesChanged()
    }

    Component.onCompleted: {
        // Start with realistic initial distances
        updateSensorData()
    }
}