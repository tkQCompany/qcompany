#include <QtTest/QtTest>

#include "../TestsCommon/TestsCommon.h"
#include "CorrectiveInvoiceDialog.h"
#include "CorrectiveInvoiceDialog.cpp"
#include "Database.h"

namespace Ui
{
class InvoiceDialog;
}

struct CorrectiveInvoiceDialogPublic: public CorrectiveInvoiceDialog
{
    CorrectiveInvoiceDialogPublic(QWidget *parent,
                                  Database *db,
                                  InvoiceTypeData::Type invoiceType,
                                  const QModelIndex &idInvoice = QModelIndex())
        : CorrectiveInvoiceDialog(parent, db, invoiceType, idInvoice)
    {
    }

    const Ui::InvoiceDialog *ui() {return pImpl_->ui;}
};

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
    CorrectiveInvoiceDialogPublic corrInvoiceDialog(0, &db, InvoiceTypeData::CORRECTIVE_VAT);
    QCOMPARE(corrInvoiceDialog.windowTitle(), trUtf8("Nowy dokument - %1 [*]").arg(InvoiceTypeData::name(InvoiceTypeData::CORRECTIVE_VAT)));
    QCOMPARE(corrInvoiceDialog.ui()->comboBoxInvoiceType->currentText(), InvoiceTypeData::name(InvoiceTypeData::CORRECTIVE_VAT));
    QCOMPARE(corrInvoiceDialog.ui()->dateEditDateOfIssuance->date(), QDate::currentDate());
    QCOMPARE(corrInvoiceDialog.ui()->dateEditDateOfSell->date(), QDate::currentDate());
    QCOMPARE(corrInvoiceDialog.ui()->dateEditDayOfPayment->date(), QDate::currentDate());
    QCOMPARE(corrInvoiceDialog.ui()->tableWidgetCommodities->rowCount(), 0);
    QCOMPARE(corrInvoiceDialog.ui()->checkBoxDiscount->isChecked(), false);
    QCOMPARE(corrInvoiceDialog.ui()->spinBoxDiscount->value(), 0);

    QLocale locale;
    QCOMPARE(corrInvoiceDialog.ui()->labelSumNetVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(corrInvoiceDialog.ui()->labelDiscountVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(corrInvoiceDialog.ui()->labelSumGrossVal->text(), locale.toString(0.0, 'f', 2));
}


QTEST_MAIN(CorrectiveInvoiceDialogTest)

#include "tst_correctiveinvoicedialogtest.moc"
