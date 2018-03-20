#include <QTime>
#include <QDebug>
#include <QPainter>

#include "soundbyte.h"

static const int WIDTH = 43;
Soundbyte::Soundbyte(QQuickPaintedItem * parent) : QQuickPaintedItem(parent) {
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(onTimeOut()));

    qsrand(QTime::currentTime().second());
    for (int i = 0; i < LEN; i++) {
        mArr[i] = qrand() % 10 + 1;    // 取1 ～ 10
    }

    QImage img(":/images/rect2.png");
    mImgUp = img.copy();
    mImgDown = img.copy();

    // 设置透明度
    QImage alpha(mImgUp.width(), mImgUp.height(), QImage::Format_Grayscale8);
    alpha.fill(85);    // opacity = 1 -> 255
    mImgUp.setAlphaChannel(alpha);
    alpha.fill(15);
    mImgDown.setAlphaChannel(alpha);
}

void Soundbyte::paint(QPainter *painter) {

    int start = 0;
    const int posX = 0;
    const int posY = 100;
    const int rectWidth = 8;
    const int rectHeight = 8;
    const int margin = 2;

    for (int i = 0; i < WIDTH; ++i) {
        start = mIndex + i;
        int tempX = posX + i * rectWidth + (i - 1) * margin;
        for (int j = 0; j < mArr[start]; j++) {
            // 上半部分
            int tempY = posY - j * rectHeight - (j - 1) * margin;
            painter->drawImage(tempX, tempY, mImgUp);
            // 倒影
            tempY = posY + j * rectHeight + (j - 1) * margin + 12;
            painter->drawImage(tempX, tempY, mImgDown);
        }
    }
}

void Soundbyte::onTimeOut() {
    mIndex += WIDTH;
    if (mIndex >= LEN / 4 * 3)
        mIndex = 0;
    update();
}

void Soundbyte::show() {
    mTimer.start(1000 / 5);
}
void Soundbyte::hide() {
    mTimer.stop();
}

