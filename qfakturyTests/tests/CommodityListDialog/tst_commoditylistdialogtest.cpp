#include <QTest>
#include <QSqlQuery>
#include <QThread>

#include "CommodityData.h"
#include "CommodityTypeData.h"
#include "CommodityListDialog.h"
//#include "CommodityListDialog.cpp"
#include "ui_CommodityListDialog.h"
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
    void testGUI_AddedOneCommodity();
    void testGUI_AddedOneCommodity_data();
private:
    void addCommodityInThread(InvoiceDialogPublic *idp, const CommodityData &cd, const int netValIndex);
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


void CommodityListDialogTest::testGUI_AddedOneCommodity()
{
    QFETCH(CommodityTypeData::CommodityType, commodityType);
    QFETCH(QString, commodityName);
    QFETCH(Money_t, net1);
    QFETCH(Money_t, net2);
    QFETCH(Money_t, net3);
    QFETCH(Money_t, net4);
    QFETCH(double, quantity);
    QFETCH(double, discount);
    QFETCH(int, netValIndex);
    QFETCH(Money_t, resultNetVal);

    QSqlQuery query(db_->modelCommodity()->query());
    query.prepare("INSERT INTO commodity(name, abbreviation, pkwiu, type_id, unit_id, net1, net2, net3, net4, vat, quantity) VALUES(:name, :abbreviation, :pkwiu, :type_id, :unit_id, :net1, :net2, :net3, :net4, :vat, :quantity)");
    query.bindValue(":name", commodityName);
    query.bindValue(":abbreviation", "abbrev");
    query.bindValue(":pkwiu", "pkwiu");
    query.bindValue(":type_id", commodityType);
    query.bindValue(":unit_id", UnitData::PACKAGE);
    query.bindValue(":net1", net1.toString());
    query.bindValue(":net2", net2.toString());
    query.bindValue(":net3", net3.toString());
    query.bindValue(":net4", net4.toString());
    query.bindValue(":vat",  QString("%1").arg(qrand() % 100));
    query.bindValue(":quantity", quantity);
    query.exec();

    QCOMPARE(discount, 0.0);
    QCOMPARE(netValIndex, 3);

}


void CommodityListDialogTest::testGUI_AddedOneCommodity_data()
{
    QTest::addColumn<CommodityTypeData::CommodityType>("commodityType");
    QTest::addColumn<QString>("commodityName");
    QTest::addColumn<Money_t>("net1");
    QTest::addColumn<Money_t>("net2");
    QTest::addColumn<Money_t>("net3");
    QTest::addColumn<Money_t>("net4");
    QTest::addColumn<double>("quantity");
    QTest::addColumn<double>("discount");
    QTest::addColumn<int>("netValIndex");
    QTest::addColumn<Money_t>("resultNetVal");

    QTest::newRow("0") << CommodityTypeData::GOODS
                       << QString("name0")
                       << Money_t(QString("%1,%2").arg(qrand() % 10000).arg(qrand() % 1000))
                       << Money_t(QString("%1,%2").arg(qrand() % 10000).arg(qrand() % 1000))
                       << Money_t(QString("%1,%2").arg(qrand() % 10000).arg(qrand() % 1000))
                       << Money_t(QString("%1,%2").arg(qrand() % 10000).arg(qrand() % 1000))
                       << (double)(qrand() % 100000)
                       << (double)(qrand() % 100)
                       << qrand() % 4
                       << Money_t(0.1);
}


void CommodityListDialogTest::addCommodityInThread(InvoiceDialogPublic *idp, const CommodityData &cd, const int netValIndex)
{
    GuiUserAddCommodity userAddCommod(idp, cd, netValIndex);
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
