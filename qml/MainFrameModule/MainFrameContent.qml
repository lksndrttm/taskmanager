import QtQuick 2.0
import StyleSettings 1.0
import QtQuick.Controls 2.12
ScrollView {
    id: root
    //height: parent.heightWorkspace
    width: parent.width - Style.mainFrameMargin


    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    ScrollBar.vertical.policy: ScrollBar.AsNeeded

    //anchors.bottom: parent.bottom
    //anchors.bottomMargin: Style.mainFrameButtonsSize
    anchors.horizontalCenter: parent.horizontalCenter

    clip: true
    Rectangle{
        id: _border
        anchors.fill: parent
        border.color: Style.mainFrameBorderColor
        border.width: Style.mainFrameBorderWidth
    }
}
