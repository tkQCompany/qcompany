#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

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

void CorrectiveInvoiceDialogTest::cleanupTestCase()
{
}

void CorrectiveInvoiceDialogTest::testGUI_InitialState()
{
    Database db;
    CorrectiveInvoiceDialog corrInvoiceDialog(0, &db);
    QCOMPARE(corrInvoiceDialog.windowTitle(), InvoiceTypeData::InvoiceTypeToString(InvoiceTypeData::CORRECTIVE_VAT));
    QCOMPARE(corrInvoiceDialog.comboBoxInvoiceType->currentText(), InvoiceTypeData::InvoiceTypeToString(InvoiceTypeData::CORRECTIVE_VAT));
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
