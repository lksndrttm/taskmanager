import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtQml.Models 2.12
import StyleSettings 1.0
import QtQml 2.15
import NotesModel 1.0
import Notes 1.0


Rectangle {
    id:root
    property alias selectionController: selectionController
    clip: true

    CurrentCategory {
        id: currentTagPlace
        selectionController: root.selectionController
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    NavPopup {
        id :navPopup
        currentTag: currentTagPlace.currentTag
        x:0
        y: header.y + 10
        width: root.width
        height: (root.height / 100) * 70
    }


    SelectionControlls {
        id:selectionFooter
        selectionController: root.selectionController
        width: parent.width
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        z: notesFlick.z+1
        visible: root.selectionController.selectMode
    }

    SearchBox {
        id: header
        width: parent.width
        anchors.top: currentTagPlace.bottom
        anchors.topMargin: 5
        visible: !root.selectionController.selectMode
    }

    NotesView {
        id: notesFlick
        anchors.left: parent.left
        anchors.right: parent.right
        selectionController: root.selectionController

        anchors.top: header.bottom
        anchors.bottom: root.selectionController.selectMode ? selectionFooter.top : parent.bottom
    }

    AddNoteButton {
        id: addButton
        editPopup: editPopup

        height: 40
        width: height
        x: parent.width - width - 5
        y: parent.height - height - 5
        z: 99
        visible: !root.selectionController.selectMode
    }

    NoteEditor {
        id: editPopup
        x:0
        y:-Style.tabHeight
        width: parent.width
        height: parent.height+Style.tabHeight
    }


    AddCatDialog {
        id:catAddDialog
        width: parent.width - 2*x
        height: 100
        y: parent.height - height - 20
        x: 20
    }


    SelectCategoryDialog {
        id:catUpdateAddDialog
        height: 100
        width: parent.width - 2*x
        y: parent.height - height - 20
        x: 20
    }


    CatColorsModel {
        id: catColors
    }

    NotesModel {
        id:notesModel
    }

    //TODO : Избавиться от миганий (желательно адекватно)
    //Android specific
    //Непонятно как работающий костыль от миганий при нажатия на TextEdit (editNote)
    //Полнстью проблема не решена (мигает при появлении/скрытии вирт. клавиатуры
    //https://falsinsoft.blogspot.com/2017/04/qml-resize-comtrols-when-android.html
    Connections{
        target: Qt.inputMethod
        function onVisibleChanged(){
            editPopup.height += 0
        }
        function onKeyboardRectangleChanged(){
            editPopup.height += 0

        }
    }

    function replaseNotes(indexList){

    }

    function getCatByName(name){
        for(var i = 0; i < notesModel.categories.length; i++){
           if(notesModel.categories[i].name === name ) {
               return i;
           }
        }
        return -1;
    }

    Item {
        id: selectionController
        property bool selectMode: false
        property var selectedElements: []
        property int countOfSelected: 0

        property var exitFromSelectMode: selectionController._exitFromSelectMode
        property var selectItem: selectionController._selectItem
        property var selectAllNotes: selectionController._selectAllNotes
        property var deleteNoete: selectionController._deleteNoete
        property var sortSelected: selectionController._sortSelected
        property var deleteSelected: selectionController._deleteSelected

        function _exitFromSelectMode(){
            for(var i=0; i < selectionController.selectedElements.length; i++){
                var gitem = notesFlick.repeater.itemAt(selectionController.selectedElements[i]);
                gitem.selected = false;
            }
            selectionController.selectedElements = [];
            root.selectionController.selectMode = false;
        }

        function _selectItem(index){
            var note = notesFlick.repeater.itemAt(index);
            if(selectionController.selectedElements.find(element => element === index) === undefined){
                selectionController.selectedElements.push(index);
                note.selected = true;
            }
            selectionController.countOfSelected = selectionController.selectedElements.length;
        }

        function _selectAllNotes(){
            for(var i = 0; i < notesFlick.model.length; i++){
                selectionController._selectItem(i);
            }
        }

        function _deleteNoete(index){
            var res = notesModel.deleteNote(index);
            if(res){
                var idInSelectedList = selectionController.selectedElements.indexOf(index);
                selectionController.selectedElements.splice(idInSelectedList, 1)
            }
        }

        function _sortSelected(){
            selectionController.selectedElements.sort(function(a,b){return a-b});
        }

        function _deleteSelected(){
            selectionController._sortSelected();
            for(var i = selectionController.selectedElements.length-1; i >= 0; i--){
                selectionController._deleteNoete(selectionController.selectedElements[i]);
            }
            selectionController.countOfSelected = 0;
        }

    }
}


