import QtQuick 2.12
import QtQuick.Controls 2.12
import InputModule.Base 1.0
import StyleSettings 1.0

Rectangle {
    id:root
    property string prior: ""
    property string taskName: ""
    property alias inputText: _name.currentText
    property alias currentTaskName: _name.currentDisplayText
    property string currentPrior: _prior.color === _prior.color0 ? "3" : (_prior.color === _prior.color1 ? "2" : "1")
    signal textEdited()
    BaseTextInput{
        id: _name
        currentText: root.taskName
        border.color: "#9E9E9E"
        labelText: "subtaskname"
        height: parent.height
        anchors.right: parent.right
        anchors.left: _prior.right

        radius: 2

        onInputTextEdited:{
            root.textEdited()
        }
    }

    ColorSelectItem {
        id: _prior
        anchors.left: parent.left
        height: _name.height
        width: height
        Component.onCompleted: {
            _prior.color = root.prior === "High" ? Style.taskHighPriorColor : (root.prior === "Mid" ? Style.taskMidPriorColor : Style.taskLowPriorColor)
        }
    }

}
