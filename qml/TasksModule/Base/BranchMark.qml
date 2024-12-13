import QtQuick 2.12

Rectangle {
    id: root
    property bool isBranchOpen: false
    z:99
    color: "transparent"

    Image {
        id: _branchStateImg
        height: parent.height
        width: parent.width

        source: root.isBranchOpen ? "qrc:/images/branch-open.png" : "qrc:/images/branch-closed.png"
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.isBranchOpen = !isBranchOpen
        }
    }
}
