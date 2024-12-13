import QtQuick 2.0
import QtQuick.Layouts 1.12
import TasksModule.Impl 1.0
import MainFrameModule 1.0
import StyleSettings 1.0


StackLayout{
    id: root
    Layout.fillHeight: true
    Layout.fillWidth: true

    signal taskModelChanged(string listname)
    signal selectModeDiactivate()

    ListsTabContent{
        id: _listsTab
        Layout.fillHeight: true
        Layout.fillWidth: true
        onListToShowSelected: {
            root.currentIndex = 1
            root.taskModelChanged(name)
        }
        onSelectModeDiactivate: {
            _listTab.selectModeDiactivate();
        }
    }


    TaskListTabContent{
        id: _tasksTab
        Layout.fillHeight: true
        Layout.fillWidth: true
        onCloseClicked: {
            root.currentIndex = 0
        }
    }

    onTaskModelChanged: {
        _tasksTab.model.setSelectedList(listname)
        _tasksTab.headerText = listname
    }

}
