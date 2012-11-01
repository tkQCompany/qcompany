#include <QtCore/QString>
#include <QtTest/QtTest>

class InvoiceDataTest : public QObject
{
    Q_OBJECT
    
public:
    InvoiceDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

InvoiceDataTest::InvoiceDataTest()
{
}

void InvoiceDataTest::initTestCase()
{
}

void InvoiceDataTest::cleanupTestCase()
{
}

void InvoiceDataTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void InvoiceDataTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(InvoiceDataTest)

#include "tst_invoicedatatest.moc"
