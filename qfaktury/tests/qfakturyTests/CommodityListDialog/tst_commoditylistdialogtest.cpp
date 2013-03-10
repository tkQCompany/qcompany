#include <QtTest/QtTest>

#include "../TestsCommon.h"

class CommodityListDialogTest : public QObject
{
    Q_OBJECT
    
public:
    CommodityListDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

CommodityListDialogTest::CommodityListDialogTest()
{
}

void CommodityListDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void CommodityListDialogTest::cleanupTestCase()
{
}

void CommodityListDialogTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void CommodityListDialogTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(CommodityListDialogTest)

#include "tst_commoditylistdialogtest.moc"
