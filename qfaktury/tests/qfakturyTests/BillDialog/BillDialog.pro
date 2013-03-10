#-------------------------------------------------
#
# Project created by QtCreator 2012-11-01T17:12:48
#
#-------------------------------------------------

QT       += testlib

TARGET = tst_BillDialogTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_BillDialogTest.cpp

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

HEADERS +=
