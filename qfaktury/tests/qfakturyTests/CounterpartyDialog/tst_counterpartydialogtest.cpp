#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

class CounterpartyDialogTest : public QObject
{
    Q_OBJECT
    
public:
    CounterpartyDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

CounterpartyDialogTest::CounterpartyDialogTest()
{
}

void CounterpartyDialogTest::initTestCase()
{
}

void CounterpartyDialogTest::cleanupTestCase()
{
}

void CounterpartyDialogTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void CounterpartyDialogTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(CounterpartyDialogTest)

#include "tst_counterpartydialogtest.moc"
