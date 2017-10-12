#-------------------------------------------------
#
# Project created by QtCreator 2017-08-18T16:13:47
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HotlineDesktop
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#win32:RC_ICONS = C:/Develop/HotlineDesktop/resource/Icon/hotline.ico
#RC_FILE += HotlineDesktop.rc

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    connectdialog.cpp \
    brendsdialog.cpp \
    qdynamicbutton.cpp \
    objectwindow.cpp \
    addazsdialog.cpp \
    addrrodialog.cpp \
    infoobjectdialog.cpp \
    addpcdialog.cpp \
    findwindow.cpp

HEADERS += \
        mainwindow.h \
    options.h \
    database.h \
    connectdialog.h \
    brendsdialog.h \
    qdynamicbutton.h \
    objectwindow.h \
    addazsdialog.h \
    addrrodialog.h \
    infoobjectdialog.h \
    addpcdialog.h \
    findwindow.h

FORMS += \
        mainwindow.ui \
    connectdialog.ui \
    brendsdialog.ui \
    objectwindow.ui \
    addazsdialog.ui \
    addrrodialog.ui \
    infoobjectdialog.ui \
    addpcdialog.ui \
    findwindow.ui

RESOURCES += \
    resource.qrc

DISTFILES +=
