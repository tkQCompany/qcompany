#include <QString>
#include <QtTest>

#define private public

#include "InvoiceComposer.h"

class InvoiceComposerTest : public QObject
{
    Q_OBJECT

public:
    InvoiceComposerTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseDefault();
    void testCase1();
    void testCase1_data();
};

InvoiceComposerTest::InvoiceComposerTest()
{
}

void InvoiceComposerTest::initTestCase()
{
}

void InvoiceComposerTest::cleanupTestCase()
{
}

void InvoiceComposerTest::testCaseDefault()
{
    InvoiceComposer ic;
    QVERIFY(ic.getInvoiceHtml().isEmpty());
}

void InvoiceComposerTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void InvoiceComposerTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(InvoiceComposerTest)

#include "tst_invoicecomposertest.moc"
