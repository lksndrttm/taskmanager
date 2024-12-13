import QtQuick 2.12
import Qt.labs.platform 1.1

FileDialog {
    id: root
    folder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)
    nameFilters: ["Images (*.jpg *.png *.jpeg)"]
}
