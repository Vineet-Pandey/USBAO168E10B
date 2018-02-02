#-------------------------------------------------
#
# Project created by QtCreator 2017-07-24T14:48:17
#
#-------------------------------------------------

QT       -= core gui

TARGET = USBAO168E10B
TEMPLATE = app
OBJECTS_DIR = ./BIN/
MOC_DIR = ./BIN/
QMAKE_CXXFLAGS	 += -g -Wall

CONFIG += debug


LIBS+= -L/usr/local/lib -laiousbcpp

INCLUDEPATH+= /usr/local/include/aiousb\
              /usr/include/libusb-1.0


DEFINES += QT_DEPRECATED_WARNINGS



SOURCES += USBAO168E10B.cpp \
    main.cpp

HEADERS += USBAO168E10B.h


