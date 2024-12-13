import QtQuick 2.12
import QtQuick.Controls 2.12
import InputModule.Base 1.0
import StyleSettings 1.0

BaseAddDialog {
    id: _popup
    signal addButtonClicked()
    property string listname: _input.currentDisplayText
    property string listtype: _typeBox.displayText

    contentItem: Rectangle {
        id: _content
        height: Style.addDialogHeight
        width: parent.width


        BaseTextInput{
            id: _input
            labelText: "listname"
            height: Style.addDialogButtonsHeight
            anchors.right: _typeBox.left
            anchors.left: parent.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 5
            radius: 2

        }

        BaseComboBox {
            id: _typeBox
            model: ["tasklist","schedule"]
            height: Style.addDialogButtonsHeight
            width: Style.addDialogComboBoxWidth
            anchors.right: _acceptButton.left
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 10
        }

        Button{
            id: _acceptButton
            text: qsTr("add")
            height: Style.addDialogButtonsHeight
            width: Style.addDialogButtonsWidth
            background: Rectangle {
                anchors.fill: parent
                border.color: "#3A3A3A"
                border.width: 1
                radius: 2
                color: _addArea.containsMouse ? "#3A3A3A" : "white"
            }
            contentItem: Text {
                text: _acceptButton.text
                font: _acceptButton.font
                //opacity: enabled ? 1.0 : 0.3
                color: _addArea.containsMouse ? "white" : "#3A3A3A"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }


            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter

            MouseArea{
                id: _addArea
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    _popup.addButtonClicked()
                    _popup.close()
                }

            }
        }
        focus: true
        Keys.onEscapePressed: {
            _popup.close()
        }
    }
}
