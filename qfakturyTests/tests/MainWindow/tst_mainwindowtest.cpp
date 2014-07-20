#include <QTest>
#include <QThread>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>

#include "../../TestsCommon/TestsCommon.h"
#include "../../TestsCommon/GuiUserAddCommodity.h"
#include "../../TestsCommon/GuiUserAddNewCommodity.h"
#include "../../TestsCommon/GuiUserAddCounterparty.h"
#include "../../TestsCommon/GuiUserEditInvoice.h"
#include "../../TestsCommon/Counterparty_t.h"
#include "../../TestsCommon/MainWindowPublic.h"
#include "CounterpartyDialog.h"
#include "ModelCommodity.h"
#include "ModelCounterparty.h"
#include "ModelCountry.h"
#include "ModelInvoice.h"
#include "CommodityTypeData.h"
#include "PaymentTypeData.h"
#include "UnitData.h"
#include "CounterpartyTypeData.h"



class MainWindowTest : public QObject
{
    Q_OBJECT
        
private Q_SLOTS:
    void initTestCase();
    void testCaseGUI_InitialState();
    void testCaseEditInvoice();
    void testCaseEditInvoice_data();

private:
    void insertCounterparty(Database *db, const Counterparty_t &counterparty);
    void insertCommodity(Database *db, const CommodityData &cd);
    void prepareUserThread(GuiUser *guiUser, QThread *thread) const;
    void convert(const Counterparty_t &counterparty, QSqlRecord &ret) const;
    void convert(const QList<CommodityData> &commodities, const DecVal &discount, const Money_t &netVal, QList<CommodityVisualData> &ret) const;
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

    const int maxCMSections = mwp.ui()->tableViewCommodities->horizontalHeader()->count();
    const int maxCPSections = mwp.ui()->tableViewCounterparties->horizontalHeader()->count();
    const int maxISections = mwp.ui()->tableViewInvoices->horizontalHeader()->count();

    for(int i = 0; i < maxCMSections; ++i)
    {
        QCOMPARE(mwp.ui()->tableViewCommodities->horizontalHeader()->sectionResizeMode(i), QHeaderView::ResizeToContents);
    }

    for(int i = 0; i < maxCPSections; ++i)
    {
        QCOMPARE(mwp.ui()->tableViewCounterparties->horizontalHeader()->sectionResizeMode(i), QHeaderView::ResizeToContents);
    }

    for(int i = 0; i < maxISections; ++i)
    {
        QCOMPARE(mwp.ui()->tableViewInvoices->horizontalHeader()->sectionResizeMode(i), QHeaderView::ResizeToContents);
    }

    QVERIFY(mwp.ui()->tableViewCommodities->horizontalHeader()->stretchLastSection());
    QVERIFY(mwp.ui()->tableViewCounterparties->horizontalHeader()->stretchLastSection());
    QVERIFY(mwp.ui()->tableViewInvoices->horizontalHeader()->stretchLastSection());
}



void MainWindowTest::testCaseEditInvoice()
{
    QFETCH(QList<CommodityData>, lcd);
    QFETCH(Counterparty_t, counterparty);
    QFETCH(InvoiceData, invoice);
    QFETCH(Money_t, netVal);

    MainWindowPublic mwp;

    insertCounterparty(mwp.database(), counterparty);

    const int precision = 2;
    const int precisionQuantity = 3;
    foreach(const CommodityData cd, lcd)
    {
        insertCommodity(mwp.database(), cd);

        QSqlQuery query(mwp.database()->modelCommodity()->query());
        query.exec(QString("SELECT id_commodity, name, abbreviation, pkwiu, type_id, unit_id, net1, net2, net3, net4, vat, quantity FROM commodity WHERE id_commodity = (SELECT MAX(id_commodity) FROM commodity)"));
        if(!query.isActive())
        {
            const QString errMsg(QString("InvoiceDialogTest::testGUI_AddDeleteCommodities(): first SQL query failed. Reason: %1\nQuery: %2").arg(query.lastError().text()).arg(query.lastQuery()));
            QFAIL(qPrintable(errMsg));
        }
        QVERIFY(query.next());
        QCOMPARE(query.value(0).toLongLong(), cd.id());
        QCOMPARE(query.value(1).toString(), cd.name());
        QCOMPARE(query.value(2).toString(), cd.abbrev());
        QCOMPARE(query.value(3).toString(), cd.pkwiu());
        QCOMPARE(query.value(4).toLongLong(),    cd.typeId());
        QCOMPARE(query.value(5).toLongLong(),    cd.unitId());
        QCOMPARE(query.value(6).toString(), cd.net1().toString(precision));
        QCOMPARE(query.value(7).toString(), cd.net2().toString(precision));
        QCOMPARE(query.value(8).toString(), cd.net3().toString(precision));
        QCOMPARE(query.value(9).toString(), cd.net4().toString(precision));
        QCOMPARE(query.value(10).toString(), cd.vat().toString());
        QCOMPARE(query.value(11).toString(), cd.quantity().toString(precisionQuantity));
    }

    QList<CommodityVisualData> lcvd;
    convert(lcd, invoice.discount(), netVal, lcvd);
    QVERIFY(mwp.database()->invoiceWithCommoditiesInsertTransact(invoice, lcvd));

    {
        GuiUserEditInvoice userEditInvoice(&mwp);
        QThread threadInvoice;
        threadInvoice.setObjectName("threadInvoice");
        prepareUserThread(&userEditInvoice, &threadInvoice);
        mwp.ui()->tableViewInvoices->selectRow(invoice.id() - 1LL);
        mwp.editInvoice_();
        while(!threadInvoice.isFinished())
        {
            QTest::qWait(200);
        }
        QVERIFY(mwp.database()->modelInvoice()->submitAll());
        QCOMPARE(userEditInvoice.commodities().size(), lcd.size());
        QCOMPARE(userEditInvoice.invoiceData().typeID(), invoice.typeID());
    }

}


void MainWindowTest::testCaseEditInvoice_data()
{
    QTest::addColumn<QList<CommodityData> >("lcd");
    QTest::addColumn<Counterparty_t>("counterparty");
    QTest::addColumn<InvoiceData>("invoice");
    QTest::addColumn<Money_t>("netVal");

    QList<CommodityData> lcd;
    Counterparty_t counterparty;
    InvoiceData invoice;
    Money_t netVal;

    SettingsGlobal s;
    Database db;
    QString invoiceNumber;
    const QStringList vatRates(s.value(s.keyName(s.VAT_RATES)).toString().split("|"));

    const int maxInvoices = qrand() % 20 + 1; //1-20
    const int base = 10;
    int id_commodity = 1;
    for(int invNum = 0; invNum < maxInvoices; ++invNum)
    {
        const int maxCommod = qrand() % 20 + 1; //1-20 commodities per invoice
        lcd.clear();
        for(int commod = 0; commod < maxCommod; ++commod, ++id_commodity)
        {
            CommodityData cd;

            cd.setAbbrev(QString("abbrev_%1").arg(id_commodity));
            cd.setId(id_commodity);
            cd.setName(QString("name_%1").arg(id_commodity));
            cd.setNet1(Money_t(QString("%1%2%3").arg(commod).arg('.').arg(11)));
            cd.setNet2(Money_t(QString("%1%2%3").arg(commod).arg('.').arg(12)));
            cd.setNet3(Money_t(QString("%1%2%3").arg(commod).arg('.').arg(13)));
            cd.setNet4(Money_t(QString("%1%2%3").arg(commod).arg('.').arg(14)));
            cd.setPkwiu(QString("pkwiu_%1_%2").arg(invNum).arg(commod));
            cd.setQuantity(DecVal(QString("%1").arg(qrand() % 1000 + 1)));
            cd.setVat(DecVal(vatRates[qrand() % vatRates.size()]));
            const CommodityTypeData::CommodityType type = CommodityTypeData::GOODS;
            cd.setTypeId(type + 1);
            const UnitData::Name unit = (UnitData::Name)(qrand() % (UnitData::PACKAGE - UnitData::UNIT + 1));
            cd.setUnitId(unit + 1);

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
        counterparty.city = QString("city_%1").arg(invNum);
        const QStringList countries(db.modelCountry()->stringList());
        counterparty.country = QString("%1").arg(countries.at(qrand() % (countries.size())));
        counterparty.email = QString("%1@test.pl").arg(invNum);
        counterparty.name = QString("name_%1").arg(invNum);
        counterparty.phone = QString("%1").arg(qrand(), 10, base, QChar('1'));
        counterparty.postalCode = QString("postalCode_%1").arg(invNum);
        counterparty.street = QString("street_%1").arg(invNum);
        counterparty.taxID = QString("%1-%2-%3-%4")
                .arg(qrand()%1000, 3, base, QChar('0'))
                .arg(qrand()%1000, 3, base, QChar('0'))
                .arg(qrand()%100, 2, base, QChar('0'))
                .arg(qrand()%100, 2, base, QChar('0'));
        counterparty.type = CounterpartyTypeData::name( (qrand() % 2)? CounterpartyTypeData::COMPANY : CounterpartyTypeData::OFFICE);
        counterparty.www = QString("http://www.invoice%1.com").arg(invNum);
        counterparty.invNumFormat = "";

        invoiceNumber = db.modelInvoice()->generateInvoiceNumber(
                    *(InvoiceNumberFormat_t::Parse(s.value(s.DEFAULT_INV_NUM_FORMAT).toString()).get()),
                                         invoiceNumber,
                                         QDate::currentDate(),
                                         QDate::currentDate(),
                                         InvoiceTypeData::VAT);

        invoice.setId(invNum + 1);
        invoice.setAdditText(QString("addit_text_%1").arg(invNum));
        invoice.setCounterpartyID(invNum + 2);
        invoice.setCurrencyID(CurrencyData::PLN + 1);
        invoice.setDiscount(DecVal(qrand() % 100 + 1));
        invoice.setInvNumber(invoiceNumber);
        invoice.setIssuanceDate(QDate::currentDate());
        invoice.setPaymentDate(QDate::currentDate());
        invoice.setPaymentID(PaymentTypeData::CASH + 1);
        invoice.setSellingDate(QDate::currentDate());
        invoice.setTypeID(InvoiceTypeData::VAT + 1);

        netVal = lcd.at(0).net2();
        QTest::newRow(qPrintable(QString("%1").arg(invNum))) << lcd << counterparty << invoice << netVal;
    }
}



void MainWindowTest::insertCounterparty(Database *db, const Counterparty_t &counterparty)
{
    bool isOK = false;
    const int atEnd = -1;

    QSqlRecord recCounterp(db->modelCounterparty()->record());
    convert(counterparty, recCounterp);
    isOK = db->modelCounterparty()->insertRecord(atEnd, recCounterp);
    if(!isOK)
        qDebug() << "db error: " << db->modelCounterparty()->lastError().text();
    QVERIFY(isOK);

    isOK = db->modelCounterparty()->submitAll();
    if(!isOK)
        qDebug() << "db error: " << db->modelCounterparty()->lastError().text();
    QVERIFY(isOK);
}


void MainWindowTest::insertCommodity(Database *db, const CommodityData &cd)
{
    bool isOK = false;
    const int atEnd = -1;
    const int precision = 2;
    const int precisionQuantity = 3;

    QSqlRecord recCommod(db->modelCommodity()->record());
    recCommod.setValue(CommodityFields::ID_COMMODITY, cd.id());
    recCommod.setValue(CommodityFields::NAME, cd.name());
    recCommod.setValue(CommodityFields::ABBREV, cd.abbrev());
    recCommod.setValue(CommodityFields::PKWIU, cd.pkwiu());
    recCommod.setValue(CommodityFields::TYPE_ID, cd.typeId());
    recCommod.setValue(CommodityFields::UNIT_ID, cd.unitId());
    recCommod.setValue(CommodityFields::NET1, cd.net1().toString(precision));
    recCommod.setValue(CommodityFields::NET2, cd.net2().toString(precision));
    recCommod.setValue(CommodityFields::NET3, cd.net3().toString(precision));
    recCommod.setValue(CommodityFields::NET4, cd.net4().toString(precision));
    recCommod.setValue(CommodityFields::VAT, cd.vat().toString()); //TODO: check special values (e.g. "zw.")
    recCommod.setValue(CommodityFields::QUANTITY, cd.quantity().toString(precisionQuantity));

    isOK = db->modelCommodity()->insertRecord(atEnd, recCommod);
    if(!isOK)
        qDebug() << "db error: " << db->modelCommodity()->lastError().text();
    QVERIFY(isOK);

    isOK = db->modelCommodity()->submitAll();
    if(!isOK)
        qDebug() << "db error: " << db->modelCommodity()->lastError().text();
    QVERIFY(isOK);
}


void MainWindowTest::prepareUserThread(GuiUser *guiUser, QThread *thread) const
{
    guiUser->moveToThread(thread);
    connect(thread, SIGNAL(started()), guiUser, SLOT(process()));
    connect(guiUser, SIGNAL(finished()), thread, SLOT(quit()));
    thread->start();
}


void MainWindowTest::convert(const Counterparty_t &counterparty, QSqlRecord &ret) const
{
    ret.setValue(CounterpartyFields::ACCOUNT_NAME, counterparty.accountNumber);
    ret.setValue(CounterpartyFields::COUNTRY, counterparty.country);
    ret.setValue(CounterpartyFields::LOCATION, counterparty.city);
    ret.setValue(CounterpartyFields::NAME, counterparty.name);
    ret.setValue(CounterpartyFields::POSTAL_CODE, counterparty.postalCode);
    ret.setValue(CounterpartyFields::PRIMARY_EMAIL, counterparty.email);
    ret.setValue(CounterpartyFields::PRIMARY_PHONE, counterparty.phone);
    ret.setValue(CounterpartyFields::STREET, counterparty.street);
    ret.setValue(CounterpartyFields::TAX_IDENT, counterparty.taxID);
    ret.setValue(CounterpartyFields::INV_NUM_FORMAT, counterparty.invNumFormat);

    if(counterparty.type == CounterpartyTypeData::name(CounterpartyTypeData::COMPANY))
        ret.setValue(CounterpartyFields::TYPE_ID, (int)CounterpartyTypeData::COMPANY + 1);
    else
        ret.setValue(CounterpartyFields::TYPE_ID, (int)CounterpartyTypeData::OFFICE + 1);
    ret.setValue(CounterpartyFields::WWW, counterparty.www);
}


void MainWindowTest::convert(const QList<CommodityData> &commodities, const DecVal &discount, const Money_t &netVal, QList<CommodityVisualData> &ret) const
{
    for(int i = 0; i < commodities.size(); ++i)
    {
        CommodityVisualData cvd;
        cvd.setDiscount(discount);
        cvd.setID(commodities.at(i).id());
        cvd.setName(commodities.at(i).name());
        cvd.setNet(netVal);
        cvd.setPkwiu(commodities.at(i).pkwiu());
        cvd.setQuantity(commodities.at(i).quantity());

        const int type_id = commodities.at(i).typeId();
        cvd.setType(CommodityTypeData::name((CommodityTypeData::CommodityType)(type_id -1)));

        const int unit_id = commodities.at(i).unitId();
        cvd.setUnit(UnitData::name((UnitData::Name)(unit_id - 1)));

        cvd.setVat(commodities.at(i).vat());

        ret.append(cvd);
    }
}

Q_DECLARE_METATYPE(InvoiceData);

QTEST_MAIN(MainWindowTest)

#include "tst_mainwindowtest.moc"
