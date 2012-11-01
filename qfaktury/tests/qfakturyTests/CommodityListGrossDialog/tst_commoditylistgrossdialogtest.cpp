#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

class CommodityListGrossDialogTest : public QObject
{
    Q_OBJECT
    
public:
    CommodityListGrossDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

CommodityListGrossDialogTest::CommodityListGrossDialogTest()
{
}

void CommodityListGrossDialogTest::initTestCase()
{
}

void CommodityListGrossDialogTest::cleanupTestCase()
{
}

void CommodityListGrossDialogTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void CommodityListGrossDialogTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(CommodityListGrossDialogTest)

#include "tst_commoditylistgrossdialogtest.moc"
