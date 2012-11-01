#include <QtCore/QString>
#include <QtTest/QtTest>

class CommodityDataTest : public QObject
{
    Q_OBJECT
    
public:
    CommodityDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

CommodityDataTest::CommodityDataTest()
{
}

void CommodityDataTest::initTestCase()
{
}

void CommodityDataTest::cleanupTestCase()
{
}

void CommodityDataTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void CommodityDataTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(CommodityDataTest)

#include "tst_commoditydatatest.moc"
