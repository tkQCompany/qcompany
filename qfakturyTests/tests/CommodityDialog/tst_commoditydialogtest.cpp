#include <QTest>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>

#include "../../TestsCommon/TestsCommon.h"
#include "CommodityDialog.h"
#include "CommodityDialog.cpp"
#include "Database.h"
#include "CounterpartyTypeData.h"
#include "UnitData.h"

class CommodityDialogPublic: public CommodityDialog
{
    Q_OBJECT

public:
    CommodityDialogPublic(QWidget *parent, Database *db, const QModelIndex &id_edit = QModelIndex())
        : CommodityDialog(parent, db, id_edit)
    {
    }

    Ui::CommodityDialog* ui() const {return CommodityDialog::ui;}
};


class CommodityDialogTest : public QObject
{
    Q_OBJECT
    
private Q_SLOTS:
    void initTestCase();
    void testCaseInitialValues();
    void testCase_AddCommodity();
    void testCase_AddCommodity_data();
};

void CommodityDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    SettingsGlobal s;
    s.setFirstRun(true);
    TestsCommon::removeDBFile();
}


void CommodityDialogTest::testCaseInitialValues()
{
    Database db;
    SettingsGlobal s;
    s.setFirstRun(false);

    CommodityDialogPublic dialog(0, &db);
    QCOMPARE(dialog.ui()->comboBoxType->currentText(), CommodityTypeData::name(CommodityTypeData::GOODS));
    QCOMPARE(dialog.ui()->comboBoxMeasureUnit->currentText(), UnitData::name(UnitData::UNIT));
    QCOMPARE(dialog.ui()->comboBoxVat->currentIndex(), 0);
}

void CommodityDialogTest::testCase_AddCommodity()
{
    Database db;
    CommodityDialogPublic cdp(0, &db);

    QFETCH(QString, name);
    QFETCH(QString, abbr);
    QFETCH(QString, pkwiu);
    QFETCH(QString, type);
    QFETCH(QString, unit);
    QFETCH(int, vatIndex);
    QFETCH(double, net1);
    QFETCH(double, net2);
    QFETCH(double, net3);
    QFETCH(double, net4);
    QFETCH(double, quantity);

    QSqlQuery q(db.modelCommodity()->query());
    if(!q.prepare("SELECT id_commodity, abbreviation, pkwiu, type_id, unit_id, net1, net2, net3, net4, vat, quantity FROM commodity WHERE name = :name"))
    {
        qCritical() << "Preparation of SQL query has failed at line: " << __LINE__ << " (" << q.lastError().text() << ")";
    }
    q.bindValue(":name", name);
    QVERIFY(q.exec());
    if(q.isActive())
    {
        QCOMPARE(q.next(), false);
    }
    else
    {
        QFAIL("First SQL query failed.");
    }

    SettingsGlobal s;

    QTest::keyClicks(cdp.ui()->lineEditName, name);
    QTest::keyClicks(cdp.ui()->lineEditAbbreviation, abbr);
    QTest::keyClicks(cdp.ui()->lineEditPKWIU, pkwiu);
    cdp.ui()->comboBoxType->setCurrentIndex(cdp.ui()->comboBoxType->findText(type));
    cdp.ui()->comboBoxMeasureUnit->setCurrentIndex(cdp.ui()->comboBoxMeasureUnit->findText(unit));
    cdp.ui()->comboBoxVat->setCurrentIndex(vatIndex);
    QTest::keyClicks(cdp.ui()->lineEditNet1, s.numberToString(net1, 'f', 2));
    QTest::keyClicks(cdp.ui()->lineEditNet2, s.numberToString(net2, 'f', 2));
    QTest::keyClicks(cdp.ui()->lineEditNet3, s.numberToString(net3, 'f', 2));
    QTest::keyClicks(cdp.ui()->lineEditNet4, s.numberToString(net4, 'f', 2));
    cdp.ui()->doubleSpinBoxQuantity->setValue(quantity);
    QTest::mouseClick(cdp.ui()->pushButtonOK, Qt::LeftButton);
    QVERIFY(db.modelCommodity()->submitAll());
    QCOMPARE(db.modelCommodity()->lastError().type(), QSqlError::NoError);

    QVERIFY(q.exec());
    if(q.isActive())
    {
        QCOMPARE(q.next(), true);
        QCOMPARE(q.value(0).toLongLong(), QString(QTest::currentDataTag()).toLongLong());
        QCOMPARE(q.value(1).toString(), abbr);
        QCOMPARE(q.value(2).toString(), pkwiu);
        QCOMPARE(CommodityTypeData::name((CommodityTypeData::CommodityType)(q.value(3).toInt() - 1)), type);
        QCOMPARE(UnitData::name((UnitData::Name)(q.value(4).toInt() - 1)), unit);
        QCOMPARE(q.value(5).toString().replace(QChar(','), QChar('.')).toDouble(), net1);
        QCOMPARE(q.value(6).toString().replace(QChar(','), QChar('.')).toDouble(), net2);
        QCOMPARE(q.value(7).toString().replace(QChar(','), QChar('.')).toDouble(), net3);
        QCOMPARE(q.value(8).toString().replace(QChar(','), QChar('.')).toDouble(), net4);
        QCOMPARE(q.value(9).toString(), cdp.ui()->comboBoxVat->itemText(vatIndex));
        QCOMPARE(q.value(10).toDouble(), quantity);
    }
    else
    {
        QFAIL("Second SQL query failed.");
    }
}

void CommodityDialogTest::testCase_AddCommodity_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("abbr");
    QTest::addColumn<QString>("pkwiu");
    QTest::addColumn<QString>("type");
    QTest::addColumn<QString>("unit");
    QTest::addColumn<int>("vatIndex");
    QTest::addColumn<double>("net1");
    QTest::addColumn<double>("net2");
    QTest::addColumn<double>("net3");
    QTest::addColumn<double>("net4");
    QTest::addColumn<double>("quantity");

    QTest::newRow("1") << QString("nameTest1") << QString("abbrTest1") << QString("pkwiuTest1")
                       << CommodityTypeData::name(CommodityTypeData::GOODS) << UnitData::name(UnitData::UNIT) << 0
                       << 1.23 << 2.23 << 3.23 << 4.23
                       << 5.231;
    QTest::newRow("2") << QString("nameTest2") << QString("abbrTest2") << QString("pkwiuTest2")
                       << CommodityTypeData::name(CommodityTypeData::GOODS) << UnitData::name(UnitData::KG) << 0
                       << 1.23 << 2.23 << 3.23 << 4.23
                       << 5.231;
    QTest::newRow("3") << QString("nameTest3") << QString("abbrTest3") << QString("pkwiuTest3")
                       << CommodityTypeData::name(CommodityTypeData::GOODS) << UnitData::name(UnitData::G) << 0
                       << 1.23 << 2.23 << 3.23 << 4.23
                       << 5.231;
    QTest::newRow("4") << QString("nameTest4") << QString("abbrTest4") << QString("pkwiuTest4")
                       << CommodityTypeData::name(CommodityTypeData::GOODS) << UnitData::name(UnitData::M) << 0
                       << 1.23 << 2.23 << 3.23 << 4.23
                       << 5.231;
    QTest::newRow("5") << QString("nameTest5") << QString("abbrTest5") << QString("pkwiuTest5")
                       << CommodityTypeData::name(CommodityTypeData::GOODS) << UnitData::name(UnitData::KM) << 0
                       << 1.23 << 2.23 << 3.23 << 4.23
                       << 5.231;
    QTest::newRow("6") << QString("nameTest6") << QString("abbrTest6") << QString("pkwiuTest6")
                       << CommodityTypeData::name(CommodityTypeData::GOODS) << UnitData::name(UnitData::HOUR) << 0
                       << 1.23 << 2.23 << 3.23 << 4.23
                       << 5.231;
    QTest::newRow("7") << QString("nameTest7") << QString("abbrTest7") << QString("pkwiuTest7")
                       << CommodityTypeData::name(CommodityTypeData::GOODS) << UnitData::name(UnitData::PACKAGE) << 0
                       << 1.23 << 2.23 << 3.23 << 4.23
                       << 5.231;
}

QTEST_MAIN(CommodityDialogTest)

#include "tst_commoditydialogtest.moc"
