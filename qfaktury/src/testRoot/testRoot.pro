#-------------------------------------------------
#
# Project created by QtCreator 2012-10-21T16:06:19
#
#-------------------------------------------------

QT       += sql xml testlib

TARGET = tst_TestRootTest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG  += testcase

TEMPLATE = app


SOURCES += tst_TestRootTest.cpp ../ConvertAmount.cpp
HEADERS += ../SettingsGlobal.h
DEFINES += SRCDIR=\\\"$$PWD/\\\"
