import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: root
    property alias sliderVal: fontSlider.value
    Row {
        width: parent.width
        height: parent.height
        spacing: 5

        Text {
            id: fontSizeText
            text: fontSlider.value
        }

        Slider {
            id: fontSlider
            from: 8
            to: 30
            stepSize: 1
            height: parent.height
            width: parent.width - fontSizeText.implicitWidth - spacing
        }
    }
}
