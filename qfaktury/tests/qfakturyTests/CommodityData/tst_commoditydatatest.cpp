#include <QtCore/QString>
#include <QtTest/QtTest>

#include "Database.h"

class CommodityDataTest : public QObject
{
    Q_OBJECT
    
public:
    CommodityDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseCheckDBFields();
    void testCaseCheckDBFields_data();
};

CommodityDataTest::CommodityDataTest()
{
}

void CommodityDataTest::initTestCase()
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

void CommodityDataTest::cleanupTestCase()
{
}

void CommodityDataTest::testCaseCheckDBFields()
{
    QFETCH(QString, field_name);

    Database db;
    QSqlQuery query(db.modelCommodity()->query());

    QVERIFY2(query.exec(QString("SELECT %1 FROM commodity").arg(field_name)), "'commodity' table is missing a field");
}

void CommodityDataTest::testCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_name");

    QTest::newRow("id") << QString("id_commodity");
    QTest::newRow("name") << QString("name");
    QTest::newRow("abbrev") << QString("abbreviation");
    QTest::newRow("pkwiu") << QString("pkwiu");
    QTest::newRow("type_id") << QString("type_id");
    QTest::newRow("unit_id") << QString("unit_id");
    QTest::newRow("net1") << QString("net1");
    QTest::newRow("net2") << QString("net2");
    QTest::newRow("net3") << QString("net3");
    QTest::newRow("net4") << QString("net4");
    QTest::newRow("vat") << QString("vat");
    QTest::newRow("quantity") << QString("quantity");
}

QTEST_APPLESS_MAIN(CommodityDataTest)

#include "tst_commoditydatatest.moc"
