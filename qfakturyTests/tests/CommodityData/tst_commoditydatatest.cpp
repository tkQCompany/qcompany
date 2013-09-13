#include <QtTest/QtTest>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

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
    void init();
    void initTestCase();
    void cleanupTestCase();
    void testCaseCheckDBFields();
    void testCaseCheckDBFields_data();
private:
    Database *db_;
};

CommodityDataTest::CommodityDataTest()
{
}

void CommodityDataTest::initTestCase()
{
    TestsCommon::setAppData();
    SettingsGlobal s;
    s.setFirstRun(true);

    db_ = new Database();
}

void CommodityDataTest::init()
{
    TestsCommon::removeDBFile();
}

void CommodityDataTest::cleanupTestCase()
{
    delete db_;
}

void CommodityDataTest::testCaseCheckDBFields()
{
    QFETCH(QString, field_real_name);
    QFETCH(QString, field_name);
    QFETCH(int, field_num);

    QSqlQuery query(db_->modelCommodity()->query());
    const QSqlRecord rec(query.record());

    QVERIFY2(query.exec(QString("SELECT %1 FROM commodity").arg(field_real_name)), "'commodity' table is missing a field");
    QCOMPARE(rec.fieldName(field_num), field_name);

}

void CommodityDataTest::testCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_real_name");
    QTest::addColumn<QString>("field_name");
    QTest::addColumn<int>("field_num");

    QTest::newRow("id_commodity")<< QString("id_commodity") << QString("id_commodity")  << (int)CommodityFields::ID_COMMODITY;
    QTest::newRow("name")       << QString("name")          << QString("name")          << (int)CommodityFields::NAME;
    QTest::newRow("abbrev")     << QString("abbreviation")  << QString("abbreviation")  << (int)CommodityFields::ABBREV;
    QTest::newRow("pkwiu")      << QString("pkwiu")         << QString("pkwiu")         << (int)CommodityFields::PKWIU;
    QTest::newRow("type_id")    << QString("type_id")       << QString("type")          << (int)CommodityFields::TYPE_ID;
    QTest::newRow("unit_id")    << QString("unit_id")       << QString("unit_name")     << (int)CommodityFields::UNIT_ID;
    QTest::newRow("net1")       << QString("net1")          << QString("net1")          << (int)CommodityFields::NET1;
    QTest::newRow("net2")       << QString("net2")          << QString("net2")          << (int)CommodityFields::NET2;
    QTest::newRow("net3")       << QString("net3")          << QString("net3")          << (int)CommodityFields::NET3;
    QTest::newRow("net4")       << QString("net4")          << QString("net4")          << (int)CommodityFields::NET4;
    QTest::newRow("vat")        << QString("vat")           << QString("vat")           << (int)CommodityFields::VAT;
    QTest::newRow("quantity")   << QString("quantity")      << QString("quantity")      << (int)CommodityFields::QUANTITY;
}

QTEST_APPLESS_MAIN(CommodityDataTest)

#include "tst_commoditydatatest.moc"
