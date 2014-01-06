#include <QtTest/QtTest>

#include "../TestsCommon/TestsCommon.h"

class CounterpartyInfoDialogTest : public QObject
{
    Q_OBJECT
    
public:
    CounterpartyInfoDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

CounterpartyInfoDialogTest::CounterpartyInfoDialogTest()
{
}

void CounterpartyInfoDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void CounterpartyInfoDialogTest::cleanupTestCase()
{
}

void CounterpartyInfoDialogTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void CounterpartyInfoDialogTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(CounterpartyInfoDialogTest)

#include "tst_counterpartyinfodialogtest.moc"
