#include <QTest>
#include <QThread>

#include "../TestsCommon/TestsCommon.h"
#include "MainWindow.h"
#include "CounterpartyDialog.h"


struct MainWindowPublic: public MainWindow
{
    MainWindowPublic(QWidget *parent = 0)
        : MainWindow(parent)
    {
    }
};



class MainWindowTest : public QObject
{
    Q_OBJECT
        
private Q_SLOTS:
    void initTestCase();
    void init();
    void testCaseGUI_InitialState();
    void testCaseAddDelCommodity();
    void testCaseAddDelCounterparty();
};



void MainWindowTest::initTestCase()
{
    TestsCommon::setAppData();
    SettingsGlobal s;
    s.setFirstRun(true);
}

void MainWindowTest::init()
{
    TestsCommon::removeDBFile();
}

void MainWindowTest::testCaseGUI_InitialState()
{
    MainWindowPublic mwp;
    QCOMPARE(mwp.windowTitle(), trUtf8("%1 - %2").arg(qApp->applicationName()).arg(qApp->applicationVersion()));
    QVERIFY(mwp.dateEditFilterEnd->displayFormat() == mwp.dateEditFilterStart->displayFormat());
}



void MainWindowTest::testCaseAddDelCommodity()
{
    const size_t testsCount = 1;
    MainWindowPublic mwp;
    for(size_t i = 0; i < testsCount; ++i)
    {

    }
}

void MainWindowTest::testCaseAddDelCounterparty()
{

}

QTEST_MAIN(MainWindowTest)

#include "tst_mainwindowtest.moc"
