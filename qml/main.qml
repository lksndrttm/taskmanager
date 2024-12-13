import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import MainFrameModule 1.0
import StyleSettings 1.0

import TasksModule.Impl 1.0
import TabNavigation 1.0

ApplicationWindow {
    id: root
    visible: true
    height: 650
    width: 360

    TabNavigator{
        anchors.fill: parent
    }

//    TextArea {
//        id: richEdit
//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.top: parent.top
//        anchors.bottom: infoBg.top
//        textFormat: TextEdit.RichText

//        selectByMouse: true
//    }

//    Rectangle{
//        id: infoBg
//        color: "blue"
//        width: parent.width
//        height: info.implicitHeight + 20
//        anchors.bottom: parent.bottom
//        Text {
//            id: info
//            text: String(richEdit.cursorRectangle.x) + " : " + String(richEdit.cursorRectangle.y)
//        }

//        Button{
//            id: bt
//            anchors.right: parent.right
//            anchors.verticalCenter: parent.verticalCenter
//            height: 30
//            text: "AddImage"

//            onClicked: {
//                richEdit.insert(richEdit.cursorPosition, "<img src='qrc:/images/t8.jpg' width="+ richEdit.width +">")
//            }
//        }
//    }

}
