 import QtQuick 2.12
import TasksModule.Base 1.0
import QtQuick.Controls 2.12
import StyleSettings 1.0
import QtQuick.Controls 1.4
import TaskListModel 1.0
import QtQml.Models 2.12

BaseTaskListView {
    id: root
    signal taskAdded()
    signal taskDetailShowed(string taskName)

    TableViewColumn {
        //title: "Name"
        id:_treeCol
        width: parent.width

        role: "name"

        //delegate: TaskDelegate{}
    }

    itemDelegate: Loader {
        id:_delegateLoader
        width: parent.width
        height: Style.taskListItemHeight
        source: root.model.getType(styleData.index) === "schedule" ? "ScheduleTaskDelegateItem.qml" : "TaskDelegateItem.qml"
//        Component.onCompleted: {
//            item.startTime = root.model.getStartTime(styleData.index)
//            item.endTime = root.model.getEndTime(styleData.index)
//        }
        onItemChanged: {
            item.startTime = root.model.getStartTime(styleData.index)
            item.endTime = root.model.getEndTime(styleData.index)
        }

    }

    Component{
        id:_taskAddItem
        Rectangle {
            id:_rec
            property string listtype1: "targetList"
            anchors.fill: parent
            AddTaskItem{
                listtype: _rec.listtype1
                anchors.fill: parent
                onAddButtonClicked: {
                    root.closeAddTaskDialog();
                    root.taskAdded()
                    root.model.addTask(taskObject,root.rootIndex)
                }
            }
        }
    }

    Component{
        id:_taskDetailsShowItem
        Rectangle {
            property alias tdItem: _taskDetails
            anchors.fill: parent
            TaskDetailsItem{
                id:_taskDetails
                anchors.fill: parent
                onAddSubTarget: {
                    var locale = Qt.locale()
                    var task = {}
                    task["name"] = name
                    task["description"] = ""
                    task["status"] = "in_process"
                    task["createTime"] = new Date().toLocaleString(locale,"hh:mm dd.MM.yyyy")
                    task["targetType"] = "simple"
                    task["prior"] = prior
                    root.model.addTask(task,parentIndex)
                    _listloader.item.tdItem.showSubTasks(root.model.getSubTasks(parentIndex))
                }
                onDeleteSubTarget: {
                    root.model.delTask(index)
                    showSubTasks(root.model.getSubTasks(_taskDetails.index))
                }
                onSaveButtonClicked: {
                    root.model.updateTask(taskObject,_taskDetails.index)
                }
                onUpdateSubTask: {
                    root.model.changeSubTask(name,prior,index)
                }
            }
        }
    }



    Loader {
        id:_listloader
        anchors.fill: parent
        onItemChanged: {
            if(_listloader.item && _listloader.item.listtype1 === "targetList"){
                console.log("condition is true. type is - ")
                console.log(root.model.getListType())
                _listloader.item.listtype1 = root.model.getListType()
            }
        }
    }

    model: TaskListModel{}


    selection: ItemSelectionModel {
        model: root.model
     }
    selectionMode: isSelectionMode ? SelectionMode.MultiSelection : SelectionMode.NoSelection

    onClicked: {
//        root.selection.select(index,ItemSelectionModel.Toggle)
//        if(!root.isSelectionMode){
//            _listloader.sourceComponent = _taskDetailsShowItem
//            root.taskDetailShowed(root.model.getName(index))
//            _listloader.item.tdItem.taskname = root.model.getName(index)
//            _listloader.item.tdItem.description = root.model.getDescription(index)
//            _listloader.item.tdItem.tasktype =  root.model.getType(index)
//            _listloader.item.tdItem.startTime = root.model.getStartTime(index)
//            _listloader.item.tdItem.endTime = root.model.getEndTime(index)
//            _listloader.item.tdItem.createTime = root.model.getEndTime(index)
//            _listloader.item.tdItem.prior = root.model.getPrior(index)
//        }
    }

    onPressAndHold: {
        presAndHoldEvent(index)
    }


    onSelectAll: {
        for(var i = 0; i < selection.model.rowCount(); i++){
            root.selection.select(root.selection.model.index(i,0,root.rootIndex), ItemSelectionModel.Select)
        }
    }

    onExitFromSelectMode: {
        selectModeDiactivate();
    }

    onDeleteSelectedTasks: {
        var sList = []
        sList = selection.selectedIndexes.slice()
        sList.sort(function(a,b){
            var deep1 = root.model.data(a,258).split("_").length//get hierarchyAddr
            var deep2 = root.model.data(b,258).split("_").length

            if(deep1 > deep2){
                return -1;
            } else if(deep1 < deep2){
                return 1;
            } else {
                return b.row - a.row;
            }
        });
        for(var i = 0; i < sList.length ; i++){
            root.model.delTask(sList[i])
        }
        exitFromSelectMode()
    }

    function selectModeDiactivate() {
        selection.clear();
        isSelectionMode = false;
    }

    function openAddTaskDialog(){
        _treeCol.visible = false
        _listloader.sourceComponent = _taskAddItem
    }
    function closeAddTaskDialog(){
        _listloader.sourceComponent = undefined
        _treeCol.visible = true
    }
    function presAndHoldEvent(index){
        root.isSelectionMode = true;
        root.selection.select(index,ItemSelectionModel.Select)
    }
}
