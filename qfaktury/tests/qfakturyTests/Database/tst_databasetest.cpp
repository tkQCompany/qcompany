#include <QtTest/QtTest>

#include "../TestsCommon.h"
#include "Database.h"


class DatabaseTest : public QObject
{
    Q_OBJECT
    
public:
    DatabaseTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testCaseCheckDBFileName();
    void testCaseCheckDBStructure();

    void testCaseCheckModelCommodity();
    void testCaseCheckModelCommodityType();
    void testCaseCheckModelCounterparty();
    void testCaseCheckModelCounterpartyType();
    void testCaseCheckModelCountry();
    void testCaseCheckModelCurrency();
    void testCaseCheckModelEmail();
    void testCaseCheckModelInvoice();
    void testCaseCheckModelInvoiceType();
    void testCaseCheckModelInvoiceWithCommodities();
    void testCaseCheckModelPaymentType();
    void testCaseCheckModelPhone();
    void testCaseCheckModelUnit();
    void testCaseCheckModelVat();

    void testCaseCheckCommodities();
    void testCaseCheckCommodities_data();
    void testCaseCheckInvoiceWithCommoditiesInsertTransact();
    void testCaseCheckInvoiceWithCommoditiesInsertTransact_data();
    void testCaseCheckInvoiceWithCommoditiesDeleteTransact();
    void testCaseCheckInvoiceWithCommoditiesDeleteTransact_data();

private:
    QString dbFilename_;
};

DatabaseTest::DatabaseTest()
{
}

void DatabaseTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void DatabaseTest::cleanupTestCase()
{
}



void DatabaseTest::testCaseCheckDBFileName()
{
    Database db;
    QCOMPARE(db.dbFileName(), dbFilename_);
}


void DatabaseTest::testCaseCheckDBStructure()
{
    Database db;
}


void DatabaseTest::testCaseCheckModelCommodity()
{
    Database db;
    QVERIFY(db.modelCommodity() != NULL);
}


void DatabaseTest::testCaseCheckModelCommodityType()
{
    Database db;
    QVERIFY(db.modelCommodityType() != NULL);


}


void DatabaseTest::testCaseCheckModelCounterparty()
{
    Database db;
    QVERIFY(db.modelCounterparty()!= NULL);
}


void DatabaseTest::testCaseCheckModelCounterpartyType()
{
    Database db;
    QVERIFY(db.modelCounterpartyType() != NULL);
}


void DatabaseTest::testCaseCheckModelCountry()
{
    Database db;
    QVERIFY(db.modelCountry() != NULL);
}


void DatabaseTest::testCaseCheckModelCurrency()
{
    Database db;
    QVERIFY(db.modelCurrency() != NULL);
}


void DatabaseTest::testCaseCheckModelEmail()
{
    Database db;
    QVERIFY(db.modelEmail() != NULL);
}


void DatabaseTest::testCaseCheckModelInvoice()
{
    Database db;
    QVERIFY(db.modelInvoice() != NULL);
}


void DatabaseTest::testCaseCheckModelInvoiceType()
{
    Database db;
    QVERIFY(db.modelInvoiceType() != NULL);
}


void DatabaseTest::testCaseCheckModelInvoiceWithCommodities()
{
    Database db;
    QVERIFY(db.modelInvoiceWithCommodities() != NULL);
}


void DatabaseTest::testCaseCheckModelPaymentType()
{
    Database db;
    QVERIFY(db.modelPaymentType() != NULL);
}


void DatabaseTest::testCaseCheckModelPhone()
{
    Database db;
    QVERIFY(db.modelPhone() != NULL);
}


void DatabaseTest::testCaseCheckModelUnit()
{
    Database db;
    QVERIFY(db.modelUnit() != NULL);
}


void DatabaseTest::testCaseCheckModelVat()
{
    Database db;
    QVERIFY(db.modelVat() != NULL);
}


void DatabaseTest::testCaseCheckCommodities()
{
    Database db;
    QVERIFY(db.commodities(0).size() == 0);
}


void DatabaseTest::testCaseCheckCommodities_data()
{

}


void DatabaseTest::testCaseCheckInvoiceWithCommoditiesInsertTransact()
{
    Database db;
    InvoiceData invData;
    QList<CommodityVisualData> commodities;

    for(size_t i = 0; i < 1000; ++i)
    {

    }

    //db.invoiceWithCommoditiesInsertTransact(invData, commodities);
}



void DatabaseTest::testCaseCheckInvoiceWithCommoditiesInsertTransact_data()
{
    QTest::addColumn<QString>("invoice_number");
    QTest::addColumn<QDate>("selling_date");
    QTest::addColumn<qint64>("type_id");
    QTest::addColumn<qint64>("counterparty_id");
    QTest::addColumn<QDate>("issuance_date");
    QTest::addColumn<QDate>("payment_date");
    QTest::addColumn<qint64>("payment_id");
    QTest::addColumn<qint64>("currency_id");
    QTest::addColumn<QString>("additional_text");
    QTest::addColumn<short int>("discount");

    for(size_t i = 0; i < 1000; ++i)
    {
        //QTest::newRow(QString("%1").arg(i+1).toAscii()) << QString("%1").arg(i+1) << QDate::currentDate() <<;
    }
}


void DatabaseTest::testCaseCheckInvoiceWithCommoditiesDeleteTransact()
{
    Database db;
}


void DatabaseTest::testCaseCheckInvoiceWithCommoditiesDeleteTransact_data()
{

}


//void DatabaseTest::testCase1()
//{
//    QFETCH(QString, data);
//    QVERIFY2(true, "Failure");
//}

//void DatabaseTest::testCase1_data()
//{
//    QTest::addColumn<QString>("data");
//    QTest::newRow("0") << QString();
//}

QTEST_APPLESS_MAIN(DatabaseTest)

#include "tst_databasetest.moc"
