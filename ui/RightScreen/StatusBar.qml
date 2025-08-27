import QtQuick 2.15

Rectangle {
    id: statusBar
    height: 60
    color: "transparent"
    
    Row {
        id: statusRow
        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
            leftMargin: 20
        }
        spacing: 25
        
        Image {
            id: lockIcon
            width: statusBar.height / 2
            fillMode: Image.PreserveAspectFit
            source: ( systemHandler.carLocked ? "qrc:/images/padlock.png" : "qrc:/images/padlock-unlock.png" )
            anchors.verticalCenter: parent.verticalCenter
            
            MouseArea {
                anchors.fill: parent
                onClicked: systemHandler.setCarLocked( !systemHandler.carLocked )
            }
        }

        Text {
            id: timeDisplay
            text: systemHandler.currentTime
            font.pixelSize: 20
            font.bold: true
            color: "black"
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            id: temperatureDisplay
            text: systemHandler.outdoorTemp + "°C"
            font.pixelSize: 20
            font.bold: true
            color: "black"
            anchors.verticalCenter: parent.verticalCenter
        }

        Rectangle {
            id: recordingIcon
            width: 16
            height: 16
            radius: 16
            color: "red"
            border.color: "darkred"
            border.width: 1
            anchors.verticalCenter: parent.verticalCenter
            
            Rectangle {
                anchors.centerIn: parent
                width: 4
                height: 4
                radius: 8
                color: "white"
            }
        }

        Row {
            spacing: 8
            anchors.verticalCenter: parent.verticalCenter
            
            Image {
                id: userIcon
                width: statusBar.height / 2.2
                fillMode: Image.PreserveAspectFit
                source: "qrc:/images/userIcon.png"
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: userNameDisplay
                text: systemHandler.userName
                font.pixelSize: 20
                font.bold: true
                color: "black"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
    
    Row {
        id: connectivityRow
        anchors {
            right: parent.right
            verticalCenter: parent.verticalCenter
            rightMargin: 20
        }
        spacing: 15
        
        // Bluetooth icon
        Rectangle {
            width: 28
            height: 28
            radius: 4
            color: "#007ACC"
            anchors.verticalCenter: parent.verticalCenter
            
            Text {
                anchors.centerIn: parent
                text: "🔵"
                font.pixelSize: 16
                color: "white"
            }
        }
        
        // Wi-Fi icon  
        Rectangle {
            width: 28
            height: 28
            radius: 4
            color: "#00AA44"
            anchors.verticalCenter: parent.verticalCenter
            
            Text {
                anchors.centerIn: parent
                text: "📶"
                font.pixelSize: 16
                color: "white"
            }
        }
        
        // Battery icon
        Rectangle {
            width: 28
            height: 28
            radius: 4
            color: "#FF8800"
            anchors.verticalCenter: parent.verticalCenter
            
            Text {
                anchors.centerIn: parent
                text: "🔋"
                font.pixelSize: 16
                color: "white"
            }
        }
    }
}