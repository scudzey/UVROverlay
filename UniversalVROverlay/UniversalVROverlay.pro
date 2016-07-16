

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = UniversalVROverlay
DESTDIR = ../Debug
CONFIG += debug console
DEFINES += _CONSOLE
INCLUDEPATH += ./GeneratedFiles ../inc /Boost/
LIBS += -L"."
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(UniversalVROverlay.pri)
win32:RC_FILE = UniversalVROverlay.rc
