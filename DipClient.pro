QT       += core gui
QT       += network
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    OLDtcpclient.cpp \
    addtablewindow.cpp \
    adduserwindow.cpp \
    dbwindow.cpp \
    disconnectwindow.cpp \
    filelogger.cpp \
    ipeditwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    tcpclient.cpp

HEADERS += \
    OLDtcpclient.h \
    addtablewindow.h \
    adduserwindow.h \
    dbwindow.h \
    disconnectwindow.h \
    filelogger.h \
    ipeditwindow.h \
    mainwindow.h \
    tcpclient.h

FORMS += \
    addtablewindow.ui \
    adduserwindow.ui \
    dbwindow.ui \
    disconnectwindow.ui \
    ipeditwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../Icons/add_Table.png \
    ../Icons/add_User.png \
    ../Icons/delete--v2.png \
    ../Icons/deleteRow.png \
    ../Icons/edit_Table.png \
    ../Icons/exit.png \
    ../Icons/updateIcon.png
