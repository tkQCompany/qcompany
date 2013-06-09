QT       += testlib

TEMPLATE = app

CONFIG   += console
CONFIG   -= app_bundle

HEADERS += ../TestsCommon.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"


DEPENDPATH += ../../../qfaktury \
              ../../../qfaktury/src

INCLUDEPATH += ../../../qfaktury/src \
               ../../../qfaktury/src/ui \
               ../../../qfaktury/ui_files \
               ../../../qfaktury/src/model \
               ../../../qfaktury/src/model/models \
               ../../../qfaktury/src/model/models_data \
               ../../../qdecimal/decnumber \
               ../../../qdecimal/src

LIBS += ../../libqfaktury/libqfaktury.a -L../../../qdecimal/lib
