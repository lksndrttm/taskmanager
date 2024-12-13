pragma Singleton
import QtQuick 2.0

QtObject {
    property bool isDarkTheme: false
    property bool isAndroid: true

    property color background: isDarkTheme ? "black" : "white"
    property color textColor: isDarkTheme ? "white" : "black"
    property color hoverTextColor: "black"
    property color hoverBgColor: "#EEEEEE"
    property color delegateBgColor: isDarkTheme ? "transparent" : "transparent"
    property color selectedBgColor:  "black"
    property color selectedTextColor: "white"
    property real delegateTextSize: isAndroid ? 13 : 10

    property color maxPriorTaskColor: "#F44336"
    property color midPriorTaskColor: "#9C27B0"
    property color lowPriorTaskColor: "#2196F3"


    property real mainFrameMargin: 10
    property real mainFrameButtonsSize: isAndroid ? 40 : 30
    property real mainFrameBorderWidth: 1
    property color mainFrameBorderColor: "black"

    property real tabHeight: isAndroid ? 25 : 20
    property real controllButtonsTopMargin: 10

    property real listItemHeight: isAndroid ? 40 : 30
    property real listItemCheckboxRMargin: isAndroid ? 15 : 10


    property real taskListHeaderHeight: isAndroid ? 30 : 20
    property color taskListHeaderColor: "black"
    property color taskListHeaderTextColor: "white"
    property color taskListHeaderBorderColor: "#2E2E2E"
    property real taskListHeaderBorderWidth: 1
    property real taskListHeaderTextSize: isAndroid ? 30 : 20
    property real taskListHeaderCloseMargin: isAndroid ? 15 : 10
    property color taskListHeaderCheckBoxColor: "black"
    property real taskListItemHeight: isAndroid ? 35 : 30
    property color taskCheckBoxColor: "#555555"
    property real listItemCheckboxSize: taskListItemHeight/2
    property color taskHoverTextColor: "black"
    property color taskHoverBgColor: "#EEEEEE"
    property color taskDelegateBgColor: "transparent"
    property color taskSelectedBgColor:  "black"
    property color taskSelectedTextColor: "white"
    property color taskTextColor: "black"


    property color taskHighPriorColor: "#E91E63"
    property color taskMidPriorColor: "#9C27B0"
    property color taskLowPriorColor: "#2196F3"//"#9E9E9E"
    property real detailsSubTaskHeight: isAndroid ? 30 : 15
    property real detailsSubTaskFontSize: isAndroid ? 13 : 11


    property real taskBranchMarkSize: 7


    property real addDialogHeight: isAndroid ? 40 : 30
    property real addDialogInputHeight: isAndroid ? 20 : 15
    property real addDialogButtonsHeight: isAndroid ? 25 : 20
    property real addDialogTextWidth: 50
    property real addDialogComboBoxWidth: 85
    property real addDialogButtonsWidth: 50


    property real iterableTaskHeight: isAndroid ? 60 : 50
    property real iterCompliteButtonWidth: isAndroid ? 80 : 65
    property real iterCompliteButtonHeight: isAndroid ? 15 : 15
    property real iterMaxSeriesTextHeight: isAndroid ? 9 : 7
    property real iterDelegateTextHeight: isAndroid ? 13 : 9
    property real iterLevelTextHeight: isAndroid ? 13 : 9
    property real iterVertMargin: isAndroid ? 3 : 1



    property real scheduleTimeTextSize: isAndroid ? 10 : 6
    property real scheduleTaskListItemHeight: isAndroid ? 35 : 35


    //##########################################################
    property real noteBaseFont: 12

}
