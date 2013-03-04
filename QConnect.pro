#-------------------------------------------------
#
# Project created by QtCreator 2012-03-29T23:13:20
#
#-------------------------------------------------

QT  += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QConnect
TEMPLATE = app


SOURCES += main.cpp\
    qconnect.cpp \
    qdataconnection.cpp \
    terminalwidget.cpp \
    connectionlistwidget.cpp \
    connectionwidget.cpp \
    terminallistwidget.cpp \
    qterminaledit.cpp \
    livelistwidget.cpp

HEADERS  += qconnect.h \
    qdataconnection.h \
    terminalwidget.h \
    connectionlistwidget.h \
    connectionwidget.h \
    terminallistwidget.h \
    qterminaledit.h \
    livelistwidget.h

OTHER_FILES += \
    livelistwidget.css \
    terminalwidget.css \
    connectionwidget.css
