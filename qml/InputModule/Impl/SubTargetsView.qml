import QtQuick 2.12
import QtQuick.Controls 2.12
import InputModule.Base 1.0
import StyleSettings 1.0
import TasksModule.Impl 1.0

ListView{
    id:root
    signal modelInited(var subTargetList)
    signal subTargetDelete(var index)
    signal subTargetAdd(string name, string prior)
    signal saveClicked()
    signal getSaveData(string name, string prior, var index)
    spacing: 3
    clip: true
    header: Rectangle {//space rect
        SubTargetDelegate{
            isInitialized: false
            height: Style.detailsSubTaskHeight
            width: parent.width
            anchors.top: parent.top
            onSubTargetAdd: {
                root.subTargetAdd(subTaskName,subTaskPrior)
            }
        }
        width: parent.width
        height: Style.detailsSubTaskHeight + root.spacing
    }
    contentItem.anchors.topMargin: 3

    delegate:  SubTargetDelegate{
        id:_priorSelect
        //property string name: value
        prior: modelData.model.data(modelData,264) === "3" ? "High" : (modelData.model.data(modelData,264)==="2" ? "Mid" : "Low")
        taskName: modelData.model.data(modelData,256)
        itemIndex: modelData
        width: parent.width
        height : Style.detailsSubTaskHeight

        onSubTargetDelete: {
            root.subTargetDelete(index)
        }

        onGetSaveData: {

            root.getSaveData(name,prior,index)
        }

        Component.onCompleted: {
            if(_priorSelect.taskName !== ""){
                _priorSelect.isInitialized = true
            }
        }
    }

    onModelInited: {
        root.model = subTargetList
//        for(var i=0; i<subTargetList.length; i++){
//            console.log(subTargetList[i].model.data(subTargetList[i],256)," NAMAE")
//        }
    }

    onSaveClicked: {
        for(var i=0; i<root.model.length; i++){
            currentIndex = i
            root.getSaveData(currentItem.taskName, currentItem.currentPrior, root.model[i])
        }
        currentIndex = -1
    }
}
