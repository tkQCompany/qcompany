#include <QtCore/QString>
#include <QtTest/QtTest>

#include "Database.h"

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

    QVERIFY2(InvoiceTypeData::InvoiceTypeToString(input_num) == output_name, "Mismatch between numbers and names or between names");
}


void InvoiceTypeDataTest::testCaseCheckTypesNames_data()
{
    QTest::addColumn<int>("input_num");
    QTest::addColumn<QString>("output_name");

    QTest::newRow("-1") << -1 << QString();
    QTest::newRow("0") << -1 << QString();
    QTest::newRow("1") << 1 << QString("Faktura VAT");
    QTest::newRow("2") << 2 << QString("Faktura Pro Forma");
    QTest::newRow("3") << 3 << QString("Korekta");
    QTest::newRow("4") << 4 << QString("Faktura brutto");
    QTest::newRow("5") << 5 << QString("Korekta brutto");
    QTest::newRow("6") << 6 << QString("Rachunek");
    QTest::newRow("7") << 7 << QString();
    QTest::newRow("666") << 666 << QString();
}

QTEST_APPLESS_MAIN(InvoiceTypeDataTest)

#include "tst_invoicetypedatatest.moc"
