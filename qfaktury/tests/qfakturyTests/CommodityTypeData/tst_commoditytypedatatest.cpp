#include <QtCore/QString>
#include <QtTest/QtTest>

#include "Database.h"

class CommodityTypeDataTest : public QObject
{
    Q_OBJECT
    
public:
    CommodityTypeDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseCheckDBFields();
    void testCaseCheckDBFields_data();
    void testCaseCheckNames();
};

CommodityTypeDataTest::CommodityTypeDataTest()
{
}

void CommodityTypeDataTest::initTestCase()
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

void CommodityTypeDataTest::cleanupTestCase()
{
}

void CommodityTypeDataTest::testCaseCheckDBFields()
{
    QFETCH(QString, field_name);
    QFETCH(int, field_num);

    Database db;
    QSqlQuery query(db.modelCommodityType()->query());

    QVERIFY2(query.exec(QString("SELECT %1 FROM 'commodity_type'").arg(field_name)), "Missing DB field in the table 'commodity_type'");
    QCOMPARE(db.modelCommodityType()->fieldIndex(field_name), field_num);
}

void CommodityTypeDataTest::testCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_name");
    QTest::addColumn<int>("field_num");

    QTest::newRow("id_commodity_type")  << QString("id_commodity_type") << (int)CommodityTypeFields::ID_COMMODITY_TYPE;
    QTest::newRow("type")               << QString("type")              << (int)CommodityTypeFields::TYPE;
}


void CommodityTypeDataTest::testCaseCheckNames()
{
    QVERIFY2(CommodityTypeData::names(0).isEmpty(), "Bad input (0) is allowed");
    QVERIFY2(!CommodityTypeData::names(1).isEmpty(), "Good input (1) is NOT allowed");
    QVERIFY2(!CommodityTypeData::names(2).isEmpty(), "Good input (2) is NOT allowed");
    QVERIFY2(CommodityTypeData::names(3).isEmpty(), "Bad input (3) is allowed");
}

QTEST_APPLESS_MAIN(CommodityTypeDataTest)

#include "tst_commoditytypedatatest.moc"
