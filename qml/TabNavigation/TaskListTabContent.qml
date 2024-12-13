import QtQuick 2.0
import QtQuick.Layouts 1.12
import TasksModule.Impl 1.0
import MainFrameModule 1.0
import StyleSettings 1.0


MainFrame {
    id: _tasksTab
    property alias model: _view.model
    property string headerText: ""
    property string showedTaskName: ""
    property bool isAddMode: false
    property bool isTaskDetailMode: false
    signal closeClicked()
    signal addButtonClicked()
    signal selectModeDiactivate()

    MainFrameContent {
        id: _content

        anchors.top: parent.top
        anchors.bottom: _cbuttons.top
        TaskList{
            id: _view
            height: parent.height
            width: parent.width

            headerText:_tasksTab.isAddMode ? "New task" : _tasksTab.isTaskDetailMode ? _tasksTab.showedTaskName : _tasksTab.headerText
            onCloseClicked: {
                if(_tasksTab.isAddMode || _tasksTab.isTaskDetailMode){
                    _view.closeAddTaskDialog()
                    _tasksTab.isAddMode = false
                    _tasksTab.isTaskDetailMode = false

                }else {
                    _tasksTab.closeClicked()
                }
            }
            onTaskAdded: {
                _tasksTab.isAddMode = false
            }
            onTaskDetailShowed: {
                _tasksTab.showedTaskName = taskName
                _tasksTab.isTaskDetailMode = true
            }

        }
    }

    AddButton {
        id: _cbuttons
        height: Style.mainFrameButtonsSize
        width: Style.mainFrameButtonsSize
        z:100
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: Style.mainFrameMargin/2
        visible: !(_tasksTab.isAddMode || _tasksTab.isTaskDetailMode)
        onClicked: {
            addButtonClicked()
        }
    }

    MouseArea {
        anchors.left: parent.left
        anchors.right: _cbuttons.right
        anchors.bottom: parent.bottom
        height: Style.mainFrameButtonsSize
        onClicked: {
            _view.selectModeDiactivate()
        }
    }

    onAddButtonClicked:
    {
        _view.selectModeDiactivate()
        _tasksTab.isAddMode = true
        var task = {}
//        task["name"] = "NTEST1"
//        task["description"] = "testDescr"
//        task["status"] = "complite"
//        task["createTime"] = "23:52 21.02.2020"
//        task["targetType"] = "simple"
//        task["prior"] = "3"
        _view.openAddTaskDialog()
//        _view.model.addTask(task,_view.rootIndex)
    }
    onSelectModeDiactivate: {
        _view.selectModeDiactivate()
    }


}
