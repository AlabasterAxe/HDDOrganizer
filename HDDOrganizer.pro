#-------------------------------------------------
#
# Project created by QtCreator 2013-01-17T11:53:22
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HDDOrganizer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tag.cpp \
    drive.cpp \
    tagtreemodel.cpp \
    tagnamedialog.cpp \
    tagtreeview.cpp \
    notificationdialog.cpp \
    filetableview.cpp \
    tagmimedata.cpp \
    expression.cpp \
    operation.cpp

HEADERS  += mainwindow.h \
    tag.h \
    drive.h \
    tagtreemodel.h \
    tagnamedialog.h \
    tagtreeview.h \
    notificationdialog.h \
    filetableview.h \
    tagmimedata.h \
    expression.h \
    operation.h

FORMS    += mainwindow.ui \
    tagnamedialog.ui \
    notificationdialog.ui
