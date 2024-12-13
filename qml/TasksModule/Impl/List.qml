import QtQuick 2.0
import TasksModule.Base 1.0
import StyleSettings 1.0
import ListsModel 1.0

BaseListView {
    id: root    
    signal listItemClicked(string listname)
    signal selectModeDiactivate()
    signal selectAllClicked()
    //signal selectModeActivated(int id)
    header : Loader{
        id: _hloader
        z:100
        height: Style.taskListHeaderHeight
        width: parent.width
        source: root.isSelectMode ? "SelectionWidget.qml" : "ListsHeader.qml"

        Connections {
            target: _hloader.item
            function onCloseClicked(){
                root.exitFromSelectMode();
            }
            function onSelectAllToggled(state){
                if(state){
                    root.selectAll()
                }
                else{
                    root.exitFromSelectMode()
                }
            }
            function onDelButtonClicked() {
                deleteSelectedLists()
            }
        }
    }


    delegate: ListDelegate {
        id:_delegate
        width: parent.width
        height: Style.listItemHeight

        MouseArea{
            id: _mousearea
            anchors.fill: parent

            onClicked: {
                if(!root.isSelectMode){
                    root.listItemClicked(name)
                }
                else{
                    _delegate.selectModeActivated(index)
                }
            }
            onPressAndHold: {
                _delegate.selectModeActivated(index)
            }

        }

        onSelectModeActivated: {
            root.isSelectMode = true;
            currentIndex = index
            if(currentItem.isSelectedInSelectMode){
                currentItem.isSelectedInSelectMode = false
                removeElement(root.selectedIndexList,index)
                if(root.selectedIndexList.length == 0){
                    root.isSelectMode = false
                }
            }
            else{
                currentItem.isSelectedInSelectMode = true
                root.selectedIndexList.push(id)

            }
        }

    }

    model: ListsModel{}


    onSelectModeDiactivate: {
        exitFromSelectMode();
    }

    onSelectAllClicked: {
        selectAll();
    }

    function deleteList(name, row){
        return root.model.delList(name,row)
    }

    function deleteSelectedLists(){
        root.selectedIndexList.sort()
        for(var i=root.selectedIndexList.length-1; i >= 0; i--){
            root.currentIndex = root.selectedIndexList[i]
            if(root.deleteList(root.currentItem.delegateText,root.currentIndex)){
                root.selectedIndexList.length--;
            }
            //root.currentItem.isSelectedInSelectMode = false
        }
        exitFromSelectMode()
    }

    function addList(name,type){
        root.model.addList(name,type)
    }

    function removeElement(array, elem) {
        var index = array.indexOf(elem);
        if (index > -1) {
            array.splice(index, 1);
        }
    }

    function exitFromSelectMode(){
        for(var i=0; i < root.selectedIndexList.length; i++){
            root.currentIndex = root.selectedIndexList[i]
            root.currentItem.isSelectedInSelectMode = false
        }
        root.isSelectMode = false
        root.currentIndex = -1
        root.selectedIndexList.length = []
    }

    function selectAll(){
        root.selectedIndexList = []
        for(var i=0; i < root.count; i++){
            root.currentIndex = i
            root.currentItem.isSelectedInSelectMode = true
            root.selectedIndexList.push(i)

        }
        root.currentIndex = -1
    }
}
