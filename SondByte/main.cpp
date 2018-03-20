#include <QGuiApplication>
#include <QQuickView>

#include "soundbyte.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickView viewer;
    viewer.setResizeMode(QQuickView::SizeRootObjectToView);
    qmlRegisterType<Soundbyte> ("Soundbyte", 1, 0, "Soundbyte");
    viewer.setSource(QUrl("qrc:///main.qml"));
    viewer.show();
    return app.exec();
}
