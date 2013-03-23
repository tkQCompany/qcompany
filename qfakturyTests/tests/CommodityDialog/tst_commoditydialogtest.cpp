#include <QtTest/QtTest>

#include "../TestsCommon.h"

class CommodityDialogTest : public QObject
{
    Q_OBJECT
    
public:
    CommodityDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

CommodityDialogTest::CommodityDialogTest()
{
}

void CommodityDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void CommodityDialogTest::cleanupTestCase()
{
}

void CommodityDialogTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void CommodityDialogTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(CommodityDialogTest)

#include "tst_commoditydialogtest.moc"
