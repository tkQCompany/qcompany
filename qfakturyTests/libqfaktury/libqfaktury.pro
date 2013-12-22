TEMPLATE = lib

TARGET = qfaktury

DEPENDPATH += ../../qfaktury \
               ../../qfaktury/src


INCLUDEPATH +=  ../../qfaktury/src \
               ../../qfaktury/src/ui \
               ../../qfaktury/ui_files \
               ../../qfaktury/src/model \
               ../../qfaktury/src/model/models \
               ../../qfaktury/src/model/models_data

CONFIG += staticlib

OBJECTS += ../../qfaktury/obj/*.o
