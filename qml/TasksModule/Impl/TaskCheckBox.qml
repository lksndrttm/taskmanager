import QtQuick 2.12
import QtQuick.Controls 2.12
import StyleSettings 1.0

CheckBox {
  id: control
  signal checkBoxToggled(bool state)
  property color checkBoxColor: Style.taskCheckBoxColor

  indicator: Rectangle {
//      width: parent.width
//      height: parent.height
//      anchors.centerIn: parent
      anchors.fill: parent
      border.color: control.checkBoxColor
      color: "white"
      radius: 2
      Rectangle {
          anchors.top: parent.top
          anchors.bottom: parent.bottom
          anchors.left: parent.left
          anchors.right: parent.right

          anchors.topMargin: 2
          anchors.bottomMargin: 2
          anchors.rightMargin: 2
          anchors.leftMargin: 2
          color: control.checkBoxColor
          visible: control.checked
          radius: 2
      }
  }

  onToggled: {checkBoxToggled(control.checked)}

}
