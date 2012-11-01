#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

class InvoiceGrossDialogTest : public QObject
{
    Q_OBJECT
    
public:
    InvoiceGrossDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

InvoiceGrossDialogTest::InvoiceGrossDialogTest()
{
}

void InvoiceGrossDialogTest::initTestCase()
{
}

void InvoiceGrossDialogTest::cleanupTestCase()
{
}

void InvoiceGrossDialogTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void InvoiceGrossDialogTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(InvoiceGrossDialogTest)

#include "tst_invoicegrossdialogtest.moc"
