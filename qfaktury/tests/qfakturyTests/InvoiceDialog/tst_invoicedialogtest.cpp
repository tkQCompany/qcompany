#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

class InvoiceDialogTest : public QObject
{
    Q_OBJECT
    
public:
    InvoiceDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

InvoiceDialogTest::InvoiceDialogTest()
{
}

void InvoiceDialogTest::initTestCase()
{
}

void InvoiceDialogTest::cleanupTestCase()
{
}

void InvoiceDialogTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void InvoiceDialogTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(InvoiceDialogTest)

#include "tst_invoicedialogtest.moc"
