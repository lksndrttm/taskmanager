import QtQuick 2.12
import QtQuick.Controls 2.12
import InputModule.Impl 1.0
import StyleSettings 1.0

Rectangle {
    id:root
    signal subTargetAdd(string subTaskName, string subTaskPrior)
    property var itemIndex
    property alias prior: _prior.prior
    property string currentPrior: _prior.currentPrior
    property string currentNameText: _prior.currentTaskName
    property alias taskName: _prior.inputText
    property bool isInitialized: false
    signal initializationItem(string name, string prior, var index)
    signal subTargetDelete(var index)
    signal getSaveData(string name, string prior, var index)
    //signal subTargetNameEdited(string name, pri,var index)

    SubTargetAddItem{
        id:_prior
        //prior: root.prior
        //taskName: root.taskName
        anchors.right: _deleteBtn.left
        anchors.left: parent.left
        anchors.rightMargin: 5
        height : parent.height
//        onTextEdited: {
//            subTargetNameEdited(_prior.taskName,root.itemIndex)
//        }
    }

    Rectangle{
        id: _deleteBtn
        visible: root.isInitialized
        height: parent.height - 5
        width: height
        color: parent.color
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 2
        Image {
            id: _exitimg
            source: "qrc:/images/notcomplite.png"
            height: parent.height
            width: parent.width
        }

        MouseArea{
            id:_exitmarea
            anchors.fill: parent
            onClicked: {
                root.subTargetDelete(root.itemIndex)
            }
        }
    }

    Rectangle{
        id: _addBtn
        visible: !root.isInitialized
        height: parent.height - 5
        width: height
        color: parent.color
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 2
        Image {
            id: _addimg
            source: "qrc:/images/plus2.png"
            height: parent.height
            width: parent.width
        }

        MouseArea{
            id:_addmarea
            anchors.fill: parent
            onClicked: {
                if(_prior.currentTaskName.length > 0){
                    root.subTargetAdd(_prior.currentTaskName, _prior.currentPrior)
                    _prior.inputText = ""
                }
            }
        }
    }


    onInitializationItem: {
        root.itemIndex = index
        root.prior = prior
        root.taskName = name
        root.isInitialized = true
    }
}
