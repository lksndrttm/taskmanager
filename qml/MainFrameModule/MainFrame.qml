import QtQuick 2.0
import QtQuick.Controls 2.12
import StyleSettings 1.0

Rectangle {
    id: root
    //width: parent.width
    //height: parent.height - Style.tabHeight
    //anchors.bottom: parent.bottom
    readonly property real heightWorkspace: height - Style.mainFrameButtonsSize - Style.tabHeight

}
