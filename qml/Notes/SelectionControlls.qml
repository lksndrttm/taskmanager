import QtQuick 2.12

Rectangle {
    id:selectionFooter
    property real iconSize: 25
    property real elBottomMarig: -10
    property real fsize: 11

    property var selectionController


    height: 60
    Rectangle{
        height: parent.height
        width: parent.width / 3
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        Image {
            id: replaceImg
            source: "qrc:/images/intersect.svg"
            sourceSize.height: 100
            sourceSize.width: 100
            height: selectionFooter.iconSize
            width: height
            anchors.verticalCenter:  parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset: selectionFooter.elBottomMarig
        }
        Text {
            id: replaceText
            anchors.top: replaceImg.bottom
            anchors.horizontalCenter: replaceImg.horizontalCenter
            text: qsTr("Переместить")
            font.pointSize: selectionFooter.fsize
        }
        MouseArea{
            anchors.fill: parent
            onPressed: {
                parent.color = "#EEEEEE"
            }
            onReleased: {
                parent.color = "white"
            }
        }
    }
    Rectangle{
        height: parent.height
        width: parent.width / 3
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
//            anchors.topMargin: 10
        Image {
            id: deleteImg
            source: "qrc:/images/trash2.svg"
            sourceSize.height: 100
            sourceSize.width: 100
            height: selectionFooter.iconSize
            width: height
            anchors.verticalCenter:  parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset: selectionFooter.elBottomMarig

        }
        Text {
            id: deleteText
            anchors.top: deleteImg.bottom
            anchors.horizontalCenter: deleteImg.horizontalCenter
            text: qsTr("Удалить")
            font.pointSize: selectionFooter.fsize
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                root.selectionController.deleteSelected()
            }

            onPressed: {
                parent.color = "#EEEEEE"
            }
            onReleased: {
                parent.color = "white"
            }
        }
    }
    Rectangle{
        height: parent.height
        width: parent.width / 3
        anchors.right: parent.right
        anchors.bottom: parent.bottom
//            anchors.rightMargin: 60
//            anchors.topMargin: 10
        Image {
            id: selectAllImg
            source: "qrc:/images/stickies.svg"
            sourceSize.height: 100
            sourceSize.width: 100
            height: selectionFooter.iconSize
            width: height
            anchors.verticalCenter:  parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset: selectionFooter.elBottomMarig
        }
        Text {
            id: selectAllText
            anchors.top: selectAllImg.bottom
            anchors.horizontalCenter: selectAllImg.horizontalCenter
            text: qsTr("Выбрать все")
            font.pointSize: selectionFooter.fsize
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                root.selectionController.selectAllNotes();
            }
            onPressed: {
                parent.color = "#EEEEEE"
            }
            onReleased: {
                parent.color = "white"
            }
        }
    }

}

