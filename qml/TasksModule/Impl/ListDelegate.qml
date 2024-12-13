import QtQuick 2.0
import QtQuick.Controls 2.12
import TasksModule.Base 1.0
import StyleSettings 1.0

BaseListDelegate {
    id: root
    textColor: isSelectedInSelectMode ? Style.selectedTextColor : (root.isHover ? Style.hoverTextColor : Style.textColor)
    color: isSelectedInSelectMode ? Style.selectedBgColor : (root.isHover ? /*Style.hoverBgColor*/ Style.delegateBgColor :  Style.delegateBgColor)
//    border.color: "black"
//    border.width: root.isHover ? 1 : 0
}
