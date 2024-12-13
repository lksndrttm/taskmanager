import QtQuick 2.12
import QtQuick.Controls 2.12
import StyleSettings 1.0

Rectangle {
    id: root
    property string selectCountText: "test"

    signal delButtonClicked()
    signal closeClicked()
    signal selectAllToggled(bool state)

    color: "black"
    border.color: Style.taskListHeaderBorderColor
    border.width: Style.taskListHeaderBorderWidth

    height: Style.tabHeight
    TaskCheckBox{
        id:_selectAllBox
        height: Style.tabHeight -  Math.round(Style.tabHeight/5)
        width: height
        checkBoxColor: "black"
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.verticalCenter: parent.verticalCenter
        onCheckBoxToggled: {
            selectAllToggled(state)
        }
    }
    Text {
        id: _seclectCount
        text: root.selectCountText
        font.pointSize: 14
        color: "white"
        anchors.left: _selectAllBox.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 10

    }

    Rectangle{
        id: _closebtn
        height: Style.taskListHeaderHeight - 10
        width: height
        color: parent.color
        Image {
            id: _closeimg
            source: "qrc:/images/white_trash.png"
            height: parent.height
            width: parent.width
        }
        anchors.right: _exitbtn.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 10

        MouseArea{
            id:_marea
            anchors.fill: parent
            onClicked: {
                root.delButtonClicked()
            }
        }
    }

    Rectangle{
        id: _exitbtn
        height: Style.taskListHeaderHeight - 10
        width: height
        color: parent.color
        Image {
            id: _exitimg
            source: "qrc:/images/whiteX.png"
            height: parent.height
            width: parent.width
        }
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: Style.taskListHeaderCloseMargin

        MouseArea{
            id:_exitmarea
            anchors.fill: parent
            onClicked: {
                root.closeClicked()
            }
        }
    }
}
