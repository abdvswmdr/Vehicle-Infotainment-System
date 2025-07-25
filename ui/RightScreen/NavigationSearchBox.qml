import QtQuick 2.15

Rectangle {
    id: navSearchBox
    color: "#E6E6E6"
    radius: 5

    Image {
        id: searchIcon
        source: "qrc:/images/search.png"
        anchors {
            left: parent.left
            leftMargin: 20
            verticalCenter: parent.verticalCenter
        }
        height: parent.height * 0.45
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: navigationPlaceholderText
        text: "Navigate"
        color: "#373737"
        anchors {
            left: searchIcon.right
            leftMargin: 20
            verticalCenter: parent.verticalCenter
        }
        visible: navigationTextInput.text === ""
    }

    TextInput {
        id: navigationTextInput
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: searchIcon.right
            leftMargin: 20
            right: parent.right
        }
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 18
        clip: true
    }
}
