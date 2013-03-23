#include <QtTest/QtTest>

#include "../TestsCommon.h"
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
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
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
    QTest::newRow("name") << QString("name") << (int)CurrencyFields::CODE;
}

QTEST_APPLESS_MAIN(CurrencyDataTest)

#include "tst_currencydatatest.moc"
