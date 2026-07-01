#-------------------------------------------------
#
# Project created by QtCreator 2026-06-23T19:23:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = sMonitor
TEMPLATE = app

INCLUDEPATH += /usr/local/include \
               /usr/include/opencv4 \
               /opt/wkspace/opencv-3.4.1/build

LIBS += -L/root/mplayer_sysroot/lib \
        -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio \
        -lopencv_face -lopencv_objdetect -lopencv_video

SOURCES += ConfigManager.cpp VideoWorker.cpp main.cpp \
           setup.cpp menu.cpp fileIndex.cpp fullShow.cpp \
           click_lable.cpp unusual.cpp steng.cpp keyboard.cpp \
           faceEngine.cpp logindlg.cpp registerdlg.cpp

HEADERS  += ConfigManager.h VideoWorker.h menu.h \
            setup.h fileIndex.h fullShow.h click_lable.h \
            unusual.h steng.h keyboard.h \
            faceEngine.h logindlg.h registerdlg.h

FORMS    += menu.ui setup.ui fileIndex.ui fullShow.ui unusual.ui \
            logindlg.ui registerdlg.ui

RESOURCES += images.qrc
