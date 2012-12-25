QT += sql xml

SOURCES +=     ui/BillDialog.cpp \
    ui/ChangeAmountDialog.cpp \
    ui/ComboBoxCurrentTextRW.cpp \
    ui/CommodityDialog.cpp \
    ui/CommodityListDialog.cpp \
    ui/CommodityListGrossDialog.cpp \
    ui/CorrectiveInvoiceDialog.cpp \
    ui/CorrectiveInvoiceGrossDialog.cpp \
    ui/CounterpartyDialog.cpp \
    ui/CounterpartyInfoDialog.cpp \
    ui/CounterpartyTypeDialog.cpp \
    ui/CountryDialog.cpp \
    ui/CustomPaymentDialog.cpp \
    ui/DuplicateDialog.cpp \
    ui/EmailDialog.cpp \
    ui/InvoiceDialog.cpp \
    ui/InvoiceGrossDialog.cpp \
    ui/MainWindow.cpp \
    ui/PhoneDialog.cpp \
    ui/SettingsDialog.cpp \
    src/ConvertAmount.cpp \
    src/CustomPaymData.cpp \
    src/model/Database.cpp \
    src/model/models/ModelCommodity.cpp \
    src/model/models/ModelCommodityType.cpp \
    src/model/models/ModelCountry.cpp \
    src/model/models/ModelEmail.cpp \
    src/model/models/ModelPhone.cpp \
    src/model/models/ModelUnit.cpp \
    src/model/models/ModelVat.cpp \
    src/model/models/ModelInvoice.cpp \
    src/model/models/ModelCounterparty.cpp \
    src/model/models/ModelCounterpartyType.cpp \
    src/model/models/ModelCurrency.cpp \
    src/model/models/ModelPaymentType.cpp \
    src/model/models/ModelInvoiceType.cpp \
    src/model/models/ModelInvoiceWithCommodities.cpp

HEADERS += ui/BillDialog.h \
    ui/ChangeAmountDialog.h \
    ui/ComboBoxCurrentTextRW.h \
    ui/CommodityDialog.h \
    ui/CommodityListDialog.h \
    ui/CommodityListGrossDialog.h \
    ui/CorrectiveInvoiceDialog.h \
    ui/CorrectiveInvoiceGrossDialog.h \
    ui/CounterpartyDialog.h \
    ui/CounterpartyInfoDialog.h \
    ui/CounterpartyTypeDialog.h \
    ui/CountryDialog.h \
    ui/CustomPaymentDialog.h \
    ui/DuplicateDialog.h \
    ui/EmailDialog.h \
    ui/InvoiceDialog.h \
    ui/InvoiceGrossDialog.h \
    ui/MainWindow.h \
    ui/PhoneDialog.h \
    ui/SettingsDialog.h \
    src/ConvertAmount.h \
    src/CustomPaymData.h \
    src/model/Database.h \
    src/model/DB_Constants.h \
    src/model/models_data/UnitData.h \
    src/model/models_data/PhoneData.h \
    src/model/models_data/PaymentTypeData.h \
    src/model/models_data/InvoiceTypeData.h \
    src/model/models_data/InvoiceData.h \
    src/model/models_data/EmailData.h \
    src/model/models_data/CurrencyData.h \
    src/model/models_data/CountryData.h \
    src/model/models_data/CounterpartyTypeData.h \
    src/model/models_data/CounterpartyData.h \
    src/model/models_data/CommodityTypeData.h \
    src/model/models_data/CommodityData.h \
    src/model/models/ModelVat.h \
    src/model/models/ModelUnit.h \
    src/model/models/ModelPhone.h \
    src/model/models/ModelPaymentType.h \
    src/model/models/ModelInvoiceWithCommodities.h \
    src/model/models/ModelInvoiceType.h \
    src/model/models/ModelInvoice.h \
    src/model/models/ModelEmail.h \
    src/model/models/ModelCurrency.h \
    src/model/models/ModelCountry.h \
    src/model/models/ModelCounterpartyType.h \
    src/model/models/ModelCounterparty.h \
    src/model/models/ModelCommodityType.h \
    src/model/models/ModelCommodity.h \
    src/ConvertAmount.h \
    src/SettingsGlobal.h

OBJECTS_DIR = obj
MOC_DIR = moc_files

# The application version
VERSION = 0.7.0
# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

