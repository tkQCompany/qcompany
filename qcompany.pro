TEMPLATE = subdirs

SUBDIRS += qfaktury \
    qfakturyTests

CONFIG += ordered


TRANSLATIONS += qfaktury/res/translations/qcompany_de.ts \
                 qfaktury/res/translations/qcompany_en.ts

QMAKE_CXXFLAGS += -Wall -std=c++14
