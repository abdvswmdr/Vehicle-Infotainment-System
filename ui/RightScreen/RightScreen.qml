import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15
import "../MusicPlayer"
import "../Phone"

Rectangle {
    id: rightScreen

    anchors {
	top: parent.top
	bottom: bottomBar.top
	right: parent.right
    }
    
    property string currentContent: "map" // "map", "music", "phone"

    Plugin {
	id: mapPlugin
	name: "mapboxgl"
    }

    // Map view (default)
    Rectangle {
        id: mapContainer
        anchors.fill: parent
        visible: currentContent === "map"
        
        Map {
            anchors.fill: parent
            plugin: mapPlugin
            center: QtPositioning.coordinate(59.91, 10.76) //Oslo
            zoomLevel: 14
        }
        
        Image {
            id: lockIcon
            anchors {
                left: parent.left
                top: parent.top
                margins: 20 
            }

            width: parent.width / 38
            fillMode: Image.PreserveAspectFit
            source: ( systemHandler.carLocked ? "qrc:/images/padlock.png" : "qrc:/images/padlock-unlock.png" )
            MouseArea {
                anchors.fill: parent
                onClicked: systemHandler.setCarLocked( !systemHandler.carLocked )
            }
        }

        Text {
            id: timeDisplay
            anchors {
                left: lockIcon.right
                verticalCenter: lockIcon.verticalCenter
                leftMargin: 30
            }
            
            text: systemHandler.currentTime
            font.pixelSize: 20
            font.bold: true
            color: "black"
        }

        Text {
            id: temperatureDisplay
            anchors {
                left: timeDisplay.right
                verticalCenter: lockIcon.verticalCenter
                leftMargin: 25
            }
            
            text: systemHandler.outdoorTemp + "°C"
            font.pixelSize: 20
            font.bold: true
            color: "black"
        }

        Rectangle {
            id: recordingIcon
            anchors {
                left: temperatureDisplay.right
                verticalCenter: lockIcon.verticalCenter
                leftMargin: 25
            }
            
            width: 16
            height: 16
            radius: 16
            color: "red"
            border.color: "darkred"
            border.width: 1
            
            Rectangle {
                anchors.centerIn: parent
                width: 4
                height: 4
                radius: 8
                color: "white"
            }
        }

        Image {
            id: userIcon
            anchors {
                left: recordingIcon.right
                verticalCenter: lockIcon.verticalCenter
                leftMargin: 25
            }
            
            width: parent.width / 42
            fillMode: Image.PreserveAspectFit
            source: "qrc:/images/userIcon.png"
        }

        Text {
            id: userNameDisplay
            anchors {
                left: userIcon.right
                verticalCenter: lockIcon.verticalCenter
                leftMargin: 8
            }
            
            text: systemHandler.userName
            font.pixelSize: 20
            font.bold: true
            color: "black"
        }

        NavigationSearchBox {
            id: navSearchBox
            anchors {
                left: lockIcon.left
                top: lockIcon.bottom
                topMargin: 15
            }
            width: parent.width / 3
            height: parent.height / 14
        }
    }
    
    // Music Player view
    Rectangle {
        id: musicContainer
        anchors.fill: parent
        visible: currentContent === "music"
        color: "black"
        
        MusicPlayerComponent {
            anchors.fill: parent
            anchors.margins: 20
        }
        
        // Minimize button
        Rectangle {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 15
            anchors.rightMargin: 15
            width: 30
            height: 30
            radius: 15
            color: "#444444"
            
            Text {
                anchors.centerIn: parent
                text: "−"
                color: "white"
                font.pixelSize: 18
                font.bold: true
            }
            
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    currentContent = "map"
                }
            }
        }
    }
    
    // Phone view
    Rectangle {
        id: phoneContainer
        anchors.fill: parent
        visible: currentContent === "phone"
        color: "black"
        
        PhoneInterface {
            anchors.fill: parent
            anchors.margins: 20
        }
        
        // Minimize button
        Rectangle {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 15
            anchors.rightMargin: 15
            width: 30
            height: 30
            radius: 15
            color: "#444444"
            
            Text {
                anchors.centerIn: parent
                text: "−"
                color: "white"
                font.pixelSize: 18
                font.bold: true
            }
            
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    currentContent = "map"
                }
            }
        }
    }
    
    width: parent.width * 1.6/3    // this sets width ratio for right/left screen
    
    function showMap() {
        currentContent = "map"
    }
    
    function showMusic() {
        currentContent = "music"
    }
    
    function showPhone() {
        currentContent = "phone"
    }
}
