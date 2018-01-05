TARGET = BackCar
TEMPLATE = app

QT += qml quick

CONFIG += c++11

SOURCES += \
    Cxx/Backcarline.cpp \
    Cxx/Main.cpp \
    Cxx/I18n.cpp

RESOURCES += qml.qrc \
    image.qrc \
    i18n.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =


HEADERS += \
    Cxx/Backcarline.hpp \
    Cxx/I18n.hpp

CONFIG += qtquickcompiler
QMAKE_DISTCLEAN += .qtquickcomiler/*
QMAKE_DISTCLEAN += *_qtquickcompiler.qrc

#DESTDIR = bin
#QMAKE_POST_LINK += $(STRIP) $(TARGET)
