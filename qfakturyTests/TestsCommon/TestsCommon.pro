#-------------------------------------------------
#
# Project created by QtCreator 2014-03-29T00:25:37
#
#-------------------------------------------------

QT       += widgets sql testlib printsupport

TARGET = TestsCommon
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    CommodityDialogPublic.cpp \
    CommodityListDialogPublic.cpp \
    CounterpartyDialogPublic.cpp \
    GuiUser.cpp \
    GuiUserAddCommodity.cpp \
    GuiUserAddCounterparty.cpp \
    GuiUserAddNewCommodity.cpp \
    GuiUserEditInvoice.cpp \
    InvoiceDialogPublic.cpp \
    MainWindowPublic.cpp \
    InvoiceSums.cpp

HEADERS += \
    CommodityDialogPublic.h \
    CommodityListDialogPublic.h \
    Counterparty_t.h \
    CounterpartyDialogPublic.h \
    DialogWithCommodityDialog.h \
    DialogWithCommodityListDialog.h \
    DialogWithCounterpartyDialog.h \
    DialogWithInvoiceDialog.h \
    DialogWithPublicDialog.h \
    GuiUser.h \
    GuiUserAddCommodity.h \
    GuiUserAddCounterparty.h \
    GuiUserAddNewCommodity.h \
    GuiUserEditInvoice.h \
    InvoiceDialogPublic.h \
    MainWindowPublic.h \
    TestsCommon.h \
    InvoiceSums.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}


DEPENDPATH += ../../qfaktury \
               ../../qfaktury/src


INCLUDEPATH +=  ../../qfaktury/src \
               ../../qfaktury/src/ui \
               ../../qfaktury/ui_files \
               ../../qfaktury/src/model \
               ../../qfaktury/src/model/models \
               ../../qfaktury/src/model/models_data
