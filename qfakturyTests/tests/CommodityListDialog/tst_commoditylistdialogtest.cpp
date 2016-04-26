#include <QTest>
#include <QSqlQuery>
#include <QSqlError>
#include <QThread>

#include "CommodityData.h"
#include "CommodityTypeData.h"
#include "CommodityListDialog.h"
#include "Database.h"
#include "UnitData.h"
#include "Money_t.h"
#include "ModelCommodity.h"
#include "ModelVat.h"
#include "../../TestsCommon/TestsCommon.h"
#include "../../TestsCommon/InvoiceDialogPublic.h"
#include "../../TestsCommon/GuiUser.h"
#include "../../TestsCommon/GuiUserAddCommodity.h"
#include "../../TestsCommon/CommodityListDialogPublic.h"


class CommodityListDialogTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testGUI_InitialState();
    void testGUI_AddManyCommodities();
    void testGUI_AddManyCommodities_data();
private:
    void insertNewCommodity(const CommodityData &cd);
    const CommodityData generateCommodityData(const int commodityIndex) const;
    const CommodityVisualData generateCommodityVisualData(const CommodityData &cd) const;
private:
    Database *db_;
};


void CommodityListDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    SettingsGlobal s;
    s.setFirstRun(true);
    qsrand(QTime::currentTime().msec());
    TestsCommon::removeDBFile();
    db_ = new Database();
}

void CommodityListDialogTest::cleanupTestCase()
{
    delete db_;
}

void CommodityListDialogTest::testGUI_InitialState()
{
    CommodityListDialogPublic listDialog(0, db_);

    QCOMPARE(listDialog.windowTitle(), QString(trUtf8("Wybierz towar z listy")));
    QCOMPARE(listDialog.ui()->comboBoxCommodityTypes->currentText(), CommodityTypeData::name(CommodityTypeData::GOODS));
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


void CommodityListDialogTest::testGUI_AddManyCommodities()
{
    QFETCH(CommodityData, commodityData);
    QFETCH(CommodityVisualData, commodityVisualData);
    QFETCH(Money_t, resultNetVal);

    const int precision = 2;
    const int commodityIndex = QString(QTest::currentDataTag()).toInt();
    insertNewCommodity(commodityData);

    CommodityListDialogPublic listDialog(0, db_);

    listDialog.getDataWidgetMapper()->setCurrentIndex(commodityIndex);
    QCOMPARE(listDialog.ui()->listViewCommodities->model()->rowCount(), commodityIndex + 1);

    QCOMPARE(listDialog.ui()->comboBoxCommodityTypes->currentIndex(), (int)CommodityTypeData::GOODS);
    QCOMPARE(listDialog.ui()->lineEditName->text(), QString("name_%1").arg(commodityIndex));
    QCOMPARE(listDialog.ui()->lineEditPriceNet1->text(), commodityData.net1().toString(precision));
    QCOMPARE(listDialog.ui()->lineEditPriceNet2->text(), commodityData.net2().toString(precision));
    QCOMPARE(listDialog.ui()->lineEditPriceNet3->text(), commodityData.net3().toString(precision));
    QCOMPARE(listDialog.ui()->lineEditPriceNet4->text(), commodityData.net4().toString(precision));
    QCOMPARE(listDialog.ui()->comboBoxChosenNetPrice->currentIndex(), 0); //0 is default

    listDialog.ui()->comboBoxChosenNetPrice->setCurrentIndex(commodityIndex % 4);

    listDialog.ui()->doubleSpinBoxAmount->setValue(commodityVisualData.quantity().toDouble());
    listDialog.ui()->spinBoxDiscount->setValue(0);

    QCOMPARE(listDialog.ui()->labelNetVal->text(), resultNetVal.toString(precision));
}


void CommodityListDialogTest::testGUI_AddManyCommodities_data()
{
    QTest::addColumn<CommodityData>("commodityData");
    QTest::addColumn<CommodityVisualData>("commodityVisualData");
    QTest::addColumn<Money_t>("resultNetVal");

    const int maxTests = 100;
    for(int i = 0; i < maxTests; ++i)
    {
        const CommodityData cd(generateCommodityData(i));
        const CommodityVisualData cvd(generateCommodityVisualData(cd));

        const DecVal quantity(i + 1);
        const Money_t netVal(Money_t(i) + Money_t(11) / DecVal(100) + Money_t(1) / DecVal(100) * DecVal( i % 4) );
        const Money_t resultNetVal(netVal * quantity);

        QTest::newRow(qPrintable(QString("%1").arg(i)))
                           << cd
                           << cvd
                           << resultNetVal;
    }
}


void CommodityListDialogTest::insertNewCommodity(const CommodityData &cd)
{
    const int precision = 2;
    QSqlQuery query(db_->modelCommodity()->query());
    query.prepare("INSERT INTO commodity(name, abbreviation, pkwiu, type_id, unit_id, net1, net2, net3, net4, vat, quantity) VALUES(:name, :abbreviation, :pkwiu, :type_id, :unit_id, :net1, :net2, :net3, :net4, :vat, :quantity)");
    query.bindValue(":name", cd.name());
    query.bindValue(":abbreviation", cd.abbrev());
    query.bindValue(":pkwiu", cd.pkwiu());
    query.bindValue(":type_id", cd.typeId());
    query.bindValue(":unit_id", cd.unitId());
    query.bindValue(":net1", cd.net1().toString(precision));
    query.bindValue(":net2", cd.net2().toString(precision));
    query.bindValue(":net3", cd.net3().toString(precision));
    query.bindValue(":net4", cd.net4().toString(precision));
    query.bindValue(":vat",  cd.vat().toString(precision));

    const DecVal stockQuantity(DecVal(2) * cd.quantity());
    query.bindValue(":quantity", stockQuantity.toDouble()); //on stock
    const bool retQuery = query.exec();
    if(!retQuery)
    {
        qWarning("%s", qPrintable(query.lastError().text()) );
    }
    QVERIFY(retQuery);
    db_->modelCommodity()->submitAll();
}


const CommodityData CommodityListDialogTest::generateCommodityData(const int commodityIndex) const
{
    CommodityData cd;
    SettingsGlobal s;
    const QChar decPoint(s.decimalPoint());

    cd.setAbbrev(QString("%1").arg(commodityIndex));
    cd.setId(commodityIndex + 1);
    cd.setName(QString("name_%1").arg(commodityIndex));
    cd.setNet1(Money_t(QString("%1%2%3").arg(commodityIndex).arg(decPoint).arg("11")));
    cd.setNet2(Money_t(QString("%1%2%3").arg(commodityIndex).arg(decPoint).arg("12")));
    cd.setNet3(Money_t(QString("%1%2%3").arg(commodityIndex).arg(decPoint).arg("13")));
    cd.setNet4(Money_t(QString("%1%2%3").arg(commodityIndex).arg(decPoint).arg("14")));
    cd.setPkwiu(QString("pkwiu_%1").arg(commodityIndex));
    cd.setQuantity(DecVal(QString("%1").arg(commodityIndex+1)));
    cd.setTypeId(CommodityTypeData::GOODS + 1);
    cd.setUnitId(UnitData::KG + 1);

    const int listVatSize = db_->modelVat()->listVAT().size();
    const QString selectedVAT(db_->modelVat()->listVAT().at(commodityIndex % listVatSize));
    cd.setVat(DecVal(selectedVAT));

    return cd;
}


const CommodityVisualData CommodityListDialogTest::generateCommodityVisualData(const CommodityData &cd) const
{
    CommodityVisualData cvd;

    const long long commodityIndex = cd.id() - 1LL;

    cvd.setDiscount(DecVal(0));
    cvd.setID(commodityIndex);
    cvd.setName(cd.name());

    switch(commodityIndex % 4)
    {
    case 0:
        cvd.setNet(cd.net1());
        break;
    case 1:
        cvd.setNet(cd.net2());
        break;
    case 2:
        cvd.setNet(cd.net3());
        break;
    case 3:
        cvd.setNet(cd.net4());
        break;
    default:
        qDebug("CommodityListDialogTest::generateCommodityVisualData(): unexpected value in switch()");
    }

    cvd.setPkwiu(cd.pkwiu());
    cvd.setQuantity(cd.quantity());

    const CommodityTypeData::CommodityType type = (CommodityTypeData::CommodityType)cd.typeId();
    cvd.setType(CommodityTypeData::name(type));

    const UnitData::Name unit = (UnitData::Name)cd.unitId();
    cvd.setUnit(UnitData::name(unit));

    cvd.setVat(cd.vat());

    return cvd;
}


QTEST_MAIN(CommodityListDialogTest)

Q_DECLARE_METATYPE(CommodityTypeData::CommodityType)

#include "tst_commoditylistdialogtest.moc"
