#-------------------------------------------------
#
# Project created by QtCreator 2017-09-27T09:50:01
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = P2PChat
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        chatwindow.cpp \
    message.cpp \
    Networking/connection.cpp \
    Networking/peermanager.cpp \
    Networking/server.cpp \
    Networking/client.cpp \
    htmldelegate.cpp \
    messagefactory.cpp \
    textmessage.cpp \
    imagemessage.cpp \
    imagedisplaywindow.cpp \
    privatemessage.cpp \
    filemessage.cpp \
    userprofile.cpp \
    userprofiledialog.cpp \
    identitymessage.cpp \
    privatechatroom.cpp \
    actionmessage.cpp \
    privateroomdialog.cpp \
    roomactionsdialog.cpp \
    privatechatdialog.cpp \
    privatesenddialog.cpp

HEADERS += \
        chatwindow.h \
    message.h \
    Networking/connection.h \
    Networking/peermanager.h \
    Networking/server.h \
    Networking/client.h \
    htmldelegate.h \
    messagefactory.h \
    textmessage.h \
    imagemessage.h \
    imagedisplaywindow.h \
    privatemessage.h \
    filemessage.h \
    userprofile.h \
    userprofiledialog.h \
    identitymessage.h \
    privatechatroom.h \
    actionmessage.h \
    privateroomdialog.h \
    roomactionsdialog.h \
    privatechatdialog.h \
    privatesenddialog.h

FORMS += \
        chatwindow.ui \
    imagedisplaywindow.ui \
    userprofiledialog.ui \
    privateroomdialog.ui \
    roomactionsdialog.ui \
    privatechatdialog.ui \
    privatesenddialog.ui

RESOURCES += \
    resources.qrc
