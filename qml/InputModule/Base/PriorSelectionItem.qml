import QtQuick 2.12
import QtQuick.Controls 2.12
import StyleSettings 1.0

Rectangle {
    id:root
    property alias bgroup: _radioGroup
    property alias highRB: _high
    property alias midRB: _mid
    property alias lowRB: _low

    ButtonGroup {
        id: _radioGroup
        buttons: _brow.children
    }

    Row{
        id:_brow
        PriorRadioButton{
            id:_low
            buttonColor: Style.taskLowPriorColor
            text: qsTr("Low")
        }
        PriorRadioButton{
            id:_mid
            buttonColor: Style.taskMidPriorColor
            text: qsTr("Mid")
            checked: true
        }
        PriorRadioButton{
            id:_high
            buttonColor: Style.taskHighPriorColor
            text: qsTr("High")
        }
    }
}
