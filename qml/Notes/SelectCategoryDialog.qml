import QtQuick 2.12
import QtQuick.Controls 2.15

Dialog {
    id:catUpdateAddDialog
    property int catIdToEdit: -1
    property string catName: ""
    property string catColor: "#EEE"

    modal: true
    standardButtons: Dialog.Ok | Dialog.Cancel
    Rectangle{
        id: newUpdateCatColor
        height: 20
        radius: 4
        width: height
        anchors.left: parent.left
        color: catUpdateAddDialog.catColor
        MouseArea{
            id: newUpdateColorArea
            anchors.fill: parent
            onClicked: {
                colorUpdateSelect.open()
            }
        }
    }

    TextInput {
        id: catUpdateAddDialogText
        text: catUpdateAddDialog.catName
        anchors.left: newUpdateCatColor.right
        anchors.leftMargin: 10
    }
    Rectangle{
        anchors.top: catUpdateAddDialogText.bottom
        anchors.left: catUpdateAddDialogText.left
        width: catUpdateAddDialogText.width
        height: 1
        color: "#CCC"
    }

    Popup{
        id:colorUpdateSelect
        width: 20*3
        height: width
        x: newUpdateCatColor.x
        y: newUpdateCatColor.y
        padding: 0
        Rectangle{
            anchors.fill: parent

            GridView{
                id:colorsUpdateGrid
                model: catColors
                anchors.fill: parent
                cellHeight: 20
                cellWidth: 20
                delegate: Rectangle{
                    height: colorsUpdateGrid.cellHeight
                    width: colorsUpdateGrid.cellWidth
                    Rectangle{
                        anchors.fill: parent
                        anchors.topMargin: 2
                        anchors.leftMargin: 2
                        anchors.bottomMargin: 2
                        anchors.rightMargin: 2
                        radius: 4
                        color: catColor

                        MouseArea{
                            id: colorUpdateArea
                            anchors.fill: parent
                            onClicked: {
                                colorUpdateSelect.close()
                                newUpdateCatColor.color = catColor
                            }
                        }
                    }
                }
            }
        }
    }

    onAccepted: {
//            notesModel.createCategory(catAddDialogText.text, newCatColor.color);
        notesModel.updateCategory(catUpdateAddDialog.catIdToEdit ,catUpdateAddDialogText.text, newUpdateCatColor.color);
    }

}

