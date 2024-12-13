import QtQuick 2.12
import TasksModule.Base 1.0
import QtQuick.Controls 2.12
import StyleSettings 1.0
import QtQuick.Controls 1.4
import QtQml.Models 2.12

TaskDelegate{
    property string startTime: ""
    property string endTime: ""
    anchors.leftMargin: 20;
    prior: root.model.getPrior(styleData.index)
    status: root.model.getStatus(styleData.index)
    onCheckBoxToggled:{
        root.model.setTaskState(state, styleData.index)
    }
    onPressAndHolodItem: {
        presAndHoldEvent(styleData.index)
    }
    onClickedEvent: {
        root.selection.select(styleData.index,ItemSelectionModel.Toggle)
        if(!root.isSelectionMode){
            _treeCol.visible = false
            _listloader.sourceComponent = _taskDetailsShowItem
            root.taskDetailShowed(root.model.getName(styleData.index))
            _listloader.item.tdItem.taskname = root.model.getName(styleData.index)
            _listloader.item.tdItem.description = root.model.getDescription(styleData.index)
            _listloader.item.tdItem.tasktype =  root.model.getType(styleData.index)
            _listloader.item.tdItem.startTime = root.model.getStartTime(styleData.index)
            _listloader.item.tdItem.endTime = root.model.getEndTime(styleData.index)
            _listloader.item.tdItem.createTime = root.model.getEndTime(styleData.index)
            _listloader.item.tdItem.prior = root.model.getPrior(styleData.index)
            _listloader.item.tdItem.index = styleData.index
            _listloader.item.tdItem.deep = styleData.depth
            _listloader.item.tdItem.showSubTasks(root.model.getSubTasks(styleData.index))
            }
        }

}
