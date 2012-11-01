#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

class CorrectiveInvoiceGrossDialogTest : public QObject
{
    Q_OBJECT
    
public:
    CorrectiveInvoiceGrossDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

CorrectiveInvoiceGrossDialogTest::CorrectiveInvoiceGrossDialogTest()
{
}

void CorrectiveInvoiceGrossDialogTest::initTestCase()
{
}

void CorrectiveInvoiceGrossDialogTest::cleanupTestCase()
{
}

void CorrectiveInvoiceGrossDialogTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void CorrectiveInvoiceGrossDialogTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(CorrectiveInvoiceGrossDialogTest)

#include "tst_correctiveinvoicegrossdialogtest.moc"
