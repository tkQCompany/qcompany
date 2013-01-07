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
    void testGUI_InitialState();
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

    const QString dbFilename(QString("%1-%2.db3").arg(QCoreApplication::applicationName()).arg(APP_VERSION));
    if(QFile::exists(dbFilename))
    {
        QDir dir;
        dir.remove(dbFilename);
    }
}

void BillDialogTest::cleanupTestCase()
{
}


void BillDialogTest::testGUI_InitialState()
{
    Database db;
    BillDialog billDialog(0, &db);
    QCOMPARE(billDialog.windowTitle(), QString("Rachunek"));
    QCOMPARE(billDialog.comboBoxInvoiceType->currentText(), QString("Rachunek"));
    QCOMPARE(billDialog.dateEditDateOfIssuance->date(), QDate::currentDate());
    QCOMPARE(billDialog.dateEditDateOfSell->date(), QDate::currentDate());
}


QTEST_MAIN(BillDialogTest)

#include "tst_BillDialogTest.moc"
