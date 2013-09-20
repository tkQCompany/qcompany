#include <QtTest/QtTest>
#include <QSqlQuery>
#include <QSqlRecord>

#include "../TestsCommon.h"
#include "Database.h"
#include "ModelCounterparty.h"
#include "CounterpartyData.h"
#include "SettingsGlobal.h"

class CounterpartyDataTest : public QObject
{
    Q_OBJECT
    
public:
    CounterpartyDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void testCaseCheckDBFields();
    void testCaseCheckDBFields_data();
};

CounterpartyDataTest::CounterpartyDataTest()
{
}

void CounterpartyDataTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
    SettingsGlobal s;
    s.setFirstRun(true);
}


void CounterpartyDataTest::testCaseCheckDBFields()
{
    QFETCH(QString, field_name);
    QFETCH(QString, field_real_name);
    QFETCH(int, field_num);

    Database db;
    QSqlQuery query(db.modelCounterparty()->query());
    const QSqlRecord rec(query.record());

    QVERIFY2(query.exec(QString("SELECT %1 FROM 'counterparty'").arg(field_real_name)),
             QString("Missing DB field in the table 'counterparty': %1").arg(field_real_name).toAscii());
    QCOMPARE(rec.fieldName(field_num), field_name);
}

void CounterpartyDataTest::testCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_name");
    QTest::addColumn<QString>("field_real_name");
    QTest::addColumn<int>("field_num");

    QTest::newRow("id_counterparty")    << QString("id_counterparty")  << QString("id_counterparty")  << (int)CounterpartyFields::ID;
    QTest::newRow("name")               << QString("name")             << QString("name")             << (int)CounterpartyFields::NAME;
    QTest::newRow("type_id")            << QString("type")             << QString("type_id")          << (int)CounterpartyFields::TYPE_ID;
    QTest::newRow("country")            << QString("country")          << QString("country")          << (int)CounterpartyFields::COUNTRY;
    QTest::newRow("location")           << QString("location")         << QString("location")         << (int)CounterpartyFields::LOCATION;
    QTest::newRow("postal_code")        << QString("postal_code")      << QString("postal_code")      << (int)CounterpartyFields::POSTAL_CODE;
    QTest::newRow("street")             << QString("street")           << QString("street")           << (int)CounterpartyFields::STREET;
    QTest::newRow("tax_ident")          << QString("tax_ident")        << QString("tax_ident")        << (int)CounterpartyFields::TAX_IDENT;
    QTest::newRow("account_name")       << QString("account_name")     << QString("account_name")     << (int)CounterpartyFields::ACCOUNT_NAME;
    QTest::newRow("www")                << QString("www")              << QString("www")              << (int)CounterpartyFields::WWW;
    QTest::newRow("primary_email")      << QString("primary_email")    << QString("primary_email")    << (int)CounterpartyFields::PRIMARY_EMAIL;
    QTest::newRow("primary_phone")      << QString("primary_phone")    << QString("primary_phone")    << (int)CounterpartyFields::PRIMARY_PHONE;
}

QTEST_APPLESS_MAIN(CounterpartyDataTest)

#include "tst_counterpartydatatest.moc"
