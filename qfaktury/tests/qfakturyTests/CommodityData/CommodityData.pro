#-------------------------------------------------
#
# Project created by QtCreator 2012-11-01T17:23:36
#
#-------------------------------------------------

QT       += testlib

TARGET = tst_commoditydatatest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_commoditydatatest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

DEPENDPATH += ../../../ \
              ../../../src \
              ../../../translations \
              ../../../ui \
              ../../../src/model \
              ../../../src/model/models \
              ../../../src/model/models_data

INCLUDEPATH += ../../../ \
               ../../../src \
               ../../../src/model \
               ../../../src/model/models \
               ../../../src/model/models_data \
               ../../../ui

include(../../../qfaktury_common.pri)

OBJECTS_DIR = ../obj
MOC_DIR = ../obj
