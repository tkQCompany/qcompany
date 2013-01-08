#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

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

void CorrectiveInvoiceGrossDialogTest::cleanupTestCase()
{
}

void CorrectiveInvoiceGrossDialogTest::testGUI_InitialState()
{
    Database db;
    CorrectiveInvoiceGrossDialog corrInvoiceGrossDialog(0, &db);
    QCOMPARE(corrInvoiceGrossDialog.windowTitle(), InvoiceTypeData::InvoiceTypeToString(InvoiceTypeData::CORRECTIVE_GROSS));
    QCOMPARE(corrInvoiceGrossDialog.comboBoxInvoiceType->currentText(), InvoiceTypeData::InvoiceTypeToString(InvoiceTypeData::CORRECTIVE_GROSS));
    QCOMPARE(corrInvoiceGrossDialog.dateEditDateOfIssuance->date(), QDate::currentDate());
    QCOMPARE(corrInvoiceGrossDialog.dateEditDateOfSell->date(), QDate::currentDate());
    QCOMPARE(corrInvoiceGrossDialog.dateEditDayOfPayment->date(), QDate::currentDate());
    QCOMPARE(corrInvoiceGrossDialog.tableWidgetCommodities->rowCount(), 0);
    QCOMPARE(corrInvoiceGrossDialog.checkBoxDiscount->isChecked(), false);
    QCOMPARE(corrInvoiceGrossDialog.spinBoxDiscount->value(), 0);

    QLocale locale;
    QCOMPARE(corrInvoiceGrossDialog.labelSumNetVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(corrInvoiceGrossDialog.labelDiscountVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(corrInvoiceGrossDialog.labelSumGrossVal->text(), locale.toString(0.0, 'f', 2));
}


QTEST_MAIN(CorrectiveInvoiceGrossDialogTest)

#include "tst_correctiveinvoicegrossdialogtest.moc"
