QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += serialport
QT += serialport

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chemcontroller.cpp \
    config.cpp \
    formreacsettings.cpp \
    formtstatconfig.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    chemcontroller.h \
    config.h \
    formreacsettings.h \
    formtstatconfig.h \
    mainwindow.h

FORMS += \
    formreacsettings.ui \
    formtstatconfig.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ChemControllerCommand
