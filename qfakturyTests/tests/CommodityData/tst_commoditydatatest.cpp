#include <QtTest/QtTest>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

#include "../TestsCommon/TestsCommon.h"
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
    QCOMPARE(cd.field(CommodityFields::ID_COMMODITY).toLongLong(), 0LL);
    QVERIFY(cd.field(CommodityFields::NAME).toString().isEmpty());
    QVERIFY(cd.field(CommodityFields::ABBREV).toString().isEmpty());
    QVERIFY(cd.field(CommodityFields::PKWIU).toString().isEmpty());
    QCOMPARE(cd.field(CommodityFields::TYPE_ID).toInt(), 0);
    QCOMPARE(cd.field(CommodityFields::UNIT_ID).toInt(), 0);
    QCOMPARE(cd.field(CommodityFields::NET1).value<Money_t>(), Money_t("0,0"));
    QCOMPARE(cd.field(CommodityFields::NET2).value<Money_t>(), Money_t("0,0"));
    QCOMPARE(cd.field(CommodityFields::NET3).value<Money_t>(), Money_t("0,0"));
    QCOMPARE(cd.field(CommodityFields::NET4).value<Money_t>(), Money_t("0,0"));
    QCOMPARE(cd.field(CommodityFields::NET1).toFloat(), 0.0f);
    QCOMPARE(cd.field(CommodityFields::QUANTITY).toDouble(), 0.0);

    cd.setField(CommodityFields::ID_COMMODITY, 1);
    QCOMPARE(cd.field(CommodityFields::ID_COMMODITY).toLongLong(), 1LL);

    const QString name("name");
    cd.setField(CommodityFields::NAME, name);
    QCOMPARE(cd.field(CommodityFields::NAME).toString(), name);

    const QString abbrev("abbrev");
    cd.setField(CommodityFields::ABBREV, abbrev);
    QCOMPARE(cd.field(CommodityFields::ABBREV).toString(), abbrev);

    const QString pkwiu("pkwiu");
    cd.setField(CommodityFields::PKWIU, pkwiu);
    QCOMPARE(cd.field(CommodityFields::PKWIU).toString(), pkwiu);

    cd.setField(CommodityFields::TYPE_ID, 1);
    QCOMPARE(cd.field(CommodityFields::TYPE_ID).toInt(), 1);

    cd.setField(CommodityFields::UNIT_ID, 1);
    QCOMPARE(cd.field(CommodityFields::UNIT_ID).toInt(), 1);

    QVariant val;
    const Money_t net1(QString("0,33"));
    val.setValue(net1);
    cd.setField(CommodityFields::NET1, val);
    QCOMPARE(cd.field(CommodityFields::NET1).value<Money_t>(), net1);

    const Money_t net2("0,34");
    val.setValue(net2);
    cd.setField(CommodityFields::NET2, val);
    QCOMPARE(cd.field(CommodityFields::NET2).value<Money_t>(), net2);

    const Money_t net3(QString("0.35"));
    val.setValue(net3);
    cd.setField(CommodityFields::NET3, val);
    QVERIFY(cd.field(CommodityFields::NET3).value<Money_t>() == net3);

    const Money_t net4("0.36");
    val.setValue(net4);
    cd.setField(CommodityFields::NET4, val);
    QVERIFY(cd.field(CommodityFields::NET4).value<Money_t>() == net4);



    cd.setField(CommodityFields::ID_COMMODITY, 2);
    QCOMPARE(cd.field(CommodityFields::ID_COMMODITY).toLongLong(), 2LL);

    const QString name2("name2");
    cd.setField(CommodityFields::NAME, name2);
    QCOMPARE(cd.field(CommodityFields::NAME).toString(), name2);

    const QString abbrev2("abbrev2");
    cd.setField(CommodityFields::ABBREV, abbrev2);
    QCOMPARE(cd.field(CommodityFields::ABBREV).toString(), abbrev2);

    const QString pkwiu2("pkwiu2");
    cd.setField(CommodityFields::PKWIU, pkwiu2);
    QCOMPARE(cd.field(CommodityFields::PKWIU).toString(), pkwiu2);

    cd.setField(CommodityFields::TYPE_ID, 2);
    QCOMPARE(cd.field(CommodityFields::TYPE_ID).toInt(), 2);

    cd.setField(CommodityFields::UNIT_ID, 2);
    QCOMPARE(cd.field(CommodityFields::UNIT_ID).toInt(), 2);

    const Money_t net12("0,43");
    val.setValue(net12);
    cd.setField(CommodityFields::NET1, val);
    QCOMPARE(cd.field(CommodityFields::NET1).value<Money_t>(), net12);

    const Money_t net22("0,44");
    val.setValue(net22);
    cd.setField(CommodityFields::NET2, val);
    QCOMPARE(cd.field(CommodityFields::NET2).value<Money_t>(), net22);

    const Money_t net32("0,45");
    val.setValue(net32);
    cd.setField(CommodityFields::NET3, val);
    QVERIFY(cd.field(CommodityFields::NET3).value<Money_t>() == net32);

    const QString net42Str("0.46");
    const Money_t net42(net42Str);
    val.setValue(net42);
    cd.setField(CommodityFields::NET4, val);
    QCOMPARE(cd.field(CommodityFields::NET4).value<Money_t>().toString(), net42Str);
}



QTEST_APPLESS_MAIN(CommodityDataTest)

#include "tst_commoditydatatest.moc"
