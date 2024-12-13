import QtQuick 2.0
import StyleSettings 1.0

Rectangle {
    id:root
    signal selectModeActivated(int id)

    property bool isHover: false
    readonly property bool isSelected: ListView.isCurrentItem
    readonly property string delegateText: _delegateText.text
    property bool isSelectedInSelectMode: false
    property real textLeftMargin: 5
    property real textRightMargin: 10
    property color textColor: Style.textColor

    property alias textItem : _delegateText
    //color: (isHover && !isSelectedHglight) ? Style.hoverBgColor : (isSelectedHglight) ? Style.selectedBgColor : Style.delegateBgColor

    Text {
        id: _delegateText
        text: name
        color: textColor
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: root.textRightMargin
        anchors.leftMargin: root.textLeftMargin
        anchors.verticalCenter: parent.verticalCenter
        font.bold: root.isHover
        font.pointSize: Style.delegateTextSize
    }

    MouseArea{
        id:_delegateArea
        hoverEnabled: true
        anchors.fill: parent
        onEntered: {
            root.isHover = true

        }
        onExited: {
            root.isHover = false
        }



    }


}
