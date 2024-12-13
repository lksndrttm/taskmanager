import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import TasksModule.Impl 1.0
import MainFrameModule 1.0
import StyleSettings 1.0
import InputModule.Impl 1.0

MainFrame {
    id: root
    signal addButtonClicked()
    signal selectModeDiactivate()

    MainFrameContent {
        id: _content

        anchors.top: parent.top
        anchors.bottom: _cbuttons.top
        IteratorView{
            id: _view
            height: parent.height
            width: parent.width

            IterAddDialog {
                id:_addDialog
                parent: Overlay.overlay
                x:0
                y:parent.height - height - Style.mainFrameButtonsSize + Style.mainFrameBorderWidth
                width: parent.width
                height: _addDialog.taskType === "series" ? Style.addDialogHeight*4 + 10 : Style.addDialogHeight*2 + 14
                onAddButtonClicked: {
                    _view.addTask(_addDialog.resObject);
                }
            }
            onShowDetails: {
                _view.visible = false;
                _view.currentIndex = id;
                _headerLoader.sourceComponent = _header;
                _headerLoader.item.showedText = currentItem.item.taskname;
                _detailsLoader.sourceComponent = _iterDetailsShowItem;
                _detailsLoader.item.details.taskname = currentItem.item.taskname;
                _detailsLoader.item.details.tasktype = currentItem.item.tasktype;
                _detailsLoader.item.details.compliteCount = currentItem.item.totalCount;
                _detailsLoader.item.details.firstLevelScore = currentItem.item.levelUpScore;
                _detailsLoader.item.details.level = currentItem.item.currentLevel;
                _detailsLoader.item.details.series = currentItem.item.currentSeries;
                _detailsLoader.item.details.maxSeries = currentItem.item.maxSeries;
                _detailsLoader.item.details.seriesTimeStep = currentItem.item.timeStep;
                _detailsLoader.item.details.id = currentIndex;
                _detailsLoader.item.details.isMidnight = currentItem.item.isRoundBeginTime;
            }
        }


        Component{
            id:_iterDetailsShowItem
            Rectangle {
                property alias details: _iterDetails
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                anchors.topMargin: _headerLoader.height
                IterDetailsItem{
                    id:_iterDetails
                    anchors.fill: parent
                    onSaveButtonClicked: {
                        _view.model.updateTask(taskObject,id)
                        root.closeClicked()
                    }
                }
            }
        }
        Component {
            id:_header
            ListsHeader{
                //height: Style.taskListHeaderHeight
                width: parent.width
                color: "black"
                Rectangle{
                    id: _exitbtn
                    height: Style.taskListHeaderHeight - 10
                    width: height
                    color: parent.color
                    Image {
                        id: _exitimg
                        source: "qrc:/images/whiteX.png"
                        height: parent.height
                        width: parent.width
                    }
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: Style.taskListHeaderCloseMargin

                    MouseArea{
                        id:_exitmarea
                        anchors.fill: parent
                        onClicked: {
                            root.closeClicked()
                        }
                    }
                }
            }
        }

        Loader{
            id:_detailsLoader
            anchors.fill: parent
            anchors.top: _headerLoader.bottom

        }
        Loader{
            id:_headerLoader
            anchors.top: parent.top
            width: parent.width
        }


    }

    onAddButtonClicked:
    {
        _view.selectModeDiactivate()
        _addDialog.open()
    }
    onSelectModeDiactivate: {
        _view.selectModeDiactivate()
    }

    AddButton {
        id: _cbuttons
        height: Style.mainFrameButtonsSize
        width: Style.mainFrameButtonsSize
        z:100
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: Style.mainFrameMargin/2
        onClicked: {
            addButtonClicked()
        }
    }

    MouseArea {
        anchors.left: parent.left
        anchors.right: _cbuttons.right
        anchors.bottom: parent.bottom
        height: Style.mainFrameButtonsSize
        onClicked: {
            _view.selectModeDiactivate()
        }
    }

    function closeClicked(){
        _headerLoader.sourceComponent = undefined
        _detailsLoader.sourceComponent = undefined
        _view.visible = true
    }

}

