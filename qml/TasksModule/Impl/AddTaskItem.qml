import QtQuick 2.12
import QtQuick.Controls 2.12
import InputModule.Base 1.0
import StyleSettings 1.0

Rectangle {
    id:root

    signal addButtonClicked(var taskObject)
    property string taskname: _input.currentText
    property string tasktype: _typeBox.displayText
    property string listtype: ""

    border.color: "black"
    border.width: 1


    BaseTextInput{
        id: _input
        labelText: "taskname"
        height: Style.addDialogButtonsHeight
        anchors.right: _typeBox.left
        anchors.left: parent.left
        anchors.rightMargin: 5
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: Style.taskListHeaderHeight + 10
        radius: 2

    }

    BaseComboBox {
        id: _typeBox
        model: listtype === "schedule" ? ["schedule"] : ["simple","common"]
        height: Style.addDialogButtonsHeight
        width: Style.addDialogComboBoxWidth
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: Style.taskListHeaderHeight + 10
        font.pointSize: 10
    }

    PriorSelectionItem{
        id:_priorItem
        anchors.top: _input.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        height: 30
    }

    BaseTextEdit{
        id: _descr
        labelText: "description"
        anchors.top: _priorItem.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        height: 70
    }

    BaseDateTimeEdit{
        id:_startDatetime
        labelText: root.tasktype === "common" ? "begin datetime:" : "begin time:"
        inputMaskString: root.tasktype === "common" ? "99:99 99.99.9999" : "99:99"
        height: 35
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: 10
        anchors.top: _descr.bottom
        anchors.topMargin: 10
        visible : (root.tasktype === "common" || root.tasktype === "schedule")
    }
    BaseDateTimeEdit{
        id:_endDatetime
        labelText: root.tasktype === "common" ? "end datetime:" : "end time:"
        inputMaskString: root.tasktype === "common" ? "99:99 99.99.9999" : "99:99"
        height: 35
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: _descr.bottom
        anchors.topMargin: 10
        visible : (root.tasktype === "common" || root.tasktype === "schedule")
    }

     Rectangle{
        id: _addButton
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 5
        height: 25
        radius: 2
        color: "#9E9E9E"

        Text {
            id: _btext
            text: qsTr("Add")
            anchors.centerIn: parent
            color: "white"
            font.bold: true
        }

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                _addButton.color = "#3a3a3a"
            }
            onExited: {
                _addButton.color = "#9E9E9E"
            }
            onPressed: {
                _addButton.color = "black"
            }
            onReleased: {
                _addButton.color = "#3a3a3a"
            }
            onClicked: {
                var locale = Qt.locale()
                var task = {}
                task["name"] = root.taskname
                task["description"] = _descr.currentText
                task["status"] = "in_process"
                task["createTime"] = new Date().toLocaleString(locale,"hh:mm dd.MM.yyyy")
                task["targetType"] = root.tasktype
                task["prior"] = _priorItem.bgroup.checkedButton.text === "High" ? "3" : _priorItem.bgroup.checkedButton.text === "Mid" ? "2" : "1"

                if(root.tasktype === "common" || root.tasktype === "schedule"){
                    task["startTime"] = _startDatetime.currentText
                    task["endTime"] = _endDatetime.currentText
                    task["beginDateTime"] = new Date().toLocaleString(locale,"dd.MM.yyyy")
                }

                root.addButtonClicked(task)
            }
        }
    }
}
