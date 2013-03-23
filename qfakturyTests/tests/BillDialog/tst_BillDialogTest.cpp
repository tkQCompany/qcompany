#include <QtTest/QtTest>
#include <QtGui>

#include "../TestsCommon.h"
#include "BillDialog.h"
#include "Database.h"

class BillDialogTest : public QObject
{
    Q_OBJECT
    
public:
    BillDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testGUI_InitialState();
};

BillDialogTest::BillDialogTest()
{
}

void BillDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void BillDialogTest::cleanupTestCase()
{
}


void BillDialogTest::testGUI_InitialState()
{
    Database db;
    BillDialog billDialog(0, &db);
    QCOMPARE(billDialog.windowTitle(), InvoiceTypeData::name(InvoiceTypeData::BILL));
//    QCOMPARE(billDialog.ui_->comboBoxInvoiceType->currentText(), InvoiceTypeData::name(InvoiceTypeData::BILL));
//    QCOMPARE(billDialog.ui_->dateEditDateOfIssuance->date(), QDate::currentDate());
//    QCOMPARE(billDialog.ui_->dateEditDateOfSell->date(), QDate::currentDate());
//    QCOMPARE(billDialog.ui_->dateEditDayOfPayment->date(), QDate::currentDate());
//    QCOMPARE(billDialog.ui_->tableWidgetCommodities->rowCount(), 0);
//    QCOMPARE(billDialog.ui_->checkBoxDiscount->isChecked(), false);
//    QCOMPARE(billDialog.ui_->spinBoxDiscount->value(), 0);

//    QLocale locale;
//    QCOMPARE(billDialog.ui_->labelSumNetVal->text(), locale.toString(0.0, 'f', 2));
//    QCOMPARE(billDialog.ui_->labelDiscountVal->text(), locale.toString(0.0, 'f', 2));
//    QCOMPARE(billDialog.ui_->labelSumGrossVal->text(), locale.toString(0.0, 'f', 2));
}


QTEST_MAIN(BillDialogTest)

#include "tst_BillDialogTest.moc"
