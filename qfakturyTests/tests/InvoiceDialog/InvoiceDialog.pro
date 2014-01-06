#-------------------------------------------------
#
# Project created by QtCreator 2012-11-01T17:18:49
#
#-------------------------------------------------

TARGET = tst_invoicedialogtest
SOURCES += tst_invoicedialogtest.cpp \
    ../TestsCommon/InvoiceDialogPublic.cpp \
    ../TestsCommon/GuiUserAddNewCommodity.cpp \
    ../TestsCommon/GuiUserAddCounterparty.cpp \
    ../TestsCommon/GuiUserAddCommodity.cpp \
    ../TestsCommon/GuiUser.cpp \
    ../TestsCommon/CounterpartyDialogPublic.cpp \
    ../TestsCommon/CommodityDialogPublic.cpp

include(../../../qfaktury/qfaktury_common.pri)
include(../qfakturyTestsCommon.pri)

DEPENDPATH += ../CommodityListDialog
OBJECTS += ../CommodityListDialog/moc_CommodityListDialogPublic.o

HEADERS += \
    ../TestsCommon/TestsCommon.h \
    ../TestsCommon/InvoiceDialogPublic.h \
    ../TestsCommon/GuiUserAddNewCommodity.h \
    ../TestsCommon/GuiUserAddCounterparty.h \
    ../TestsCommon/GuiUserAddCommodity.h \
    ../TestsCommon/GuiUser.h \
    ../TestsCommon/Counterparty_t.h \
    ../TestsCommon/CounterpartyDialogPublic.h \
    ../TestsCommon/CommodityDialogPublic.h
