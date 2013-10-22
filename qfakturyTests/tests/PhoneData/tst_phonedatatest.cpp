#include <QtTest/QtTest>
#include <QSqlQuery>

#include "../TestsCommon.h"
#include "Database.h"
#include "ModelPhone.h"
#include "PhoneData.h"

class PhoneDataTest : public QObject
{
    Q_OBJECT
    
public:
    PhoneDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseCheckDBFields();
    void testCaseCheckDBFields_data();
};

PhoneDataTest::PhoneDataTest()
{
}

void PhoneDataTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void PhoneDataTest::cleanupTestCase()
{
}

void PhoneDataTest::testCaseCheckDBFields()
{
    QFETCH(QString, field_real_name);
    QFETCH(QString, field_name);
    QFETCH(int, field_num);

    Database db;
    QSqlQuery query(db.modelPhone()->query());

    QVERIFY2(query.exec(QString("SELECT %1 FROM 'additional_phone'").arg(field_real_name)),
             QString("Missing DB field in the table 'additional_phone': %1").arg(field_real_name).toLatin1());
    QCOMPARE(db.modelPhone()->fieldIndex(field_name), field_num);
}

void PhoneDataTest::testCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_real_name");
    QTest::addColumn<QString>("field_name");
    QTest::addColumn<int>("field_num");

    QTest::newRow("id_phone")           << QString("id_phone")        << QString("id_phone")    << (int)PhoneFields::ID_PHONE;
    QTest::newRow("counterparty_id")    << QString("counterparty_id") << QString("name")        << (int)PhoneFields::COUNTERPARTY_ID;
    QTest::newRow("number")             << QString("number")          << QString("number")      << (int)PhoneFields::NUMBER;
}

QTEST_APPLESS_MAIN(PhoneDataTest)

#include "tst_phonedatatest.moc"
