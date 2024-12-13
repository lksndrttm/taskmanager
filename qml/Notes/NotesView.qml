import QtQuick 2.12

Flickable{
   id: root
   property var selectionController
   property alias model: rep1.model
   property alias repeater: rep1
   clip: true;
   contentHeight: rep1.count ? rep1.itemAt(rep1.count-1).y - rep1.itemAt(0).y + 100 + rep1.itemAt(rep1.count-1).height : 0

    Flow {
        anchors.fill: parent
        spacing: 10
        Repeater {
            id:rep1

            model: notesModel.notes
            height: children.implicitHeight
            delegate:  Rectangle{
                id:delRoot
                property bool selected: false
                height: img.height + txt.implicitHeight + 20
                width: 165

                ////implicitHeight: img.implicitHeight + txt.implicitHeight
                //anchors.left: (index%2 && index !== 0) ? rep1.itemAt(index-1).right : parent.left
                //anchors.top: index < 2 ? parent.top : rep1.itemAt(index-2).bottom
                //anchors.topMargin: 10
                //anchors.leftMargin: 10

                Rectangle {
                    id:inerDelegat
                    //            height: 150
                    //            width: parent.width
                    anchors.fill: parent
                    anchors.topMargin: 5
                    anchors.leftMargin: 5
                    anchors.bottomMargin: 5
                    anchors.rightMargin: 5

                    color: "transparent"
                    border.width: 1
                    border.color: "#ccc"
                    radius: 6
                    clip: true
                Image {
                    id: img
                    anchors.top: parent.top
                    anchors.topMargin: 1
                    anchors.horizontalCenter: parent.horizontalCenter
                    //                anchors.verticalCenter: parent.verticalCenter
                    fillMode: Image.PreserveAspectCrop
                    source:  notesModel.notes[index].image //rep1.model[index].image
                    height: 120
                    width: parent.width - 2
                    sourceSize.width: 1000
                    sourceSize.height: 1000
                    //sourceClipRect: Qt.rect(100, 100, 512, 512)
                }
                Text {
                    id: txt
                    text: notesModel.notes[index].title
                    width: parent.width
                    anchors.top: img.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "black"
                    font.pointSize: 13
                    font.family: "sans-serif"
                    clip: true
                    wrapMode: Text.Wrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                }
                MouseArea {
                    id:delegateArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        if(!root.selectionController.selectMode){
                            editPopup.currentIndex = index
                            editPopup.open()
                        } else {
                            if(root.selectionController.selectedElements.find(element => element === index) === undefined){
                                root.selectionController.selectedElements.push(index)
                                delRoot.selected = true
                            } else {
                                root.selectionController.selectedElements.splice(root.selectionController.selectedElements.indexOf(index),1)
                                delRoot.selected = false
                            }
                        }
                        root.selectionController.countOfSelected = root.selectionController.selectedElements.length
                    }
                    onEntered: {
                        if(delRoot.selected)
                            return
                        txt.color = "white"
                        inerDelegat.color = "black"

                    }
                    onExited: {
                        if(delRoot.selected)
                            return
                        txt.color = "black"
                        inerDelegat.color = "transparent"
                    }

                    onPressAndHold: {
                        root.selectionController.selectMode = true
                        root.selectionController.selectedElements.push(index)
                        delRoot.selected = true
                        root.selectionController.countOfSelected = root.selectionController.selectedElements.length
                    }

                }

                Rectangle {
                    id:selectionMark
                    height: 15
                    width: height
                    x:10
                    y: parent.height - height - 10
                    border.width: 1
                    border.color: "#222222"
                    visible: root.selectionController.selectMode
                    radius: 4

                    Rectangle{
                        anchors.fill: parent
                        color: "white"
                        radius: 4
                        border.width: 1
                        border.color: "#222222"
                        Image {
                            id: checkImg
                            source: "qrc:/images/check-square.svg"
                            sourceSize.width: 100
                            sourceSize.height: 100
                            height: parent.height
                            width: height
                            visible: delRoot.selected
                        }
                    }
                }
                onSelectedChanged: {
                    if(delRoot.selected){
                        txt.color = "white"
                        inerDelegat.color = "black"
                    } else {
                        txt.color = "black"
                        inerDelegat.color = "transparent"
                    }
                }
            }

           onItemRemoved: {
               var tempItem = item;
               for(var i = 0; i < rep1.count; i++){
                   var nextItem = rep1.itemAt(i);
                   nextItem.anchors.left = (i%2 && i !== 0) ? rep1.itemAt(i-1).right : parent.left;
                   nextItem.anchors.top = i < 2 ? parent.top : rep1.itemAt(i-2).bottom;
                   tempItem = nextItem;
               }
           }
        }

   }
}
