import QtQuick 2.12

Rectangle{
    id: root
    height: 50


    Rectangle{
        id:searchrec
        height: 30
        //width: parent.width - 30
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        radius: height/2
        border.width: 1
        border.color: "#000"
        anchors.verticalCenter: parent.verticalCenter
//            anchors.horizontalCenter: parent.horizontalCenter
        clip: true
        Image {
            id: searchImg
            source: "qrc:/images/search.svg"
            height: parent.height-12
            width: height
            anchors.left: parent.left
            anchors.leftMargin: 15
            anchors.verticalCenter: parent.verticalCenter
            sourceSize.height: 100
            sourceSize.width: 100
        }
        TextInput{
            id:search
            anchors.fill: parent
            font.pointSize: 15
            clip: true
            maximumLength: 50
            padding: 3
            //leftPadding: 15 + searchImg.width + 5
            anchors.leftMargin: 15 + searchImg.width + 5
            rightPadding: 10
        }
    }


}

