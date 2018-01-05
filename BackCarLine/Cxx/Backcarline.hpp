#ifndef BACKCARLINE_H
#define BACKCARLINE_H

#include <QQuickPaintedItem>

struct BackCarLinePrivate;
class BackCarLine : public QQuickPaintedItem
{
    Q_OBJECT
    //倾斜角, 左边斜线为基准，单位:角度
    Q_PROPERTY(qreal slantAngle READ slantAngle WRITE setSlantAngle NOTIFY slantAngleChanged)
    //红色危险线 垂直高度比值, 取值从Item底部开始，取值范围[0, 1]
    Q_PROPERTY(qreal heightRatioDanger READ heightRatioDanger WRITE setHeightRatioDanger NOTIFY heightRatioDangerChanged)
    //黄色警告线 垂直高度比值, 取值从Item底部开始，取值范围[0, 1]
    Q_PROPERTY(qreal heightRatioWarn READ heightRatioWarn WRITE setHeightRatioWarn NOTIFY heightRatioWarnChanged)
    //绿色安全线 垂直高度比值, 取值从Item底部开始，取值范围[0, 1]
    Q_PROPERTY(qreal heightRatioSafe READ heightRatioSafe WRITE setHeightRatioSafe NOTIFY heightRatioSafeChanged)
    //"基线" 垂直高度比值, 取值范围[0, 1]
    Q_PROPERTY(qreal heightRatioBase READ heightRatioBase WRITE setHeightRatioBase NOTIFY heightRatioBaseChanged)
    // "基线"水平宽度比值 , 取值范围[0, 1]。三条线的宽度以此为基准,计算得出。水平方向上默认是居中显示的。
    Q_PROPERTY(qreal widthRatioBase READ widthRatioBase WRITE setWidthRatioBase NOTIFY widthRatioBaseChanged)
    //偏转角度，单位:角度。
    Q_PROPERTY(qreal steerAngle READ steerAngle WRITE setSteerAngle NOTIFY steerAngleChanged)
    //偏转角度的极限值, 单位角度。steerAngleLimit 值为360表示可以顺时针或者逆时针最大旋转360度,即stererAngle的取值范围是[-360, 360]
    Q_PROPERTY(qreal steerAngleLimit READ steerAngleLimit WRITE setSteerAngleLimit NOTIFY steerAngleLimitChanged)
    //安全线 的宽度, 取值范围[0, 1]。 1 表示左右连起来，0表示隐藏，0.5表示左右各占0.25
    Q_PROPERTY(qreal safeLineWidthRatio READ safeLineWidthRatio WRITE setSafeLineWidthRatio NOTIFY safeLineWidthRatioChanged)
    //警告线的宽度, 与安全线一致
    Q_PROPERTY(qreal warnLineWidthRatio READ warnLineWidthRatio WRITE setWarnLineWidthRatio NOTIFY warnLineWidthRatioChanged)
    //危险线的宽度, 与安全线一致
    Q_PROPERTY(qreal dangerLineWidthRatio READ dangerLineWidthRatio WRITE setDangerLineWidthRatio NOTIFY dangerLineWidthRatioChanged)

    //曲线之间的安全线 宽度比例
    Q_PROPERTY(qreal safeCurveWidthRatio READ safeCurveWidthRatio WRITE setSafeCurveWidthRatio NOTIFY safeCurveWidthRatioChanged)
    //曲线之间的警告线 宽度比例
    Q_PROPERTY(qreal warnCurveWidthRatio READ warnCurveWidthRatio WRITE setWarnCurveWidthRatio NOTIFY warnCurveWidthRatioChanged)
    //曲线之间的危险线 宽度比例
    Q_PROPERTY(qreal dangerCurveWidthRatio READ dangerCurveWidthRatio WRITE setDangerCurveWidthRatio NOTIFY dangerCurveWidthRatioChanged)

public:
    explicit BackCarLine(QQuickPaintedItem *parent = nullptr);
    ~BackCarLine();

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter) override;

    //properties
    qreal slantAngle() const;
    qreal heightRatioDanger() const;
    qreal heightRatioWarn() const;
    qreal heightRatioSafe() const;
    qreal widthRatioBase() const;
    qreal heightRatioBase() const;
    qreal steerAngle() const;
    qreal steerAngleLimit() const;
    qreal safeLineWidthRatio() const;
    qreal warnLineWidthRatio() const;
    qreal dangerLineWidthRatio() const;
    qreal safeCurveWidthRatio() const;
    qreal warnCurveWidthRatio() const;
    qreal dangerCurveWidthRatio() const;

    //扩展接口给qml

    /**
     * @brief manualUpdate 手动刷新item
     */
    Q_INVOKABLE void manualUpdate();

    /**
     * @brief setProperty 通过字符串设置item属性
     * @param name 要设置的属性的名字
     * @param value 要设置的属性的值
     * @return 设置成功返回true，否则false
     */
    Q_INVOKABLE bool setProperty (QString name, const QVariant &value);

    /**
     * @brief getProperty 根据字符串获取属性。返回值不要用const &, qml认不出来
     * @param name 要获取的属性的名字
     * @return  属性的值
     */
    Q_INVOKABLE QVariant getProperty(QString name);

signals:
    void slantAngleChanged();
    void heightRatioDangerChanged();
    void heightRatioWarnChanged();
    void heightRatioSafeChanged();
    void heightRatioBaseChanged();
    void widthRatioBaseChanged();
    void steerAngleChanged();
    void steerAngleLimitChanged();
    void safeLineWidthRatioChanged();
    void warnLineWidthRatioChanged();
    void dangerLineWidthRatioChanged();
    void safeCurveWidthRatioChanged();
    void warnCurveWidthRatioChanged();
    void dangerCurveWidthRatioChanged();

public slots:
    void setSlantAngle(qreal slantAngle);
    void setHeightRatioDanger(qreal heightRatioDanger);
    void setHeightRatioWarn(qreal heightRatioWarn);
    void setHeightRatioSafe(qreal heightRatioSafe);
    void setWidthRatioBase(qreal widthRatioBase);
    void setHeightRatioBase(qreal heightRatioBase);
    void setSteerAngle(qreal steerAngle);
    void setSteerAngleLimit(qreal steerAngleLimit);
    void setSafeLineWidthRatio(qreal safeLineWidthRatio);
    void setWarnLineWidthRatio(qreal warnLineWidthRatio);
    void setDangerLineWidthRatio(qreal dangerLineWidthRatio);
    void setSafeCurveWidthRatio(qreal safeCurveWidthRatio);
    void setWarnCurveWidthRatio(qreal warnCurveWidthRatio);
    void setDangerCurveWidthRatio(qreal dangerCurveWidthRatio);

private:
    //  计算曲线的各点坐标
    void calcPoints();

    //d指针封装 数据成员变量，二进制兼容，方便存取默认值，代码简洁
    BackCarLinePrivate *m_dptr;

};

#endif // BACKCARLINE_H
