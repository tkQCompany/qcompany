#include <QTest>
#include <QThread>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>

#include "../TestsCommon/TestsCommon.h"
#include "../TestsCommon/GuiUserAddCommodity.h"
#include "../TestsCommon/GuiUserAddNewCommodity.h"
#include "../TestsCommon/GuiUserAddCounterparty.h"
#include "../TestsCommon/GuiUserEditInvoice.h"
#include "../TestsCommon/Counterparty_t.h"
#include "../TestsCommon/MainWindowPublic.h"
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
    void convert(const QList<CommodityData> &commodities, const QString &discount, const QString &netVal, QList<CommodityVisualData> &ret) const;
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



void MainWindowTest::testCaseEditInvoice()
{
    QFETCH(QList<CommodityData>, lcd);
    QFETCH(Counterparty_t, counterparty);
    QFETCH(InvoiceData, invoice);
    QFETCH(QString, netVal);

    MainWindowPublic mwp;

    insertCounterparty(mwp.database(), counterparty);

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
        QCOMPARE(query.value(0).toString(), cd.field(CommodityFields::ID_COMMODITY).toString());
        QCOMPARE(query.value(1).toString(), cd.field(CommodityFields::NAME).toString());
        QCOMPARE(query.value(2).toString(), cd.field(CommodityFields::ABBREV).toString());
        QCOMPARE(query.value(3).toString(), cd.field(CommodityFields::PKWIU).toString());
        QCOMPARE(query.value(4).toInt(),    cd.field(CommodityFields::TYPE_ID).toInt());
        QCOMPARE(query.value(5).toInt(),    cd.field(CommodityFields::UNIT_ID).toInt());
        QCOMPARE(query.value(6).toString(), cd.field(CommodityFields::NET1).toString());
        QCOMPARE(query.value(7).toString(), cd.field(CommodityFields::NET2).toString());
        QCOMPARE(query.value(8).toString(), cd.field(CommodityFields::NET3).toString());
        QCOMPARE(query.value(9).toString(), cd.field(CommodityFields::NET4).toString());
        QCOMPARE(query.value(10).toString(), cd.field(CommodityFields::VAT).toString());
        QCOMPARE(query.value(11).toString(), cd.field(CommodityFields::QUANTITY).toString());
    }

    QList<CommodityVisualData> lcvd;
    convert(lcd, invoice.field(InvoiceFields::DISCOUNT).toString(), netVal, lcvd);
    QVERIFY(mwp.database()->invoiceWithCommoditiesInsertTransact(invoice, lcvd));

    {
        GuiUserEditInvoice userEditInvoice(&mwp);
        QThread threadInvoice;
        threadInvoice.setObjectName("threadInvoice");
        prepareUserThread(&userEditInvoice, &threadInvoice);
        mwp.ui()->tableViewInvoices->selectRow(invoice.field(InvoiceFields::ID_INVOICE).toInt() - 1);
        mwp.editInvoice_();
        while(!threadInvoice.isFinished())
        {
            QTest::qWait(200);
        }
        QVERIFY(mwp.database()->modelInvoice()->submitAll());
        QCOMPARE(userEditInvoice.commodities().size(), lcd.size());
        QCOMPARE(userEditInvoice.invoiceData().field(InvoiceFields::TYPE_ID).toInt(), invoice.field(InvoiceFields::TYPE_ID).toInt());
    }

}


void MainWindowTest::testCaseEditInvoice_data()
{
    QTest::addColumn<QList<CommodityData> >("lcd");
    QTest::addColumn<Counterparty_t>("counterparty");
    QTest::addColumn<InvoiceData>("invoice");
    QTest::addColumn<QString>("netVal");

    QList<CommodityData> lcd;
    Counterparty_t counterparty;
    InvoiceData invoice;
    QString netVal;

    SettingsGlobal s;
    Database db;
    QVariant v;
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

            cd.setField(CommodityFields::ABBREV, QString("abbrev_%1").arg(id_commodity));
            cd.setField(CommodityFields::ID_COMMODITY, id_commodity);
            cd.setField(CommodityFields::NAME, QString("name_%1").arg(id_commodity));

            v.setValue(Money_t(QString("%1%2%3").arg(commod).arg('.').arg(11)));
            cd.setField(CommodityFields::NET1, v);

            v.setValue(Money_t(QString("%1%2%3").arg(commod).arg('.').arg(12)));
            cd.setField(CommodityFields::NET2, v);

            v.setValue(Money_t(QString("%1%2%3").arg(commod).arg('.').arg(13)));
            cd.setField(CommodityFields::NET3, v);

            v.setValue(Money_t(QString("%1%2%3").arg(commod).arg('.').arg(14)));
            cd.setField(CommodityFields::NET4, v);

            cd.setField(CommodityFields::PKWIU, QString("pkwiu_%1_%2").arg(invNum).arg(commod));
            cd.setField(CommodityFields::QUANTITY, QString("%1").arg(qrand() % 1000 + 1));
            cd.setField(CommodityFields::VAT, vatRates[qrand() % vatRates.size()]);
            const CommodityTypeData::CommodityType type = CommodityTypeData::GOODS;
            cd.setField(CommodityFields::TYPE_ID, type + 1);
            const UnitData::Name unit = (UnitData::Name)(qrand() % (UnitData::PACKAGE - UnitData::UNIT + 1));
            cd.setField(CommodityFields::UNIT_ID, unit + 1);

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

        invoice.setField(InvoiceFields::ID_INVOICE, invNum + 1);
        invoice.setField(InvoiceFields::ADDIT_TEXT, QString("addit_text_%1").arg(invNum));
        invoice.setField(InvoiceFields::COUNTERPARTY_ID, invNum + 2);
        invoice.setField(InvoiceFields::CURRENCY_ID, CurrencyData::PLN + 1);
        invoice.setField(InvoiceFields::DISCOUNT, qrand() % 100 + 1);
        invoice.setField(InvoiceFields::INV_NUMBER, invoiceNumber);
        invoice.setField(InvoiceFields::ISSUANCE_DATE, QDate::currentDate());
        invoice.setField(InvoiceFields::PAYMENT_DATE, QDate::currentDate());
        invoice.setField(InvoiceFields::PAYMENT_ID, PaymentTypeData::CASH + 1);
        invoice.setField(InvoiceFields::SELLING_DATE, QDate::currentDate());
        invoice.setField(InvoiceFields::TYPE_ID, InvoiceTypeData::VAT + 1);

        netVal = lcd.at(0).field(CommodityFields::NET2).toString();
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

    QSqlRecord recCommod(db->modelCommodity()->record());
    const int maxIndex = CommodityFields::QUANTITY - CommodityFields::ID_COMMODITY + 1;
    for(int i = 0; i < maxIndex; ++i)
    {
        recCommod.setValue(i, cd.field((CommodityFields::Field)i));
    }


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


void MainWindowTest::convert(const QList<CommodityData> &commodities, const QString &discount, const QString &netVal, QList<CommodityVisualData> &ret) const
{
    for(int i = 0; i < commodities.size(); ++i)
    {
        CommodityVisualData cvd;
        cvd.setField(CommodityVisualFields::DISCOUNT, discount);
        cvd.setField(CommodityVisualFields::ID, commodities.at(i).field(CommodityFields::ID_COMMODITY));
        cvd.setField(CommodityVisualFields::NAME, commodities.at(i).field(CommodityFields::NAME));
        cvd.setField(CommodityVisualFields::NET, netVal);
        cvd.setField(CommodityVisualFields::PKWIU, commodities.at(i).field(CommodityFields::PKWIU));
        cvd.setField(CommodityVisualFields::QUANTITY, commodities.at(i).field(CommodityFields::QUANTITY));

        const int type_id = commodities.at(i).field(CommodityFields::TYPE_ID).toInt();
        cvd.setField(CommodityVisualFields::TYPE, CommodityTypeData::name((CommodityTypeData::CommodityType)(type_id -1)));

        const int unit_id = commodities.at(i).field(CommodityFields::UNIT_ID).toInt();
        cvd.setField(CommodityVisualFields::UNIT, UnitData::name((UnitData::Name)(unit_id - 1)));

        cvd.setField(CommodityVisualFields::VAT, commodities.at(i).field(CommodityFields::VAT));

        ret.append(cvd);
    }
}

Q_DECLARE_METATYPE(InvoiceData);

QTEST_MAIN(MainWindowTest)

#include "tst_mainwindowtest.moc"
