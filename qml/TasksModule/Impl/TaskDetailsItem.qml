import QtQuick 2.12
import QtQuick.Controls 2.12
import InputModule.Base 1.0
import InputModule.Impl 1.0
import StyleSettings 1.0

Rectangle {
    id:root
    signal saveButtonClicked(var taskObject)
    signal showTask(var task)
    signal showSubTasks(var subTasksList)
    signal addSubTarget(string name, string prior, var parentIndex)
    signal deleteSubTarget(var index)
    signal updateSubTask(string name, string prior, var index)

    property string taskname: ""
    property string tasktype: ""
    property string description: ""
    property string startTime: ""
    property string endTime: ""
    property string createTime: ""
    property string prior: ""
    property int deep: 1
    property var index



    border.color: "black"
    border.width: 1

    Text {
        id: _descrText
        text: qsTr("Description:")
        anchors.top: parent.top
        anchors.topMargin: Style.taskListHeaderHeight + 2
        anchors.left: parent.left
        anchors.leftMargin: 10
        font.pointSize: 10
        color: "#808080"
    }

    TextEdit{
        id: _descr
        text: root.description
        anchors.top: _descrText.bottom
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        height: 30
        topPadding: 2
        leftPadding: 3
        font.pointSize: 10
        wrapMode: TextEdit.Wrap
        Rectangle{
            id:_border
            border.color: "#9E9E9E"
            border.width: 1
            radius: 2
            anchors.fill: parent
            z:-99
        }
    }

    Rectangle{
        id:_priorPlace
        anchors.top: _descr.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        height: 36

        Text {
            id: _prior
            text: qsTr("Priority:")
            anchors.bottom: _priorItem.top
            font.pointSize: 10
            color: "#808080"
        }
        PriorSelectionItem{
            id:_priorItem
            height: 26
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            highRB.checked: root.prior === "3"
            midRB.checked: root.prior === "2"
            lowRB.checked: root.prior === "1"
        }
    }

    Rectangle{
        id:_subsBorder
        visible: !root.deep
        color: "transparent"
        radius: 2
        width: parent.width
        anchors.top: (root.tasktype === "common" || root.tasktype === "schedule") ? _startDatetime.bottom : _priorPlace.bottom
        anchors.bottom: _typeBox.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 15
        anchors.topMargin: 10
        Text {
            id: _subTasks
            text: qsTr("Subtasks:")
            font.pointSize: 10
            color: "#808080"
            anchors.topMargin: 5
        }
        SubTargetsView{
            id:_priorSelect
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: _subTasks.bottom
            anchors.bottom: parent.bottom
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            anchors.bottomMargin: 5

            onSubTargetAdd: {
                root.addSubTarget(name,prior,root.index)
            }
            onSubTargetDelete: {
                root.deleteSubTarget(index)
            }
            onGetSaveData: {
                root.updateSubTask(name,prior,index)
            }
        }
    }

    Text {
        id: _typeBox
        text: root.tasktype
        anchors.right: parent.right
//        anchors.rightMargin: 10
//        anchors.top: _priorItem.bottom
//        anchors.topMargin: 10
        anchors.bottom: _saveButton.top
        anchors.bottomMargin: 5
        anchors.rightMargin: 10
        font.pointSize: 10
        //height: 10
        color: "#9E9E9E"
    }



    Text{
        id:_startDatetime
        text: "begin: " + root.startTime
        //height: 35
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: 10
        anchors.top: _priorPlace.bottom
        anchors.topMargin: 10
        color: "#9E9E9E"
        font.bold: true
        visible : (root.tasktype === "common" || root.tasktype === "schedule")
        font.pointSize: 9
    }
    Text{
        id:_endDatetime
        text: "end: " + root.endTime
        //height: 35
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: _priorPlace.bottom
        anchors.topMargin: 10
        color: "#9E9E9E"
        font.bold: true
        visible : (root.tasktype === "common" || root.tasktype === "schedule")
        font.pointSize: 9
    }

     Rectangle{
        id: _saveButton
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
            text: qsTr("Save")
            anchors.centerIn: parent
            color: "white"
            font.bold: true
        }

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                _saveButton.color = "#3a3a3a"
            }
            onExited: {
                _saveButton.color = "#9E9E9E"
            }
            onPressed: {
                _saveButton.color = "black"
            }
            onReleased: {
                _saveButton.color = "#3a3a3a"
            }
            onClicked: {
                var task = {}
                task["name"] = root.taskname
                task["description"] = _descr.text
                task["prior"] = _priorItem.bgroup.checkedButton.text === "High" ? "3" : _priorItem.bgroup.checkedButton.text === "Mid" ? "2" : "1"

                if(root.tasktype === "common" || root.tasktype === "schedule"){
                    task["startTime"] = _startDatetime.currentText
                    task["endTime"] = _endDatetime.currentText
                }
                else {
                    task["startTime"] = ""
                    task["endTime"] = ""
                }
                _priorSelect.saveClicked()
                root.saveButtonClicked(task)
            }
        }
    }

     onShowSubTasks: {
         _priorSelect.modelInited(subTasksList)
     }

}
