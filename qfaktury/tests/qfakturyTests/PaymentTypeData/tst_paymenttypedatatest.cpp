#include <QtCore/QString>
#include <QtTest/QtTest>

class PaymentTypeDataTest : public QObject
{
    Q_OBJECT
    
public:
    PaymentTypeDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

PaymentTypeDataTest::PaymentTypeDataTest()
{
}

void PaymentTypeDataTest::initTestCase()
{
}

void PaymentTypeDataTest::cleanupTestCase()
{
}

void PaymentTypeDataTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void PaymentTypeDataTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(PaymentTypeDataTest)

#include "tst_paymenttypedatatest.moc"
