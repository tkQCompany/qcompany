#-------------------------------------------------
#
# Project created by QtCreator 2012-10-23T10:46:02
#
#-------------------------------------------------

QT       += sql xml testlib

TARGET = tst_TestModelTest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG  += testcase

TEMPLATE = app


SOURCES += tst_TestModelTest.cpp \
    ../Database.cpp \
    ../models/ModelVat.cpp \
    ../models/ModelUnit.cpp \
    ../models/ModelPhone.cpp \
    ../models/ModelPaymentType.cpp \
    ../models/ModelInvoiceWithCommodities.cpp \
    ../models/ModelInvoiceType.cpp \
    ../models/ModelInvoice.cpp \
    ../models/ModelEmail.cpp \
    ../models/ModelCurrency.cpp \
    ../models/ModelCountry.cpp \
    ../models/ModelCounterpartyType.cpp \
    ../models/ModelCounterparty.cpp \
    ../models/ModelCommodityType.cpp \
    ../models/ModelCommodity.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../DB_Constants.h \
    ../Database.h \
    ../models/ModelVat.h \
    ../models/ModelUnit.h \
    ../models/ModelPhone.h \
    ../models/ModelPaymentType.h \
    ../models/ModelInvoiceWithCommodities.h \
    ../models/ModelInvoiceType.h \
    ../models/ModelInvoice.h \
    ../models/ModelEmail.h \
    ../models/ModelCurrency.h \
    ../models/ModelCountry.h \
    ../models/ModelCounterpartyType.h \
    ../models/ModelCounterparty.h \
    ../models/ModelCommodityType.h \
    ../models/ModelCommodity.h \
    ../../SettingsGlobal.h \
    ../models_data/UnitData.h \
    ../models_data/PhoneData.h \
    ../models_data/PaymentTypeData.h \
    ../models_data/InvoiceTypeData.h \
    ../models_data/InvoiceData.h \
    ../models_data/EmailData.h \
    ../models_data/CurrencyData.h \
    ../models_data/CountryData.h \
    ../models_data/CounterpartyTypeData.h \
    ../models_data/CounterpartyData.h \
    ../models_data/CommodityTypeData.h \
    ../models_data/CommodityData.h
