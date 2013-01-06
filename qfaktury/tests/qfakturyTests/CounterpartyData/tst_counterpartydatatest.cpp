#include <QtCore/QString>
#include <QtTest/QtTest>

#include "Database.h"

class CounterpartyDataTest : public QObject
{
    Q_OBJECT
    
public:
    CounterpartyDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseCheckDBFields();
    void testCaseCheckDBFields_data();
};

CounterpartyDataTest::CounterpartyDataTest()
{
}

void CounterpartyDataTest::initTestCase()
{
    QCoreApplication::setApplicationName("QFaktury");
    QCoreApplication::setOrganizationName("www.e-linux.pl");
    QCoreApplication::setOrganizationDomain("www.e-linux.pl");
    QCoreApplication::setApplicationVersion(APP_VERSION);

    const QString dbFilename(QString("%1-%2.db3").arg(QCoreApplication::applicationName()).arg(APP_VERSION));
    if(QFile::exists(dbFilename))
    {
        QDir dir;
        dir.remove(dbFilename);
    }
}

void CounterpartyDataTest::cleanupTestCase()
{
}

void CounterpartyDataTest::testCaseCheckDBFields()
{
    QFETCH(QString, field_name);
    QFETCH(int, field_num);

    Database db;
    QSqlQuery query(db.modelCounterparty()->query());

    QVERIFY2(query.exec(QString("SELECT %1 FROM 'counterparty'").arg(field_name)),
             QString("Missing DB field in the table 'counterparty': %1").arg(field_name).toAscii());
    QCOMPARE(db.modelCounterparty()->fieldIndex(field_name), field_num);
}

void CounterpartyDataTest::testCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_name");
    QTest::addColumn<int>("field_num");

    QTest::newRow("id_counterparty") << QString("id_counterparty") << (int)CounterpartyFields::ID;
    QTest::newRow("name") << QString("name") << (int)CounterpartyFields::NAME;
    QTest::newRow("type_id") << QString("type_id") << (int)CounterpartyFields::TYPE_ID;
    QTest::newRow("country_id") << QString("country_id") << (int)CounterpartyFields::COUNTRY_ID;
    QTest::newRow("location") << QString("location") << (int)CounterpartyFields::LOCATION;
    QTest::newRow("postal_code") << QString("postal_code") << (int)CounterpartyFields::POSTAL_CODE;
    QTest::newRow("street") << QString("street") << (int)CounterpartyFields::STREET;
    QTest::newRow("tax_ident") << QString("tax_ident") << (int)CounterpartyFields::TAX_IDENT;
    QTest::newRow("account_name") << QString("account_name") << (int)CounterpartyFields::ACCOUNT_NAME;
    QTest::newRow("www") << QString("www") << (int)CounterpartyFields::WWW;
    QTest::newRow("primary_email") << QString("primary_email") << (int)CounterpartyFields::PRIMARY_EMAIL;
    QTest::newRow("primary_phone") << QString("primary_phone") << (int)CounterpartyFields::PRIMARY_PHONE;
}

QTEST_APPLESS_MAIN(CounterpartyDataTest)

#include "tst_counterpartydatatest.moc"
