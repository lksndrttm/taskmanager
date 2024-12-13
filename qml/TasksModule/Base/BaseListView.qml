import QtQuick 2.12
import StyleSettings 1.0

ListView {
    property bool isSelectMode: false
    property var selectedIndexList: []
    highlight: Rectangle { color: Style.selectedBgColor}
    highlightFollowsCurrentItem: false
    headerPositioning: ListView.OverlayHeader
}
