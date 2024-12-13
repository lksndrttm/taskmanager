import QtQuick 2.12
import QtQuick.Controls 2.12
import StyleSettings 1.0

Rectangle {
    id:root
    signal selectModeActivated(int id)
    signal changeDaylyStatus(string name)
    signal decrease(string name)
    signal increase(string name)

    property bool isHover: false
    property bool isSelectMode: false
    readonly property bool isSelected: ListView.isCurrentItem
    readonly property string delegateText: _delegateText.text
    property bool isSelectedInSelectMode: false
    property real textLeftMargin: 5
    property real textRightMargin: 10
    property color textColor: isSelectedInSelectMode ? Style.selectedTextColor : (root.isHover ? Style.hoverTextColor : Style.textColor)
    property var levelInfo
    property int currentPeriod: compliteCount


    property string taskname: name
    property string tasktype: type
    property string totalCount: compliteCount
    property string maxSeries: maxseries
    property string currentSeries: series
    property string timeStep: seriesTimeStep
    property int currentLevel: level
    property int levelUpScore: upLevelScore
    property string isRoundBeginTime: isMidnight

    property alias textItem : _delegateText
    //textColor: isSelectedInSelectMode ? Style.selectedTextColor : (root.isHover ? Style.hoverTextColor : Style.textColor)
    color: isSelectedInSelectMode ? Style.selectedBgColor : (root.isHover ? /*Style.hoverBgColor*/ Style.delegateBgColor :  Style.delegateBgColor)
    //color: (isHover && !isSelectedHglight) ? Style.hoverBgColor : (isSelectedHglight) ? Style.selectedBgColor : Style.delegateBgColor

    Rectangle{
        id:_seriesPointsView
        width: parent.height - 10
        height: parent.height - 10
        radius: width
        border.color: "black"
        border.width: 1
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter

        Text {
            id: _series
            text: level+""
            anchors.centerIn: parent
            font.bold: true
            font.pointSize: Style.iterLevelTextHeight
            //fontSizeMode: Text.Fit
        }
    }


    Text {
        id: _delegateText
        text: name
        color: textColor
        anchors.left: _seriesPointsView.right
        anchors.right: parent.right
        anchors.rightMargin: root.textRightMargin
        anchors.leftMargin: root.textLeftMargin
        anchors.top: _seriesPointsView.top
        font.bold: true
        font.pointSize: Style.iterDelegateTextHeight
    }


    Text {
        id: _maxSeries
        text: qsTr("Max series: ") + maxseries
        anchors.top: _delegateText.bottom
        anchors.topMargin: Style.iterVertMargin
        anchors.left: _delegateText.left
        color: root.textColor
        font.pointSize: Style.iterMaxSeriesTextHeight
    }
    Text {
        id: _totalCount
        text: qsTr("Total: ") + compliteCount
        anchors.top: _maxSeries.bottom
        anchors.topMargin: Style.iterVertMargin
        anchors.left: _delegateText.left
        color: root.textColor
        font.pointSize: Style.iterMaxSeriesTextHeight
    }

    Text {
        id: _nextCompliteTime
        text:  qsTr("break at: ") + nextTime
        color: "#555555"
        font.pointSize: Style.iterMaxSeriesTextHeight
        anchors.right: _stepsToNextLevel.left
        anchors.rightMargin: 5
        anchors.bottom: _progresbar.top
    }

    Text {
        id: _stepsToNextLevel
        text:  "[" + (series % upLevelScore) + "/" + upLevelScore + "]"
        color: "#555555"
        font.pointSize: Style.iterMaxSeriesTextHeight
        anchors.right: _progresbar.right
        anchors.bottom: _progresbar.top
    }

    Rectangle {
        id: _compliteButton
        width: Style.iterCompliteButtonWidth
        height: Style.iterCompliteButtonHeight
        anchors.right: _progresbar.right
        anchors.verticalCenter: parent.verticalCenter
        color: root.textColor
        radius: 2
        Text {
            id: _changeStatTo
            text: !stat ? "complite" : "uncomplite"
            color: root.textColor === Style.selectedTextColor ? "black" : "white"
            anchors.centerIn: parent
        }
        MouseArea {
            id: _changeStatMarea
            anchors.fill: parent
            onClicked: {
                root.changeDaylyStatus(name)
            }
        }
    }

    IteratorsProgressBar{
        id:_progresbar
        height: 2
        anchors.left: _seriesPointsView.right
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2
//        from: root.levelInfo[2]
//        to: root.levelInfo[0]
        value: (series/ upLevelScore ) < 1 ? series/ upLevelScore : (series % upLevelScore)/ upLevelScore
    }

    MouseArea{
        id:_delegateArea
        hoverEnabled: true
        anchors.fill: parent
        propagateComposedEvents: true
        onEntered: {
            root.isHover = true

        }
        onExited: {
            root.isHover = false
        }

        onPressAndHold: {
            root.selectModeActivated(index)
        }

    }



}

