#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>

#include "Backcarline.hpp"
#include "I18n.hpp"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<BackCarLine>("BackCarLine", 1, 0, "BackCarLine");
    qmlRegisterType<I18n>("I18n", 1, 0, "I18n");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/Qml/Main.qml")));

    return app.exec();
}
