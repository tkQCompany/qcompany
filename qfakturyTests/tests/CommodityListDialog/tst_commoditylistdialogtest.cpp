#include <QtTest/QtTest>

#include "../TestsCommon.h"
#include "CommodityListDialogPublic.h"

class CommodityListDialogTest : public QObject
{
    Q_OBJECT
    
public:
    CommodityListDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testGUI_InitialState();
    void testGUI_AddedOneCommodity();
    void testGUI_AddedOneCommodity_data();
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

void CommodityListDialogTest::testGUI_InitialState()
{

}


void CommodityListDialogTest::testGUI_AddedOneCommodity()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}


void CommodityListDialogTest::testGUI_AddedOneCommodity_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(CommodityListDialogTest)

#include "tst_commoditylistdialogtest.moc"
