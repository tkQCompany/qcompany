#-------------------------------------------------
#
# Project created by QtCreator 2012-11-01T17:24:49
#
#-------------------------------------------------

QT       += testlib

TARGET = tst_counterpartytypedatatest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_counterpartytypedatatest.cpp
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
