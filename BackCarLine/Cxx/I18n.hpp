#ifndef I18N_H
#define I18N_H

#include <QObject>
#include <QString>
#include <QHash>
static const QString EN = QStringLiteral("en");
static const QString CN = QStringLiteral("zh");

class I18n : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList languageList READ languageList NOTIFY languageListChanged)
    Q_PROPERTY(QString currentLanguage READ currentLanguage WRITE setCurrentLanguage NOTIFY currentLanguageChanged)
    Q_PROPERTY(QString dirver READ dirver NOTIFY dirverChanged)
public:
    explicit I18n(QObject *parent = 0);

    Q_INVOKABLE QString tr(QString key);

    const QStringList &languageList() const { return mLanguageList; }

    const QString &currentLanguage() const { return mCurrentLanguage;}

    const QString &dirver() const { return mDirver; }

public slots:
    void setCurrentLanguage(const QString &currentLanguage);

private:
    void setDirver(const QString &dirver);

signals:

    void languageListChanged(const QStringList &languageList);

    void currentLanguageChanged(const QString &currentLanguage);

    void dirverChanged(QString dirver);

private:
    void loadTrans();
    QHash<QString, QString > trans;
    QStringList mLanguageList {EN, CN};
    QString mCurrentLanguage {CN};
    QString mDirver {QStringLiteral("")};
};

#endif // I18N_H
