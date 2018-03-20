import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Extras 1.4
import BackCarLine 1.0
import I18n 1.0
ApplicationWindow {
    visible: true
    width: 1024
    height: 600
    title: qsTr("BackCar")
    Image {
        id: backGroundImage
        //16 ： 9
        width: 600
        height: 337.5
        source: "qrc:/Image/backGround.jpg"
        anchors.centerIn: parent
        BackCarLine{
            id: backCarLine
            width: backGroundImage.width
            height: backGroundImage.height
            anchors.fill: parent
        }
    }
    I18n{
        id: trans
    }
    Column {
        anchors.left: parent.left
        spacing: 3
        Repeater {
            model: ListModel {
                ListElement {name: "slantAngle"}
                ListElement {name: "steerAngleLimit"}
                ListElement {name: "heightRatioBase"}
                ListElement {name: "heightRatioDanger"}
                ListElement {name: "heightRatioWarn"}
                ListElement {name: "heightRatioSafe"}
                ListElement {name: "widthRatioBase"}
                ListElement {name: "safeLineWidthRatio"}
                ListElement {name: "warnLineWidthRatio"}
                ListElement {name: "dangerLineWidthRatio"}
                ListElement {name: "safeCurveWidthRatio"}
                ListElement {name: "warnCurveWidthRatio"}
                ListElement {name: "dangerCurveWidthRatio"}
            }
            Row {
                spacing: 5
                Label {
                    text: trans.tr(model.name) + trans.driver
                    onTextChanged: {
                        console.log(text)
                    }
                    width: 130
                }
                SpinBox {
                    width: 100
                    minimumValue: 0
                    maximumValue: {
                        if (model.name == "slantAngle" || model.name == "spacing")
                            return 90;
                        else if (model.name == "steerAngleLimit")
                            return 720;
                        else
                            return 1;
                    }
                    stepSize: {
                        if (maximumValue == 1)
                            return 0.05;
                        else
                            return 5;
                    }
                    decimals: {
                        if (maximumValue == 1)
                            return 2;
                        else
                            return 0;
                    }
                    Component.onCompleted: {
                        value = backCarLine.getProperty(model.name);
                    }
                    onValueChanged:  {
                        backCarLine.setProperty(model.name, value);
                        backCarLine.manualUpdate();
                    }
                }
            }
        }
        Dial {
            id: steerAngleDial
            minimumValue: 0
            maximumValue: 1
            stepSize: 0.05
            onValueChanged: {
                backCarLine.steerAngle = (value - 0.5) * 2 * backCarLine.steerAngleLimit;
                backCarLine.manualUpdate()
            }
            Component.onCompleted: {
                value = ((backCarLine.steerAngle / backCarLine.steerAngleLimit) + 1 ) / 2;
            }
        }
        Text {
            id: steerAngleText
            text: ((steerAngleDial.value - 0.5) * 2 * backCarLine.steerAngleLimit).toFixed(0)
            anchors.horizontalCenter: steerAngleDial.horizontalCenter
        }
    }
    ComboBox {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        model: trans.languageList
        Component.onCompleted: {
            currentIndex = find(trans.currentLanguage);
        }
        onCurrentTextChanged: {
            trans.currentLanguage = currentText;
        }
    }
    Column {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 5
        spacing: 5
        Text {
            color: "green"
            text: "上面绿色的横线是安全线"
        }
        Text{
            color: "yellow"
            text:"中间黄色的横线是警告线"
        }
        Text{
            color: "red"
            text:"下面红色的横线是危险线"
        }
    }

}
