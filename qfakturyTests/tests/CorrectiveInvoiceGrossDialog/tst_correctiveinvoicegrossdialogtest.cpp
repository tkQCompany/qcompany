#include <QtTest/QtTest>

#include "../TestsCommon.h"
#include "CorrectiveInvoiceGrossDialog.h"
#include "Database.h"

class CorrectiveInvoiceGrossDialogTest : public QObject
{
    Q_OBJECT
    
public:
    CorrectiveInvoiceGrossDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testGUI_InitialState();
};

CorrectiveInvoiceGrossDialogTest::CorrectiveInvoiceGrossDialogTest()
{
}

void CorrectiveInvoiceGrossDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void CorrectiveInvoiceGrossDialogTest::cleanupTestCase()
{
}

void CorrectiveInvoiceGrossDialogTest::testGUI_InitialState()
{
    //Database db;
    //CorrectiveInvoiceGrossDialog corrInvoiceGrossDialog(0, &db, InvoiceTypeData::CORRECTIVE_GROSS);
//    QCOMPARE(corrInvoiceGrossDialog.windowTitle(), InvoiceTypeData::name(InvoiceTypeData::CORRECTIVE_GROSS));
//    QCOMPARE(corrInvoiceGrossDialog.comboBoxInvoiceType->currentText(), InvoiceTypeData::name(InvoiceTypeData::CORRECTIVE_GROSS));
//    QCOMPARE(corrInvoiceGrossDialog.dateEditDateOfIssuance->date(), QDate::currentDate());
//    QCOMPARE(corrInvoiceGrossDialog.dateEditDateOfSell->date(), QDate::currentDate());
//    QCOMPARE(corrInvoiceGrossDialog.dateEditDayOfPayment->date(), QDate::currentDate());
//    QCOMPARE(corrInvoiceGrossDialog.tableWidgetCommodities->rowCount(), 0);
//    QCOMPARE(corrInvoiceGrossDialog.checkBoxDiscount->isChecked(), false);
//    QCOMPARE(corrInvoiceGrossDialog.spinBoxDiscount->value(), 0);

//    QLocale locale;
//    QCOMPARE(corrInvoiceGrossDialog.labelSumNetVal->text(), locale.toString(0.0, 'f', 2));
//    QCOMPARE(corrInvoiceGrossDialog.labelDiscountVal->text(), locale.toString(0.0, 'f', 2));
//    QCOMPARE(corrInvoiceGrossDialog.labelSumGrossVal->text(), locale.toString(0.0, 'f', 2));
}


QTEST_MAIN(CorrectiveInvoiceGrossDialogTest)

#include "tst_correctiveinvoicegrossdialogtest.moc"
