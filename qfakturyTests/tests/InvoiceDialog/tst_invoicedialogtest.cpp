#include <QtTest/QtTest>

#include "../TestsCommon.h"
#include "InvoiceDialog.h"
#include "InvoiceDialog.cpp"
#include "Database.h"
#include "SettingsGlobal.h"
#include "CommodityVisualData.h"
#include "PaymentTypeData.h"
#include "CurrencyData.h"

struct InvoiceDialogPublic: public InvoiceDialog
{
    InvoiceDialogPublic(QWidget *parent, Database *db, InvoiceTypeData::Type invoiceType, const QModelIndex &idEdit = QModelIndex())
        : InvoiceDialog(parent, db, invoiceType, idEdit)
    {

    }

    Ui::InvoiceDialog *ui() {return pImpl_->ui; }
};

class InvoiceDialogTest : public QObject
{
    Q_OBJECT
    
private Q_SLOTS:
    void initTestCase();
    void testGUI_InitialState();
};


void InvoiceDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
    SettingsGlobal s;
    s.setFirstRun(true);
}



void InvoiceDialogTest::testGUI_InitialState()
{
    SettingsGlobal s;
    Database db;
    InvoiceDialogPublic invoiceDialog(0, &db, InvoiceTypeData::VAT);

    QCOMPARE(invoiceDialog.windowTitle(), trUtf8("Nowy dokument - %1 [*]").arg(InvoiceTypeData::name(InvoiceTypeData::VAT)));
    QVERIFY( ! invoiceDialog.ui()->lineEditInvNumFormat->text().isEmpty());
    QCOMPARE(invoiceDialog.ui()->dateEditDateOfIssuance->date(), QDate::currentDate());
    QCOMPARE(invoiceDialog.ui()->comboBoxInvoiceType->currentText(), InvoiceTypeData::name(InvoiceTypeData::VAT));
    QVERIFY( ! invoiceDialog.ui()->lineEditInvNumber->text().isEmpty());

    QCOMPARE(invoiceDialog.ui()->tableWidgetCommodities->rowCount(), 0);
    QCOMPARE(invoiceDialog.ui()->tableWidgetCommodities->columnCount(), CommodityVisualFields::DISCOUNT - CommodityVisualFields::ID + 1);
    QCOMPARE(invoiceDialog.ui()->checkBoxDiscount->isChecked(), false);
    QCOMPARE(invoiceDialog.ui()->spinBoxDiscount->value(), 0);

    QCOMPARE(invoiceDialog.ui()->dateEditDateOfSell->date(), QDate::currentDate());
    QCOMPARE(invoiceDialog.ui()->dateEditDayOfPayment->date(), QDate::currentDate());
    QCOMPARE(invoiceDialog.ui()->tableWidgetCommodities->rowCount(), 0);
    QVERIFY(invoiceDialog.ui()->comboBoxReasonOfCorrection->currentText().isEmpty());
    QCOMPARE(invoiceDialog.ui()->comboBoxPayment->currentText(), PaymentTypeData::name(PaymentTypeData::CASH));
    QCOMPARE(invoiceDialog.ui()->comboBoxCurrency->currentText(), s.value(s.DEFAULT_CURRENCY).toString());

    QLocale locale;
    QCOMPARE(invoiceDialog.ui()->labelSumNetVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(invoiceDialog.ui()->labelDiscountVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(invoiceDialog.ui()->labelSumGrossVal->text(), locale.toString(0.0, 'f', 2));

    QVERIFY(invoiceDialog.ui()->lineEditAdditionalText->text() == s.value(s.ADDIT_TEXT).toString());
}


QTEST_MAIN(InvoiceDialogTest)

#include "tst_invoicedialogtest.moc"
