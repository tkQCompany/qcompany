#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

#include "InvoiceDialog.h"
#include "Database.h"

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
};

InvoiceDialogTest::InvoiceDialogTest()
{
}

void InvoiceDialogTest::initTestCase()
{
    QCoreApplication::setApplicationName("QFaktury");
    QCoreApplication::setOrganizationName("www.e-linux.pl");
    QCoreApplication::setOrganizationDomain("www.e-linux.pl");
    QCoreApplication::setApplicationVersion(APP_VERSION);

    const QString dbFilename(QString("%1-%2.db3").arg(QCoreApplication::applicationName()).arg(APP_VERSION));
    if(QFile::exists(dbFilename))
    {
        QDir dir;
        dir.remove(dbFilename);
    }
}

void InvoiceDialogTest::cleanupTestCase()
{
}

void InvoiceDialogTest::testGUI_InitialState()
{
    Database db;
    InvoiceDialog invoiceDialog(0, &db);
    QCOMPARE(invoiceDialog.windowTitle(), InvoiceTypeData::InvoiceTypeToString(InvoiceTypeData::VAT));
    QCOMPARE(invoiceDialog.comboBoxInvoiceType->currentText(), InvoiceTypeData::InvoiceTypeToString(InvoiceTypeData::VAT));
    QCOMPARE(invoiceDialog.dateEditDateOfIssuance->date(), QDate::currentDate());
    QCOMPARE(invoiceDialog.dateEditDateOfSell->date(), QDate::currentDate());
    QCOMPARE(invoiceDialog.dateEditDayOfPayment->date(), QDate::currentDate());
    QCOMPARE(invoiceDialog.tableWidgetCommodities->rowCount(), 0);
    QCOMPARE(invoiceDialog.checkBoxDiscount->isChecked(), false);
    QCOMPARE(invoiceDialog.spinBoxDiscount->value(), 0);

    QLocale locale;
    QCOMPARE(invoiceDialog.labelSumNetVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(invoiceDialog.labelDiscountVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(invoiceDialog.labelSumGrossVal->text(), locale.toString(0.0, 'f', 2));
}


void InvoiceDialogTest::test_generateInvoiceNumber()
{

}

QTEST_MAIN(InvoiceDialogTest)

#include "tst_invoicedialogtest.moc"
