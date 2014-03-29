#include <QtTest/QtTest>
#include <QSqlQuery>

#include "../../TestsCommon/TestsCommon.h"
#include "Database.h"
#include "ModelCommodityType.h"
#include "CommodityTypeData.h"
#include "SettingsGlobal.h"

class CommodityTypeDataTest : public QObject
{
    Q_OBJECT
    
public:
    CommodityTypeDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseCheckDBFields();
    void testCaseCheckDBFields_data();
    void testCaseCheckNames();
};

CommodityTypeDataTest::CommodityTypeDataTest()
{
}

void CommodityTypeDataTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
    SettingsGlobal s;
    s.setFirstRun(true);
}

void CommodityTypeDataTest::cleanupTestCase()
{
}

void CommodityTypeDataTest::testCaseCheckDBFields()
{
    QFETCH(QString, field_name);
    QFETCH(int, field_num);

    Database db;
    QSqlQuery query(db.modelCommodityType()->query());

    QVERIFY2(query.exec(QString("SELECT %1 FROM 'commodity_type'").arg(field_name)), "Missing DB field in the table 'commodity_type'");
    QCOMPARE(db.modelCommodityType()->fieldIndex(field_name), field_num);
}

void CommodityTypeDataTest::testCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_name");
    QTest::addColumn<int>("field_num");

    QTest::newRow("id_commodity_type")  << QString("id_commodity_type") << (int)CommodityTypeFields::ID_COMMODITY_TYPE;
    QTest::newRow("type")               << QString("type")              << (int)CommodityTypeFields::TYPE;
}


void CommodityTypeDataTest::testCaseCheckNames()
{
    QVERIFY2(!CommodityTypeData::name((CommodityTypeData::CommodityType)0).isEmpty(), "Good input (0) is NOT allowed - and it should be");
    QVERIFY2(!CommodityTypeData::name((CommodityTypeData::CommodityType)1).isEmpty(), "Good input (1) is NOT allowed - and it should be");
    QVERIFY2( CommodityTypeData::name((CommodityTypeData::CommodityType)2).isEmpty(), "Bad input (2) is allowed - and it should not be");
}

QTEST_APPLESS_MAIN(CommodityTypeDataTest)

#include "tst_commoditytypedatatest.moc"
