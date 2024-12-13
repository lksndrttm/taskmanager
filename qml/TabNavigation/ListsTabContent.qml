import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import TasksModule.Impl 1.0
import MainFrameModule 1.0
import StyleSettings 1.0
import InputModule.Impl 1.0

MainFrame {
    id: _listsTab
    signal listToShowSelected(string name)
    signal addButtonClicked()
    signal selectModeDiactivate()

    MainFrameContent {
        id: _content

        anchors.top: parent.top
        anchors.bottom: _cbuttons.top
        List{
            id: _view
            height: parent.height
            width: parent.width
            onListItemClicked:
            {
                _listsTab.listToShowSelected(listname)
            }

            ListAddDialog {
                id:_addDialog
                parent: Overlay.overlay
                x:0
                y:parent.height - height - Style.mainFrameButtonsSize + Style.mainFrameBorderWidth
                width: parent.width
                height: Style.addDialogHeight
                onAddButtonClicked: {
                    _view.addList(_addDialog.listname,_addDialog.listtype);
                }
            }
        }

    }
    onAddButtonClicked:
    {
        _view.selectModeDiactivate()
        _addDialog.open()
    }
    onSelectModeDiactivate: {
        _view.selectModeDiactivate()
    }

    AddButton {
        id: _cbuttons
        height: Style.mainFrameButtonsSize
        width: Style.mainFrameButtonsSize
        z:100
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: Style.mainFrameMargin/2
        onClicked: {
            addButtonClicked()
        }
    }

    MouseArea {
        anchors.left: parent.left
        anchors.right: _cbuttons.right
        anchors.bottom: parent.bottom
        height: Style.mainFrameButtonsSize
        onClicked: {
            _view.selectModeDiactivate()
        }
    }

}

