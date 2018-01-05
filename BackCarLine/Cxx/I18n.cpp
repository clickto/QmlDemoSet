#include "I18n.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
I18n::I18n(QObject *parent) : QObject(parent)
{
    loadTrans();
}

QString I18n::tr(QString key)
{
    if (trans.contains(key)) {
        return trans[key];
    } else {
        return key;
    }
}

void I18n::loadTrans()
{
    const QString transFilePath = QString(":/I18n/I18n.json");
    QFile file(transFilePath);
    if (file.open(QFile::ReadOnly)) {
        auto data = file.readAll();
        auto doc = QJsonDocument::fromJson(data);
        auto array = doc.array();
        QJsonObject obj;
        for (auto i : array) {
            obj = i.toObject();
            if (obj.contains("en") && obj.contains("zh")) {
                trans.insert(obj["en"].toString(), obj["zh"].toString());
            }
        }
        file.close();
    } else {
        qDebug() << "load trans file failed :" << file.errorString();
    }
}
