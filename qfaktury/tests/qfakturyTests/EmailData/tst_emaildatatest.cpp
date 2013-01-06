#include <QtCore/QString>
#include <QtTest/QtTest>

#include "Database.h"

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
             QString("Missing DB field in the table 'additional_email': %1").arg(field_name).toAscii());
    QCOMPARE(db.modelEmail()->fieldIndex(field_name), field_num);
}

void EmailDataTest::testCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_name");
    QTest::addColumn<int>("field_num");

    QTest::newRow("id_email") << QString("id_email") << (int)EmailFields::ID_EMAIL;
    QTest::newRow("counterparty_id") << QString("counterparty_id") << (int)EmailFields::COUNTERPARTY_ID;
    QTest::newRow("email") << QString("email") << (int)EmailFields::EMAIL;
}

QTEST_APPLESS_MAIN(EmailDataTest)

#include "tst_emaildatatest.moc"
