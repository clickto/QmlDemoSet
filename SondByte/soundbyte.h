#ifndef SOUNDBYTE_H
#define SOUNDBYTE_H

#include <QQuickPaintedItem>
#include <QTimer>
#include <QImage>
const static int LEN = 1000;


class Soundbyte : public QQuickPaintedItem {
    Q_OBJECT

public:
    Soundbyte(QQuickPaintedItem *parent = 0);
protected:
    void paint(QPainter *painter) override;
    Q_INVOKABLE void show();
    Q_INVOKABLE void hide();
public slots:
    void onTimeOut();

private:
    int mArr[LEN];
    int mIndex = 0;
    QTimer mTimer;
    QImage mImgUp;
    QImage mImgDown;
};

#endif // SOUNDBYTE_H
