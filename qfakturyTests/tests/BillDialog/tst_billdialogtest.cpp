#include <QtTest/QtTest>
#include <QtGui>

#include "../../TestsCommon/TestsCommon.h"
#include "BillDialog.h"
#include "BillDialog.cpp"
#include "Database.h"
#include "InvoiceTypeData.h"

namespace Ui
{
class InvoiceDialog;
}

struct BillDialogPublic: public BillDialog
{
    BillDialogPublic(QWidget *parent, Database *database)
        : BillDialog(parent, database)
    {

    }

    Ui::InvoiceDialog *ui() {return pImpl_->ui;}
};

class BillDialogTest : public QObject
{
    Q_OBJECT
    
public:
    BillDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void init();
    void testGUI_InitialState();
};

BillDialogTest::BillDialogTest()
{
}

void BillDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    SettingsGlobal s;
    s.setFirstRun(true);
}

void BillDialogTest::init()
{
    TestsCommon::removeDBFile();
}


void BillDialogTest::testGUI_InitialState()
{
    Database db;
    BillDialogPublic billDialog(0, &db);
    QCOMPARE(billDialog.windowTitle(), trUtf8("Nowy dokument - %1 [*]").arg(InvoiceTypeData::name(InvoiceTypeData::BILL)));
    QCOMPARE(billDialog.ui()->comboBoxInvoiceType->currentText(), InvoiceTypeData::name(InvoiceTypeData::BILL));
    QCOMPARE(billDialog.ui()->dateEditDateOfIssuance->date(), QDate::currentDate());
    QCOMPARE(billDialog.ui()->dateEditDateOfSell->date(), QDate::currentDate());
    QCOMPARE(billDialog.ui()->dateEditDayOfPayment->date(), QDate::currentDate());
    QCOMPARE(billDialog.ui()->tableWidgetCommodities->rowCount(), 0);
    QCOMPARE(billDialog.ui()->checkBoxDiscount->isChecked(), false);
    QCOMPARE(billDialog.ui()->spinBoxDiscount->value(), 0);

    QLocale locale;
    QCOMPARE(billDialog.ui()->labelSumNetVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(billDialog.ui()->labelDiscountVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(billDialog.ui()->labelSumGrossVal->text(), locale.toString(0.0, 'f', 2));
}


QTEST_MAIN(BillDialogTest)

#include "tst_billdialogtest.moc"
