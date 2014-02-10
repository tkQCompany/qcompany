#-------------------------------------------------
#
# Project created by QtCreator 2012-11-01T17:19:36
#
#-------------------------------------------------

TARGET = tst_mainwindowtest
SOURCES += tst_mainwindowtest.cpp \
    ../TestsCommon/GuiUserAddNewCommodity.cpp \
    ../TestsCommon/GuiUserAddCounterparty.cpp \
    ../TestsCommon/GuiUserAddCommodity.cpp \
    ../TestsCommon/GuiUser.cpp \
    ../TestsCommon/CounterpartyDialogPublic.cpp \
    ../TestsCommon/CommodityDialogPublic.cpp \
    ../TestsCommon/CommodityListDialogPublic.cpp \
    ../TestsCommon/MainWindowPublic.cpp \
    ../TestsCommon/GuiUserEditInvoice.cpp \
    ../TestsCommon/InvoiceDialogPublic.cpp

include(../../../qfaktury/qfaktury_common.pri)
include(../qfakturyTestsCommon.pri)

HEADERS += ../TestsCommon/TestsCommon.h \
    ../TestsCommon/GuiUserAddNewCommodity.h \
    ../TestsCommon/GuiUserAddCounterparty.h \
    ../TestsCommon/GuiUserAddCommodity.h \
    ../TestsCommon/GuiUser.h \
    ../TestsCommon/Counterparty_t.h \
    ../TestsCommon/CounterpartyDialogPublic.h \
    ../TestsCommon/CommodityDialogPublic.h \
    ../TestsCommon/CommodityListDialogPublic.h \
    ../TestsCommon/MainWindowPublic.h \
    ../TestsCommon/DialogWithCommodityListDialog.h \
    ../TestsCommon/DialogWithCommodityDialog.h \
    ../TestsCommon/DialogWithCounterpartyDialog.h \
    ../TestsCommon/DialogWithPublicDialog.h \
    ../TestsCommon/GuiUserEditInvoice.h \
    ../TestsCommon/DialogWithInvoiceDialog.h \
    ../TestsCommon/InvoiceDialogPublic.h
