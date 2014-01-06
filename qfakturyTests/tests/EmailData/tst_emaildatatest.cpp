#include <QtTest/QtTest>
#include <QSqlQuery>

#include "../TestsCommon/TestsCommon.h"
#include "Database.h"
#include "ModelEmail.h"
#include "EmailData.h"

class EmailDataTest : public QObject
{
    Q_OBJECT
    
public:
    EmailDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseCheckDBFields();
    void testCaseCheckDBFields_data();
};

EmailDataTest::EmailDataTest()
{
}

void EmailDataTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void EmailDataTest::cleanupTestCase()
{
}

void EmailDataTest::testCaseCheckDBFields()
{
    QFETCH(QString, field_name);
    QFETCH(int, field_num);

    Database db;
    QSqlQuery query(db.modelEmail()->query());

    QVERIFY2(query.exec(QString("SELECT %1 FROM 'additional_email'").arg(field_name)),
             QString("Missing DB field in the table 'additional_email': %1").arg(field_name).toLatin1());
    QCOMPARE(db.modelEmail()->fieldIndex(field_name), field_num);
}

void EmailDataTest::testCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_name");
    QTest::addColumn<int>("field_num");

    QTest::newRow("id_email")           << QString("id_email")          << (int)EmailFields::ID_EMAIL;
    QTest::newRow("counterparty_id")    << QString("counterparty_id")   << (int)EmailFields::COUNTERPARTY_ID;
    QTest::newRow("email")              << QString("email")             << (int)EmailFields::EMAIL;
}

QTEST_APPLESS_MAIN(EmailDataTest)

#include "tst_emaildatatest.moc"
