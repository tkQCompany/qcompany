#include <QtCore/QString>
#include <QtTest/QtTest>

class CounterpartyTypeDataTest : public QObject
{
    Q_OBJECT
    
public:
    CounterpartyTypeDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

CounterpartyTypeDataTest::CounterpartyTypeDataTest()
{
}

void CounterpartyTypeDataTest::initTestCase()
{
}

void CounterpartyTypeDataTest::cleanupTestCase()
{
}

void CounterpartyTypeDataTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void CounterpartyTypeDataTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(CounterpartyTypeDataTest)

#include "tst_counterpartytypedatatest.moc"
