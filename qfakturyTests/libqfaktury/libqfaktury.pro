TEMPLATE = lib

TARGET = qfaktury

DEPENDPATH += ../../qfaktury \
               ../../qfaktury/src \
               ../../qdecimal/decnumber \
               ../../qdecimal/src


INCLUDEPATH +=  ../../qfaktury/src \
               ../../qfaktury/src/ui \
               ../../qfaktury/ui_files \
               ../../qfaktury/src/model \
               ../../qfaktury/src/model/models \
               ../../qfaktury/src/model/models_data \
               ../../qdecimal/decnumber \
               ../../qdecimal/src

CONFIG += staticlib

OBJECTS += ../../qfaktury/obj/*.o ../../qdecimal/src/obj/*.o ../../qdecimal/decnumber/obj/*.o

LIBS += -L../../qdecimal/lib -ldecnumber -lqdecimal
