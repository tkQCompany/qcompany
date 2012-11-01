#include <QtCore/QString>
#include <QtTest/QtTest>

class CurrencyDataTest : public QObject
{
    Q_OBJECT
    
public:
    CurrencyDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

CurrencyDataTest::CurrencyDataTest()
{
}

void CurrencyDataTest::initTestCase()
{
}

void CurrencyDataTest::cleanupTestCase()
{
}

void CurrencyDataTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void CurrencyDataTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(CurrencyDataTest)

#include "tst_currencydatatest.moc"
