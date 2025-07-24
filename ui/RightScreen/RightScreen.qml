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
	name: "osm"
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
	    top: parent.top
	    margins: 20
	}
	
	text: systemHandler.currentTime
	font.pixelSize: parent.width / 30
	font.bold: true
	color: "white"
    }

    width: parent.width * 2/3
}
