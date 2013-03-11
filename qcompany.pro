TEMPLATE = subdirs

SUBDIRS += qfaktury \
    qfaktury/tests/qfakturyTests \
    qdecimal

TRANSLATIONS += qfaktury/translations/qcompany_de.ts \
                 qfaktury/translations/qcompany_en.ts

QMAKE_CXXFLAGS += -std=c++11 -Wall
