#include <QtTest/QtTest>

#include "../TestsCommon.h"
#include "CorrectiveInvoiceDialog.h"
#include "Database.h"

class CorrectiveInvoiceDialogTest : public QObject
{
    Q_OBJECT
    
public:
    CorrectiveInvoiceDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testGUI_InitialState();
};

CorrectiveInvoiceDialogTest::CorrectiveInvoiceDialogTest()
{
}

void CorrectiveInvoiceDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void CorrectiveInvoiceDialogTest::cleanupTestCase()
{
}

void CorrectiveInvoiceDialogTest::testGUI_InitialState()
{
    Database db;
    CorrectiveInvoiceDialog corrInvoiceDialog(0, &db);
    QCOMPARE(corrInvoiceDialog.windowTitle(), InvoiceTypeData::name(InvoiceTypeData::CORRECTIVE_VAT));
    QCOMPARE(corrInvoiceDialog.comboBoxInvoiceType->currentText(), InvoiceTypeData::name(InvoiceTypeData::CORRECTIVE_VAT));
    QCOMPARE(corrInvoiceDialog.dateEditDateOfIssuance->date(), QDate::currentDate());
    QCOMPARE(corrInvoiceDialog.dateEditDateOfSell->date(), QDate::currentDate());
    QCOMPARE(corrInvoiceDialog.dateEditDayOfPayment->date(), QDate::currentDate());
    QCOMPARE(corrInvoiceDialog.tableWidgetCommodities->rowCount(), 0);
    QCOMPARE(corrInvoiceDialog.checkBoxDiscount->isChecked(), false);
    QCOMPARE(corrInvoiceDialog.spinBoxDiscount->value(), 0);

    QLocale locale;
    QCOMPARE(corrInvoiceDialog.labelSumNetVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(corrInvoiceDialog.labelDiscountVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(corrInvoiceDialog.labelSumGrossVal->text(), locale.toString(0.0, 'f', 2));
}


QTEST_MAIN(CorrectiveInvoiceDialogTest)

#include "tst_correctiveinvoicedialogtest.moc"
