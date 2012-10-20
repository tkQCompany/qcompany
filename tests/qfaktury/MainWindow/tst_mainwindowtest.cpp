#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

class MainWindowTest : public QObject
{
    Q_OBJECT
    
public:
    MainWindowTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
};

MainWindowTest::MainWindowTest()
{
}

void MainWindowTest::initTestCase()
{
}

void MainWindowTest::cleanupTestCase()
{
}

void MainWindowTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_MAIN(MainWindowTest)

#include "tst_mainwindowtest.moc"
