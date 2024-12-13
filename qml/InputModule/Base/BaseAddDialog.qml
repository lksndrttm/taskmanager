import QtQuick 2.0
import QtQuick.Controls 2.12
import StyleSettings 1.0

Popup{
    id: _popup
    padding: 1
    focus: true
    modal: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

//                x: Math.round((parent.width - width) / 2)
//                y: Math.round((parent.height - height))

    background:  Rectangle{
        anchors.fill: parent
        color: "black"
    }

}
