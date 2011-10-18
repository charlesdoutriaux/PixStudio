#-------------------------------------------------
#
# Project created by Charles Doutriaux 2011-10-17
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -arch x86_64
QMAKE_CFLAGS += -arch x86_64

QT       += core gui

TARGET = PixStudio
TEMPLATE = app


SOURCES += Src/main.cpp \
	   Src/mainwindow.cpp \
           Src/preferences.cpp

HEADERS += \
    Include/mainwindow.h \
    Include/preferences.h



