#-------------------------------------------------
#
# Project created by QtCreator 2012-11-01T17:18:49
#
#-------------------------------------------------

QT       += testlib

TARGET = tst_invoicedialogtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_invoicedialogtest.cpp
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
