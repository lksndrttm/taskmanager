import QtQuick 2.12
import QtQuick.Controls 2.12


ProgressBar {
    id:root
    background: Rectangle {
        height: parent.height
        width: parent.width
        radius: 2
        color: "#D3D3D3"
    }
    contentItem: Item{
        height: parent.height
        width: parent.width
        Rectangle {
            height: parent.height
            width: root.value * root.width
            radius: 2
            color: "black"
            border.width: 0.1
            border.color: "#D3D3D3"
        }
    }
}
