#include <QtCore/QString>
#include <QtTest/QtTest>

#include "Database.h"

class CurrencyDataTest : public QObject
{
    Q_OBJECT
    
public:
    CurrencyDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseCheckDBFields();
    void testCaseCheckDBFields_data();
};

CurrencyDataTest::CurrencyDataTest()
{
}

void CurrencyDataTest::initTestCase()
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

void CurrencyDataTest::cleanupTestCase()
{
}

void CurrencyDataTest::testCaseCheckDBFields()
{
    QFETCH(QString, field_name);
    QFETCH(int, field_num);

    Database db;
    QSqlQuery query(db.modelCurrency()->query());

    QVERIFY2(query.exec(QString("SELECT %1 FROM 'currency'").arg(field_name)),
             QString("Missing DB field in the table 'currency': %1").arg(field_name).toAscii());
    QCOMPARE(db.modelCurrency()->fieldIndex(field_name), field_num);
}

void CurrencyDataTest::testCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_name");
    QTest::addColumn<int>("field_num");

    QTest::newRow("id_currency") << QString("id_currency") << (int)CurrencyFields::ID_CURRENCY;
    QTest::newRow("name") << QString("name") << (int)CurrencyFields::NAME;
}

QTEST_APPLESS_MAIN(CurrencyDataTest)

#include "tst_currencydatatest.moc"
