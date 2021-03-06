QT       += testlib

TEMPLATE = app

CONFIG   += console
CONFIG   -= app_bundle

DEFINES += SRCDIR=\\\"$$PWD/\\\"


DEPENDPATH += ../../../qfaktury \
              ../../../qfaktury/src

INCLUDEPATH += ../../../qfaktury/src \
               ../../../qfaktury/src/ui \
               ../../../qfaktury/ui_files \
               ../../../qfaktury/src/model \
               ../../../qfaktury/src/model/models \
               ../../../qfaktury/src/model/models_data

LIBS += ../../TestsCommon/libTestsCommon.a ../../libqfaktury/libqfaktury.a -lgmpxx -lgmp
