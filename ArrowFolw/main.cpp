#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQuickView view;
    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.show();

    return app.exec();
}
