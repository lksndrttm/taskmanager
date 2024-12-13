import QtQuick 2.12
import QtQuick.Controls 2.12

ScrollView {
    id: root
    property alias textArea: area

    TextArea {
        id: area

        clip: true
        textFormat: TextEdit.RichText
        selectByMouse: true
        persistentSelection: true

        text: editPopup.currentIndex > -1 ? notesFlick.model[editPopup.currentIndex].content : ""
        wrapMode: TextEdit.Wrap


    }

}
