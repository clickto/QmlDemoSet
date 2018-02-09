import QtQuick 2.6
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.0
Rectangle {
    width: 800
    height: 480
    color: "gray"
    Item {
        id: arrowItem
        x: 10
        y: 10
        width: 300
        height: 300
        Arrow {
            id: arrow1
        }
        Arrow {
            id: arrow2
        }
        Arrow {
            id: arrow3
        }
        Arrow {
            id: arrow4
        }
        Arrow {
            id: arrow5
        }
        Arrow {
            id: arrow6
        }
        Arrow {
            id: arrow7
        }
        Arrow {
            id: arrow8
        }
        Arrow {
            id: arrow9
        }
    }
    Item {
        id: mirrorItem
        x: arrowItem.x
        y: arrowItem.y + arrowItem.height
        width: arrowItem.width
        height: arrowItem.height
        opacity: 0.3
        layer.enabled: true
        layer.effect: Component {
            ShaderEffectSource {
                sourceItem: arrowItem
                textureMirroring: ShaderEffectSource.MirrorVertically
            }
        }
        transform: Rotation {
            origin.x: mirrorItem.width / 2
            origin.y: mirrorItem.height / 2
            axis {x: 1; y: 0; z: 0}
            angle: 180
        }
    }
    Component.onCompleted: {
        seAnimation.start()
    }
    SequentialAnimation {
        id: seAnimation
        ScriptAction {script: arrow1.run()}
        PauseAnimation {duration: 100 }
        ScriptAction {script: arrow2.run()}
        PauseAnimation {duration: 100 }
        ScriptAction {script: arrow3.run()}

        PauseAnimation {duration: 500 }

        ScriptAction {script: arrow4.run()}
        PauseAnimation {duration: 100 }
        ScriptAction {script: arrow5.run()}
        PauseAnimation {duration: 100 }
        ScriptAction {script: arrow6.run()}

        PauseAnimation {duration: 500 }

        ScriptAction {script: arrow7.run()}
        PauseAnimation {duration: 100 }
        ScriptAction {script: arrow8.run()}
        PauseAnimation {duration: 100 }
        ScriptAction {script: arrow9.run()}
        PauseAnimation {duration: 100 }
    }
}
