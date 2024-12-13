import QtQuick 2.12
import QtQuick.Controls 2.15

Popup {
    id:root
    closePolicy: Popup.CloseOnPressOutside
    property var currentTag //display name of current category
    padding: 0

    Flickable {
        id: navFlick
        anchors.fill: parent
        clip: true
        flickableDirection: Flickable.VerticalFlick
        contentHeight: specialNotesCats.height +  navCol.height

        Rectangle{
            id:specialNotesCats
            height: 70
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            anchors.topMargin: 10
            Rectangle{
                id:allNotesCat

                anchors.top: parent.top
                anchors.left: parent.left
                width: parent.width
                height: 30
                Image {
                    id: allCatImg
                    source: "qrc:/images/sticky-fill.svg"
                    sourceSize.height: 100
                    sourceSize.width: 100
                    height: 20
                    width: height
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    id: allCatText
                    text: "Все заметки"
                    anchors.left: allCatImg.right
                    anchors.leftMargin: 10
                    font.pointSize: 14
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    id: allCount
                    text: notesModel.categories_counts[notesModel.categories_counts.length-1]
                    anchors.verticalCenter: allCatText.verticalCenter
                    anchors.right: parent.right
                }
                MouseArea{
                    id: allCatArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        notesModel.showAll();
                        currentTag.text = "Все заметки";
                        root.close();
                    }
                }

            }
            Rectangle{
                id:mainNotesCat

                anchors.top: allNotesCat.bottom
                anchors.left: parent.left
                width: parent.width
                height: 30
                Image {
                    id: mainCatImg
                    source: "qrc:/images/bookmark-fill.svg"
                    sourceSize.height: 100
                    sourceSize.width: 100
                    height: 20
                    width: height
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    id: mainCatText
                    text: "Избранное"
                    anchors.left: mainCatImg.right
                    anchors.leftMargin: 10
                    font.pointSize: 14
                    font.bold: true
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    id: mainCount
                    text: notesModel.categories_counts[notesModel.categories_counts.length-2]
                    anchors.verticalCenter: mainCatText.verticalCenter
                    anchors.right: parent.right
                }
                MouseArea{
                    id: mainCatArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        notesModel.showFavorite();
                        currentTag.text = "Избранное";
                        root.close();
                    }
                }

            }

            Rectangle{
                id:spacer
                height: 5
                width: parent.width
                anchors.top: mainNotesCat.bottom
                anchors.topMargin: 5
                color: "#F5F5F5"
            }
        }

        Column{
            id:navCol
            anchors.top: specialNotesCats.bottom
            anchors.topMargin: 10
            spacing: 10
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 10
            height: navList.model.count * 20 + navList.model.count*navCol.spacing + 70 + addNewCat.height
            Repeater{//ListView{
            id: navList
            model: notesModel.categories

            delegate: Rectangle{
                height: 20
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 10
                Rectangle{
                    id:catColorMark
                    width: 3
                    height: parent.height
                    anchors.left: parent.left
                    anchors.top: parent.top

                    color: navList.model[index].color
                }

                Text {
                    id: catDelegat
                    anchors.left: catColorMark.right
                    anchors.leftMargin: 10
                    text: navList.model[index].name
                    width: parent.width
                    clip: true
                    font.pointSize: 13
                    font.bold: navDelegateArea.containsMouse
                }
                Text {
                    id: catCount
                    text: notesModel.categories_counts[index]
                    anchors.verticalCenter: catDelegat.verticalCenter
                    anchors.right: parent.right
                }
                MouseArea{
                    id: navDelegateArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        currentTag.text = catDelegat.text;
                        notesModel.showCategory(currentTag.text);
                        root.close();
                    }

                    onPressAndHold: {
//                                dropNavMenu.x = (navDelegateArea.mouseX + dropNavMenu.width >= parent.width/2) ? navDelegateArea.mouseX - dropNavMenu.width : navDelegateArea.mouseX
//                                dropNavMenu.y = (navDelegateArea.mouseY + dropNavMenu.height >= navCol.height) ? navDelegateArea.mouseY - dropNavMenu.height : navDelegateArea.mouseY
//                                if(dropNavMenu.x < 0)
//                                    dropNavMenu.x = 0;
//                                if(dropNavMenu.y < 0)
//                                    dropNavMenu.y = 0;
                        dropNavMenu.x = navDelegateArea.mouseX
                        dropNavMenu.y = navDelegateArea.mouseY
                        dropNavMenu.menuForId = index
                        dropNavMenu.open()
                    }
                }
            }
            }

            Popup {
                id: dropNavMenu
                property int menuForId: -1
                height: 60
                width: 120
                modal: true
                dim: false
                padding: 0
                closePolicy: Popup.CloseOnPressOutside

                Rectangle{
                    id: editElem
                    width: parent.width
                    height: 30
                    anchors.top: parent.top
                    color: "#EEE"
                    Text {
                        id: editCat
                        anchors.fill: parent
                        text: qsTr("Изменить")
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: 13
                        font.bold: editCatArea.containsMouse

                    }
                    MouseArea{
                        id: editCatArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            catUpdateAddDialog.catIdToEdit = dropNavMenu.menuForId
                            catUpdateAddDialog.catColor = notesModel.categories[dropNavMenu.menuForId].color
                            catUpdateAddDialog.catName = notesModel.categories[dropNavMenu.menuForId].name
                            dropNavMenu.close();
                            catUpdateAddDialog.open();
                        }
                    }
                }
                Rectangle{
                    id: deleteItem
                    width: parent.width
                    height: 30
                    anchors.top: editElem.bottom
                    color: "#EEEEEE"
                    Text {
                        id: deleteCat
                        anchors.fill: parent
                        text: qsTr("Удалить")
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: 13
                        font.bold: delCatArea.containsMouse
                    }
                    MouseArea{
                        id: delCatArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            catDeleteDialog.open()
                            dropNavMenu.close()
                            catDeleteDialog.title = "Удалить " + notesModel.categories[dropNavMenu.menuForId].name + " ?"
                        }
                    }
                }
            }

            Dialog{
                id: catDeleteDialog
                title: ""
                standardButtons: Dialog.Ok | Dialog.Cancel
                height: 90
//                        width: 300
//                        height: 50
                anchors.centerIn: parent
                onAccepted: {
                    notesModel.deleteCategory(dropNavMenu.menuForId)
                    catDeleteDialog.close()
                }
                onRejected: {
                    catDeleteDialog.close()
                }
            }

            Rectangle {
                id:addNewCat
                height: 20
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 10

                Image {
                    id: addCatImg
                    source: "qrc:/images/plus-circle.svg"
                    sourceSize.height: 100
                    sourceSize.width: 100
                    height: 20
                    width: height
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    id: addCatText
                    anchors.left: addCatImg.right
                    anchors.leftMargin: 10
                    text: "Создать"
                    width: parent.width
                    clip: true
                    font.pointSize: 13
                    font.bold: catCreateArea.containsMouse
                    verticalAlignment: Text.AlignVCenter
                }
                MouseArea{
                    id:catCreateArea
                    anchors.fill:parent
                    hoverEnabled: true
                    onClicked: {
                        root.close()
                        catAddDialog.open()
                    }
                }
            }
        }
    }
}

