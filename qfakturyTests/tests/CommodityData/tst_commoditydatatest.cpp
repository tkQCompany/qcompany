#include <QtTest/QtTest>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

#include "../../TestsCommon/TestsCommon.h"
#include "Database.h"
#include "ModelCommodity.h"
#include "CommodityData.h"

class CommodityDataTest : public QObject
{
    Q_OBJECT
        
private Q_SLOTS:
    void init();
    void initTestCase();
    void testCaseCheckDBFields();
    void testCaseCheckDBFields_data();
    void testCaseCheckFields();
};


void CommodityDataTest::initTestCase()
{
    TestsCommon::setAppData();
    SettingsGlobal s;
    s.setFirstRun(true);
}

void CommodityDataTest::init()
{
    TestsCommon::removeDBFile();
}


void CommodityDataTest::testCaseCheckDBFields()
{
    QFETCH(QString, field_real_name);
    QFETCH(QString, field_name);
    QFETCH(int, field_num);

    Database db;
    QSqlQuery query(db.modelCommodity()->query());
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


void CommodityDataTest::testCaseCheckFields()
{
    CommodityData cd;
    QCOMPARE(cd.id(), 0LL);
    QVERIFY(cd.name().isEmpty());
    QVERIFY(cd.abbrev().isEmpty());
    QVERIFY(cd.pkwiu().isEmpty());
    QCOMPARE(cd.typeId(), 0LL);
    QCOMPARE(cd.unitId(), 0LL);
    QCOMPARE(cd.net1(), Money_t(0));
    QCOMPARE(cd.net2(), Money_t(0));
    QCOMPARE(cd.net3(), Money_t(0));
    QCOMPARE(cd.net4(), Money_t(0));
    QCOMPARE(cd.quantity().toDouble(), 0.0);

    cd.setId(1LL);
    QCOMPARE(cd.id(), 1LL);

    const QString name("name");
    cd.setName(name);
    QCOMPARE(cd.name(), name);

    const QString abbrev("abbrev");
    cd.setAbbrev(abbrev);
    QCOMPARE(cd.abbrev(), abbrev);

    const QString pkwiu("pkwiu");
    cd.setPkwiu(pkwiu);
    QCOMPARE(cd.pkwiu(), pkwiu);

    cd.setTypeId(1LL);
    QCOMPARE(cd.typeId(), 1LL);

    cd.setUnitId(1LL);
    QCOMPARE(cd.unitId(), 1LL);

    const Money_t net1(QString("0,33"));
    cd.setNet1(net1);
    QCOMPARE(cd.net1(), net1);

    const Money_t net2("0,34");
    cd.setNet2(net2);
    QCOMPARE(cd.net2(), net2);

    const Money_t net3(QString("0.35"));
    cd.setNet3(net3);
    QVERIFY(cd.net3() == net3);

    const Money_t net4("0.36");
    cd.setNet4(net4);
    QVERIFY(cd.net4() == net4);



    cd.setId(2LL);
    QCOMPARE(cd.id(), 2LL);

    const QString name2("name2");
    cd.setName(name2);
    QCOMPARE(cd.name(), name2);

    const QString abbrev2("abbrev2");
    cd.setAbbrev(abbrev2);
    QCOMPARE(cd.abbrev(), abbrev2);

    const QString pkwiu2("pkwiu2");
    cd.setPkwiu(pkwiu2);
    QCOMPARE(cd.pkwiu(), pkwiu2);

    cd.setTypeId(2LL);
    QCOMPARE(cd.typeId(), 2LL);

    cd.setUnitId(2LL);
    QCOMPARE(cd.unitId(), 2LL);

    const Money_t net12("0,43");
    cd.setNet1(net12);
    QCOMPARE(cd.net1(), net12);

    const Money_t net22("0,44");
    cd.setNet2(net22);
    QCOMPARE(cd.net2(), net22);

    const Money_t net32("0,45");
    cd.setNet3(net32);
    QVERIFY(cd.net3() == net32);

    const QString net42Str("0.46");
    const Money_t net42(net42Str);
    cd.setNet4(net42);
    QCOMPARE(cd.net4(), net42);
}



QTEST_APPLESS_MAIN(CommodityDataTest)

#include "tst_commoditydatatest.moc"
