import QtQuick 2.12
import QtQuick.Controls 2.12
import TasksModule.Base 1.0
import StyleSettings 1.0

BaseTaskListDelegate {
    id: root
    signal checkBoxToggled(bool state)
    property string status: "in_process"

    TaskCheckBox{
        id: _checkbox
        z:99
        height: Style.listItemCheckboxSize
        width: Style.listItemCheckboxSize
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: Style.listItemCheckboxRMargin + (styleData.depth * height)
        checked: status === "complite" ? true : false
        onCheckBoxToggled: {root.checkBoxToggled(state)}
    }


//    Text {
//        id: _timeSpaceText
//        visible: false//type === "common" || type === "schedule"
//        text: startTime + "-" + endTime
//        anchors.top: textItem.bottom
//        anchors.bottom: parent.bottom
//        color: textItem.color
//        anchors.horizontalCenter: parent.horizontalCenter

//    }
}
