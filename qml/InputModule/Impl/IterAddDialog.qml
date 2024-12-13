import QtQuick 2.12
import QtQuick.Controls 2.12
import InputModule.Base 1.0
import StyleSettings 1.0

BaseAddDialog {
    id: _popup
    signal addButtonClicked()
    property string taskType: _typebox.currentText
    property string taskname: _input.currentDisplayText
    property var resObject : {"name":"","type":"","firstLevelScore":"","seriesTimeStep":""}
    contentItem: Rectangle {
        id: _content
        height: Style.addDialogHeight
        width: parent.width


        BaseTextInput{
            id: _input
            labelText: qsTr("iterator name")
            height: Style.addDialogButtonsHeight
            anchors.right: _typebox.left
            anchors.left: parent.left
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 5
            radius: 2

        }

        ComboBox{
            id:_typebox
            model: [qsTr("counter"),qsTr("series")]
            currentIndex: 0
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.top: _input.top
            height: Style.addDialogButtonsHeight
            width: Style.addDialogComboBoxWidth
        }
        BaseTextInput{
            id:_timeStepBox
            anchors.left: parent.left
            anchors.top: _input.bottom
            anchors.topMargin: 5
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.right: _stepMult.left
            height: Style.addDialogButtonsHeight
            labelText: qsTr("execution period")
            visible: _popup.taskType === "series"
            radius: 2
            inputValidator: RegExpValidator {regExp: /[0-9]+/}
        }
        ComboBox{
            id:_stepMult
            model: [qsTr("days"),qsTr("months")]
            anchors.right: parent.right
            anchors.top: _input.bottom
            anchors.rightMargin: 5
            anchors.topMargin: 5
            width: Style.addDialogComboBoxWidth
            height: Style.addDialogButtonsHeight
            visible: _popup.taskType === "series"
        }

        BaseTextInput{
            id:_firstLevelScore
            labelText: qsTr("score to new level")
            height: Style.addDialogButtonsHeight
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.top: _popup.taskType === "series" ? _timeStepBox.bottom : _input.bottom
            anchors.topMargin: 5
            radius: 2
            inputValidator: RegExpValidator {regExp: /[0-9]+/}
        }
        BaseCheckBox{
            id:_isBeginAtMidnight
            text: qsTr("From midnight")
            buttonColor: "#181818"
            //width: Style.addDialogButtonsHeight - 5
            //height: Style.addDialogButtonsHeight - 5
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.top: _firstLevelScore.bottom
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            visible: false//_popup.taskType === "series"
        }

        Button{
            id: _acceptButton
            text: qsTr("add")
            height: Style.addDialogButtonsHeight
            width: Style.addDialogButtonsWidth
            background: Rectangle {
                anchors.fill: parent
                border.color: "#3A3A3A"
                border.width: 1
                radius: 2
                color: _addArea.containsMouse ? "#3A3A3A" : "white"
            }
            contentItem: Text {
                text: _acceptButton.text
                font: _acceptButton.font
                //opacity: enabled ? 1.0 : 0.3
                color: _addArea.containsMouse ? "white" : "#3A3A3A"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }


            anchors.right: parent.right
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            //anchors.top: _popup.taskType === "series" ? _isBeginAtMidnight.bottom : _firstLevelScore.bottom
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            MouseArea{
                id: _addArea
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    Qt.inputMethod.hide()// testcode
                    resObject["name"] = _popup.taskname
                    resObject["type"] = _popup.taskType
                    resObject["firstLevelScore"] = _firstLevelScore.currentDisplayText
                    if(_popup.taskType === "series"){
                        var stepmult = _stepMult.currentText === "hours" ? 1 : _stepMult.currentText === "days" ? 24 : 744
                        resObject["seriesTimeStep"] = (+_timeStepBox.currentDisplayText) * stepmult + ""
                        resObject["isMidnight"] = "true"//_isBeginAtMidnight.checked ? "true" : "false"
                    }
                    else{
                        resObject["seriesTimeStep"] = ""
                    }

                    _popup.addButtonClicked()
                    _popup.close()
                }

            }
        }
        focus: true
        Keys.onEscapePressed: {
            _popup.close()
        }
    }
}
