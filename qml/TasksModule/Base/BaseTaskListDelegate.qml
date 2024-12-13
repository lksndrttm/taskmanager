import QtQuick 2.12
import QtQuick.Controls 2.12
import InputModule.Base 1.0
import TasksModule.Impl 1.0
import StyleSettings 1.0

Rectangle{
    id:root
    signal pressAndHolodItem()
    signal clickedEvent()
    property string prior: ""
    //color: styleData.selected ? Style.taskSelectedBgColor : "transparent"
    color: styleData.selected ? Style.taskSelectedBgColor : Style.taskDelegateBgColor
    Rectangle {
        id: _prior
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 3
        anchors.bottomMargin: 3
        width: 7
        anchors.left: parent.left
        color: root.prior === "3" ? Style.taskHighPriorColor : root.prior === "2" ? Style.taskMidPriorColor : Style.taskLowPriorColor

    }

    Text {
        id:_taskName
        anchors.left: _prior.right
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.verticalCenter
        //color: styleData.selected ? "white" : "black"
        color:  styleData.selected ? Style.taskSelectedTextColor : Style.taskTextColor
        elide: styleData.elideMode
        text: styleData.value//root.model.getTimePeriod(styleData.index)
    }

    MouseArea{
        id:_marea
        anchors.fill: parent
        onPressAndHold: {
            pressAndHolodItem()
        }
        onClicked: {
            root.clickedEvent()
        }
    }


}
