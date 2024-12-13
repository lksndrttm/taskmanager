import QtQuick 2.12

Image {
    id: root

    property var action

    source: ""
    sourceSize.height: 100
    sourceSize.width: 100
    height: 15
    width: height

    MouseArea{
        id:closeSelectArea
        anchors.fill: parent
        onClicked: {
            root.action()
        }
    }
}
