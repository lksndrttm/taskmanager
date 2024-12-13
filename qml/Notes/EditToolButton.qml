import QtQuick 2.12

Rectangle {
    id:root
    property var action
    property string image: ""

    Image {
        id: toolImg
        source: root.image
        sourceSize.height: 300
        sourceSize.width: 300
        width: parent.width
        height: parent.width
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            root.action()
        }
    }
}
