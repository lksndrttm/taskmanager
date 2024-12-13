import QtQuick 2.12
import QtQuick.Controls 2.12
import StyleSettings 1.0

Rectangle {
      id: root
      property color color0: Style.taskHighPriorColor
      property color color1: Style.taskMidPriorColor
      property color color2: Style.taskLowPriorColor

      Popup {
          id:_popup
          width: parent.width
          height: root.height * 3
          contentWidth: parent.width
          contentHeight: root.height * 3
          y: (-root.height * 2)
          x:0
          topMargin: 0
          bottomMargin: 0
          topInset: 0
          bottomInset: 0
          padding: 0
          Column {
              anchors.fill: parent
              Rectangle{
                  id:_col0
                  height: root.height
                  width: _popup.width
                  color: root.color0
                  MouseArea{
                      anchors.fill: parent
                      onClicked: {
                          root.color = parent.color
                          _popup.close()
                      }
                  }
              }
              Rectangle{
                  id:_col1
                  height: root.height
                  width: _popup.width
                  color: root.color1
                  MouseArea{
                      anchors.fill: parent
                      onClicked: {
                          root.color = parent.color
                          _popup.close()
                      }
                  }
              }
              Rectangle{
                  id:_col2
                  height: root.height
                  width: _popup.width
                  color: root.color2
                  MouseArea{
                      anchors.fill: parent
                      onClicked: {
                          root.color = parent.color
                          _popup.close()
                      }
                  }
              }
          }
      }


      MouseArea {
          anchors.fill: parent
          onClicked: {
              _popup.open()
          }
      }


  }
