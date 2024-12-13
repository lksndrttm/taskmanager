import QtQuick 2.12
import StyleSettings 1.0

Rectangle {
    id:_header
    signal closeClicked()
    property string headerText: ""
    z:100
    height: Style.taskListHeaderHeight
    width: parent.width
    anchors.top: parent.top
    Rectangle {
        border.color: Style.taskListHeaderBorderColor
        border.width: Style.taskListHeaderBorderWidth
        height: Style.taskListHeaderHeight
        width: parent.width
        color: Style.taskListHeaderColor
        Text {
            id: _headerText
            text: _header.headerText
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            color: Style.taskListHeaderTextColor
            font.bold: true
        }
        Rectangle{
            id: _closebtn
            height: Style.taskListHeaderHeight - 10
            width: height
            color: parent.color
            Image {
                id: _closeimg
                source: "qrc:/images/whiteX.png"
                height: parent.height
                width: parent.width
            }
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: Style.taskListHeaderCloseMargin

            MouseArea{
                id:_marea
                anchors.fill: parent
                onClicked: {
                    _header.closeClicked()
                }
            }
        }

    }
}
