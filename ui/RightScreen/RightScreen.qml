import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15

Rectangle {
    id: rightScreen

    anchors {
	top: parent.top
	bottom: bottomBar.top
	right: parent.right
    }

    Plugin {
	id: mapPlugin
	name: "mapboxgl"
    }

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
	    margins:20 
	}

	width: parent.width / 40
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
	    // top: parent.top
	    bottom: lockIcon.bottom
	    leftMargin: 40
	}
	
	text: systemHandler.currentTime
	// font.pixelSize: parent.width / 30
	font.pixelSize: 15
	font.bold: true
	color: "black"
    }

    Text {
	id: temperatureDisplay
	anchors {
	    left: timeDisplay.right
	    bottom: lockIcon.bottom
	    leftMargin: 40
	}
	
	text: systemHandler.outdoorTemp + "°C"
	font.pixelSize: 15
	font.bold: true
	color: "black"
    }

    Rectangle {
	id: recordingIcon
	anchors {
	    left: temperatureDisplay.right
	    bottom: lockIcon.bottom
	    leftMargin: 40
	}
	
	width: 16
	height: 16
	radius: 8
	color: "red"
	border.color: "darkred"
	border.width: 1
	
	// Add a small white dot in center to indicate recording
	Rectangle {
	    anchors.centerIn: parent
	    width: 4
	    height: 4
	    radius: 2
	    color: "white"
	}
    }

    Text {
	id: userNameDisplay
	anchors {
	    left: recordingIcon.right
	    bottom: lockIcon.bottom
	    leftMargin: 40
	}
	
	text: systemHandler.userName
	font.pixelSize: 15
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
	height: parent.height / 12
    }

    width: parent.width * 2/3
}
