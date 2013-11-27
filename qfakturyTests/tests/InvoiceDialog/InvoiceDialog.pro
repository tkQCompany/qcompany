#-------------------------------------------------
#
# Project created by QtCreator 2012-11-01T17:18:49
#
#-------------------------------------------------

TARGET = tst_invoicedialogtest
SOURCES += tst_invoicedialogtest.cpp \
    GuiUser.cpp \
    GuiUserAddCommodity.cpp \
    GuiUserAddCounterparty.cpp \
    CommodityDialogPublic.cpp \
    InvoiceDialogPublic.cpp \
    CounterpartyDialogPublic.cpp \
    GuiUserAddNewCommodity.cpp

include(../../../qfaktury/qfaktury_common.pri)
include(../qfakturyTestsCommon.pri)

HEADERS += \
    GuiUser.h \
    GuiUserAddCommodity.h \
    GuiUserAddCounterparty.h \
    CommodityDialogPublic.h \
    InvoiceDialogPublic.h \
    CounterpartyDialogPublic.h \
    GuiUserAddNewCommodity.h \
    Counterparty_t.h

DEPENDPATH += ../CommodityListDialog
# INCLUDEPATH += ../CommodityListDialog
OBJECTS += ../CommodityListDialog/moc_CommodityListDialogPublic.o
