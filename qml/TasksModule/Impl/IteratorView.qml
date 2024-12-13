import QtQuick 2.12
import TasksModule.Base 1.0
import StyleSettings 1.0
import IterModel 1.0


BaseListView {
    id: root
    signal selectModeDiactivate()
    signal selectAllClicked()
    signal showDetails(int id)
    property bool isSelectMode : false
    //signal selectModeActivated(int id)
    header : Loader{
        id: _hloader
        z:100
        height: Style.taskListHeaderHeight
        width: parent.width
        source: root.isSelectMode ? "SelectionWidget.qml" : "ListsHeader.qml"

        onSourceChanged: {
            if(!root.isSelectMode){
                _hloader.item.showedText = "Iterators"
            }
        }

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
                deleteSelectedItems()
            }
        }
    }


    delegate: Loader{
        id:_iterLoader
        width: parent.width
        height: Style.iterableTaskHeight
        source: type === "counter" ? "IteratorDelegate.qml" : "IterableSeriesDelegate.qml"

        MouseArea{
            id: _mousearea
            anchors.fill: parent

            onClicked: {
                if(root.isSelectMode){
                    _iterLoader.item.selectModeActivated(index)
                }else{
                    root.showDetails(index)
                }
            }


        }
        Connections{
            target: item
            function onChangeDaylyStatus(name) {
                root.model.changeDaylyStatus(name)
            }

            function onIncrease(name) {
                root.model.complite(name)
            }
            function onDecrease(name) {
                root.model.uncomplite(name)
            }

            function onSelectModeActivated(id) {
                root.isSelectMode = true;
                root.currentIndex = id
                if(root.currentItem.item.isSelectedInSelectMode){
                    root.currentItem.item.isSelectedInSelectMode = false
                    removeElement(root.selectedIndexList,id)
                    if(root.selectedIndexList.length === 0){
                        root.isSelectMode = false
                    }
                }
                else{
                    root.currentItem.item.isSelectedInSelectMode = true
                    root.selectedIndexList.push(id)

                }
            }
        }
    }


    model: IterModel{}


    onSelectModeDiactivate: {
        exitFromSelectMode();
    }

    onSelectAllClicked: {
        selectAll();
    }

    function deleteTask(row){
        return root.model.delIterator(row)
    }

    function deleteSelectedItems(){
        root.selectedIndexList.sort()
        for(var i=root.selectedIndexList.length-1; i >= 0; i--){
            root.currentIndex = root.selectedIndexList[i]
            if(root.deleteTask(root.currentIndex)){
                root.selectedIndexList.length--;
            }
            //root.currentItem.isSelectedInSelectMode = false
        }
        exitFromSelectMode()
    }

    function addTask(name){
        root.model.addIterator(name)
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
            root.currentItem.item.isSelectedInSelectMode = false
        }
        root.isSelectMode = false
        root.currentIndex = -1
        root.selectedIndexList.length = []
    }

    function selectAll(){
        root.selectedIndexList = []
        for(var i=0; i < root.count; i++){
            root.currentIndex = i
            root.currentItem.item.isSelectedInSelectMode = true
            root.selectedIndexList.push(i)

        }
        root.currentIndex = -1
    }
}
