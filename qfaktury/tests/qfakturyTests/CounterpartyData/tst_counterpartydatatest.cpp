#include <QtCore/QString>
#include <QtTest/QtTest>

class CounterpartyDataTest : public QObject
{
    Q_OBJECT
    
public:
    CounterpartyDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

CounterpartyDataTest::CounterpartyDataTest()
{
}

void CounterpartyDataTest::initTestCase()
{
}

void CounterpartyDataTest::cleanupTestCase()
{
}

void CounterpartyDataTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void CounterpartyDataTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(CounterpartyDataTest)

#include "tst_counterpartydatatest.moc"
