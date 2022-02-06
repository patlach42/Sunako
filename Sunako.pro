QT += core gui widgets

CONFIG += c++11 link_pkgconfig

DEFINES += QT_DEPRECATED_WARNINGS

PKGCONFIG += carla-standalone carla-utils jack
DEPENDPATH += $$PWD/src
INCLUDEPATH += $$PWD/src
# FIXME for x11 systems only
QT += x11extras
PKGCONFIG += x11
VPATH = src/

DESTDIR = release
OBJECTS_DIR = release/.obj
MOC_DIR = release/.moc
RCC_DIR = release/.rcc
UI_DIR = release/.ui

SOURCES += \
    SunakoSettings.cpp \
    SunakoCli.cpp \
    ChibiEmbedWidget.cpp \
    main.cpp \
    ChibiWindow.cpp

HEADERS += \
    SunakoSettings.h \
    SunakoCli.h \
    ChibiEmbedWidget.h \
    ChibiWindow.h

FORMS += \
    resources/chibiwindow.ui

TRANSLATIONS += \
    i18n/en_GB.ts

# Default rules for deployment.
# qnx: target.path = /tmp/$${TARGET}/bin
# else: unix:!android: target.path = /opt/$${TARGET}/bin
# !isEmpty(target.path): INSTALLS += target
