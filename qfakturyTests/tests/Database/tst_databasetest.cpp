#include <QtTest/QtTest>
#include <QSqlRecord>

#include "../TestsCommon.h"
#include "Database.h"
#include "CommodityVisualData.h"
#include "ModelCommodity.h"
#include "ModelCommodityType.h"
#include "ModelCounterparty.h"
#include "ModelCounterpartyType.h"
#include "ModelCountry.h"
#include "ModelCurrency.h"
#include "ModelEmail.h"
#include "ModelInvoice.h"
#include "ModelInvoiceType.h"
#include "ModelInvoiceWithCommodities.h"
#include "ModelPaymentType.h"
#include "ModelPhone.h"
#include "ModelUnit.h"
#include "ModelVat.h"
#include "CounterpartyData.h"
#include "CommodityData.h"
#include "EmailData.h"
#include "InvoiceWithCommodities.h"
#include "PhoneData.h"


class DatabaseTest : public QObject
{
    Q_OBJECT
    
public:
    DatabaseTest();
    
private Q_SLOTS:
    void initTestCase();
    void init();

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

};

DatabaseTest::DatabaseTest()
{
}

void DatabaseTest::initTestCase()
{
    TestsCommon::setAppData();
    //SettingsGlobal s;
    //s.setFirstRun(true);
}

void DatabaseTest::init()
{
    //TestsCommon::removeDBFile();
}



void DatabaseTest::testCaseCheckDBFileName()
{
    Database db;
    const QString dbFilename(QString("%1-%2.db3").arg(QCoreApplication::applicationName()).arg(APP_VERSION));
    QCOMPARE(db.dbFileName(), dbFilename);
}


void DatabaseTest::testCaseCheckDBStructure()
{
    Database db;
    const QStringList tables(db.database().tables());
    QVERIFY(tables.contains("additional_email"));
    QVERIFY(tables.contains("additional_phone"));
    QVERIFY(tables.contains("commodity"));
    QVERIFY(tables.contains("commodity_type"));
    QVERIFY(tables.contains("counterparty"));
    QVERIFY(tables.contains("counterparty_type"));
    QVERIFY(tables.contains("currency"));
    QVERIFY(tables.contains("invoice"));
    QVERIFY(tables.contains("invoice_type"));
    QVERIFY(tables.contains("payment_type"));
    QVERIFY(tables.contains("table_invoice_commodity"));
    QVERIFY(tables.contains("unit"));
}


void DatabaseTest::testCaseCheckModelCommodity()
{
    Database db;
    QVERIFY(db.modelCommodity() != 0);
    QVERIFY(db.modelCommodity()->relation(CommodityFields::TYPE_ID).tableName()   == "commodity_type");
    QVERIFY(db.modelCommodity()->relation(CommodityFields::TYPE_ID).indexColumn() == "id_commodity_type");
    QVERIFY(db.modelCommodity()->relation(CommodityFields::UNIT_ID).tableName()   == "unit");
    QVERIFY(db.modelCommodity()->relation(CommodityFields::UNIT_ID).indexColumn() == "id_unit");
}


void DatabaseTest::testCaseCheckModelCommodityType()
{
    Database db;
    QVERIFY(db.modelCommodityType() != 0);
}


void DatabaseTest::testCaseCheckModelCounterparty()
{
    Database db;
    QVERIFY(db.modelCounterparty()!= 0);
    QVERIFY(db.modelCounterparty()->relation(CounterpartyFields::TYPE_ID).tableName() == "counterparty_type");
    QVERIFY(db.modelCounterparty()->relation(CounterpartyFields::TYPE_ID).indexColumn() == "id_counterparty_type");
}


void DatabaseTest::testCaseCheckModelCounterpartyType()
{
    Database db;
    QVERIFY(db.modelCounterpartyType() != 0);
    QVERIFY(db.modelCounterpartyType()->fieldIndex("id_counterparty_type") == 0);
    QVERIFY(db.modelCounterpartyType()->fieldIndex("type") == 1);
}


void DatabaseTest::testCaseCheckModelCountry()
{
    Database db;
    QVERIFY(db.modelCountry() != 0);
    QVERIFY(db.modelCountry()->stringList().count() > 0);
}


void DatabaseTest::testCaseCheckModelCurrency()
{
    Database db;
    QVERIFY(db.modelCurrency() != 0);
}


void DatabaseTest::testCaseCheckModelEmail()
{
    Database db;
    QVERIFY(db.modelEmail() != 0);

    QVERIFY(db.modelEmail()->fieldIndex("id_email") == 0);
    QVERIFY(db.modelEmail()->fieldIndex("counterparty_id") == 1);
    QVERIFY(db.modelEmail()->fieldIndex("email") == 2);

    QVERIFY(db.modelEmail()->relation(EmailFields::COUNTERPARTY_ID).tableName() == "counterparty");
    QVERIFY(db.modelEmail()->relation(EmailFields::COUNTERPARTY_ID).indexColumn() == "id_counterparty");
}


void DatabaseTest::testCaseCheckModelInvoice()
{
    Database db;
    QVERIFY(db.modelInvoice() != 0);
    QVERIFY(db.modelInvoice()->relation(InvoiceFields::TYPE_ID).tableName() == "invoice_type");
    QVERIFY(db.modelInvoice()->relation(InvoiceFields::TYPE_ID).indexColumn() == "id_invoice_type");
    QVERIFY(db.modelInvoice()->relation(InvoiceFields::COUNTERPARTY_ID).tableName() == "counterparty");
    QVERIFY(db.modelInvoice()->relation(InvoiceFields::COUNTERPARTY_ID).indexColumn() == "id_counterparty");
    QVERIFY(db.modelInvoice()->relation(InvoiceFields::PAYMENT_ID).tableName() == "payment_type");
    QVERIFY(db.modelInvoice()->relation(InvoiceFields::PAYMENT_ID).indexColumn() == "id_payment_type");
    QVERIFY(db.modelInvoice()->relation(InvoiceFields::CURRENCY_ID).tableName() == "currency");
    QVERIFY(db.modelInvoice()->relation(InvoiceFields::CURRENCY_ID).indexColumn() == "id_currency");
}



void DatabaseTest::testCaseCheckModelInvoiceType()
{
    Database db;
    QVERIFY(db.modelInvoiceType() != 0);

    QVERIFY(db.modelInvoiceType()->fieldIndex("id_invoice_type") == 0);
    QVERIFY(db.modelInvoiceType()->fieldIndex("invoice_type") == 1);
}


void DatabaseTest::testCaseCheckModelInvoiceWithCommodities()
{
    Database db;
    QVERIFY(db.modelInvoiceWithCommodities() != 0);

    QVERIFY(db.modelInvoiceWithCommodities()->fieldIndex("id") == 0);
    QVERIFY(db.modelInvoiceWithCommodities()->fieldIndex("invoice_id") == 1);
    QVERIFY(db.modelInvoiceWithCommodities()->fieldIndex("commodity_id") == 2);
    QVERIFY(db.modelInvoiceWithCommodities()->fieldIndex("net") == 3);
    QVERIFY(db.modelInvoiceWithCommodities()->fieldIndex("quantity") == 4);
    QVERIFY(db.modelInvoiceWithCommodities()->fieldIndex("discount") == 5);

    QVERIFY(db.modelInvoiceWithCommodities()->relation(InvoiceWithCommoditiesFields::INVOICE_ID).tableName() == "invoice");
    QVERIFY(db.modelInvoiceWithCommodities()->relation(InvoiceWithCommoditiesFields::INVOICE_ID).indexColumn() == "id_invoice");
    QVERIFY(db.modelInvoiceWithCommodities()->relation(InvoiceWithCommoditiesFields::COMMODITY_ID).tableName() == "commodity");
    QVERIFY(db.modelInvoiceWithCommodities()->relation(InvoiceWithCommoditiesFields::COMMODITY_ID).indexColumn() == "id_commodity");
}


void DatabaseTest::testCaseCheckModelPaymentType()
{
    Database db;
    QVERIFY(db.modelPaymentType() != 0);

    QVERIFY(db.modelPaymentType()->fieldIndex("id_payment_type") == 0);
    QVERIFY(db.modelPaymentType()->fieldIndex("type") == 1);
}


void DatabaseTest::testCaseCheckModelPhone()
{
    Database db;
    QVERIFY(db.modelPhone() != 0);

    QVERIFY(db.modelPhone()->relation(PhoneFields::COUNTERPARTY_ID).tableName() == "counterparty");
    QVERIFY(db.modelPhone()->relation(PhoneFields::COUNTERPARTY_ID).indexColumn() == "id_counterparty");
}


void DatabaseTest::testCaseCheckModelUnit()
{
    Database db;
    QVERIFY(db.modelUnit() != 0);
}


void DatabaseTest::testCaseCheckModelVat()
{
    Database db;
    QVERIFY(db.modelVat() != 0);

    QVERIFY(db.modelVat()->stringList().count() > 0);
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

QTEST_APPLESS_MAIN(DatabaseTest)

#include "tst_databasetest.moc"
