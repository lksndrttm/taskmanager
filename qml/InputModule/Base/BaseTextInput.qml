import QtQuick 2.12
import QtQuick.Controls 2.12
import StyleSettings 1.0

Rectangle{
    id: root
    border.color: "black"
    border.width: 1
    width: 200
    height: 50

    property alias edit: textEdit
    property alias currentText: textEdit.text
    property alias currentDisplayText: textEdit.displayText
    property alias inputValidator: textEdit.validator
    property string labelText: ""
    signal inputTextEdited()

    TextInput {
        id: textEdit
        color: "#808080"
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.verticalCenter: parent.verticalCenter
        inputMethodHints: Qt.ImhNoPredictiveText
        font.pointSize: Style.detailsSubTaskFontSize
        verticalAlignment: TextInput.AlignVCenter
        //topPadding: 2
        validator: RegExpValidator {regExp: /[^_]+/}
        clip: true
        Text {
            text: root.labelText
            color: "#aaa"
            visible: !textEdit.displayText
            font.pointSize: Style.detailsSubTaskFontSize
            anchors.fill: parent
            font.italic: true
            verticalAlignment: Text.AlignVCenter
        }
        onEditingFinished: {
            root.inputTextEdited()
        }
//        onTextEdited: {
//            root.inputTextEdited()
//        }
    }
}
