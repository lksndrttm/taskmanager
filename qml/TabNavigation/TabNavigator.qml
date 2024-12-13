import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import TasksModule.Impl 1.0
import MainFrameModule 1.0
import StyleSettings 1.0

Item {
    id: root
    anchors.fill: parent

    TabBar {
        id: bar
        //width: parent.width
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: Style.mainFrameMargin/2
        anchors.rightMargin: Style.mainFrameMargin/2
        contentHeight: Style.tabHeight
        anchors.top: parent.top


        Tab {
            text: qsTr("Tasks")
            width: implicitWidth
        }
        Tab {
            text: qsTr("Iterarors")
            width: implicitWidth
        }
        Tab {
            text: qsTr("Notes")
            width: implicitWidth
        }
        Tab {
            text: qsTr("Settings")
            width: implicitWidth
        }

    }

    StackLayout {
        id: _stack
        currentIndex: bar.currentIndex
        width: parent.width
        anchors.top: bar.bottom
        anchors.bottom: parent.bottom

        ListStackItem{
            id: _listTab
        }


        IterStackItem {
            id: _iterTab
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        NotesStackItem {
            id: _notesTab
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        SettingsStackItem{
            id:_settingsTab
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

}
