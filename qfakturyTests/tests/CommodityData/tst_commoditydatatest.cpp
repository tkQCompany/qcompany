#include <QtTest/QtTest>
#include <QSqlQuery>

#include "../TestsCommon.h"
#include "Database.h"
#include "ModelCommodity.h"
#include "CommodityData.h"

class CommodityDataTest : public QObject
{
    Q_OBJECT
    
public:
    CommodityDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseCheckDBFields();
    void testCaseCheckDBFields_data();
};

CommodityDataTest::CommodityDataTest()
{
}

void CommodityDataTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void CommodityDataTest::cleanupTestCase()
{
}

void CommodityDataTest::testCaseCheckDBFields()
{
    QFETCH(QString, field_name);
    QFETCH(int, field_num);

    Database db;
    QSqlQuery query(db.modelCommodity()->query());

    QVERIFY2(query.exec(QString("SELECT %1 FROM 'commodity'").arg(field_name)), "'commodity' table is missing a field");
    QCOMPARE(db.modelCommodity()->fieldIndex(field_name), field_num);
}

void CommodityDataTest::testCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_name");
    QTest::addColumn<int>("field_num");

    QTest::newRow("id")         << QString("id_commodity")  << (int)CommodityFields::ID;
    QTest::newRow("name")       << QString("name")          << (int)CommodityFields::NAME;
    QTest::newRow("abbrev")     << QString("abbreviation")  << (int)CommodityFields::ABBREV;
    QTest::newRow("pkwiu")      << QString("pkwiu")         << (int)CommodityFields::PKWIU;
    QTest::newRow("type_id")    << QString("type_id")       << (int)CommodityFields::TYPE_ID;
    QTest::newRow("unit_id")    << QString("unit_id")       << (int)CommodityFields::UNIT_ID;
    QTest::newRow("net1")       << QString("net1")          << (int)CommodityFields::NET1;
    QTest::newRow("net2")       << QString("net2")          << (int)CommodityFields::NET2;
    QTest::newRow("net3")       << QString("net3")          << (int)CommodityFields::NET3;
    QTest::newRow("net4")       << QString("net4")          << (int)CommodityFields::NET4;
    QTest::newRow("vat")        << QString("vat")           << (int)CommodityFields::VAT;
    QTest::newRow("quantity")   << QString("quantity")      << (int)CommodityFields::QUANTITY;
}

QTEST_APPLESS_MAIN(CommodityDataTest)

#include "tst_commoditydatatest.moc"
