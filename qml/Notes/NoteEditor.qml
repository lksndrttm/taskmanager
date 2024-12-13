import QtQuick 2.12
import QtQuick.Controls 2.15

Popup {
    id:editPopup
    property int currentIndex: -1
    property bool isEditMode: false
    modal: true
    dim: false
    closePolicy: Popup.NoAutoClose

    Rectangle{
        id: editHeader
        height: 40
        anchors.left: parent.left
        anchors.top: parent.top
        width: parent.width

        Image {
            id: closeEditMode
            source: "qrc:/images/arrow-left-short.svg"
            sourceSize.height: 100
            sourceSize.width: 100
            height: 30
            width: height
            anchors.left: parent.left
            anchors.top: parent.top
            MouseArea{
                id:closeEditArea
                anchors.fill: parent
                onClicked: {
                    editPopup.exitFromEdit()
                }
            }
        }

        Image {
            id: undoImg
            source: "qrc:/images/arrow-return-left.svg"
            sourceSize.height: 100
            sourceSize.width: 100
            height: 20
            width: height
            anchors.right: redoImg.left
            anchors.top: parent.top
            anchors.rightMargin: 10
            anchors.topMargin: 5
            visible: (editNote.textArea.canUndo || editNote.textArea.canRedo)
            MouseArea{
                id:undoArea
                anchors.fill: parent
                onClicked: {
                    editNote.textArea.undo()
                }
            }
        }

        Image {
            id: redoImg
            source: "qrc:/images/arrow-return-right.svg"
            sourceSize.height: 100
            sourceSize.width: 100
            height: 20
            width: height
            anchors.right: saveImg.left
            anchors.rightMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 5
            visible: (editNote.textArea.canUndo || editNote.textArea.canRedo)

            MouseArea{
                id:redoArea
                anchors.fill: parent
                onClicked: {
                    editNote.textArea.redo()
                }
            }
        }

        Image {
            id: favoriteImg
            property bool isChecked: false
            source:  isChecked ? "qrc:/images/bookmark-fill.svg" : "qrc:/images/bookmark.svg"
            sourceSize.height: 100
            sourceSize.width: 100
            height: 20
            width: height
            anchors.right: undoImg.visible ? undoImg.left : (saveImg.visible ? saveImg.left : parent.right)
            anchors.rightMargin: 20
            anchors.verticalCenter: undoImg.verticalCenter
            MouseArea{
                id:favoriteArea
                anchors.fill: parent
                onClicked: {
                    if(editPopup.currentIndex >=0){
                        notesModel.changeNoteFavorite(editPopup.currentIndex);
                        favoriteImg.isChecked =  notesModel.notes[editPopup.currentIndex].favorite
                    } else {
                        favoriteImg.isChecked = !favoriteImg.isChecked
                    }
                }
            }
        }

        Image {
            id: saveImg
            source: "qrc:/images/check2.svg"
            sourceSize.height: 100
            sourceSize.width: 100
            height: 30
            width: height
            anchors.right: parent.right
//                anchors.rightMargin: 20
            anchors.top: parent.top
            visible: (editNote.textArea.canUndo || editNote.textArea.canRedo) || (titleEdit.canRedo || titleEdit.canUndo)
            MouseArea{
                id:saveArea
                anchors.fill: parent
                onClicked: {
                    editPopup.saveChanges();
                }
            }
        }
    }

    Rectangle{
        anchors.top: editHeader.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        TextInput{
            id:titleEdit
            height: contentHeight
            font.pointSize: 21
            font.bold: true
            horizontalAlignment: TextInput.AlignHCenter
            width: parent.width
            text: editPopup.currentIndex > -1 ? notesFlick.model[editPopup.currentIndex].title : "untitled"
            anchors.top:  parent.top
            anchors.left: parent.left
            autoScroll: false
            wrapMode: TextInput.Wrap
            maximumLength: 124
            clip: true
            MouseArea{
                width: parent.width
                height: titleEdit.readOnly ? titleEdit.height : 0
                anchors.top: parent.top
                anchors.left: parent.left
                onClicked: {
                    editPopup.activateEditMode()
                    titleEdit.focus = true;
                    Qt.inputMethod.show();
                }
            }
        }

        Rectangle{
            id:headerSub1
            anchors.top:  titleEdit.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 30
            Rectangle{
                id: catSelect
                anchors.left: parent.left
                anchors.top: parent.top
                height: parent.height
                width: 220
                Rectangle{
                    id:currentCatColorMark
                    width: 3
                    height: parent.height
                    anchors.left: parent.left
                    anchors.top: parent.top

                    color: "#FF0000"
                }
                Text {
                    id: catName
                    text: "Без категории"
                    anchors.left: currentCatColorMark.right
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                }
                Image {
                    id: openCatSelect
                    source: "qrc:/images/caret-down-fill.svg"
                    sourceSize.height: 100
                    sourceSize.width: 100
                    height: 10
                    width: height
                    anchors.left: catName.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 10
                    visible: !navPopup.visible && !root.selectionController.selectMode
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        Qt.inputMethod.hide();
                        catSelectPopup.open()
                    }
                }

                Popup {
                    id: catSelectPopup
                    width: parent.width
                    height: 300
                    modal: true
                    dim: false
                    closePolicy: Popup.CloseOnPressOutside
                    x:0
                    y:0
                    ListView{
                        id: catSelectList
                        anchors.fill: parent
                        clip: true
                        spacing: 20
                        model: notesModel.categories

                        delegate: Rectangle{
                            height: 20
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.leftMargin: 10
                            Rectangle{
                                id:catSelectColorMark
                                width: 3
                                height: parent.height
                                anchors.left: parent.left
                                anchors.top: parent.top

                                color: catSelectList.model[index].color
                            }

                            Text {
                                id: catSelectDelegat
                                anchors.left: catSelectColorMark.right
                                anchors.leftMargin: 10
                                text: catSelectList.model[index].name
                                width: parent.width
                                clip: true
                                font.pointSize: 13
                                font.bold: selectNavDelegateArea.containsMouse
                            }
                            MouseArea{
                                id: selectNavDelegateArea
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: {
                                    currentCatColorMark.color = catSelectList.model[index].color
                                    catName.text = catSelectList.model[index].name
                                    if(editPopup.currentIndex !== -1){
                                        notesModel.updateNote(editPopup.currentIndex,
                                                              notesModel.notes[editPopup.currentIndex].title,
                                                              notesModel.notes[editPopup.currentIndex].text,
                                                              notesModel.notes[editPopup.currentIndex].lastUpdateDateTime,
                                                              catName.text === "Без категории" ? "" : catName.text,
                                                              notesModel.notes[editPopup.currentIndex].tags)
                                    }

                                    catSelectPopup.close()
                                }
                            }
                        }
                    }
                }

            }
            Text {
                id: createdate
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                text: "31.03.2021 02:26"
            }
        }

        //        EditorTextArea {
        //            id: editNote
        //            width: parent.width
        //            anchors.top: headerSub1.bottom
        //            anchors.bottom: editTools.top//parent.bottom
        //            anchors.left: parent.left

        //            //visible: editPopup.isEditMode
        //        }
        RichTextEditor {
            id: editNote
            width: parent.width
            anchors.top: headerSub1.bottom
            anchors.bottom: editTools.top//parent.bottom
            anchors.left: parent.left
            textArea.text: editPopup.currentIndex > -1 ? notesFlick.model[editPopup.currentIndex].content : ""
        }

        EditToolsPanel {
            id: editTools
            textArea: editNote.textArea
            document: editNote.document
            fontDialog: editNote.fontDialog
            colorDialog: editNote.colorDialog

            visible: editNote.textArea.focus
            width: parent.width
            anchors.bottom: parent.bottom
        }

    }
    onOpened: {
        console.log(editPopup.height + " START EDIT HEGHT")
        //editedNote.text = editPopup.currentIndex > -1 ? notesFlick.model[editPopup.currentIndex].content : "";

        if(editPopup.currentIndex !== -1){
            var catIndex = root.getCatByName(notesFlick.model[editPopup.currentIndex].category);
            favoriteImg.isChecked =  notesModel.notes[editPopup.currentIndex].favorite
            if(catIndex >= 0){
                currentCatColorMark.color = notesModel.categories[catIndex].color
                catName.text = notesFlick.model[editPopup.currentIndex].category;
                createdate.text = notesFlick.model[editPopup.currentIndex].createDateTime;
            } else {
                currentCatColorMark.color = "#EEE"
                catName.text = "Без категории"
            }

        }
    }

    Dialog{
        id: editExitDialod
        title: "Save changes?"
        standardButtons: Dialog.Save | Dialog.Discard | Dialog.Cancel
        width: parent.width/100 * 80
        x:(parent.width - width)/2

        onAccepted: {
            editPopup.exitWSave()
            editExitDialod.close()
//                editPopup.currentIndex = -1
        }

        onDiscarded: {
            editPopup.exitWOSave()
            editExitDialod.close()
//                editPopup.currentIndex = -1
        }

        onRejected: {
            editExitDialod.close()
        }
    }

    onCurrentIndexChanged: {
        if(editPopup.currentIndex == -1){
            favoriteImg.isChecked = false;
        }
    }

    onWidthChanged: {
        const re = /(<img src=['"].*['"] width=['"])(\d+)(['"][ ]?[/]?>)/g;
        editNote.textArea.text = editNote.textArea.text.replace(re, "$1"+ width +"$3");
        console.log(editNote.textArea.text)
        console.log(width)
    }

    function exitFromEdit(){
        Qt.inputMethod.hide();
        if(editPopup.isEditMode){
            if(titleEdit.canUndo || titleEdit.canRedo || editNote.textArea.canUndo || editNote.textArea.canRedo)
                editExitDialod.open()
            else
                editPopup.exitWOSave();
        } else {
            editPopup.exitWOSave();
            editPopup.currentIndex = -1
        }
    }
    function exitWOSave(){
        editPopup.close()
        editPopup.currentIndex = -1
    }
    function exitWSave(){
        if(editPopup.currentIndex === -1){
            editPopup.addNote();
        } else {
            editPopup.updateNote()
        }

        editPopup.close()
        editPopup.currentIndex = -1
    }
    function activateEditMode(){
        editNote.textArea.focus = true;
    }

    function saveChanges(){
        Qt.inputMethod.hide();
        if(editPopup.currentIndex === -1){
            editPopup.addNote();
        } else {
            editPopup.updateNote()
        }
    }

    function deleteFilePathPrefix(text){
        var res = String(text).replace(/file:/g,"")
        return res//String(text).replace(/file:/g,"")
    }

    function addNote(){
        var locale = Qt.locale()
        var addRes = notesModel.addNote(titleEdit.displayText,
                           deleteFilePathPrefix(editNote.textArea.text),
                           new Date().toLocaleString(locale,"hh:mm dd.MM.yyyy"),
                           catName.text === "Без категории" ? "" : catName.text ,
                                        "debug,", favoriteImg.isChecked);
        if(addRes){
            editPopup.currentIndex = 0;
        }
    }
    function updateNote(){
        var locale = Qt.locale()
//            var itemId = notesModel.notes[editPopup.currentIndex].index
        notesModel.updateNote(editPopup.currentIndex, titleEdit.displayText,
                           deleteFilePathPrefix(editNote.textArea.text),
                           new Date().toLocaleString(locale,"hh:mm dd.MM.yyyy"),
                           catName.text === "Без категории" ? "" : catName.text,
                              "debug,", favoriteImg.isChecked);
    }
}
