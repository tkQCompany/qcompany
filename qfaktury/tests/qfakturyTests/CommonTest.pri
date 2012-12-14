QT += sql

SOURCES += ../../../ui/CounterpartyDialog.cpp \
    ../../../ui/CustomPaymentDialog.cpp \
    ../../../ui/CommodityListDialog.cpp \
    ../../../ui/ChangeAmountDialog.cpp \
    ../../../ui/PhoneDialog.cpp \
    ../../../ui/EmailDialog.cpp \
    ../../../ui/CountryDialog.cpp \
    ../../../ui/CounterpartyTypeDialog.cpp \
    ../../../ui/CounterpartyInfoDialog.cpp \
    ../../../src/ConvertAmount.cpp \
    ../../../src/model/Database.cpp \
    ../../../src/model/models/ModelCommodity.cpp \
    ../../../src/model/models/ModelCommodityType.cpp \
    ../../../src/model/models/ModelCountry.cpp \
    ../../../src/model/models/ModelEmail.cpp \
    ../../../src/model/models/ModelPhone.cpp \
    ../../../src/model/models/ModelUnit.cpp \
    ../../../src/model/models/ModelVat.cpp \
    ../../../src/model/models/ModelInvoice.cpp \
    ../../../src/model/models/ModelCounterparty.cpp \
    ../../../src/model/models/ModelCounterpartyType.cpp \
    ../../../src/model/models/ModelCurrency.cpp \
    ../../../src/model/models/ModelPaymentType.cpp \
    ../../../src/model/models/ModelInvoiceType.cpp \
    ../../../src/model/models/ModelInvoiceWithCommodities.cpp \
    ../../../ui/BillDialog.cpp \
    ../../../ui/InvoiceDialog.cpp

HEADERS += ../../../src/model/Database.h \
    ../../../src/model/DB_Constants.h \
    ../../../src/model/models_data/UnitData.h \
    ../../../src/model/models_data/PhoneData.h \
    ../../../src/model/models_data/PaymentTypeData.h \
    ../../../src/model/models_data/InvoiceTypeData.h \
    ../../../src/model/models_data/InvoiceData.h \
    ../../../src/model/models_data/EmailData.h \
    ../../../src/model/models_data/CurrencyData.h \
    ../../../src/model/models_data/CountryData.h \
    ../../../src/model/models_data/CounterpartyTypeData.h \
    ../../../src/model/models_data/CounterpartyData.h \
    ../../../src/model/models_data/CommodityTypeData.h \
    ../../../src/model/models_data/CommodityData.h \
    ../../../src/model/models/ModelVat.h \
    ../../../src/model/models/ModelUnit.h \
    ../../../src/model/models/ModelPhone.h \
    ../../../src/model/models/ModelPaymentType.h \
    ../../../src/model/models/ModelInvoiceWithCommodities.h \
    ../../../src/model/models/ModelInvoiceType.h \
    ../../../src/model/models/ModelInvoice.h \
    ../../../src/model/models/ModelEmail.h \
    ../../../src/model/models/ModelCurrency.h \
    ../../../src/model/models/ModelCountry.h \
    ../../../src/model/models/ModelCounterpartyType.h \
    ../../../src/model/models/ModelCounterparty.h \
    ../../../src/model/models/ModelCommodityType.h \
    ../../../src/model/models/ModelCommodity.h \
    ../../../src/ConvertAmount.h \
    ../../../src/SettingsGlobal.h \
    ../../../ui/BillDialog.h \
    ../../../ui/InvoiceDialog.h \
    ../../../ui/CounterpartyDialog.h \
    ../../../ui/CustomPaymentDialog.h \
    ../../../ui/CommodityListDialog.h \
    ../../../ui/ChangeAmountDialog.h \
    ../../../ui/PhoneDialog.h \
    ../../../ui/EmailDialog.h \
    ../../../ui/CountryDialog.h \
    ../../../ui/CounterpartyTypeDialog.h \
    ../../../ui/CounterpartyInfoDialog.h

INCLUDEPATH += ../../../ui \
	../../../src/model \
	../../../src/model/models
