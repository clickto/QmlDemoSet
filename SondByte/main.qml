import QtQuick 2.6
import QtQuick.Controls 2.0
import Soundbyte 1.0

Rectangle {
    id: root
    width: 800
    height: 600
    color: "black"
    property bool isRun: false
    Button {
        text: isRun ? "Stop" : "Start"
        onClicked: {
            isRun = !isRun;
            if (isRun) {
                soundbyte.show();
            } else {
                soundbyte.hide();
            }
        }
    }
    Item {
        anchors.centerIn: parent
        width: 417
        height: 264
        Soundbyte {
            id: soundbyte
            anchors.fill: parent
        }
    }
}
