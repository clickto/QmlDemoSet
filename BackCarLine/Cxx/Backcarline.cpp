#include "Backcarline.hpp"

#include <QtMath>
#include <QPainter>
#include <QtGlobal>
#include <QQuickItem>
#include <QJsonObject>
#include <QJsonDocument>

struct BackCarLinePrivate {
    BackCarLinePrivate()
    {
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
    ~BackCarLinePrivate()
    {
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
BackCarLine::BackCarLine(QQuickPaintedItem *parent):
    QQuickPaintedItem(parent),
    m_dptr(new BackCarLinePrivate)
{
    //本来应该是在ComponentComplete的时候，执行第一次calcPoints，即对点的初始化.
    //但是测试发现，ComponentComplete中拿不到item的有效宽高
    //就在宽度改变的时候，进行初始化
    connect(this, &QQuickPaintedItem::widthChanged, [=](){
        calcPoints();
    });
}

BackCarLine::~BackCarLine()
{
    delete m_dptr;
    m_dptr = nullptr;
}
void BackCarLine::paint(QPainter *painter)
{
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
        painter->drawLine(m_dptr->leftWarn, m_dptr->leftSafe);
        auto width = (m_dptr->rightSafe.x() - m_dptr->leftSafe.x()) * safeLineWidthRatio() / 2;
        painter->drawLine(m_dptr->leftSafe, m_dptr->leftSafe + QPointF(width, 0));
        painter->drawLine(m_dptr->rightSafe - QPointF(width, 0), m_dptr->rightSafe);
        painter->drawLine(m_dptr->rightSafe, m_dptr->rightWarn);
    }
    //黄色警告线
    {
        pen.setColor(Qt::yellow);
        brush.setColor(Qt::yellow);
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawLine(m_dptr->leftDanger, m_dptr->leftWarn);
        auto width = (m_dptr->rightWarn.x() - m_dptr->leftWarn.x()) * warnLineWidthRatio() / 2;
        painter->drawLine(m_dptr->leftWarn, m_dptr->leftWarn + QPointF(width, 0));
        painter->drawLine(m_dptr->rightWarn - QPointF(width, 0), m_dptr->rightWarn);
        painter->drawLine(m_dptr->rightWarn, m_dptr->rightDanger);
    }
    //红色危险线
    {
        pen.setColor(Qt::red);
        brush.setColor(Qt::red);
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawLine(m_dptr->leftBase, m_dptr->leftDanger);
        auto width = (m_dptr->rightDanger.x() - m_dptr->leftDanger.x()) * dangerLineWidthRatio() / 2;
        painter->drawLine(m_dptr->leftDanger, m_dptr->leftDanger + QPointF(width, 0));
        painter->drawLine(m_dptr->rightDanger - QPointF(width, 0), m_dptr->rightDanger);
        painter->drawLine(m_dptr->rightDanger, m_dptr->rightBase);
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

        leftPath.moveTo(m_dptr->leftBaseOffset);
        leftPath.cubicTo(m_dptr->leftControlHigh, m_dptr->leftControlLow, m_dptr->leftSafeOffset);

        rightPath.moveTo(m_dptr->rightBaseOffset);
        rightPath.cubicTo(m_dptr->rightControlHigh, m_dptr->rightControlLow, m_dptr->rightSafeOffset);

        painter->drawPath(leftPath);
        painter->drawPath(rightPath);

        //曲线之间的安全线
        {
            auto diff = m_dptr->rightSafeOffset - m_dptr->leftSafeOffset;
            painter->drawLine(m_dptr->leftSafeOffset, m_dptr->leftSafeOffset + QPointF(diff.x() / 2 * safeCurveWidthRatio(), diff.y() / 2 * safeCurveWidthRatio()));
            painter->drawLine(m_dptr->rightSafeOffset - QPointF(diff.x() / 2 * safeCurveWidthRatio(), diff.y() / 2 * safeCurveWidthRatio()), m_dptr->rightSafeOffset);
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
void BackCarLine::calcPoints()
{
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
    m_dptr->leftBase = QPointF (leftBaseX, leftBaseY);
    m_dptr->leftDanger = QPointF (leftDangerX, leftDangerY);
    m_dptr->leftWarn = QPointF(leftWarnX, leftWarnY);
    m_dptr->leftSafe = QPointF(leftSafeX, leftSafeY);


    m_dptr->rightBase = QPointF (rightBaseX, rightBaseY);
    m_dptr->rightDanger = QPointF (rightDangerX, rightDangerY);
    m_dptr->rightWarn = QPointF(rightWarnX, rightWarnY);
    m_dptr->rightSafe = QPointF(rightSafeX, rightSafeY);

    //曲线各点坐标
    m_dptr->leftSafeOffset = m_dptr->leftSafe + QPointF(safeOffsetX, -safeOffsetY);
    m_dptr->leftWarnOffset = m_dptr->leftWarn + QPointF(warnOffsetX, -warnOffsetY);
    m_dptr->leftDangerOffset = m_dptr->leftDanger + QPointF(dangerOffsetX, -dangerOffsetY);
    m_dptr->leftBaseOffset = m_dptr->leftBase + QPointF(baseOffsetX, baseOffsetY);

    m_dptr->rightSafeOffset = m_dptr->rightSafe + QPointF(safeOffsetX, safeOffsetY);
    m_dptr->rightWarnOffset = m_dptr->rightWarn + QPointF(warnOffsetX, warnOffsetY);
    m_dptr->rightDangerOffset = m_dptr->rightDanger + QPointF(dangerOffsetX, dangerOffsetY);
    m_dptr->rightBaseOffset = m_dptr->rightBase + QPointF(baseOffsetX, baseOffsetY);
    //两个控制点放在中间
    m_dptr->leftControlHigh = QPointF(leftMidX + midOffsetX, leftMidY + midOffsetY);
    m_dptr->leftControlLow = QPointF(leftMidX + midOffsetX, leftMidY + midOffsetY);

    //两个控制点放在中间
    m_dptr->rightControlHigh = QPointF(rightMidX + midOffsetX, rightMidY + midOffsetY);
    m_dptr->rightControlLow = QPointF(rightMidX + midOffsetX, rightMidY + midOffsetY);
}

bool BackCarLine::setProperty(QString name, const QVariant &value)
{
    //QString 转QByteArray,再转char *,不要省略中间变量。
    auto data = name.toLatin1();
    return this->QObject::setProperty(data.constData(), value);
}
QVariant BackCarLine::getProperty(QString name)
{
    //QString 转QByteArray,再转char *,不要省略中间变量。
    auto data = name.toLatin1();
    return this->QObject::property(data.constData());
}

void BackCarLine::manualUpdate()
{
    calcPoints();
    this->update();
}
//×××××××××××××××××××Propertys*************************//
qreal BackCarLine::slantAngle () const
{
    return m_dptr->slantAngle;
}
void BackCarLine::setSlantAngle (qreal value)
{
    if ( m_dptr->slantAngle == value) return;
    m_dptr->slantAngle = value;
    emit slantAngleChanged();
}
qreal BackCarLine::heightRatioDanger () const
{
    return m_dptr->heightRatioDanger;
}
void BackCarLine::setHeightRatioDanger (qreal value)
{
    if ( m_dptr->heightRatioDanger == value) return;
    m_dptr->heightRatioDanger = value;
    emit heightRatioDangerChanged();
}
qreal BackCarLine::heightRatioWarn () const
{
    return m_dptr->heightRatioWarn;
}
void BackCarLine::setHeightRatioWarn (qreal value)
{
    if ( m_dptr->heightRatioWarn == value) return;
    m_dptr->heightRatioWarn = value;
    emit heightRatioWarnChanged();
}
qreal BackCarLine::heightRatioSafe () const
{
    return m_dptr->heightRatioSafe;
}
void BackCarLine::setHeightRatioSafe (qreal value)
{
    if ( m_dptr->heightRatioSafe == value) return;
    m_dptr->heightRatioSafe = value;
    emit heightRatioSafeChanged();
}
qreal BackCarLine::heightRatioBase () const
{
    return m_dptr->heightRatioBase;
}
void BackCarLine::setHeightRatioBase (qreal value)
{
    if ( m_dptr->heightRatioBase == value) return;
    m_dptr->heightRatioBase = value;
    emit heightRatioBaseChanged();
}
qreal BackCarLine::widthRatioBase () const
{
    return m_dptr->widthRatioBase;
}
void BackCarLine::setWidthRatioBase (qreal value)
{
    if ( m_dptr->widthRatioBase == value) return;
    m_dptr->widthRatioBase = value;
    emit widthRatioBaseChanged();
}
qreal BackCarLine::steerAngle () const
{
    return m_dptr->steerAngle;
}
void BackCarLine::setSteerAngle (qreal value)
{
    if ( m_dptr->steerAngle == value) return;
    m_dptr->steerAngle = value;
    emit steerAngleChanged();
}
void BackCarLine::setSafeLineWidthRatio(qreal safeLineWidthRatio)
{
    if (m_dptr->safeLineWidthRatio == safeLineWidthRatio)
        return;

    m_dptr->safeLineWidthRatio = safeLineWidthRatio;
    emit safeLineWidthRatioChanged();
}
void BackCarLine::setWarnLineWidthRatio(qreal warnLineWidthRatio)
{
    if (m_dptr->warnLineWidthRatio == warnLineWidthRatio)
        return;

    m_dptr->warnLineWidthRatio = warnLineWidthRatio;
    emit warnLineWidthRatioChanged();
}
void BackCarLine::setDangerLineWidthRatio(qreal dangerLineWidthRatio)
{
    if (m_dptr->dangerLineWidthRatio == dangerLineWidthRatio)
        return;

    m_dptr->dangerLineWidthRatio = dangerLineWidthRatio;
    emit dangerLineWidthRatioChanged();
}
void BackCarLine::setSafeCurveWidthRatio(qreal safeCurveWidthRatio)
{
    if (m_dptr->safeCurveWidthRatio == safeCurveWidthRatio)
        return;

    m_dptr->safeCurveWidthRatio = safeCurveWidthRatio;
    emit safeCurveWidthRatioChanged();
}

void BackCarLine::setWarnCurveWidthRatio(qreal warnCurveWidthRatio)
{
    if (m_dptr->warnCurveWidthRatio == warnCurveWidthRatio)
        return;

    m_dptr->warnCurveWidthRatio = warnCurveWidthRatio;
    emit warnCurveWidthRatioChanged();
}

void BackCarLine::setDangerCurveWidthRatio(qreal dangerCurveWidthRatio)
{
    if (m_dptr->dangerCurveWidthRatio == dangerCurveWidthRatio)
        return;

    m_dptr->dangerCurveWidthRatio = dangerCurveWidthRatio;
    emit dangerCurveWidthRatioChanged();
}
qreal BackCarLine::steerAngleLimit() const
{
    return m_dptr->steerAngleLimit;
}
void BackCarLine::setSteerAngleLimit(qreal value)
{
    if (steerAngleLimit() == value) return;

    m_dptr->steerAngleLimit = value;
    emit steerAngleLimitChanged();
}
qreal BackCarLine::safeLineWidthRatio() const
{
    return m_dptr->safeLineWidthRatio;
}

qreal BackCarLine::warnLineWidthRatio() const
{
    return m_dptr->warnLineWidthRatio;
}

qreal BackCarLine::dangerLineWidthRatio() const
{
    return m_dptr->dangerLineWidthRatio;
}
qreal BackCarLine::safeCurveWidthRatio() const
{
    return m_dptr->safeCurveWidthRatio;
}

qreal BackCarLine::warnCurveWidthRatio() const
{
    return m_dptr->warnCurveWidthRatio;
}

qreal BackCarLine::dangerCurveWidthRatio() const
{
    return m_dptr->dangerCurveWidthRatio;
}
