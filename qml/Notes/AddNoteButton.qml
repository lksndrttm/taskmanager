import QtQuick 2.12

Rectangle{
    id: root
    property var editPopup

    radius: 4
    color: "transparent"
    Rectangle{
        anchors.fill: parent
        anchors.leftMargin: 2
        anchors.rightMargin: 2
        anchors.bottomMargin: 2
        anchors.topMargin: 2
        color: "white"
        z:addImg.z-1
    }

    Image {
        id: addImg
        source: "qrc:/images/plus-square-fill.svg"
        sourceSize.height: 100
        sourceSize.width: 100
        anchors.fill: parent
    }
    MouseArea{
        id: addNoteArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            root.editPopup.open()
        }
    }
}
