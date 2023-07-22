QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 release

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    frontend/sources/viewer.cpp \
    frontend/sources/main.cpp \
    frontend/sources/mainwindow.cpp \
    frontend/sources/settings.cpp \
    backend/sources/parser.c \
    backend/sources/matrix.c \
    backend/sources/calculate.c

HEADERS += \
    frontend/headers/mainwindow.h \
    frontend/headers/viewer.h \
    frontend/headers/settings.h \
    backend/headers/parser.h \
    backend/headers/parser_s.h \
    backend/headers/matrix_s.h \
    backend/headers/calculate.h

FORMS += \
    frontend/forms/mainwindow.ui \

RESOURCES += \
    resources/icon.qrc

macx: {
    ICON = $${PWD}/../icons/viewer.icns
    QT += openglwidgets
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
