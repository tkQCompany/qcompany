#include <QtCore/QString>
#include <QtTest/QtTest>

class CommodityTypeDataTest : public QObject
{
    Q_OBJECT
    
public:
    CommodityTypeDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

CommodityTypeDataTest::CommodityTypeDataTest()
{
}

void CommodityTypeDataTest::initTestCase()
{
}

void CommodityTypeDataTest::cleanupTestCase()
{
}

void CommodityTypeDataTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void CommodityTypeDataTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(CommodityTypeDataTest)

#include "tst_commoditytypedatatest.moc"
