import QtQuick 2.15

Rectangle {
    id: phoneInterface
    width: 350
    height: 450
    color: "#1a1a1a"
    radius: 8
    border.color: "#333"
    border.width: 1

    property bool inCall: false
    property string currentContact: ""
    property string callDuration: "00:00"
    property int callSeconds: 0

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
            text: "Phone"
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

    // Call status area
    Rectangle {
        id: callStatus
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: inCall ? 100 : 0
        color: "#2a4a2a"
        visible: inCall
        
        Behavior on height {
            SmoothedAnimation { duration: 300 }
        }

        Column {
            anchors.centerIn: parent
            spacing: 5
            visible: inCall

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Connected"
                color: "#00ff00"
                font.pixelSize: 14
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: currentContact
                color: "#ffffff"
                font.pixelSize: 18
                font.bold: true
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: callDuration
                color: "#aaa"
                font.pixelSize: 16
                font.family: "monospace"
            }
        }
    }

    // Recent calls / Contacts list
    Rectangle {
        id: contactsList
        anchors.top: callStatus.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.bottom: controlButtons.top
        anchors.bottomMargin: 10
        color: "transparent"

        Text {
            id: contactsHeader
            anchors.top: parent.top
            anchors.left: parent.left
            text: "Recent Contacts"
            color: "#aaa"
            font.pixelSize: 14
            font.bold: true
        }

        ListView {
            id: contactsListView
            anchors.top: contactsHeader.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            clip: true

            model: ListModel {
                ListElement {
                    name: "John Smith"
                    number: "+1 (555) 123-4567"
                    callType: "outgoing"
                    time: "2 min ago"
                }
                ListElement {
                    name: "Sarah Johnson"
                    number: "+1 (555) 987-6543"
                    callType: "incoming"
                    time: "15 min ago"
                }
                ListElement {
                    name: "Mike Davis"
                    number: "+1 (555) 456-7890"
                    callType: "missed"
                    time: "1 hour ago"
                }
                ListElement {
                    name: "Emergency Services"
                    number: "911"
                    callType: "outgoing"
                    time: "Yesterday"
                }
                ListElement {
                    name: "Mom"
                    number: "+1 (555) 111-2222"
                    callType: "incoming"
                    time: "Yesterday"
                }
            }

            delegate: Rectangle {
                width: contactsListView.width
                height: 60
                color: "transparent"
                border.color: "#333"
                border.width: 1
                radius: 4

                Row {
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 10

                    // Call type icon
                    Rectangle {
                        width: 30
                        height: 30
                        radius: 15
                        color: {
                            switch(callType) {
                                case "incoming": return "#00aa00"
                                case "outgoing": return "#0088ff"
                                case "missed": return "#ff4444"
                                default: return "#666"
                            }
                        }

                        Text {
                            anchors.centerIn: parent
                            text: {
                                switch(callType) {
                                    case "incoming": return "📞"
                                    case "outgoing": return "📤"
                                    case "missed": return "📵"
                                    default: return "📞"
                                }
                            }
                            color: "#fff"
                            font.pixelSize: 12
                        }
                    }

                    Column {
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 2

                        Text {
                            text: name
                            color: "#fff"
                            font.pixelSize: 16
                            font.bold: true
                        }

                        Text {
                            text: number
                            color: "#aaa"
                            font.pixelSize: 12
                        }
                    }
                }

                Text {
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    text: time
                    color: "#666"
                    font.pixelSize: 11
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (!inCall) {
                            makeCall(name, number)
                        }
                    }
                }
            }
        }
    }

    // Control buttons
    Row {
        id: controlButtons
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20

        // Call/End call button
        Rectangle {
            width: 60
            height: 60
            color: inCall ? "#ff4444" : "#00aa44"
            radius: 30
            
            Text {
                anchors.centerIn: parent
                text: inCall ? "📴" : "📞"
                color: "#fff"
                font.pixelSize: 24
            }
            
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (inCall) {
                        endCall()
                    } else {
                        // Quick dial or voice command
                        console.log("Voice dial activated")
                    }
                }
            }
            
            Behavior on color {
                ColorAnimation { duration: 200 }
            }
        }

        // Mute button
        Rectangle {
            width: 50
            height: 50
            color: "#666"
            radius: 25
            opacity: inCall ? 1.0 : 0.5
            enabled: inCall
            
            Text {
                anchors.centerIn: parent
                text: "🔇"
                color: "#fff"
                font.pixelSize: 20
            }
            
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (inCall) {
                        console.log("Mute toggled")
                    }
                }
            }
        }

        // Speaker button
        Rectangle {
            width: 50
            height: 50
            color: "#666"
            radius: 25
            opacity: inCall ? 1.0 : 0.5
            enabled: inCall
            
            Text {
                anchors.centerIn: parent
                text: "🔊"
                color: "#fff"
                font.pixelSize: 20
            }
            
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (inCall) {
                        console.log("Speaker toggled")
                    }
                }
            }
        }
    }

    Timer {
        id: callTimer
        interval: 1000
        running: inCall
        repeat: true
        onTriggered: {
            callSeconds++
            var minutes = Math.floor(callSeconds / 60)
            var seconds = callSeconds % 60
            callDuration = (minutes < 10 ? "0" : "") + minutes + ":" + (seconds < 10 ? "0" : "") + seconds
        }
    }

    function makeCall(contactName, phoneNumber) {
        currentContact = contactName
        inCall = true
        callSeconds = 0
        callDuration = "00:00"
        console.log("Calling:", contactName, phoneNumber)
    }

    function endCall() {
        inCall = false
        currentContact = ""
        callSeconds = 0
        callDuration = "00:00"
        console.log("Call ended")
    }

    // Emergency call button
    Rectangle {
        anchors.bottom: controlButtons.top
        anchors.bottomMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        width: 100
        height: 30
        color: "#cc0000"
        radius: 15
        visible: !inCall

        Text {
            anchors.centerIn: parent
            text: "Emergency"
            color: "#fff"
            font.pixelSize: 12
            font.bold: true
        }

        MouseArea {
            anchors.fill: parent
            onClicked: makeCall("Emergency Services", "911")
        }
    }
}