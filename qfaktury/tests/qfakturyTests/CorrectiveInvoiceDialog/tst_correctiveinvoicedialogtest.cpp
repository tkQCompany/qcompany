#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

class CorrectiveInvoiceDialogTest : public QObject
{
    Q_OBJECT
    
public:
    CorrectiveInvoiceDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

CorrectiveInvoiceDialogTest::CorrectiveInvoiceDialogTest()
{
}

void CorrectiveInvoiceDialogTest::initTestCase()
{
}

void CorrectiveInvoiceDialogTest::cleanupTestCase()
{
}

void CorrectiveInvoiceDialogTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void CorrectiveInvoiceDialogTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(CorrectiveInvoiceDialogTest)

#include "tst_correctiveinvoicedialogtest.moc"
