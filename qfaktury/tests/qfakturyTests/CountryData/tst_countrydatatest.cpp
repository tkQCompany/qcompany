#include <QtTest/QtTest>

#include "../TestsCommon.h"
#include "Database.h"

class CountryDataTest : public QObject
{
    Q_OBJECT
    
public:
    CountryDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
};

CountryDataTest::CountryDataTest()
{
}

void CountryDataTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void CountryDataTest::cleanupTestCase()
{
}

QTEST_APPLESS_MAIN(CountryDataTest)

#include "tst_countrydatatest.moc"
