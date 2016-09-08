#include <QTest>
#include <QSqlQuery>

#include "../../TestsCommon/TestsCommon.h"
#include "Database.h"
#include "ModelInvoice.h"

class InvoiceDataTest : public QObject
{
    Q_OBJECT
    
public:
    InvoiceDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void testCaseCaseCheckDBFields();
    void testCaseCaseCheckDBFields_data();
};

InvoiceDataTest::InvoiceDataTest()
{
}

void InvoiceDataTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
    SettingsGlobal s;
    s.setFirstRun(true);
    Database db;
    s.setFirstRun(false);
}

void InvoiceDataTest::testCaseCaseCheckDBFields()
{
    QFETCH(QString, field_name);
    QFETCH(QString, field_real_name);
    QFETCH(int, field_num);

    Database db;
    QSqlQuery query(db.modelInvoice()->query());

    QVERIFY2(query.exec(QString("SELECT %1 FROM 'invoice'").arg(field_real_name)),
             QString("Missing DB field in the table 'invoice': %1").arg(field_real_name).toLatin1());
    QCOMPARE(db.modelInvoice()->fieldIndex(field_name), field_num);
}

void InvoiceDataTest::testCaseCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_name");
    QTest::addColumn<QString>("field_real_name");
    QTest::addColumn<int>("field_num");

    QTest::newRow("id_invoice")         << QString("id_invoice")     << QString("id_invoice")        << (int)InvoiceFields::ID_INVOICE;
    QTest::newRow("inv_number")         << QString("inv_number")     << QString("inv_number")        << (int)InvoiceFields::INV_NUMBER;
    QTest::newRow("selling_date")       << QString("selling_date")   << QString("selling_date")      << (int)InvoiceFields::SELLING_DATE;
    QTest::newRow("type_id")            << QString("invoice_type")   << QString("type_id")           << (int)InvoiceFields::TYPE_ID;
    QTest::newRow("counterparty_id")    << QString("name")           << QString("counterparty_id")   << (int)InvoiceFields::COUNTERPARTY_ID;
    QTest::newRow("issuance_date")      << QString("issuance_date")  << QString("issuance_date")     << (int)InvoiceFields::ISSUANCE_DATE;
    QTest::newRow("payment_date")       << QString("payment_date")   << QString("payment_date")      << (int)InvoiceFields::PAYMENT_DATE;
    QTest::newRow("payment_id")         << QString("type")           << QString("payment_id")        << (int)InvoiceFields::PAYMENT_ID;
    QTest::newRow("currency_id")        << QString("code")           << QString("currency_id")       << (int)InvoiceFields::CURRENCY_ID;
    QTest::newRow("additional_text")    << QString("additional_text")<< QString("additional_text")   << (int)InvoiceFields::ADDIT_TEXT;
    QTest::newRow("discount")           << QString("discount")       << QString("discount")          << (int)InvoiceFields::DISCOUNT;
}

QTEST_MAIN(InvoiceDataTest)

#include "tst_invoicedatatest.moc"
