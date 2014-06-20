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
    QCOMPARE(listDialog.ui()->lineEditPriceNet1->text(), commodityData.field(CommodityFields::NET1).value<Money_t>().toString(precision));
    QCOMPARE(listDialog.ui()->lineEditPriceNet2->text(), commodityData.field(CommodityFields::NET2).value<Money_t>().toString(precision));
    QCOMPARE(listDialog.ui()->lineEditPriceNet3->text(), commodityData.field(CommodityFields::NET3).value<Money_t>().toString(precision));
    QCOMPARE(listDialog.ui()->lineEditPriceNet4->text(), commodityData.field(CommodityFields::NET4).value<Money_t>().toString(precision));
    QCOMPARE(listDialog.ui()->comboBoxChosenNetPrice->currentIndex(), 0); //0 is default

    listDialog.ui()->comboBoxChosenNetPrice->setCurrentIndex(commodityIndex % 4);

    listDialog.ui()->doubleSpinBoxAmount->setValue(commodityVisualData.field(CommodityVisualFields::QUANTITY).value<DecVal>().toDouble());
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
    query.bindValue(":name", cd.field(CommodityFields::NAME));
    query.bindValue(":abbreviation", cd.field(CommodityFields::ABBREV));
    query.bindValue(":pkwiu", cd.field(CommodityFields::PKWIU));
    query.bindValue(":type_id", cd.field(CommodityFields::TYPE_ID));
    query.bindValue(":unit_id", cd.field(CommodityFields::UNIT_ID));
    query.bindValue(":net1", cd.field(CommodityFields::NET1).value<Money_t>().toString(precision));
    query.bindValue(":net2", cd.field(CommodityFields::NET2).value<Money_t>().toString(precision));
    query.bindValue(":net3", cd.field(CommodityFields::NET3).value<Money_t>().toString(precision));
    query.bindValue(":net4", cd.field(CommodityFields::NET4).value<Money_t>().toString(precision));
    query.bindValue(":vat",  cd.field(CommodityFields::VAT).value<DecVal>().toString(precision));

    const DecVal stockQuantity(DecVal(2) * cd.field(CommodityFields::QUANTITY).value<DecVal>());
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
    QVariant v;
    const QChar decPoint(s.decimalPointStr().at(0));

    cd.setField(CommodityFields::ABBREV, QString("%1").arg(commodityIndex));
    cd.setField(CommodityFields::ID_COMMODITY, QString("%1").arg(commodityIndex + 1));
    cd.setField(CommodityFields::NAME, QString("name_%1").arg(commodityIndex));

    v.setValue(Money_t(QString("%1%2%3").arg(commodityIndex).arg(decPoint).arg("11")));
    cd.setField(CommodityFields::NET1, v);

    v.setValue(Money_t(QString("%1%2%3").arg(commodityIndex).arg(decPoint).arg("12")));
    cd.setField(CommodityFields::NET2, v);

    v.setValue(Money_t(QString("%1%2%3").arg(commodityIndex).arg(decPoint).arg("13")));
    cd.setField(CommodityFields::NET3, v);

    v.setValue(Money_t(QString("%1%2%3").arg(commodityIndex).arg(decPoint).arg("14")));
    cd.setField(CommodityFields::NET4, v);
    cd.setField(CommodityFields::PKWIU, QString("pkwiu_%1").arg(commodityIndex));

    v.setValue(DecVal(QString("%1").arg(commodityIndex+1)));
    cd.setField(CommodityFields::QUANTITY, v);
    cd.setField(CommodityFields::TYPE_ID, CommodityTypeData::GOODS + 1);
    cd.setField(CommodityFields::UNIT_ID, UnitData::KG + 1);

    const int listVatSize = db_->modelVat()->listVAT().size();
    const QString selectedVAT(db_->modelVat()->listVAT().at(commodityIndex % listVatSize));
    v.setValue(DecVal(selectedVAT));
    cd.setField(CommodityFields::VAT, v);

    return cd;
}


const CommodityVisualData CommodityListDialogTest::generateCommodityVisualData(const CommodityData &cd) const
{
    CommodityVisualData cvd;
    QVariant v;

    const int commodityIndex = cd.field(CommodityFields::ID_COMMODITY).toInt() - 1;

    cvd.setField(CommodityVisualFields::DISCOUNT, 0);
    cvd.setField(CommodityVisualFields::ID, commodityIndex);
    cvd.setField(CommodityVisualFields::NAME, cd.field(CommodityFields::NAME));

    v.setValue(cd.field(CommodityFields::Field(CommodityFields::NET1 + commodityIndex % 4)).value<Money_t>());
    cvd.setField(CommodityVisualFields::NET, v);

    cvd.setField(CommodityVisualFields::PKWIU,cd.field(CommodityFields::PKWIU));

    v.setValue(cd.field(CommodityFields::QUANTITY).value<DecVal>());
    cvd.setField(CommodityVisualFields::QUANTITY, v);

    const CommodityTypeData::CommodityType type = cd.field(CommodityFields::TYPE_ID).value<CommodityTypeData::CommodityType>();
    cvd.setField(CommodityVisualFields::TYPE, type);

    const UnitData::Name unit = (UnitData::Name)cd.field(CommodityFields::UNIT_ID).toInt();
    cvd.setField(CommodityVisualFields::UNIT, unit);

    v.setValue(cd.field(CommodityFields::VAT).value<DecVal>());
    cvd.setField(CommodityVisualFields::VAT, v);

    return cvd;
}


QTEST_MAIN(CommodityListDialogTest)

Q_DECLARE_METATYPE(CommodityTypeData::CommodityType)

#include "tst_commoditylistdialogtest.moc"
