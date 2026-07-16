QT       += core gui widgets network sql
CONFIG   += c++11
TARGET    = sMonitor_server
TEMPLATE  = app

SOURCES += \
    main.cpp \
    src/loginwidget.cpp \
    src/registerwidget.cpp \
    src/mainwindow.cpp \
    src/monitorwidget.cpp \
    src/multimonitorwidget.cpp \
    src/alarmwidget.cpp \
    src/setupwidget.cpp \
    src/filemanagerwidget.cpp \
    src/mediapreview.cpp \
    src/mqttmanager.cpp \
    src/databasemanager.cpp \
    src/configmanager.cpp \
    src/faceEngine.cpp \
    src/facelogindialog.cpp \
    src/faceRegisterDialog.cpp

HEADERS += \
    src/loginwidget.h \
    src/registerwidget.h \
    src/mainwindow.h \
    src/monitorwidget.h \
    src/multimonitorwidget.h \
    src/alarmwidget.h \
    src/setupwidget.h \
    src/filemanagerwidget.h \
    src/mediapreview.h \
    src/mqttmanager.h \
    src/databasemanager.h \
    src/configmanager.h \
    src/faceEngine.h \
    src/facelogindialog.h \
    src/faceRegisterDialog.h

FORMS += \
    loginwidget.ui \
    registerwidget.ui \
    mainwindow.ui \
    monitorwidget.ui \
    setupwidget.ui \
    alarmwidget.ui \
    filemanagerwidget.ui \
    mediapreview.ui \
    faceRegisterDialog.ui

RESOURCES += resources.qrc

INCLUDEPATH += src

LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_imgcodecs \
        -lopencv_objdetect -lopencv_face -lopencv_highgui -lopencv_videoio
INCLUDEPATH += /usr/local/include
