#include <QtTest/QtTest>

#include "../TestsCommon.h"
#include "InvoiceGrossDialog.h"
#include "Database.h"

class InvoiceGrossDialogTest : public QObject
{
    Q_OBJECT
    
public:
    InvoiceGrossDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testGUI_InitialState();
};

InvoiceGrossDialogTest::InvoiceGrossDialogTest()
{
}

void InvoiceGrossDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void InvoiceGrossDialogTest::cleanupTestCase()
{
}

void InvoiceGrossDialogTest::testGUI_InitialState()
{
    Database db;
    InvoiceGrossDialog invoiceGrossDialog(0, &db);
    QCOMPARE(invoiceGrossDialog.windowTitle(), InvoiceTypeData::name(InvoiceTypeData::GROSS));
    QCOMPARE(invoiceGrossDialog.comboBoxInvoiceType->currentText(), InvoiceTypeData::name(InvoiceTypeData::GROSS));
    QCOMPARE(invoiceGrossDialog.dateEditDateOfIssuance->date(), QDate::currentDate());
    QCOMPARE(invoiceGrossDialog.dateEditDateOfSell->date(), QDate::currentDate());
    QCOMPARE(invoiceGrossDialog.dateEditDayOfPayment->date(), QDate::currentDate());
    QCOMPARE(invoiceGrossDialog.tableWidgetCommodities->rowCount(), 0);
    QCOMPARE(invoiceGrossDialog.checkBoxDiscount->isChecked(), false);
    QCOMPARE(invoiceGrossDialog.spinBoxDiscount->value(), 0);

    QLocale locale;
    QCOMPARE(invoiceGrossDialog.labelSumNetVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(invoiceGrossDialog.labelDiscountVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(invoiceGrossDialog.labelSumGrossVal->text(), locale.toString(0.0, 'f', 2));
}



QTEST_MAIN(InvoiceGrossDialogTest)

#include "tst_invoicegrossdialogtest.moc"
