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
    void addCommodityInThread(InvoiceDialogPublic *idp, const CommodityData &cd, const int netValIndex, const DecVal &discount);
    void startUserThread(GuiUser *guiUser, QThread *thread, QPushButton *buttonStart) const;
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


void CommodityListDialogTest::testGUI_AddManyCommodities()
{
    QFETCH(CommodityTypeData::CommodityType, commodityType);
    QFETCH(QString, commodityName);
    QFETCH(Money_t, net1);
    QFETCH(Money_t, net2);
    QFETCH(Money_t, net3);
    QFETCH(Money_t, net4);
    QFETCH(DecVal, quantity);
    QFETCH(DecVal, discount);
    QFETCH(int, netValIndex);
    QFETCH(Money_t, resultNetVal);

    QSqlQuery query(db_->modelCommodity()->query());
    const int precision = 2;
    query.prepare("INSERT INTO commodity(name, abbreviation, pkwiu, type_id, unit_id, net1, net2, net3, net4, vat, quantity) VALUES(:name, :abbreviation, :pkwiu, :type_id, :unit_id, :net1, :net2, :net3, :net4, :vat, :quantity)");
    query.bindValue(":name", commodityName);
    query.bindValue(":abbreviation", "abbrev");
    query.bindValue(":pkwiu", "pkwiu");
    query.bindValue(":type_id", commodityType + 1);
    query.bindValue(":unit_id", UnitData::PACKAGE + 1);
    query.bindValue(":net1", net1.toString(precision));
    query.bindValue(":net2", net2.toString(precision));
    query.bindValue(":net3", net3.toString(precision));
    query.bindValue(":net4", net4.toString(precision));
    query.bindValue(":vat",  QString("%1").arg(qrand() % 100));

    const DecVal stockQuantity(DecVal(2) * quantity);
    query.bindValue(":quantity", stockQuantity.toString()); //on stock
    const bool retQuery = query.exec();
    if(!retQuery)
    {
        qWarning("%s", qPrintable(query.lastError().text()));
    }
    QVERIFY(retQuery);
    db_->modelCommodity()->submitAll();

    CommodityListDialogPublic listDialog(0, db_);

    static int rowNumber = 0;
    listDialog.getDataWidgetMapper()->setCurrentIndex(rowNumber);
    QCOMPARE(listDialog.ui()->listViewCommodities->model()->rowCount(), ++rowNumber);

    QCOMPARE(listDialog.ui()->comboBoxCommodities->currentIndex(), (int)commodityType);
    QCOMPARE(listDialog.ui()->lineEditName->text(), commodityName);
    QCOMPARE(listDialog.ui()->lineEditPriceNet1->text(), net1.toString(precision));
    QCOMPARE(listDialog.ui()->lineEditPriceNet2->text(), net2.toString(precision));
    QCOMPARE(listDialog.ui()->lineEditPriceNet3->text(), net3.toString(precision));
    QCOMPARE(listDialog.ui()->lineEditPriceNet4->text(), net4.toString(precision));
    QCOMPARE(listDialog.ui()->comboBoxChosenNetPrice->currentIndex(), 0); //0 is default

    listDialog.ui()->comboBoxChosenNetPrice->setCurrentIndex(netValIndex);
    listDialog.ui()->doubleSpinBoxAmount->setValue(quantity.toDouble());
    listDialog.ui()->spinBoxDiscount->setValue((int)discount.toDouble());

    QCOMPARE(listDialog.ui()->labelNetVal->text(), resultNetVal.toString(precision));
}


void CommodityListDialogTest::testGUI_AddManyCommodities_data()
{
    QTest::addColumn<CommodityTypeData::CommodityType>("commodityType");
    QTest::addColumn<QString>("commodityName");
    QTest::addColumn<Money_t>("net1");
    QTest::addColumn<Money_t>("net2");
    QTest::addColumn<Money_t>("net3");
    QTest::addColumn<Money_t>("net4");
    QTest::addColumn<DecVal>("quantity");
    QTest::addColumn<DecVal>("discount");
    QTest::addColumn<int>("netValIndex");
    QTest::addColumn<Money_t>("resultNetVal");

    const int maxNetVals = 4;
    Money_t netVal, nets[maxNetVals];

    for(int i = 0; i < maxNetVals; ++i)
    {
        const QString netStr(QString("%1,%2").arg(qrand() % 10000 + 1).arg(qrand() % 100));
        nets[i] = Money_t(netStr);
    }

    const int maxTests = 100;
    for(int i = 0; i < maxTests; ++i)
    {
        const int netValIndex = qrand() % maxNetVals;
        const DecVal discountPercent = DecVal(qrand() % 100);
        netVal = nets[netValIndex];
        const DecVal quantity = DecVal((qrand() % 100000) + 10);
        const Money_t totalNetVal(netVal * quantity);
        const DecVal onePercent(DecVal(1)/DecVal(100));
        const Money_t discountedTotalNetVal(totalNetVal - (totalNetVal * discountPercent * onePercent));

        QTest::newRow(qPrintable(QString("%1").arg(i))) << CommodityTypeData::GOODS
                           << QString("name_%1").arg(i)
                           << nets[0]
                           << nets[1]
                           << nets[2]
                           << nets[3]
                           << quantity
                           << discountPercent
                           << netValIndex
                           << discountedTotalNetVal;
    }
}


void CommodityListDialogTest::addCommodityInThread(InvoiceDialogPublic *idp, const CommodityData &cd, const int netValIndex, const DecVal &discount)
{
    GuiUserAddCommodity userAddCommod(idp, cd, netValIndex, discount);
    QThread threadCommodity;
    threadCommodity.setObjectName("threadCommodity");
    startUserThread(&userAddCommod, &threadCommodity, idp->ui()->pushButtonAddCommodity);
    while(!threadCommodity.isFinished())
    {
        QTest::qWait(100);
    }
}


void CommodityListDialogTest::startUserThread(GuiUser *guiUser, QThread *thread, QPushButton *buttonStart) const
{
    guiUser->moveToThread(thread);
    connect(thread, SIGNAL(started()), guiUser, SLOT(process()));
    connect(guiUser, SIGNAL(finished()), thread, SLOT(quit()));
    thread->start();
    QTest::mouseClick(buttonStart, Qt::LeftButton);
}

QTEST_MAIN(CommodityListDialogTest)

Q_DECLARE_METATYPE(CommodityTypeData::CommodityType)

#include "tst_commoditylistdialogtest.moc"
