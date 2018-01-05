import QtQuick 2.7

Rectangle {
    color: "transparent"
    border.width: 4
    border.color: "black"
    width: parent.width
    height: parent.height
    property Item controller: parent
    MouseArea {
        anchors.fill: parent
        anchors.margins: 4
        hoverEnabled: true
        property int lastX: 0
        property int lastY: 0
        onContainsMouseChanged: {
            if (containsMouse) {
                cursorShape = Qt.SizeAllCursor;
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
                controller.x +=mouseX - lastX;
                controller.y +=mouseY - lastY;
            }
        }
    }
    DragRect {
        posType: posLeftTop
        onPosChange: {
            //不要简化这个判断条件，至少让以后维护的人能看懂。我当然知道能化简，化简过几分钟之后我自己都看不懂了。
            if (controller.x + xOffset < controller.x + controller.width)
                controller.x += xOffset;
            if (controller.y + yOffset < controller.y + controller.height)
                controller.y += yOffset;
            if (controller.width - xOffset > 0)
                controller.width-= xOffset;
            if (controller.height -yOffset > 0)
                controller.height -= yOffset;
        }
    }
    DragRect {
        posType: posMidTop
        x: (parent.width - width) / 2
        onPosChange: {
            if (controller.y + yOffset < controller.y + controller.height)
                controller.y += yOffset;
            if (controller.height - yOffset > 0)
                controller.height -= yOffset;
        }
    }
    DragRect {
        posType: posRightTop
        x: parent.width - width
        onPosChange: {
            //向左拖动时，xOffset为负数
            if (controller.width + xOffset > 0)
                controller.width += xOffset;
            if (controller.height - yOffset > 0)
                controller.height -= yOffset;
            if (controller.y + yOffset < controller.y + controller.height)
                controller.y += yOffset;
        }
    }
    DragRect {
        posType: posLeftMid
        y: (parent.height - height) / 2
        onPosChange: {
            if (controller.x + xOffset < controller.x + controller.width)
                controller.x += xOffset;
            if (controller.width - xOffset > 0)
                controller.width-= xOffset;
        }
    }
    DragRect {
        posType: posRightMid
        x: parent.width - width
        y: (parent.height - height) / 2
        onPosChange: {
            if (controller.width + xOffset > 0)
                controller.width += xOffset;
        }
    }
    DragRect {
        posType: posLeftBottom
        y: parent.height - height
        onPosChange: {
            if (controller.x + xOffset < controller.x + controller.width)
                controller.x += xOffset;
            if (controller.width - xOffset > 0)
                controller.width-= xOffset;
            if (controller.height + yOffset > 0)
                controller.height += yOffset;
        }
    }
    DragRect {
        posType: posMidBottom
        x: (parent.width - width) / 2
        y: parent.height - height
        onPosChange: {
            if (controller.height + yOffset > 0)
                controller.height += yOffset;
        }
    }
    DragRect {
        posType: posRightBottom
        x: parent.width - width
        y: parent.height - height
        onPosChange: {
            if (controller.width + xOffset > 0)
                controller.width += xOffset;
            if (controller.height + yOffset > 0)
                controller.height += yOffset;
        }
    }

}
