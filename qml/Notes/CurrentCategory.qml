import QtQuick 2.12

Rectangle{
    id: root
    property alias currentTag: currentTag

    property var selectionController

    implicitHeight: currentTag.implicitHeight + 20


    Rectangle{
        id:selectModeHeader
        visible: root.selectionController.selectMode
        anchors.fill: parent
        Text {
            id: selectionInfo
            text: "Выбрано элементов: " + root.selectionController.countOfSelected
            font.pointSize: 16
            anchors.left: closeSelectImg.right
            anchors.top: parent.top
            anchors.topMargin: 30
            anchors.leftMargin: 10
        }

        NavImage {
            id: closeSelectImg
            action: root.selectionController.exitFromSelectMode
            source: "qrc:/images/close.svg"
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: selectionInfo.verticalCenter
        }
    }

    Text {
        id: currentTag
        text: "Все заметки"
        font.pointSize: 16
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
        visible: !root.selectionController.selectMode
    }

    NavImage {
        id: opennav
        action: navPopup.open
        source: "qrc:/images/caret-down-fill.svg"
        visible: !navPopup.visible && !root.selectionController.selectMode
        anchors.left: currentTag.right
        anchors.leftMargin: 10
        anchors.verticalCenter: currentTag.verticalCenter
    }

    NavImage {
        id: closenav
        action: navPopup.close
        source: "qrc:/images/caret-up-fill.svg"
        visible: navPopup.visible && !root.selectionController.selectMode
        anchors.left: currentTag.right
        anchors.leftMargin: 10
        anchors.verticalCenter: currentTag.verticalCenter
    }

}
