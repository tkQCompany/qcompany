#-------------------------------------------------
#
# Project created by QtCreator 2012-11-01T17:20:36
#
#-------------------------------------------------

QT       += xml testlib

TARGET = tst_databasetest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_databasetest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

include(../CommonTest.pri)
