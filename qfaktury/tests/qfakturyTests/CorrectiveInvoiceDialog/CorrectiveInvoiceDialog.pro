#-------------------------------------------------
#
# Project created by QtCreator 2012-11-01T17:16:32
#
#-------------------------------------------------

QT       += testlib

TARGET = tst_correctiveinvoicedialogtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_correctiveinvoicedialogtest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

DEPENDPATH += ../../../ \
              ../../../src \
              ../../../translations \
              ../../../ui \
              ../../../src/model \
              ../../../src/model/models \
              ../../../src/model/models_data \
               ../../../../qdecimal/decnumber

INCLUDEPATH += ../../../ \
               ../../../src \
               ../../../src/model \
               ../../../src/model/models \
               ../../../src/model/models_data \
               ../../../ui \
               ../../../../qdecimal/decnumber

include(../../../qfaktury_common.pri)

OBJECTS_DIR = ../obj
MOC_DIR = ../obj
