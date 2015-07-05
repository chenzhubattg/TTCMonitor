#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T14:56:04
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ../TTCbin/TTCMonitor
TEMPLATE = app


SOURCES += main.cpp\
        treewidget.cpp \
    TCPServer.cpp \
    tcpclientsocket.cpp \
    common.cpp \
#    TCPClient.cpp

HEADERS  += treewidget.h \
    TCPServer.h \
    tcpclientsocket.h \
    common.h \
 #   TCPClient.h

FORMS    += treewidget.ui
#LIBS += /home/ttc/qttest/build-xmltest-Desktop_Qt_5_4_0_GCC_64bit-Debug/libtinyxml.a
LIBS += /usr/local/lib/libtinyxml.so
