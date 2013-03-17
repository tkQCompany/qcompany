QT       += testlib

TEMPLATE = app

CONFIG   += console
CONFIG   -= app_bundle

HEADERS += ../TestsCommon.h

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

OBJECTS_DIR = ../obj
MOC_DIR = ../moc_files
