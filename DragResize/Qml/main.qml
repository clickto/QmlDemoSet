import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello Drag")

    Rectangle {
        color: "red"
        x: 100
        y: 100
        width: 120
        height: 240
        Border {
        }
    }
}
