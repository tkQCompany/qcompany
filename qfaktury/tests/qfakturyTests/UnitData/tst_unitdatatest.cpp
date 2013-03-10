#include <QtTest/QtTest>

#include "../TestsCommon.h"
#include "Database.h"

class UnitDataTest : public QObject
{
    Q_OBJECT
    
public:
    UnitDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseCheckDBFields();
    void testCaseCheckDBFields_data();
    void testCaseCheckTypesNames();
    void testCaseCheckTypesNames_data();
};

UnitDataTest::UnitDataTest()
{
}

void UnitDataTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void UnitDataTest::cleanupTestCase()
{
}

void UnitDataTest::testCaseCheckDBFields()
{
    QFETCH(QString, field_name);
    QFETCH(int, field_num);

    Database db;
    QSqlQuery query(db.modelUnit()->query());

    QVERIFY2(query.exec(QString("SELECT %1 FROM 'unit'").arg(field_name)),
             QString("Missing DB field in the table 'unit': %1").arg(field_name).toAscii());
    QCOMPARE(db.modelUnit()->fieldIndex(field_name), field_num);
}

void UnitDataTest::testCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_name");
    QTest::addColumn<int>("field_num");

    QTest::newRow("id_unit") << QString("id_unit") << (int)UnitFields::ID_UNIT;
    QTest::newRow("name") << QString("name") << (int)UnitFields::NAME;
}


void UnitDataTest::testCaseCheckTypesNames()
{
    QFETCH(int, input_num);
    QFETCH(QString, output_name);

    QVERIFY2(UnitData::name(input_num) == output_name, "Mismatch between numbers and names or between names");
}


void UnitDataTest::testCaseCheckTypesNames_data()
{
    QTest::addColumn<int>("input_num");
    QTest::addColumn<QString>("output_name");

    QTest::newRow("-1") << -1 << QString();
    QTest::newRow("0") << 0 << QString();
    QTest::newRow("1") << 1 << trUtf8("szt.");
    QTest::newRow("2") << 2 << trUtf8("kg");
    QTest::newRow("3") << 3 << trUtf8("g");
    QTest::newRow("4") << 4 << trUtf8("m");
    QTest::newRow("5") << 5 << trUtf8("km");
    QTest::newRow("6") << 6 << trUtf8("godz.");
    QTest::newRow("7") << 7 << trUtf8("kmpl.");
    QTest::newRow("8") << 8 << QString();
    QTest::newRow("666") << 666 << QString();
}

QTEST_APPLESS_MAIN(UnitDataTest)

#include "tst_unitdatatest.moc"
