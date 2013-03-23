TEMPLATE = subdirs

SUBDIRS += qdecimal \
    qfaktury \
    qfakturyTests

CONFIG += ordered


TRANSLATIONS += qfaktury/translations/qcompany_de.ts \
                 qfaktury/translations/qcompany_en.ts

QMAKE_CXXFLAGS += -std=c++11 -Wall
