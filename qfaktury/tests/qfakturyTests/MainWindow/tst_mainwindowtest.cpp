#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include <QSettings>

#include "MainWindow.h"
#include "CounterpartyDialog.h"

class MainWindowTest : public QObject
{
    Q_OBJECT
    
public:
    MainWindowTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
    void testCaseInitDeinit();
    void testCaseAddDelCommodity();
    void testCaseAddDelCounterparty();

private:
    QSettings settings_;
};

MainWindowTest::MainWindowTest()
{
}

void MainWindowTest::initTestCase()
{
    QCoreApplication::setApplicationName("QFaktury");
    QCoreApplication::setOrganizationName("www.e-linux.pl");
    QCoreApplication::setOrganizationDomain("www.e-linux.pl");
}

void MainWindowTest::cleanupTestCase()
{
    settings_.clear();
}

void MainWindowTest::testCaseInitDeinit()
{
    settings_.clear();
    MainWindow mw;

}

void MainWindowTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void MainWindowTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

void MainWindowTest::testCaseAddDelCommodity()
{
    const size_t testsCount = 1;
    //MainWindow mw;
    for(size_t i = 0; i < testsCount; ++i)
    {
        //mw.actionCommodities_Add
    }
}

void MainWindowTest::testCaseAddDelCounterparty()
{

}

QTEST_MAIN(MainWindowTest)

#include "tst_mainwindowtest.moc"
