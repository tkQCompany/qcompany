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
    void testCase1_data();
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
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void MainWindowTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(MainWindowTest)

#include "tst_mainwindowtest.moc"
