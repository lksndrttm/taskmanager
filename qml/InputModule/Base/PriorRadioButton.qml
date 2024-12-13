import QtQuick 2.12
import QtQuick.Controls 2.12
import StyleSettings 1.0

RadioButton {
    id: control
    property color buttonColor: Style.midPriorTaskColor


    indicator: Rectangle {
    implicitWidth: 26
    implicitHeight: 26
    x: control.leftPadding
    y: parent.height / 2 - height / 2
    border.color: control.buttonColor

    Rectangle {
      width: 14
      height: 14
      x: 6
      y: 6
      color: control.buttonColor
      visible: control.checked
    }
    }

    contentItem: Text {
      text: control.text
      font: control.font
      opacity: enabled ? 1.0 : 0.3
      color: control.buttonColor
      verticalAlignment: Text.AlignVCenter
      leftPadding: control.indicator.width + control.spacing
    }
}

