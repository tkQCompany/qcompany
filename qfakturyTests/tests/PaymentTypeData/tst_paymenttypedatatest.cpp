#include <QtTest/QtTest>
#include <QSqlQuery>

#include "../TestsCommon.h"
#include "Database.h"
#include "ModelPaymentType.h"
#include "PaymentTypeData.h"

class PaymentTypeDataTest : public QObject
{
    Q_OBJECT
    
public:
    PaymentTypeDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseCheckDBFields();
    void testCaseCheckDBFields_data();
    void testCaseCheckTypesNames();
    void testCaseCheckTypesNames_data();
};

PaymentTypeDataTest::PaymentTypeDataTest()
{
}

void PaymentTypeDataTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void PaymentTypeDataTest::cleanupTestCase()
{
}

void PaymentTypeDataTest::testCaseCheckDBFields()
{
    QFETCH(QString, field_name);
    QFETCH(int, field_num);

    Database db;
    QSqlQuery query(db.modelPaymentType()->query());

    QVERIFY2(query.exec(QString("SELECT %1 FROM 'payment_type'").arg(field_name)),
             QString("Missing DB field in the table 'payment_type': %1").arg(field_name).toAscii());
    QCOMPARE(db.modelPaymentType()->fieldIndex(field_name), field_num);
}

void PaymentTypeDataTest::testCaseCheckDBFields_data()
{
    QTest::addColumn<QString>("field_name");
    QTest::addColumn<int>("field_num");

    QTest::newRow("id_payment_type") << QString("id_payment_type") << (int)PaymentTypeFields::ID_PAYMENT_TYPE;
    QTest::newRow("type") << QString("type") << (int)PaymentTypeFields::TYPE;
}


void PaymentTypeDataTest::testCaseCheckTypesNames()
{
    QFETCH(int, input_num);
    QFETCH(QString, output_name);

    QVERIFY2(PaymentTypeData::name((PaymentTypeData::PaymentTypes)input_num) == output_name, "Mismatch between numbers and names or between names");
}


void PaymentTypeDataTest::testCaseCheckTypesNames_data()
{
    QTest::addColumn<int>("input_num");
    QTest::addColumn<QString>("output_name");

    QTest::newRow("-1") << -1 << QString();
    QTest::newRow("0") << (int)PaymentTypeData::CASH << trUtf8("gotówka");
    QTest::newRow("1") << (int)PaymentTypeData::PREPAYMENT << trUtf8("zaliczka");
    QTest::newRow("2") << (int)PaymentTypeData::TRANSFER << trUtf8("przelew");
    QTest::newRow("3") << 3 << QString();
    QTest::newRow("666") << 666 << QString();
}

QTEST_APPLESS_MAIN(PaymentTypeDataTest)

#include "tst_paymenttypedatatest.moc"
