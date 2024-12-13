import QtQuick 2.12
import QtQuick.Controls 2.12
import InputModule.Base 1.0
import InputModule.Impl 1.0
import StyleSettings 1.0

Rectangle {
    id:root

    signal saveButtonClicked(var taskObject, int id)
    signal showTask(var task)
    signal showSubTasks(var subTasksList)


    property string taskname: ""
    property string tasktype: ""
    property string compliteCount: ""
    property string firstLevelScore: ""
    property string level: ""
    property string series: ""
    property string createTime: ""
    property string maxSeries: ""
    property string seriesTimeStep: ""
    property string isMidnight: ""
    property int  id: -1



    border.color: "black"
    border.width: 1

    Text {
        id: _name
        text: qsTr("Name:")
        anchors.left: parent.left
        anchors.top: parent.top
        font.pointSize: 10
        color: "#9E9E9E"
        anchors.leftMargin: 10
        anchors.topMargin: 5
    }

    BaseTextInput{
        id:_nameInput
        currentText: root.taskname
        height: Style.addDialogButtonsHeight
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: _name.bottom
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.topMargin: 5

    }


    Text {
        id: _countBox
        text: qsTr("total count:")
        anchors.left: parent.left
        anchors.top: _nameInput.bottom
        font.pointSize: 10
        color: "#9E9E9E"
        anchors.leftMargin: 10
        anchors.topMargin: 5
    }
    Text {
        id: _seriesBox
        text: qsTr("current series:")
        anchors.left: parent.horizontalCenter
        anchors.top: _nameInput.bottom
        font.pointSize: 10
        color: "#9E9E9E"
        anchors.leftMargin: 10
        anchors.topMargin: 5
        visible: root.tasktype === "series"
    }
    BaseTextInput{
        id:_compliteCount
        currentText: root.compliteCount
        height: Style.addDialogButtonsHeight
        anchors.right: parent.horizontalCenter
        anchors.left: parent.left
        anchors.top: _countBox.bottom
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.topMargin: 5
        onInputTextEdited: {
            if((+_levelUpInput.currentDisplayText) <= 0){
                _levelUpInput.currentText = "1"
            }
            if(root.tasktype === "counter"){
                var currentCount = 0
                if(currentDisplayText.length !== 0){
                    currentCount = (+currentDisplayText)
                }
                _levelInput.currentText = Math.floor(currentCount/(+_levelUpInput.currentDisplayText))
            }
            else if(root.tasktype === "series"){
                if((+currentDisplayText) > 0 && +_maxSeriesInput.currentDisplayText === 0){
                    _maxSeriesInput.currentText = "1"
                } else  if((+currentDisplayText) === 0){
                    _maxSeriesInput.currentText = "0"
                }
            }
        }

    }
    BaseTextInput{
        id:_series
        currentText: root.series
        height: Style.addDialogButtonsHeight
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter
        anchors.top: _countBox.bottom
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.topMargin: 5
        visible: root.tasktype === "series"
        onInputTextEdited: {
            if((+currentDisplayText) > (+_maxSeriesInput.currentDisplayText)){
                _maxSeriesInput.currentText = currentDisplayText
                if(Math.floor(+currentDisplayText/+_levelUpInput.currentDisplayText) !== +_levelInput.currentDisplayText){
                    _levelInput.currentText = Math.floor(+currentDisplayText/+_levelUpInput.currentDisplayText) + ""
                }
                if(+_compliteCount.currentDisplayText < +currentDisplayText){
                    _compliteCount.currentText = currentDisplayText
                }
            }
        }

    }


    Text {
        id: _level
        text: qsTr("level:")
        anchors.left: parent.left
        anchors.top: _series.bottom
        font.pointSize: 10
        color: "#9E9E9E"
        anchors.leftMargin: 10
        anchors.topMargin: 5
    }
    Text {
        id: _levelUpScore
        text: qsTr("Level up score:")
        anchors.left: parent.horizontalCenter
        anchors.top: _series.bottom
        font.pointSize: 10
        color: "#9E9E9E"
        anchors.leftMargin: 10
        anchors.topMargin: 5
    }
    BaseTextInput{
        id:_levelInput
        currentText: root.level
        height: Style.addDialogButtonsHeight
        anchors.right: parent.horizontalCenter
        anchors.left: parent.left
        anchors.top: _level.bottom
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.topMargin: 5
        onInputTextEdited: {
            if(root.tasktype === "counter"){
                _compliteCount.currentText = (+currentDisplayText)*(+_levelUpInput.currentDisplayText)
            }
            else if(root.tasktype === "series"){
                if(+currentDisplayText > +_maxSeriesInput.currentDisplayText/+_levelUpInput.currentDisplayText){
                    if(_maxSeriesInput.currentDisplayText < _levelUpInput.currentDisplayText){
                        _maxSeriesInput.currentText = _levelUpInput.currentDisplayText//+currentDisplayText * +_levelUpInput.currentDisplayText
                    }
                    if(+_compliteCount.currentDisplayText < +currentDisplayText * +_levelUpInput.currentDisplayText){
                        _compliteCount.currentText = +currentDisplayText * +_levelUpInput.currentDisplayText
                    }
                    if(+_compliteCount.currentDisplayText !== 0 && +_levelInput.currentDisplayText === 0){
                        _maxSeriesInput.currentText = "1"
                    }
                }
            }
        }

    }
    BaseTextInput{
        id:_levelUpInput
        currentText: root.firstLevelScore
        height: Style.addDialogButtonsHeight
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter
        anchors.top: _level.bottom
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.topMargin: 5
        onInputTextEdited: {
            if((+_levelUpInput.currentDisplayText) <= 0){
                _levelUpInput.currentText = "1"
            }

            if(root.tasktype === "counter"){
                //_compliteCount.currentText = (+currentDisplayText)*(+_levelUpInput.currentDisplayText)
                _levelInput.currentText = Math.floor(_compliteCount.currentDisplayText/(+_levelUpInput.currentDisplayText))
            }
        }

    }

    Text {
        id: _maxSeries
        text: qsTr("maxSeries:")
        anchors.left: parent.left
        anchors.top: _levelInput.bottom
        font.pointSize: 10
        color: "#9E9E9E"
        anchors.leftMargin: 10
        anchors.topMargin: 5
        visible: root.tasktype === "series"
    }
    BaseTextInput{
        id:_maxSeriesInput
        currentText: root.maxSeries
        height: Style.addDialogButtonsHeight
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: _maxSeries.bottom
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.topMargin: 5
        visible: root.tasktype === "series"
        onInputTextEdited: {
            if((+currentDisplayText)/+_levelUpInput.currentDisplayText > +_levelInput.currentDisplayText){
                _levelInput.currentText = Math.floor((+currentDisplayText)/+_levelUpInput.currentDisplayText)
            }
            if(+currentDisplayText > +_compliteCount.currentDisplayText){
                _compliteCount.currentText = currentDisplayText
            }
        }

    }

    Text {
        id: _timeStep
        text: qsTr("Series time step:")
        anchors.left: parent.left
        anchors.top: _maxSeriesInput.bottom
        font.pointSize: 10
        color: "#9E9E9E"
        anchors.leftMargin: 10
        anchors.topMargin: 5
        visible: root.tasktype === "series"
    }
    BaseTextInput{
        id:_timeStepInput
        currentText: root.seriesTimeStep
        height: Style.addDialogButtonsHeight
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: _timeStep.bottom
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.topMargin: 5
        visible: root.tasktype === "series"

    }

    BaseCheckBox{
        id:_isBeginAtMidnight
        text: qsTr("Round time: ")
        buttonColor: "#181818"
        checked: root.isMidnight == "true"
        //width: Style.addDialogButtonsHeight - 5
        //height: Style.addDialogButtonsHeight - 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.top: _timeStepInput.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false//root.tasktype === "series"
    }

    Text {
        id: _typeBox
        text: root.tasktype
        anchors.right: parent.right
//        anchors.rightMargin: 10
//        anchors.top: _priorItem.bottom
//        anchors.topMargin: 10
        anchors.bottom: _saveButton.top
        anchors.bottomMargin: 5
        anchors.rightMargin: 10
        font.pointSize: 10
        //height: 10
        color: "#9E9E9E"
    }





     Rectangle{
        id: _saveButton
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.bottomMargin: 5
        height: 25
        radius: 2
        color: "#9E9E9E"

        Text {
            id: _btext
            text: qsTr("Save")
            anchors.centerIn: parent
            color: "white"
            font.bold: true
        }

        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                _saveButton.color = "#3a3a3a"
            }
            onExited: {
                _saveButton.color = "#9E9E9E"
            }
            onPressed: {
                _saveButton.color = "black"
            }
            onReleased: {
                _saveButton.color = "#3a3a3a"
            }

            onClicked: {
                var task = {}
                task["name"] = _nameInput.currentDisplayText
                task["type"] = root.tasktype
                task["compliteCount"] = _compliteCount.currentDisplayText
                task["firstLevelScore"] = _levelUpInput.currentDisplayText
                task["level"] = _levelInput.currentDisplayText
                task["series"] = _series.currentDisplayText
                task["maxSeries"] = _maxSeriesInput.currentDisplayText
                task["seriesTimeStep"] = _timeStepInput.currentDisplayText
                task["isMidnight"] = "true"//_isBeginAtMidnight.checked ? "true" : "false"

                root.saveButtonClicked(task, root.id)
            }
        }
    }



     onShowSubTasks: {
         _priorSelect.modelInited(subTasksList)
     }

}
