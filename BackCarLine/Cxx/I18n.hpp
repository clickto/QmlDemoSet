#ifndef I18N_H
#define I18N_H

#include <QObject>
#include <QString>
#include <QHash>
class I18n : public QObject
{
    Q_OBJECT
public:
    explicit I18n(QObject *parent = 0);
    Q_INVOKABLE QString tr(QString key);
signals:

public slots:
private:
    void loadTrans();
    QHash<QString, QString > trans;
};

#endif // I18N_H
