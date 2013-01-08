#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

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

void InvoiceGrossDialogTest::cleanupTestCase()
{
}

void InvoiceGrossDialogTest::testGUI_InitialState()
{
    Database db;
    InvoiceGrossDialog invoiceGrossDialog(0, &db);
    QCOMPARE(invoiceGrossDialog.windowTitle(), InvoiceTypeData::InvoiceTypeToString(InvoiceTypeData::GROSS));
    QCOMPARE(invoiceGrossDialog.comboBoxInvoiceType->currentText(), InvoiceTypeData::InvoiceTypeToString(InvoiceTypeData::GROSS));
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
