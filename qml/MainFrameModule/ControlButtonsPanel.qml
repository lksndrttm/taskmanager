import QtQuick 2.0
import StyleSettings 1.0


Rectangle{
    id: _controlButtons

    height: Style.mainFrameButtonsSize
    width: parent.width
    z:100
    signal addButtonClicked()
    signal delButtonClicked()
    AddButton{
        id: _add
        anchors.right: _del.left
        anchors.bottom: parent.bottom
        height: Style.mainFrameButtonsSize
        width: Style.mainFrameButtonsSize
        onClicked: {
            addButtonClicked()
        }
    }

    DelButton{
        id: _del
        width: _add.width
        height: _add.height
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: Style.mainFrameMargin/2
        onClicked: {
            delButtonClicked()
        }
    }

}
