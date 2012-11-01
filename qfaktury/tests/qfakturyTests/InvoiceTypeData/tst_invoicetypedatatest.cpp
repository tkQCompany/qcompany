#include <QtCore/QString>
#include <QtTest/QtTest>

class InvoiceTypeDataTest : public QObject
{
    Q_OBJECT
    
public:
    InvoiceTypeDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

InvoiceTypeDataTest::InvoiceTypeDataTest()
{
}

void InvoiceTypeDataTest::initTestCase()
{
}

void InvoiceTypeDataTest::cleanupTestCase()
{
}

void InvoiceTypeDataTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void InvoiceTypeDataTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(InvoiceTypeDataTest)

#include "tst_invoicetypedatatest.moc"
