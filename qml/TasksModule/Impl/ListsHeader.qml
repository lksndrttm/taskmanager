import QtQuick 2.12
import StyleSettings 1.0

Rectangle {
    signal closeClicked()
    signal delButtonClicked()
    signal selectAllToggled(bool state)
    property string showedText: "Task lists"
    height: Style.taskListHeaderHeight
    color: "black"
    border.color: Style.taskListHeaderBorderColor
    border.width: Style.taskListHeaderBorderWidth
    Text {
        id: _listsText
        text: showedText
        font.pointSize: 12
        color: "white"
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
    }
}
