#include <QTest>
#include <QSqlQuery>

#include "../../TestsCommon/TestsCommon.h"
#include "Database.h"
#include "ModelCounterpartyType.h"
#include "CounterpartyTypeData.h"

class CounterpartyTypeDataTest : public QObject
{
    Q_OBJECT
    
public:
    CounterpartyTypeDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseCheckDBFields();
    void testCaseCheckDBFields_data();
};

CounterpartyTypeDataTest::CounterpartyTypeDataTest()
{
}

void CounterpartyTypeDataTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void CounterpartyTypeDataTest::cleanupTestCase()
{
}

void CounterpartyTypeDataTest::testCaseCheckDBFields()
{
    QFETCH(QString, field_name);
    QFETCH(int, field_num);

    Database db;
    QSqlQuery query(db.modelCounterpartyType()->query());

    QVERIFY2(query.exec(QString("SELECT %1 FROM 'counterparty_type'").arg(field_name)),
             QString("Missing DB field in the table 'counterparty_type': %1").arg(field_name).toLatin1());
    QCOMPARE(db.modelCounterpartyType()->fieldIndex(field_name), field_num);
}

void CounterpartyTypeDataTest::testCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_name");
    QTest::addColumn<int>("field_num");

    QTest::newRow("id_counterparty_type") << QString("id_counterparty_type") << (int)CounterpartyTypeFields::ID;
    QTest::newRow("type") << QString("type") << (int)CounterpartyTypeFields::TYPE;
}

QTEST_APPLESS_MAIN(CounterpartyTypeDataTest)

#include "tst_counterpartytypedatatest.moc"
