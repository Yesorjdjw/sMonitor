#-------------------------------------------------
#
# Project created by QtCreator 2026-06-23T19:23:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sMonitor
TEMPLATE = app

INCLUDEPATH += /usr/local/include \
               /opt/wkspace/opencv-3.4.1/build

LIBS += -L/root/mplayer_sysroot/lib \
        -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio

SOURCES += main.cpp         setup.cpp         menu.cpp         fops.cpp         unusual.cpp         steng.cpp

HEADERS  += menu.h         setup.h         fops.h         unusual.h         steng.h

FORMS    += menu.ui         setup.ui         fops.ui         unusual.ui

RESOURCES +=     images.qrc
