#include <QTest>
#include <QThread>
#include <QSqlQuery>
#include <QSqlError>

#include "../TestsCommon/TestsCommon.h"
#include "../TestsCommon/GuiUserAddCommodity.h"
#include "../TestsCommon/GuiUserAddNewCommodity.h"
#include "../TestsCommon/GuiUserAddCounterparty.h"
#include "../TestsCommon/Counterparty_t.h"
#include "../TestsCommon/MainWindowPublic.h"
#include "CounterpartyDialog.h"
#include "ModelCommodity.h"
#include "ModelCountry.h"
#include "ModelInvoice.h"
#include "CommodityTypeData.h"
#include "UnitData.h"
#include "CounterpartyTypeData.h"



class MainWindowTest : public QObject
{
    Q_OBJECT
        
private Q_SLOTS:
    void initTestCase();
    void testCaseGUI_InitialState();
    void testCaseAddDelCommodity();
    void testCaseAddDelCommodity_data();
    void testCaseAddDelCounterparty();
    void testCaseAddDelCounterparty_data();

private:
    void startUserThread(GuiUser *guiUser, QThread *thread, QAction *actionStart) const;
};



void MainWindowTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
    SettingsGlobal s;
    s.setFirstRun(true);
    qsrand(QTime::currentTime().msec());
}


void MainWindowTest::testCaseGUI_InitialState()
{
    MainWindowPublic mwp;
    QCOMPARE(mwp.windowTitle(), trUtf8("%1 - %2").arg(qApp->applicationName()).arg(qApp->applicationVersion()));
    QVERIFY(mwp.ui()->dateEditFilterEnd->displayFormat() == mwp.ui()->dateEditFilterStart->displayFormat());
}



void MainWindowTest::testCaseAddDelCommodity()
{
    QFETCH(QList<CommodityData>, lcd);
    QFETCH(Counterparty_t, counterparty);
    QFETCH(QString, invoiceNumber);
    QFETCH(InvoiceTypeData::Type, invoiceType);

    MainWindowPublic mwp;
    foreach(const CommodityData cd, lcd)
    {
        {
            GuiUserAddNewCommodity userAddNewCommod(&mwp, cd);
            QThread threadNewCommodity;
            threadNewCommodity.setObjectName("threadNewCommodity");
            startUserThread(&userAddNewCommod, &threadNewCommodity, mwp.ui()->actionCommodities_Add);
            while(!threadNewCommodity.isFinished())
            {
                QTest::qWait(200);
            }

            QVERIFY(mwp.database()->modelCommodity()->submitAll());
        }

        QSqlQuery query(mwp.database()->modelCommodity()->query());
        query.exec(QString("SELECT name, abbreviation, pkwiu, type_id, unit_id, net1, net2, net3, net4, vat, quantity FROM commodity WHERE id_commodity = (SELECT MAX(id_commodity) FROM commodity)"));
        if(!query.isActive())
        {
            const QString errMsg(QString("InvoiceDialogTest::testGUI_AddDeleteCommodities(): first SQL query failed. Reason: %1\nQuery: %2").arg(query.lastError().text()).arg(query.lastQuery()));
            QFAIL(qPrintable(errMsg));
        }
        QVERIFY(query.next());
        QCOMPARE(query.value(0).toString(), cd.field(CommodityFields::NAME).toString());
        QCOMPARE(query.value(1).toString(), cd.field(CommodityFields::ABBREV).toString());
        QCOMPARE(query.value(2).toString(), cd.field(CommodityFields::PKWIU).toString());
        QCOMPARE(query.value(3).toInt() - 1, cd.field(CommodityFields::TYPE_ID).toInt()); //-1 because SQL starts from 1
        QCOMPARE(query.value(4).toInt() - 1, cd.field(CommodityFields::UNIT_ID).toInt()); //-1 because SQL starts from 1
        QCOMPARE(query.value(5).toString(), cd.field(CommodityFields::NET1).toString());
        QCOMPARE(query.value(6).toString(), cd.field(CommodityFields::NET2).toString());
        QCOMPARE(query.value(7).toString(), cd.field(CommodityFields::NET3).toString());
        QCOMPARE(query.value(8).toString(), cd.field(CommodityFields::NET4).toString());
        QCOMPARE(query.value(9).toString(), cd.field(CommodityFields::VAT).toString());
        QCOMPARE(query.value(10).toString(), cd.field(CommodityFields::QUANTITY).toString());
    }

}


void MainWindowTest::testCaseAddDelCommodity_data()
{
    QTest::addColumn<QList<CommodityData> >("lcd");
    QTest::addColumn<Counterparty_t>("counterparty");
    QTest::addColumn<QString>("invoiceNumber");
    QTest::addColumn<InvoiceTypeData::Type>("invoiceType");

    QList<CommodityData> lcd;
    Counterparty_t counterparty;
    QString invNum;
    InvoiceTypeData::Type invType;

    SettingsGlobal s;
    Database db;
    QVariant v;
    const QStringList vatRates(s.value(s.keyName(s.VAT_RATES)).toString().split("|"));

    const int maxRows = qrand() % 20 + 1; //1-20
    const int base = 10;
    for(int row = 0; row < maxRows; ++row)
    {
        const int maxCommod = qrand() % 20 + 1; //1-20 commodities per invoice
        lcd.clear();
        for(int commod = 0; commod < maxCommod; ++commod)
        {
            CommodityData cd;

            cd.setField(CommodityFields::ABBREV, QString("abbrev_%1").arg(row));
            cd.setField(CommodityFields::NAME, QString("name_%1_%2").arg(row).arg(commod));

            v.setValue(Money_t(QString("%1%2%3").arg(commod).arg('.').arg(11)));
            cd.setField(CommodityFields::NET1, v);

            v.setValue(Money_t(QString("%1%2%3").arg(commod).arg('.').arg(12)));
            cd.setField(CommodityFields::NET2, v);

            v.setValue(Money_t(QString("%1%2%3").arg(commod).arg('.').arg(13)));
            cd.setField(CommodityFields::NET3, v);

            v.setValue(Money_t(QString("%1%2%3").arg(commod).arg('.').arg(14)));
            cd.setField(CommodityFields::NET4, v);

            cd.setField(CommodityFields::PKWIU, QString("pkwiu_%1_%2").arg(row).arg(commod));
            cd.setField(CommodityFields::QUANTITY, QString("%1").arg(qrand() % 1000 + 1));
            cd.setField(CommodityFields::VAT, vatRates[qrand() % vatRates.size()]);
            const CommodityTypeData::CommodityType type = CommodityTypeData::GOODS;
            cd.setField(CommodityFields::TYPE_ID, type);
            const UnitData::Name unit = UnitData::Name(qrand() % (UnitData::PACKAGE - UnitData::UNIT + 1));
            cd.setField(CommodityFields::UNIT_ID, unit);

            lcd.append(cd);
        }
        counterparty.accountNumber = QString("%1-%2-%3-%4-%5-%6-%7")
                .arg(qrand() % 100, 2, base, QChar('0'))
                .arg(qrand() % 10000, 4, base, QChar('0'))
                .arg(qrand() % 10000, 4, base, QChar('0'))
                .arg(qrand() % 10000, 4, base, QChar('0'))
                .arg(qrand() % 10000, 4, base, QChar('0'))
                .arg(qrand() % 10000, 4, base, QChar('0'))
                .arg(qrand() % 10000, 4, base, QChar('0'));
        counterparty.city = QString("city_%1").arg(row);
        const QStringList countries(db.modelCountry()->stringList());
        counterparty.country = QString("%1").arg(countries.at(qrand() % (countries.size())));
        counterparty.email = QString("%1@test.pl").arg(row);
        counterparty.name = QString("name_%1").arg(row);
        counterparty.phone = QString("%1").arg(qrand(), 10, base, QChar('1'));
        counterparty.postalCode = QString("postalCode_%1").arg(row);
        counterparty.street = QString("street_%1").arg(row);
        counterparty.taxID = QString("%1-%2-%3-%4")
                .arg(qrand()%1000, 3, base, QChar('0'))
                .arg(qrand()%1000, 3, base, QChar('0'))
                .arg(qrand()%100, 2, base, QChar('0'))
                .arg(qrand()%100, 2, base, QChar('0'));
        counterparty.type = CounterpartyTypeData::name( (qrand() % 2)? CounterpartyTypeData::COMPANY : CounterpartyTypeData::OFFICE);
        counterparty.www = QString("http://www.row%1.com").arg(row);

        invType = InvoiceTypeData::VAT;

        invNum = db.modelInvoice()->generateInvoiceNumber(
                    *(InvoiceNumberFormat_t::Parse(s.value(s.DEFAULT_INV_NUM_FORMAT).toString()).get()),
                                         invNum,
                                         QDate::currentDate(),
                                         QDate::currentDate(),
                                         invType);

        QTest::newRow(qPrintable(QString("%1").arg(row))) << lcd << counterparty << invNum << invType;
    }
}



void MainWindowTest::testCaseAddDelCounterparty()
{

}



void MainWindowTest::testCaseAddDelCounterparty_data()
{

}


void MainWindowTest::startUserThread(GuiUser *guiUser, QThread *thread, QAction *actionStart) const
{
    guiUser->moveToThread(thread);
    connect(thread, SIGNAL(started()), guiUser, SLOT(process()));
    connect(guiUser, SIGNAL(finished()), thread, SLOT(quit()));
    thread->start();
    actionStart->trigger();
}


QTEST_MAIN(MainWindowTest)

#include "tst_mainwindowtest.moc"
