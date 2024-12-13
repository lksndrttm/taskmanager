import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import ListsModel 1.0

Item {
    id: root
    property string info: ""

    ListsModel{
        id:model
    }

    Text {
        id: _info
        text: root.info
        anchors.top: parent.top
    }
    Button {
        id: _dbCopyButton
        text: "Create db copy"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 50
        anchors.rightMargin: 50
        anchors.top: _info.bottom
        height: 40
        onClicked: {
            root.info = model.createDatabaseCopy() ? "OK" : "FAILED";
        }
    }
}
