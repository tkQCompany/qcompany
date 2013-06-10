#include <QtTest/QtTest>
#include <QSqlQuery>

#include "../TestsCommon.h"
#include "Database.h"
#include "ModelInvoiceType.h"
#include "InvoiceData.h"
#include "InvoiceTypeData.h"

class InvoiceTypeDataTest : public QObject
{
    Q_OBJECT
    
public:
    InvoiceTypeDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseCheckDBFields();
    void testCaseCheckDBFields_data();
    void testCaseCheckTypesNames();
    void testCaseCheckTypesNames_data();
};

InvoiceTypeDataTest::InvoiceTypeDataTest()
{
}

void InvoiceTypeDataTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void InvoiceTypeDataTest::cleanupTestCase()
{
}

void InvoiceTypeDataTest::testCaseCheckDBFields()
{
    QFETCH(QString, field_name);
    QFETCH(int, field_num);

    Database db;
    QSqlQuery query(db.modelInvoiceType()->query());

    QVERIFY2(query.exec(QString("SELECT %1 FROM 'invoice_type'").arg(field_name)),
             QString("Missing DB field in the table 'invoice_type': %1").arg(field_name).toAscii());
    QCOMPARE(db.modelInvoiceType()->fieldIndex(field_name), field_num);
}

void InvoiceTypeDataTest::testCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_name");
    QTest::addColumn<int>("field_num");

    QTest::newRow("id_invoice_type") << QString("id_invoice_type") << (int)InvoiceTypeFields::ID_INVOICE_TYPE;
    QTest::newRow("type") << QString("type") << (int)InvoiceTypeFields::TYPE;
}


void InvoiceTypeDataTest::testCaseCheckTypesNames()
{
    QFETCH(int, input_num);
    QFETCH(QString, output_name);

    QVERIFY2(InvoiceTypeData::name((InvoiceTypeData::Type)input_num) == output_name, "Mismatch between numbers and names or between names");
}


void InvoiceTypeDataTest::testCaseCheckTypesNames_data()
{
    QTest::addColumn<int>("input_num");
    QTest::addColumn<QString>("output_name");

    QTest::newRow("-1") << -1 << QString();
    QTest::newRow("0") << (int)InvoiceTypeData::VAT << QString("Faktura VAT");
    QTest::newRow("1") << (int)InvoiceTypeData::PRO_FORMA << QString("Faktura Pro Forma");
    QTest::newRow("2") << (int)InvoiceTypeData::CORRECTIVE_VAT << QString("Korekta");
    QTest::newRow("3") << (int)InvoiceTypeData::GROSS << QString("Faktura brutto");
    QTest::newRow("4") << (int)InvoiceTypeData::CORRECTIVE_GROSS << QString("Korekta brutto");
    QTest::newRow("5") << (int)InvoiceTypeData::BILL << QString("Rachunek");
    QTest::newRow("6") << 6 << QString();
    QTest::newRow("666") << 666 << QString();
}

QTEST_APPLESS_MAIN(InvoiceTypeDataTest)

#include "tst_invoicetypedatatest.moc"
