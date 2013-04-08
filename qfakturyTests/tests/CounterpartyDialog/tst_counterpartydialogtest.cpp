#include <QtTest/QtTest>

#include "../TestsCommon.h"
#include "CounterpartyDialog.h"
#include "Database.h"
#include "SettingsGlobal.h"

class CounterpartyDialogTest : public QObject
{
    Q_OBJECT
    
public:
    CounterpartyDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseInitialValues();
    void testCase1();
    void testCase1_data();
};

CounterpartyDialogTest::CounterpartyDialogTest()
{
}

void CounterpartyDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void CounterpartyDialogTest::cleanupTestCase()
{
}


void CounterpartyDialogTest::testCaseInitialValues()
{
    Database db;
    SettingsGlobal s;
    CounterpartyDialog dialog(0, &db, QModelIndex());
//    QCOMPARE(dialog.comboBoxType->currentText(), CounterpartyTypeData::name(CounterpartyTypeData::COMPANY));
//    QCOMPARE(dialog.comboBoxCountry->currentText(), s.value(s.COUNTRY).toString());
}

void CounterpartyDialogTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void CounterpartyDialogTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(CounterpartyDialogTest)

#include "tst_counterpartydialogtest.moc"
