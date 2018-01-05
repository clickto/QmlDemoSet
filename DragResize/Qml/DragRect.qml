import QtQuick 2.7

Rectangle {
    id: root
    property alias containsMouse: mouseArea.containsMouse
    signal posChange(int xOffset, int yOffset)
    implicitWidth: 12
    implicitHeight: 12
    color: "white"
    border.color: "black"
    border.width: 2
    property int posType: Qt.ArrowCursor
    readonly property int posLeftTop: Qt.SizeFDiagCursor
    readonly property int posLeftMid: Qt.SizeHorCursor
    readonly property int posLeftBottom: Qt.SizeBDiagCursor
    readonly property int posMidTop: Qt.SizeVerCursor
    readonly property int posMidBottom: Qt.SizeVerCursor
    readonly property int posRightTop: Qt.SizeBDiagCursor
    readonly property int posRightMid: Qt.SizeHorCursor
    readonly property int posRightBottom: Qt.SizeFDiagCursor
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        property int lastX: 0
        property int lastY: 0
        onContainsMouseChanged: {
            if (containsMouse) {
                cursorShape = posType;
            } else {
                cursorShape = Qt.ArrowCursor;
            }
        }
        onPressedChanged: {
            if (containsPress) {
                lastX = mouseX;
                lastY = mouseY;
            }
        }
        onPositionChanged: {
            if (pressed) {
                posChange(mouseX - lastX, mouseY - lastY)
            }
        }
    }
}
