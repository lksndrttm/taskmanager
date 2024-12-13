import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.15
import Qt.labs.platform 1.1

Flow {
    id: root
    property real buttonsHeight: 30
    property real buttonsWidth: 30
    property var textArea
    property var document
    property var fontDialog
    property var colorDialog

    spacing: 20

    Rectangle {
        id: border
        width: parent.width
        height: 5
        gradient: Gradient{
            GradientStop {
                position: 0.0
                color: "#FFF"
            }
            GradientStop {
                position: 1.0
                color: "#DFDFDF"
            }
        }
    }


    EditToolButton{
        id: formatText
        image: "qrc:/images/type.svg"
        height: root.buttonsHeight
        width: root.buttonsWidth

        action: () => {
            toolbarPopup.open();
        }
    }


    MessageDialog {
        id: errorDialog
    }

    Popup {
        id: toolbarPopup
        width: parent.width
        height: toolbar.implicitHeight
        y: parent.height - height
        closePolicy: Popup.CloseOnPressOutside
        padding: 0
        background: Rectangle{}

        ToolBar {
            id: toolbar
            //anchors.bottom: parent.bottom
            //anchors.left: parent.left
            //anchors.right: parent.right
            anchors.fill: parent

            Flow {
                id: flow
                width: parent.width

                Row {
                    id: editRow
                    ToolButton {
                        id: copyButton
                        text: "\uF0C5" // icon-docs
                        font.family: "fontello"
                        focusPolicy: Qt.TabFocus
                        enabled: textArea.selectedText
                        onClicked: textArea.copy()
                    }
                    ToolButton {
                        id: cutButton
                        text: "\uE802" // icon-scissors
                        font.family: "fontello"
                        focusPolicy: Qt.TabFocus
                        enabled: textArea.selectedText
                        onClicked: textArea.cut()
                    }
                    ToolButton {
                        id: pasteButton
                        text: "\uF0EA" // icon-paste
                        font.family: "fontello"
                        focusPolicy: Qt.TabFocus
                        enabled: textArea.canPaste
                        onClicked: textArea.paste()
                    }
                    ToolSeparator {
                        contentItem.visible: editRow.y === formatRow.y
                    }
                }

                Row {
                    id: formatRow
                    ToolButton {
                        id: boldButton
                        text: "\uE800" // icon-bold
                        font.family: "fontello"
                        focusPolicy: Qt.TabFocus
                        checkable: true
                        checked: document.bold
                        onClicked: document.bold = !document.bold
                    }
                    ToolButton {
                        id: italicButton
                        text: "\uE801" // icon-italic
                        font.family: "fontello"
                        focusPolicy: Qt.TabFocus
                        checkable: true
                        checked: document.italic
                        onClicked: document.italic = !document.italic
                    }
                    ToolButton {
                        id: underlineButton
                        text: "\uF0CD" // icon-underline
                        font.family: "fontello"
                        focusPolicy: Qt.TabFocus
                        checkable: true
                        checked: document.underline
                        onClicked: document.underline = !document.underline
                    }
                    ToolButton {
                        id: fontFamilyToolButton
                        text: qsTr("\uE808") // icon-font
                        font.family: "fontello"
                        font.bold: document.bold
                        font.italic: document.italic
                        font.underline: document.underline
                        onClicked: {
                            fontDialog.currentFont.family = document.fontFamily;
                            fontDialog.currentFont.pointSize = document.fontSize;
                            fontDialog.open();
                        }
                    }
                    ToolButton {
                        id: textColorButton
                        text: "\uF1FC" // icon-brush
                        font.family: "fontello"
                        focusPolicy: Qt.TabFocus
                        onClicked: colorDialog.open()

                        Rectangle {
                            width: aFontMetrics.width + 3
                            height: 2
                            color: document.textColor
                            parent: textColorButton.contentItem
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.baseline: parent.baseline
                            anchors.baselineOffset: 6

                            TextMetrics {
                                id: aFontMetrics
                                font: textColorButton.font
                                text: textColorButton.text
                            }
                        }
                    }
                    ToolSeparator {
                        contentItem.visible: formatRow.y === alignRow.y
                    }
                }

                Row {
                    id: alignRow
                    ToolButton {
                        id: alignLeftButton
                        text: "\uE803" // icon-align-left
                        font.family: "fontello"
                        focusPolicy: Qt.TabFocus
                        checkable: true
                        checked: document.alignment === Qt.AlignLeft
                        onClicked: document.alignment = Qt.AlignLeft
                    }
                    ToolButton {
                        id: alignCenterButton
                        text: "\uE804" // icon-align-center
                        font.family: "fontello"
                        focusPolicy: Qt.TabFocus
                        checkable: true
                        checked: document.alignment === Qt.AlignHCenter
                        onClicked: document.alignment = Qt.AlignHCenter
                    }
                    ToolButton {
                        id: alignRightButton
                        text: "\uE805" // icon-align-right
                        font.family: "fontello"
                        focusPolicy: Qt.TabFocus
                        checkable: true
                        checked: document.alignment === Qt.AlignRight
                        onClicked: document.alignment = Qt.AlignRight
                    }
                    ToolButton {
                        id: alignJustifyButton
                        text: "\uE806" // icon-align-justify
                        font.family: "fontello"
                        focusPolicy: Qt.TabFocus
                        checkable: true
                        checked: document.alignment === Qt.AlignJustify
                        onClicked: document.alignment = Qt.AlignJustify
                    }
                }
            }
        }

    }


//    NoteToolPopup{
//        id: formatPopup
//        width: parent.width
//        height: formatFlow.height + padding*2
//        x: 0
//        y: -height + root.height

//        Flow {
//            id: formatFlow
//            spacing: 10
//            width: parent.width

//            EditToolButton{
//                id: bold
//                image: "qrc:/images/type-bold.svg"
//                height: root.buttonsHeight
//                width: root.buttonsWidth

//                action: () => {
//                    if(editor.selectedText !== ""){
//                        editor.insert(editor.selectionStart,"<b>" + editor.selectedText +"<\\b>")
//                        editor.remove(editor.selectionStart, editor.selectionEnd)

//                        console.log(editor.text)

//                        //editor.font.bold = !editor.font.bold;
//                    }
//                }

//            }
//            EditToolButton{
//                id: stiked
//                image: "qrc:/images/type-strikethrough.svg"
//                height: root.buttonsHeight
//                width: root.buttonsWidth

//                action: () => {
//                    if(editor.selectedText !== ""){
//                        editor.insert(editor.selectionStart,"<s>" + editor.selectedText +"<//s>")
//                        editor.remove(editor.selectionStart, editor.selectionEnd)
//                    }
//                }

//            }

//            EditToolButton{
//                id: underlined
//                image: "qrc:/images/type-underline.svg"
//                height: root.buttonsHeight
//                width: root.buttonsWidth

//                action: () => {
//                    if(editor.selectedText !== ""){
//                        editor.insert(editor.selectionStart,"<u>" + editor.selectedText +"<//u>")
//                        editor.remove(editor.selectionStart, editor.selectionEnd)
//                    }
//                }

//            }
//            EditToolButton{
//                id: italic
//                image: "qrc:/images/type-italic.svg"
//                height: root.buttonsHeight
//                width: root.buttonsWidth

//                action: () => {
//                    if(editor.selectedText !== ""){
//                        editor.insert(editor.selectionStart,"<i>" + editor.selectedText +"<//i>")
//                        editor.remove(editor.selectionStart, editor.selectionEnd)
//                    }
//                }

//            }

//            Rectangle {//breaker
//                width: parent.width
//                height:1
//            }


//            EditToolButton{
//                id: addListUl
//                image: "qrc:/images/list-ul.svg"
//                height: root.buttonsHeight
//                width: root.buttonsWidth

//                action: () => {
//                    editor.insert(editor.cursorPosition,"<ul> <li> &nbsp; <//li> <//ul>")
//                    editor.cursorPosition = editor.cursorPosition-1
//                }

//            }
//            EditToolButton{
//                id: addListOl
//                image: "qrc:/images/list-ol.svg"
//                height: root.buttonsHeight
//                width: root.buttonsWidth

//                action: () => {
//                    editor.insert(editor.cursorPosition,"<ol> <li> &nbsp; <\li> </ol>")
//                    editor.cursorPosition = editor.cursorPosition-1
//                }

//            }
//            Rectangle {//breaker
//                width: parent.width
//                height:1
//            }

//            FontSizeSlider{
//                width: parent.width
//                height: 20
//                onSliderValChanged: {
//                    console.log("@@@@@@@@@@@@@@@@@@@@@@",editor.text.slice(editor.selectionStart,editor.selectionEnd), "@@@@@@@@@@@@@@@@@@@@@@@@@@")
//                    var plainSelected =  editor.getFormattedText(editor.selectionStart,editor.selectionEnd)
//                    //editor.insert(editor.selectionStart,"<font size='"+ String(sliderVal) +"'>" + plainSelected + "<//font>")
//                    editor.insert(editor.selectionStart, plainSelected)
//                    editor.remove(editor.selectionStart, editor.selectionEnd)
//                    console.log(editor.text)
//                }

//                function getCurrentFont() {

//                }
//            }

//        }
//    }

    EditToolButton{
        id: addImage
        image: "qrc:/images/image.svg"
        height: root.buttonsHeight
        width: root.buttonsWidth

        action: () => {
            imageDialog.open()
        }

        SelectImageDialog {
            id: imageDialog

            onAccepted: {
                textArea.insert(textArea.cursorPosition, "<br><img src='" + imageDialog.file + "' width='" + textArea.width + "'><br>")
            }
        }
    }
}
