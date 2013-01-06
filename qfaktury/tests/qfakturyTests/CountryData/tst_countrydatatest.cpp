#include <QtCore/QString>
#include <QtTest/QtTest>

#include "Database.h"

class CountryDataTest : public QObject
{
    Q_OBJECT
    
public:
    CountryDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseCheckDBFields();
    void testCaseCheckDBFields_data();
};

CountryDataTest::CountryDataTest()
{
}

void CountryDataTest::initTestCase()
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

void CountryDataTest::cleanupTestCase()
{
}

void CountryDataTest::testCaseCheckDBFields()
{
    QFETCH(QString, field_name);
    QFETCH(int, field_num);

    Database db;
    QSqlQuery query(db.modelCountry()->query());

    QVERIFY2(query.exec(QString("SELECT %1 FROM 'country'").arg(field_name)),
             QString("Missing DB field in the table 'country': %1").arg(field_name).toAscii());
    QCOMPARE(db.modelCountry()->fieldIndex(field_name), field_num);
}

void CountryDataTest::testCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_name");
    QTest::addColumn<int>("field_num");

    QTest::newRow("id_country") << QString("id_country") << (int)CountryFields::ID_COUNTRY;
    QTest::newRow("name") << QString("name") << (int)CountryFields::NAME;
}

QTEST_APPLESS_MAIN(CountryDataTest)

#include "tst_countrydatatest.moc"
