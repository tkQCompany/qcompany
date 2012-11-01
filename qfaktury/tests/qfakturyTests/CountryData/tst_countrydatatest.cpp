#include <QtCore/QString>
#include <QtTest/QtTest>

class CountryDataTest : public QObject
{
    Q_OBJECT
    
public:
    CountryDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

CountryDataTest::CountryDataTest()
{
}

void CountryDataTest::initTestCase()
{
}

void CountryDataTest::cleanupTestCase()
{
}

void CountryDataTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void CountryDataTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(CountryDataTest)

#include "tst_countrydatatest.moc"
