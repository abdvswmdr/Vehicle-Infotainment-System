import QtQuick 2.15
import QtQuick.Window 2.15
import QtLocation 5.15
import QtPositioning 5.15
import "ui/BottomBar"
import "ui/RightScreen"
import "ui/LeftScreen"
import "ui/Dashboard"
import "ui/MusicPlayer"
import "ui/Phone"
import "ui/ParkAssist"

Window {
    width: 1280
    height: 720
    visible: true
    title: qsTr("Vehicle Infotainment")
    
    property bool modalVisible: false
    property string currentModal: ""
    
    LeftScreen {
        id: leftScreen
        visible: !modalVisible
    }
    
    RightScreen {
        id: rightScreen
        visible: !modalVisible
    }
    
    BottomBar {
        id: bottomBar
    }
    
    // Modal overlay
    Rectangle {
        id: modalOverlay
        anchors.fill: parent
        color: "black"
        opacity: modalVisible ? 0.7 : 0
        visible: modalVisible
        
        Behavior on opacity {
            NumberAnimation { duration: 300 }
        }
        
        MouseArea {
            anchors.fill: parent
            onClicked: hideModal()
        }
    }
    
    // Modal content container
    Rectangle {
        id: modalContainer
        anchors.centerIn: parent
        width: modalContent.width
        height: modalContent.height
        color: "transparent"
        visible: modalVisible
        scale: modalVisible ? 1.0 : 0.8
        opacity: modalVisible ? 1.0 : 0
        
        Behavior on scale {
            NumberAnimation { duration: 300; easing.type: Easing.OutBack }
        }
        
        Behavior on opacity {
            NumberAnimation { duration: 300 }
        }
        
        Item {
            id: modalContent
            width: {
                switch(currentModal) {
                    case "dashboard": return 820
                    case "music": return 420
                    case "phone": return 370
                    case "parkassist": return 420
                    default: return 400
                }
            }
            height: {
                switch(currentModal) {
                    case "dashboard": return 520
                    case "music": return 320
                    case "phone": return 470
                    case "parkassist": return 370
                    default: return 300
                }
            }
            
            VehicleDashboard {
                anchors.fill: parent
                visible: currentModal === "dashboard"
            }
            
            MusicPlayerComponent {
                anchors.fill: parent
                visible: currentModal === "music"
            }
            
            PhoneInterface {
                anchors.fill: parent
                visible: currentModal === "phone"
            }
            
            ParkAssistComponent {
                anchors.fill: parent
                visible: currentModal === "parkassist"
            }
        }
        
        // Close button
        Rectangle {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: -10
            anchors.rightMargin: -10
            width: 30
            height: 30
            radius: 15
            color: "#ff4444"
            
            Text {
                anchors.centerIn: parent
                text: "×"
                color: "white"
                font.pixelSize: 18
                font.bold: true
            }
            
            MouseArea {
                anchors.fill: parent
                onClicked: hideModal()
            }
        }
    }
    
    function showDashboard() {
        currentModal = "dashboard"
        modalVisible = true
    }
    
    function showMusicPlayer() {
        currentModal = "music"
        modalVisible = true
    }
    
    function showPhoneInterface() {
        currentModal = "phone"
        modalVisible = true
    }
    
    function showParkAssist() {
        currentModal = "parkassist"
        modalVisible = true
    }
    
    function hideModal() {
        modalVisible = false
        currentModal = ""
    }
    
    // Focus for key handling
    Item {
        anchors.fill: parent
        focus: true
        Keys.onEscapePressed: hideModal()
    }
}
