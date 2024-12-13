import QtQuick 2.12
import QtQuick.Controls 2.12
import StyleSettings 1.0

Rectangle{
    id: root
    border.color: "black"
    border.width: 1


    property string currentText: textEdit.text
    property string labelText: ""
    radius: 2
    Flickable {
           id: flick

           anchors.fill: parent
           contentWidth: textEdit.paintedWidth
           contentHeight: textEdit.paintedHeight
           clip: true

           function ensureVisible(r)
           {
               if (contentX >= r.x)
                   contentX = r.x;
               else if (contentX+width <= r.x+r.width)
                   contentX = r.x+r.width-width;
               if (contentY >= r.y)
                   contentY = r.y;
               else if (contentY+height <= r.y+r.height)
                   contentY = r.y+r.height-height;
           }

        TextEdit {
            id: textEdit
            focus: true
            width: flick.width
            anchors.leftMargin: 5
            font.pointSize: 12
            topPadding: 2
            wrapMode: TextEdit.Wrap
            clip: true
            selectionColor: "#555"
            onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)
            //inputMethodHints: Qt.ImhNoPredictiveText
            Text {
                text: root.labelText
                color: "#aaa"
                visible: !textEdit.text
                font.pointSize: 13
                anchors.fill: parent
            }
        }
    }
}
