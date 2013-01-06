#include <QtCore/QString>
#include <QtTest/QtTest>

#include "Database.h"

class InvoiceDataTest : public QObject
{
    Q_OBJECT
    
public:
    InvoiceDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseCaseCheckDBFields();
    void testCaseCaseCheckDBFields_data();
};

InvoiceDataTest::InvoiceDataTest()
{
}

void InvoiceDataTest::initTestCase()
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

void InvoiceDataTest::cleanupTestCase()
{
}

void InvoiceDataTest::testCaseCaseCheckDBFields()
{
    QFETCH(QString, field_name);
    QFETCH(int, field_num);

    Database db;
    QSqlQuery query(db.modelInvoice()->query());

    QVERIFY2(query.exec(QString("SELECT %1 FROM 'invoice'").arg(field_name)),
             QString("Missing DB field in the table 'invoice': %1").arg(field_name).toAscii());
    QCOMPARE(db.modelInvoice()->fieldIndex(field_name), field_num);
}

void InvoiceDataTest::testCaseCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_name");
    QTest::addColumn<int>("field_num");

    QTest::newRow("id_invoice")         << QString("id_invoice")        << (int)InvoiceFields::ID_INVOICE;
    QTest::newRow("inv_number")         << QString("inv_number")        << (int)InvoiceFields::INV_NUMBER;
    QTest::newRow("selling_date")       << QString("selling_date")      << (int)InvoiceFields::SELLING_DATE;
    QTest::newRow("type_id")            << QString("type_id")           << (int)InvoiceFields::TYPE_ID;
    QTest::newRow("counterparty_id")    << QString("counterparty_id")   << (int)InvoiceFields::COUNTERPARTY_ID;
    QTest::newRow("issuance_date")      << QString("issuance_date")     << (int)InvoiceFields::ISSUANCE_DATE;
    QTest::newRow("payment_date")       << QString("payment_date")      << (int)InvoiceFields::PAYMENT_DATE;
    QTest::newRow("payment_id")         << QString("payment_id")        << (int)InvoiceFields::PAYMENT_ID;
    QTest::newRow("currency_id")        << QString("currency_id")       << (int)InvoiceFields::CURRENCY_ID;
    QTest::newRow("additional_text")    << QString("additional_text")   << (int)InvoiceFields::ADDIT_TEXT;
    QTest::newRow("discount")           << QString("discount")          << (int)InvoiceFields::DISCOUNT;
}

QTEST_APPLESS_MAIN(InvoiceDataTest)

#include "tst_invoicedatatest.moc"
