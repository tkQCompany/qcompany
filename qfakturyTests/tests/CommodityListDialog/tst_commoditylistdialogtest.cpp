#include <QtTest/QtTest>

#include "../TestsCommon/TestsCommon.h"
#include "../TestsCommon/CommodityListDialogPublic.h"
#include "CommodityListDialog.cpp"
#include "CommodityTypeData.h"
#include "Database.h"

class CommodityListDialogTest : public QObject
{
    Q_OBJECT
    
public:
    CommodityListDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void init();
    void testGUI_InitialState();
    void testGUI_AddedOneCommodity();
    void testGUI_AddedOneCommodity_data();
};

CommodityListDialogTest::CommodityListDialogTest()
{
}

void CommodityListDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    SettingsGlobal s;
    s.setFirstRun(true);
}

void CommodityListDialogTest::init()
{
    TestsCommon::removeDBFile();
}

void CommodityListDialogTest::testGUI_InitialState()
{
    Database db;
    CommodityListDialogPublic listDialog(0, &db);

    QCOMPARE(listDialog.windowTitle(), QString(trUtf8("Wybierz towar z listy")));
    QCOMPARE(listDialog.ui()->comboBoxCommodities->currentText(), CommodityTypeData::name(CommodityTypeData::GOODS));
    QCOMPARE(listDialog.ui()->listViewCommodities->model()->rowCount(), 0);
    QVERIFY(listDialog.ui()->lineEditName->text().isEmpty());
    QVERIFY(listDialog.ui()->lineEditPriceNet1->text().isEmpty());
    QVERIFY(listDialog.ui()->lineEditPriceNet2->text().isEmpty());
    QVERIFY(listDialog.ui()->lineEditPriceNet3->text().isEmpty());
    QVERIFY(listDialog.ui()->lineEditPriceNet4->text().isEmpty());

    QLocale locale;
    QCOMPARE(listDialog.ui()->doubleSpinBoxAmount->value(), 0.0);
    QCOMPARE(listDialog.ui()->spinBoxDiscount->value(), 0);
    QCOMPARE(listDialog.ui()->comboBoxChosenNetPrice->currentText(), CommodityData::header(CommodityFields::NET1));
    QCOMPARE(listDialog.ui()->labelNetVal->text(), locale.toString(0.0, 'f', 2));
}


void CommodityListDialogTest::testGUI_AddedOneCommodity()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}


void CommodityListDialogTest::testGUI_AddedOneCommodity_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(CommodityListDialogTest)

#include "tst_commoditylistdialogtest.moc"
