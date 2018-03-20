#include "Backcarline.hpp"

#include <QtMath>
#include <QPainter>
#include <QtGlobal>
#include <QQuickItem>
#include <QJsonObject>
#include <QJsonDocument>

struct BackCarLinePrivate {
    BackCarLinePrivate() {
        //通过json文件读取默认值
        QFile file(CONFIG_FILE_NAME);
        if (file.open(QFile::ReadOnly)) {
            auto data = file.readAll();
            file.close();
            auto doc = QJsonDocument::fromJson(data);
            auto obj = doc.object();
            if (obj.contains("slantAngle")) slantAngle = obj["slantAngle"].toDouble();
            if (obj.contains("heightRatioDanger")) heightRatioDanger = obj["heightRatioDanger"].toDouble();
            if (obj.contains("heightRatioWarn")) heightRatioWarn = obj["heightRatioWarn"].toDouble();
            if (obj.contains("heightRatioSafe")) heightRatioSafe = obj["heightRatioSafe"].toDouble();
            if (obj.contains("heightRatioBase")) heightRatioBase = obj["heightRatioBase"].toDouble();
            if (obj.contains("widthRatioBase")) widthRatioBase = obj["widthRatioBase"].toDouble();
            if (obj.contains("steerAngle")) steerAngle = obj["steerAngle"].toDouble();
            if (obj.contains("steerAngleLimit")) steerAngleLimit = obj["steerAngleLimit"].toDouble();
            if (obj.contains("safeLineWidthRatio")) safeLineWidthRatio = obj["safeLineWidthRatio"].toDouble();
            if (obj.contains("warnLineWidthRatio")) warnLineWidthRatio = obj["warnLineWidthRatio"].toDouble();
            if (obj.contains("dangerLineWidthRatio")) dangerLineWidthRatio = obj["dangerLineWidthRatio"].toDouble();
            if (obj.contains("safeCurveWidthRatio")) safeCurveWidthRatio = obj["safeCurveWidthRatio"].toDouble();
            if (obj.contains("warnCurveWidthRatio")) warnCurveWidthRatio = obj["warnCurveWidthRatio"].toDouble();
            if (obj.contains("dangerCurveWidthRatio")) dangerCurveWidthRatio = obj["dangerCurveWidthRatio"].toDouble();
        }
    }
    ~BackCarLinePrivate() {
        //保存当前值到json文件
        QFile file(CONFIG_FILE_NAME);
        if (file.open(QFile::WriteOnly)) {
            QJsonObject json;
            json["slantAngle"] = slantAngle;
            json["heightRatioDanger"] = heightRatioDanger;
            json["heightRatioWarn"] = heightRatioWarn;
            json["heightRatioSafe"] = heightRatioSafe;
            json["heightRatioBase"] = heightRatioBase;
            json["widthRatioBase"] = widthRatioBase;
            json["steerAngle"] = steerAngle;
            json["steerAngleLimit"] = steerAngleLimit;
            json["safeLineWidthRatio"] = safeLineWidthRatio;
            json["warnLineWidthRatio"] = warnLineWidthRatio;
            json["dangerLineWidthRatio"] = dangerLineWidthRatio;
            json["safeCurveWidthRatio"] = safeCurveWidthRatio;
            json["warnCurveWidthRatio"] = warnCurveWidthRatio;
            json["dangerCurveWidthRatio"] = dangerCurveWidthRatio;
            QJsonDocument doc(json);
            file.write(doc.toJson());
            file.close();
        }
    }
    qreal slantAngle = 80;
    qreal heightRatioDanger = 0.2;
    qreal heightRatioWarn = 0.4;
    qreal heightRatioSafe = 0.8;
    qreal heightRatioBase = 0.1;
    qreal widthRatioBase = 0.8;
    qreal steerAngle = 0;
    qreal steerAngleLimit = 360;
    qreal safeLineWidthRatio = 0.3;
    qreal warnLineWidthRatio = 0.3;
    qreal dangerLineWidthRatio = 0.3;
    qreal safeCurveWidthRatio = 1.0;
    qreal warnCurveWidthRatio = 1.0;
    qreal dangerCurveWidthRatio = 1.0;
    //梯形上的点
    QPointF leftBase;
    QPointF  leftSafe;
    QPointF  leftWarn;
    QPointF  leftDanger;
    QPointF  rightBase;
    QPointF  rightSafe;
    QPointF  rightWarn;
    QPointF  rightDanger;

    //曲线上的点
    QPointF leftSafeOffset;
    QPointF leftWarnOffset;
    QPointF leftDangerOffset;
    QPointF leftControlHigh;
    QPointF leftControlLow;
    QPointF leftBaseOffset;

    QPointF rightSafeOffset;
    QPointF rightWarnOffset;
    QPointF rightDangerOffset;
    QPointF rightControlHigh;
    QPointF rightControlLow;
    QPointF rightBaseOffset;

    const QString CONFIG_FILE_NAME = QString("config.json");
};
BackCarLine::BackCarLine(QQuickPaintedItem *parent): QQuickPaintedItem(parent), mDptr(new BackCarLinePrivate) {
    //本来应该是在ComponentComplete的时候，执行第一次calcPoints，即对点的初始化.
    //但是测试发现，ComponentComplete中拿不到item的有效宽高
    //就在宽度改变的时候，进行初始化
    connect(this, &QQuickPaintedItem::widthChanged, [=](){
        calcPoints();
    });
}

BackCarLine::~BackCarLine() {
    delete mDptr;
    mDptr = nullptr;
}
void BackCarLine::paint(QPainter *painter) {
    painter->save();
    //开启 反走样(抗锯齿)
    painter->setRenderHint(QPainter::Antialiasing);
    QBrush brush;
    QPen pen;
    //交叉部分绘制为圆角
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(6);

    //绿色安全线
    {
        pen.setColor(Qt::green);
        brush.setColor(Qt::green);
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawLine(mDptr->leftWarn, mDptr->leftSafe);
        auto width = (mDptr->rightSafe.x() - mDptr->leftSafe.x()) * safeLineWidthRatio() / 2;
        painter->drawLine(mDptr->leftSafe, mDptr->leftSafe + QPointF(width, 0));
        painter->drawLine(mDptr->rightSafe - QPointF(width, 0), mDptr->rightSafe);
        painter->drawLine(mDptr->rightSafe, mDptr->rightWarn);
    }
    //黄色警告线
    {
        pen.setColor(Qt::yellow);
        brush.setColor(Qt::yellow);
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawLine(mDptr->leftDanger, mDptr->leftWarn);
        auto width = (mDptr->rightWarn.x() - mDptr->leftWarn.x()) * warnLineWidthRatio() / 2;
        painter->drawLine(mDptr->leftWarn, mDptr->leftWarn + QPointF(width, 0));
        painter->drawLine(mDptr->rightWarn - QPointF(width, 0), mDptr->rightWarn);
        painter->drawLine(mDptr->rightWarn, mDptr->rightDanger);
    }
    //红色危险线
    {
        pen.setColor(Qt::red);
        brush.setColor(Qt::red);
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawLine(mDptr->leftBase, mDptr->leftDanger);
        auto width = (mDptr->rightDanger.x() - mDptr->leftDanger.x()) * dangerLineWidthRatio() / 2;
        painter->drawLine(mDptr->leftDanger, mDptr->leftDanger + QPointF(width, 0));
        painter->drawLine(mDptr->rightDanger - QPointF(width, 0), mDptr->rightDanger);
        painter->drawLine(mDptr->rightDanger, mDptr->rightBase);
    }
    //曲线, 橙色
    {
        pen.setColor(QColor("#FF8C00"));
        pen.setWidth(8);
        brush.setColor(QColor("#FF8C00"));
        painter->setPen(pen);
        painter->setBrush(brush);

        //两条曲线
        QPainterPath leftPath, rightPath;

        leftPath.moveTo(mDptr->leftBaseOffset);
        leftPath.cubicTo(mDptr->leftControlHigh, mDptr->leftControlLow, mDptr->leftSafeOffset);

        rightPath.moveTo(mDptr->rightBaseOffset);
        rightPath.cubicTo(mDptr->rightControlHigh, mDptr->rightControlLow, mDptr->rightSafeOffset);

        painter->drawPath(leftPath);
        painter->drawPath(rightPath);

        //曲线之间的安全线
        {
            auto diff = mDptr->rightSafeOffset - mDptr->leftSafeOffset;
            painter->drawLine(mDptr->leftSafeOffset, mDptr->leftSafeOffset + QPointF(diff.x() / 2 * safeCurveWidthRatio(), diff.y() / 2 * safeCurveWidthRatio()));
            painter->drawLine(mDptr->rightSafeOffset - QPointF(diff.x() / 2 * safeCurveWidthRatio(), diff.y() / 2 * safeCurveWidthRatio()), mDptr->rightSafeOffset);
        }
        //曲线之间的警告线
        {
            //计算警告线高度的占比
            auto ratio = (heightRatioWarn() - heightRatioBase()) / (heightRatioSafe() - heightRatioBase());
            //path长度
            auto leftLength = leftPath.length();
            //长度 乘以 占比，作为要取点的新长度，通过percentAtLength函数获得在path中对应的percent
            auto leftPercent = leftPath.percentAtLength(leftLength * ratio);

            auto rightLength = rightPath.length();
            auto rightPercent = rightPath.percentAtLength(rightLength * ratio);

            //根据percent得到path中点的坐标
            const auto start = leftPath.pointAtPercent(leftPercent);
            const auto end = rightPath.pointAtPercent(rightPercent);
            const auto diff = end - start;
            painter->drawLine(start, start + QPointF(diff.x() / 2 * warnCurveWidthRatio(), diff.y() / 2 * warnCurveWidthRatio()));
            painter->drawLine(end - QPointF(diff.x() / 2 * warnCurveWidthRatio(), diff.y() / 2 * warnCurveWidthRatio()), end);
        }
        //曲线之间的危险线
        {
            auto ratio = (heightRatioDanger() - heightRatioBase()) / (heightRatioSafe() - heightRatioBase());
            auto leftLength = leftPath.length();
            auto leftPercent = leftPath.percentAtLength(leftLength * ratio);
            auto rightLength = rightPath.length();
            auto rightPercent = rightPath.percentAtLength(rightLength * ratio);

            const auto start = leftPath.pointAtPercent(leftPercent);
            const auto end = rightPath.pointAtPercent(rightPercent);
            const auto diff = end - start;
            painter->drawLine(start, start + QPointF(diff.x() / 2 * dangerCurveWidthRatio(), diff.y() / 2 * dangerCurveWidthRatio()));
            painter->drawLine(end - QPointF(diff.x() / 2 * dangerCurveWidthRatio(), diff.y() / 2 * dangerCurveWidthRatio()), end);
        }
    }
    painter->restore();
}
void BackCarLine::calcPoints() {
    auto w = width();
    auto h = height();

    auto widthBase = w * widthRatioBase();

    auto leftBaseX = (w - widthBase) / 2;
    auto leftBaseY = h * (1 - heightRatioBase());
    auto leftDangerX = leftBaseX + (heightRatioDanger() - heightRatioBase()) * h / tan(qDegreesToRadians(slantAngle()));
    auto leftDangerY = h * (1 - heightRatioDanger());
    auto leftWarnX = leftBaseX + (heightRatioWarn() - heightRatioBase()) * h / tan(qDegreesToRadians(slantAngle()));
    auto leftWarnY = h * (1 - heightRatioWarn());
    auto leftSafeX = leftBaseX + (heightRatioSafe() - heightRatioBase()) * h / tan(qDegreesToRadians(slantAngle()));
    auto leftSafeY = h * (1 - heightRatioSafe());

    auto rightBaseX = w - leftBaseX;
    auto rightBaseY = leftBaseY;
    auto rightDangerX = w - leftDangerX;
    auto rightDangerY = leftDangerY;
    auto rightWarnX = w - leftWarnX;
    auto rightWarnY = leftWarnY;
    auto rightSafeX = w - leftSafeX;
    auto rightSafeY = leftSafeY;

    auto leftMidX = (leftBaseX + leftSafeX) / 2;
    auto leftMidY = (leftBaseY + leftSafeY) / 2;
    auto rightMidX = w - leftMidX;
    auto rightMidY = leftMidY;

    //偏转角与偏转角最大值的比值
    auto angleRatio = steerAngle() / fabs(steerAngleLimit());

    auto safeOffsetX = leftSafeX * angleRatio ;
    auto safeOffsetY =  leftSafeY * angleRatio / 4;
    auto warnOffsetX = leftWarnX * angleRatio;
    auto warnOffsetY =  leftWarnY * angleRatio ;
    auto dangerOffsetX = leftDangerX * angleRatio;
    auto dangerOffsetY = leftDangerY * angleRatio;
    auto baseOffsetX = leftBaseX * angleRatio / 2;
    //基线y轴不动
    auto baseOffsetY = 0;

    auto midOffsetX = leftMidX * angleRatio / 2;
    auto midOffsetY = 0;

    //梯形各点坐标
    mDptr->leftBase = QPointF (leftBaseX, leftBaseY);
    mDptr->leftDanger = QPointF (leftDangerX, leftDangerY);
    mDptr->leftWarn = QPointF(leftWarnX, leftWarnY);
    mDptr->leftSafe = QPointF(leftSafeX, leftSafeY);


    mDptr->rightBase = QPointF (rightBaseX, rightBaseY);
    mDptr->rightDanger = QPointF (rightDangerX, rightDangerY);
    mDptr->rightWarn = QPointF(rightWarnX, rightWarnY);
    mDptr->rightSafe = QPointF(rightSafeX, rightSafeY);

    //曲线各点坐标
    mDptr->leftSafeOffset = mDptr->leftSafe + QPointF(safeOffsetX, -safeOffsetY);
    mDptr->leftWarnOffset = mDptr->leftWarn + QPointF(warnOffsetX, -warnOffsetY);
    mDptr->leftDangerOffset = mDptr->leftDanger + QPointF(dangerOffsetX, -dangerOffsetY);
    mDptr->leftBaseOffset = mDptr->leftBase + QPointF(baseOffsetX, baseOffsetY);

    mDptr->rightSafeOffset = mDptr->rightSafe + QPointF(safeOffsetX, safeOffsetY);
    mDptr->rightWarnOffset = mDptr->rightWarn + QPointF(warnOffsetX, warnOffsetY);
    mDptr->rightDangerOffset = mDptr->rightDanger + QPointF(dangerOffsetX, dangerOffsetY);
    mDptr->rightBaseOffset = mDptr->rightBase + QPointF(baseOffsetX, baseOffsetY);
    //两个控制点放在中间
    mDptr->leftControlHigh = QPointF(leftMidX + midOffsetX, leftMidY + midOffsetY);
    mDptr->leftControlLow = QPointF(leftMidX + midOffsetX, leftMidY + midOffsetY);

    //两个控制点放在中间
    mDptr->rightControlHigh = QPointF(rightMidX + midOffsetX, rightMidY + midOffsetY);
    mDptr->rightControlLow = QPointF(rightMidX + midOffsetX, rightMidY + midOffsetY);
}

bool BackCarLine::setProperty(QString name, const QVariant &value) {
    //QString 转QByteArray,再转char *,不要省略中间变量。
    auto data = name.toLatin1();
    return this->QObject::setProperty(data.constData(), value);
}
QVariant BackCarLine::getProperty(QString name) {
    //QString 转QByteArray,再转char *,不要省略中间变量。
    auto data = name.toLatin1();
    return this->QObject::property(data.constData());
}

void BackCarLine::manualUpdate() {
    calcPoints();
    this->update();
}
//×××××××××××××××××××Propertys*************************//
qreal BackCarLine::slantAngle () const {
    return mDptr->slantAngle;
}
void BackCarLine::setSlantAngle (qreal value) {
    if ( mDptr->slantAngle == value) return;
    mDptr->slantAngle = value;
    emit slantAngleChanged();
}
qreal BackCarLine::heightRatioDanger () const {
    return mDptr->heightRatioDanger;
}
void BackCarLine::setHeightRatioDanger (qreal value) {
    if ( mDptr->heightRatioDanger == value) return;
    mDptr->heightRatioDanger = value;
    emit heightRatioDangerChanged();
}
qreal BackCarLine::heightRatioWarn () const {
    return mDptr->heightRatioWarn;
}
void BackCarLine::setHeightRatioWarn (qreal value) {
    if ( mDptr->heightRatioWarn == value) return;
    mDptr->heightRatioWarn = value;
    emit heightRatioWarnChanged();
}
qreal BackCarLine::heightRatioSafe () const {
    return mDptr->heightRatioSafe;
}
void BackCarLine::setHeightRatioSafe (qreal value) {
    if ( mDptr->heightRatioSafe == value) return;
    mDptr->heightRatioSafe = value;
    emit heightRatioSafeChanged();
}
qreal BackCarLine::heightRatioBase () const {
    return mDptr->heightRatioBase;
}
void BackCarLine::setHeightRatioBase (qreal value) {
    if ( mDptr->heightRatioBase == value) return;
    mDptr->heightRatioBase = value;
    emit heightRatioBaseChanged();
}
qreal BackCarLine::widthRatioBase () const {
    return mDptr->widthRatioBase;
}
void BackCarLine::setWidthRatioBase (qreal value) {
    if ( mDptr->widthRatioBase == value) return;
    mDptr->widthRatioBase = value;
    emit widthRatioBaseChanged();
}
qreal BackCarLine::steerAngle () const {
    return mDptr->steerAngle;
}
void BackCarLine::setSteerAngle (qreal value) {
    if ( mDptr->steerAngle == value) return;
    mDptr->steerAngle = value;
    emit steerAngleChanged();
}
void BackCarLine::setSafeLineWidthRatio(qreal safeLineWidthRatio) {
    if (mDptr->safeLineWidthRatio == safeLineWidthRatio)
        return;

    mDptr->safeLineWidthRatio = safeLineWidthRatio;
    emit safeLineWidthRatioChanged();
}
void BackCarLine::setWarnLineWidthRatio(qreal warnLineWidthRatio) {
    if (mDptr->warnLineWidthRatio == warnLineWidthRatio)
        return;

    mDptr->warnLineWidthRatio = warnLineWidthRatio;
    emit warnLineWidthRatioChanged();
}
void BackCarLine::setDangerLineWidthRatio(qreal dangerLineWidthRatio) {
    if (mDptr->dangerLineWidthRatio == dangerLineWidthRatio)
        return;

    mDptr->dangerLineWidthRatio = dangerLineWidthRatio;
    emit dangerLineWidthRatioChanged();
}
void BackCarLine::setSafeCurveWidthRatio(qreal safeCurveWidthRatio) {
    if (mDptr->safeCurveWidthRatio == safeCurveWidthRatio)
        return;

    mDptr->safeCurveWidthRatio = safeCurveWidthRatio;
    emit safeCurveWidthRatioChanged();
}

void BackCarLine::setWarnCurveWidthRatio(qreal warnCurveWidthRatio) {
    if (mDptr->warnCurveWidthRatio == warnCurveWidthRatio)
        return;

    mDptr->warnCurveWidthRatio = warnCurveWidthRatio;
    emit warnCurveWidthRatioChanged();
}

void BackCarLine::setDangerCurveWidthRatio(qreal dangerCurveWidthRatio) {
    if (mDptr->dangerCurveWidthRatio == dangerCurveWidthRatio)
        return;

    mDptr->dangerCurveWidthRatio = dangerCurveWidthRatio;
    emit dangerCurveWidthRatioChanged();
}
qreal BackCarLine::steerAngleLimit() const {
    return mDptr->steerAngleLimit;
}
void BackCarLine::setSteerAngleLimit(qreal value) {
    if (steerAngleLimit() == value) return;

    mDptr->steerAngleLimit = value;
    emit steerAngleLimitChanged();
}
qreal BackCarLine::safeLineWidthRatio() const {
    return mDptr->safeLineWidthRatio;
}

qreal BackCarLine::warnLineWidthRatio() const {
    return mDptr->warnLineWidthRatio;
}

qreal BackCarLine::dangerLineWidthRatio() const {
    return mDptr->dangerLineWidthRatio;
}
qreal BackCarLine::safeCurveWidthRatio() const {
    return mDptr->safeCurveWidthRatio;
}

qreal BackCarLine::warnCurveWidthRatio() const {
    return mDptr->warnCurveWidthRatio;
}

qreal BackCarLine::dangerCurveWidthRatio() const {
    return mDptr->dangerCurveWidthRatio;
}
