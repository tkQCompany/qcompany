#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include <QtGui>

#include "BillDialog.h"
#include "Database.h"

class BillDialogTest : public QObject
{
    Q_OBJECT
    
public:
    BillDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testGUI_CheckWindowTitle();
    void testGUI_CheckInvoiceType();
    void testGUI_CheckDates();
    //void testCase1_data();
};

BillDialogTest::BillDialogTest()
{
}

void BillDialogTest::initTestCase()
{
    QCoreApplication::setApplicationName("QFaktury");
    QCoreApplication::setOrganizationName("www.e-linux.pl");
    QCoreApplication::setOrganizationDomain("www.e-linux.pl");
    QCoreApplication::setApplicationVersion(APP_VERSION);
}

void BillDialogTest::cleanupTestCase()
{
}



void BillDialogTest::testGUI_CheckWindowTitle()
{
    Database db;
    BillDialog billDialog(0, &db);
    QCOMPARE(billDialog.windowTitle(), QString("Rachunek"));
}


void BillDialogTest::testGUI_CheckInvoiceType()
{
    Database db;
    BillDialog billDialog(0, &db);
    QCOMPARE(billDialog.comboBoxInvoiceType->currentText(), QString("Rachunek"));
}


void BillDialogTest::testGUI_CheckDates()
{
    Database db;
    BillDialog billDialog(0, &db);
    QCOMPARE(billDialog.dateEditDateOfIssuance->date(), QDate::currentDate());
    QCOMPARE(billDialog.dateEditDateOfSell->date(), QDate::currentDate());
}

//void BillDialogTest::testCase1()
//{
//    QFETCH(QString, data);
//    QVERIFY2(true, "Failure");
//}

//void BillDialogTest::testCase1_data()
//{
//    QTest::addColumn<QString>("data");
//    QTest::newRow("0") << QString();
//}

QTEST_MAIN(BillDialogTest)

#include "tst_BillDialogTest.moc"
