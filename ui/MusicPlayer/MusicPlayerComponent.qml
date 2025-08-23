import QtQuick 2.15

Rectangle {
    id: musicPlayer
    width: 400
    height: 300
    color: "#1a1a1a"
    radius: 8
    border.color: "#333"
    border.width: 1

    property bool isPlaying: false
    property string currentSong: "No Track Selected"
    property string currentArtist: "Unknown Artist"
    property int currentTime: 0
    property int totalTime: 240
    property real volume: audioController.targetVolume

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
            text: "Music Player"
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

    // Album art placeholder
    Rectangle {
        id: albumArt
        anchors.top: header.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        width: 80
        height: 80
        color: "#333"
        radius: 4
        
        Image {
            anchors.centerIn: parent
            width: 40
            height: 40
            source: "qrc:/images/musicIcon.png"
            fillMode: Image.PreserveAspectFit
        }
    }

    // Song info
    Column {
        anchors.top: header.bottom
        anchors.topMargin: 20
        anchors.left: albumArt.right
        anchors.leftMargin: 15
        anchors.right: parent.right
        anchors.rightMargin: 20
        spacing: 5

        Text {
            text: currentSong
            color: "#ffffff"
            font.pixelSize: 16
            font.bold: true
            elide: Text.ElideRight
            width: parent.width
        }

        Text {
            text: currentArtist
            color: "#aaa"
            font.pixelSize: 14
            elide: Text.ElideRight
            width: parent.width
        }

        // Progress bar
        Rectangle {
            width: parent.width
            height: 4
            color: "#444"
            radius: 2
            anchors.topMargin: 10

            Rectangle {
                id: progressBar
                width: parent.width * (currentTime / Math.max(totalTime, 1))
                height: parent.height
                color: "#00aaff"
                radius: 2
                
                Behavior on width {
                    SmoothedAnimation { duration: 500 }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var newTime = (mouse.x / width) * totalTime
                    currentTime = newTime
                }
            }
        }

        Row {
            spacing: 10
            Text {
                text: formatTime(currentTime)
                color: "#aaa"
                font.pixelSize: 12
            }
            Text {
                text: "/"
                color: "#666"
                font.pixelSize: 12
            }
            Text {
                text: formatTime(totalTime)
                color: "#aaa"
                font.pixelSize: 12
            }
        }
    }

    // Control buttons
    Row {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20

        // Previous button
        Rectangle {
            width: 40
            height: 40
            color: "#333"
            radius: 20
            
            Text {
                anchors.centerIn: parent
                text: "⏮"
                color: "#fff"
                font.pixelSize: 16
            }
            
            MouseArea {
                anchors.fill: parent
                onClicked: previousTrack()
            }
        }

        // Play/Pause button
        Rectangle {
            width: 50
            height: 50
            color: isPlaying ? "#ff4444" : "#00aa44"
            radius: 25
            
            Text {
                anchors.centerIn: parent
                text: isPlaying ? "⏸" : "▶"
                color: "#fff"
                font.pixelSize: 20
            }
            
            MouseArea {
                anchors.fill: parent
                onClicked: togglePlayPause()
            }
            
            Behavior on color {
                ColorAnimation { duration: 200 }
            }
        }

        // Next button
        Rectangle {
            width: 40
            height: 40
            color: "#333"
            radius: 20
            
            Text {
                anchors.centerIn: parent
                text: "⏭"
                color: "#fff"
                font.pixelSize: 16
            }
            
            MouseArea {
                anchors.fill: parent
                onClicked: nextTrack()
            }
        }
    }

    // Volume control
    Row {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.right: parent.right
        anchors.rightMargin: 20
        spacing: 10

        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: "♪"
            color: "#aaa"
            font.pixelSize: 16
        }

        Rectangle {
            width: 80
            height: 4
            color: "#444"
            radius: 2
            anchors.verticalCenter: parent.verticalCenter

            Rectangle {
                width: parent.width * (volume / 100)
                height: parent.height
                color: "#00aaff"
                radius: 2
                
                Behavior on width {
                    SmoothedAnimation { duration: 200 }
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var newVolume = (mouse.x / width) * 100
                    audioController.setTargetVolume(Math.round(newVolume))
                }
            }
        }
    }

    // Mock playlist
    property var playlist: [
        { title: "Highway Dreams", artist: "Electric Vibes", duration: 240 },
        { title: "City Nights", artist: "Neon Pulse", duration: 195 },
        { title: "Journey Home", artist: "Acoustic Soul", duration: 268 },
        { title: "Digital Horizons", artist: "Synth Wave", duration: 312 },
        { title: "Midnight Drive", artist: "Chrome Hearts", duration: 289 }
    ]
    
    property int currentTrackIndex: 0

    function formatTime(seconds) {
        var mins = Math.floor(seconds / 60)
        var secs = Math.floor(seconds % 60)
        return mins + ":" + (secs < 10 ? "0" : "") + secs
    }

    function togglePlayPause() {
        isPlaying = !isPlaying
        if (isPlaying) {
            playbackTimer.start()
        } else {
            playbackTimer.stop()
        }
    }

    function nextTrack() {
        currentTrackIndex = (currentTrackIndex + 1) % playlist.length
        loadCurrentTrack()
    }

    function previousTrack() {
        currentTrackIndex = currentTrackIndex > 0 ? currentTrackIndex - 1 : playlist.length - 1
        loadCurrentTrack()
    }

    function loadCurrentTrack() {
        var track = playlist[currentTrackIndex]
        currentSong = track.title
        currentArtist = track.artist
        totalTime = track.duration
        currentTime = 0
    }

    Timer {
        id: playbackTimer
        interval: 1000
        running: false
        repeat: true
        onTriggered: {
            if (isPlaying && currentTime < totalTime) {
                currentTime++
            } else if (currentTime >= totalTime) {
                nextTrack()
                if (isPlaying) {
                    togglePlayPause()
                }
            }
        }
    }

    Component.onCompleted: {
        loadCurrentTrack()
    }
}