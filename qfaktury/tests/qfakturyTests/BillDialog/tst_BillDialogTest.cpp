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
    void testGUI();
    //void testCase1_data();
};

BillDialogTest::BillDialogTest()
{
}

void BillDialogTest::initTestCase()
{
}

void BillDialogTest::cleanupTestCase()
{
}



void BillDialogTest::testGUI()
{
    Database db;
    BillDialog billDialog(0, &db);
    QCOMPARE(billDialog.windowTitle(), QString("Rachunek"));
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
