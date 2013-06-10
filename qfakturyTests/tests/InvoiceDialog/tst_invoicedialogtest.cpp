#include <QtTest/QtTest>

#include "../TestsCommon.h"
#include "InvoiceDialog.h"
#include "InvoiceDialog.cpp"
#include "Database.h"
#include "SettingsGlobal.h"

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
    
public:
    InvoiceDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testGUI_InitialState();
    void test_generateInvoiceNumber();
    void test_compatibilityWithOldGenerateInvoiceNumber();

private:
    QString generateInvoiceNumberOldVer(InvoiceTypeData::Type invType);
    QString numbersCount(int in, int x);
};

InvoiceDialogTest::InvoiceDialogTest()
{
}

void InvoiceDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void InvoiceDialogTest::cleanupTestCase()
{
}

void InvoiceDialogTest::testGUI_InitialState()
{
    Database db;
    InvoiceDialogPublic invoiceDialog(0, &db, InvoiceTypeData::VAT);
    QCOMPARE(invoiceDialog.windowTitle(), trUtf8("Nowy dokument - %1 [*]").arg(InvoiceTypeData::name(InvoiceTypeData::VAT)));
    QCOMPARE(invoiceDialog.ui()->comboBoxInvoiceType->currentText(), InvoiceTypeData::name(InvoiceTypeData::VAT));
    QCOMPARE(invoiceDialog.ui()->dateEditDateOfIssuance->date(), QDate::currentDate());
    QCOMPARE(invoiceDialog.ui()->dateEditDateOfSell->date(), QDate::currentDate());
    QCOMPARE(invoiceDialog.ui()->dateEditDayOfPayment->date(), QDate::currentDate());
    QCOMPARE(invoiceDialog.ui()->tableWidgetCommodities->rowCount(), 0);
    QCOMPARE(invoiceDialog.ui()->checkBoxDiscount->isChecked(), false);
    QCOMPARE(invoiceDialog.ui()->spinBoxDiscount->value(), 0);

    QLocale locale;
    QCOMPARE(invoiceDialog.ui()->labelSumNetVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(invoiceDialog.ui()->labelDiscountVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(invoiceDialog.ui()->labelSumGrossVal->text(), locale.toString(0.0, 'f', 2));
}


void InvoiceDialogTest::test_generateInvoiceNumber()
{
    //InvoiceDialog::InvoiceNumChosenPeriod week = InvoiceDialog::WEEK;
    QFAIL("implement me");
}

void InvoiceDialogTest::test_compatibilityWithOldGenerateInvoiceNumber()
{
    QFAIL("implement me");
}


QString InvoiceDialogTest::generateInvoiceNumberOldVer(InvoiceTypeData::Type invType)
{//old code - for checking compatibility with previous versions
    QString tmp, prefix, suffix;
    SettingsGlobal s;

    if(invType == InvoiceTypeData::PRO_FORMA)
    {
        tmp = s.value("fpro").toString();
    }
    else
    {
        tmp = s.value("fvat").toString();
    }

    prefix = s.value("prefix").toString();

    QStringList one1 = tmp.split("/");
    one1[0] = one1[0].remove(prefix);

    int nr = one1[0].toInt() + 1;
    QString lastInvoice = prefix + numbersCount(nr, 0);

    if (s.value("day") .toBool())
        lastInvoice += "/" + QDate::currentDate().toString("dd");

    if (false) //sett().value("month") .toBool()
        lastInvoice += "/" + QDate::currentDate().toString("MM");

    if (s.value("year") .toBool()) {
        if (!s.value("shortYear") .toBool())
            lastInvoice += "/" + QDate::currentDate().toString("yy");
        else
            lastInvoice += "/" + QDate::currentDate().toString("yyyy");
    }
    suffix = s.value("sufix").toString();
    lastInvoice += suffix;

    return lastInvoice;
}


QString InvoiceDialogTest::numbersCount(int in, int x) {//old code - for checking compatibility with previous versions
    SettingsGlobal s;
    QString tmp2, tmp = s.numberToString(in);
    tmp2 = "";
    int incr = x - tmp.length();
    for (int i = 0; i < incr; ++i)
        tmp2 += "0";
    return tmp2 + tmp;
}

QTEST_MAIN(InvoiceDialogTest)

#include "tst_invoicedialogtest.moc"
