import QtQuick 2.12
import QtQuick.Controls 2.12
import StyleSettings 1.0

Rectangle{
    id:root
    property string labelText: ""
    property string inputMaskString: ""
    property string currentText: _input.text
    color: "transparent"
    Text {
        id: _label
        text: root.labelText
        font.pointSize: 12
        //font.bold: true
        anchors.top: parent.top
        anchors.left: parent.left
    }

    TextInput {
        id:_input
        focus: true
        inputMask: root.inputMaskString
        inputMethodHints: Qt.ImhTime
        anchors.top: _label.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        font.pointSize: 12
        topPadding: 2
        leftPadding: 2
        anchors.topMargin: 2


        Rectangle{
            id:_border
            anchors.fill: parent
            border.color: "black"
            border.width: 1
            radius: 2
            z:-99
            color: "transparent"
        }
    }
}
