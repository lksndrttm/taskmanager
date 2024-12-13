import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import StyleSettings 1.0
import TasksModule.Impl 1.0

TreeView {
    id:root
    signal closeClicked()
    signal exitFromSelectMode()
    signal selectAll()
    signal deleteSelectedTasks()
    property string headerText: ""
    property bool isSelectionMode: false

    horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
    contentItem.anchors.topMargin: Style.taskListHeaderHeight
    backgroundVisible: false
    headerVisible: false
    style: TreeViewStyle {
        branchDelegate: Rectangle{
            width: Style.taskListItemHeight - 25
            height: Style.taskListItemHeight
            color: "transparent"
            Image {
                id: _indicator
                anchors.centerIn: parent
                width: Style.taskListItemHeight - 25
                height: Style.taskListItemHeight - 25

                source: styleData.isExpanded ? (styleData.selected ? "qrc:/images/branch-open-white.png" : "qrc:/images/branch-open.png") :  (styleData.selected ? "qrc:/images/branch-closed-white.png" : "qrc:/images/branch-closed.png")
                visible: styleData.hasChildren
                sourceSize.height: 512
                sourceSize.width: 512
                fillMode: Image.PreserveAspectFit
            }
        }
    }

    Rectangle {
        id:_header
        z:100
        height: Style.taskListHeaderHeight
        width: parent.width
        anchors.top: parent.top

        Component {
            id: _defaultHeader
            Rectangle {
                border.color: Style.taskListHeaderBorderColor
                border.width: Style.taskListHeaderBorderWidth
                height: Style.taskListHeaderHeight
                width: parent.width
                color: Style.taskListHeaderColor
                Text {
                    id: _headerText
                    text: qsTr(headerText)
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    color: Style.taskListHeaderTextColor
                    font.bold: true
                }
                Rectangle{
                    id: _closebtn
                    height: Style.taskListHeaderHeight - 10
                    width: height
                    color: parent.color
                    Image {
                        id: _closeimg
                        source: "qrc:/images/whiteX.png"
                        height: parent.height
                        width: parent.width
                    }
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: Style.taskListHeaderCloseMargin

                    MouseArea{
                        id:_marea
                        anchors.fill: parent
                        onClicked: {
                            root.closeClicked()
                        }
                    }
                }

            }
        }

        Component {
            id: _selectionModeHeader
            SelectionWidget{
                onCloseClicked:{
                    root.exitFromSelectMode();
                }
                onSelectAllToggled:{
                    if(state){
                        root.selectAll()
                    }
                    else{
                        root.exitFromSelectMode()
                    }
                }
                onDelButtonClicked: {
                    root.deleteSelectedTasks()
                }
            }
        }


        Loader {
            id: _hcontent
            //z:100
            height: parent.height
            width: parent.width
            sourceComponent: isSelectionMode ? _selectionModeHeader : _defaultHeader
        }


    }

    rowDelegate: Rectangle {
        height: Style.taskListItemHeight
        color: styleData.selected ? "black" : "transparent"
    }

    Component {
        id: _hollowStyle
        TreeViewStyle {}
    }
    Component.onCompleted: {
        if(!Style.isAndroid){
            root.style = _hollowStyle;
        }
    }
}
