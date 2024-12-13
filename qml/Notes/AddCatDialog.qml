import QtQuick 2.12
import QtQuick.Controls 2.15

Dialog {
    id:catAddDialog

    modal: true
    standardButtons: Dialog.Ok | Dialog.Cancel
    Rectangle{
        id: newCatColor
        height: 20
        radius: 4
        width: height
        anchors.left: parent.left
        color: "#F44336"
        MouseArea{
            id: newColorArea
            anchors.fill: parent
            onClicked: {
                colorSelect.open()
            }
        }
    }

    TextInput {
        id: catAddDialogText
        text: "Some text"
        anchors.left: newCatColor.right
        anchors.leftMargin: 10
    }
    Rectangle{
        anchors.top: catAddDialogText.bottom
        anchors.left: catAddDialogText.left
        width: catAddDialogText.width
        height: 1
        color: "#CCC"
    }

    Popup{
        id:colorSelect
        width: 20*3
        height: width
        x: newCatColor.x
        y: newCatColor.y
        padding: 0
        Rectangle{
            anchors.fill: parent

            GridView{
                id:colorsGrid
                model: catColors
                anchors.fill: parent
                cellHeight: 20
                cellWidth: 20
                delegate: Rectangle{
                    height: colorsGrid.cellHeight
                    width: colorsGrid.cellWidth
                    Rectangle{
                        anchors.fill: parent
                        anchors.topMargin: 2
                        anchors.leftMargin: 2
                        anchors.bottomMargin: 2
                        anchors.rightMargin: 2
                        radius: 4
                        color: catColor

                        MouseArea{
                            id: colorArea
                            anchors.fill: parent
                            onClicked: {
                                colorSelect.close()
                                newCatColor.color = catColor
                            }
                        }
                    }
                }
            }
        }
    }

    onAccepted: {
        notesModel.createCategory(catAddDialogText.text, newCatColor.color);
    }

}
