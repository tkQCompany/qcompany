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
#include "../../TestsCommon/InvoiceSums.h"
#include "DecVal.h"
#include "Money_t.h"
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
    void convert(const QList<CommodityData> &commodities, const QList<DecVal> &discountPerCommodities,
                 const QList<int> &netValIndices, QList<CommodityVisualData> &ret) const;
    void checkCommodities(Database *db, const QList<CommodityData> &lcd);
    void checkCounterparty(Database *db, const Counterparty_t &counterparty);
    void checkInvoice(Database *db, const InvoiceData &invData);
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
    QFETCH(QList<int>, netValIndices);
    QFETCH(QList<DecVal>, discountsPerCommodities);
    QFETCH(Money_t, totalNetVal);
    QFETCH(Money_t, totalDiscountVal);
    QFETCH(Money_t, totalGrossVal);

    MainWindowPublic mwp;

    insertCounterparty(mwp.database(), counterparty);
    checkCounterparty(mwp.database(), counterparty);

    foreach(const CommodityData cd, lcd)
    {
        insertCommodity(mwp.database(), cd);
    }
    checkCommodities(mwp.database(), lcd);

    QList<CommodityVisualData> lcvd;
    convert(lcd, discountsPerCommodities, netValIndices, lcvd);
    QVERIFY(mwp.database()->invoiceWithCommoditiesInsertTransact(invoice, lcvd));

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

    const int precision = 2;
    QCOMPARE(userEditInvoice.commodities().size(), lcd.size());
    QCOMPARE(userEditInvoice.totalNetVal().toString(precision), totalNetVal.toString(precision));
    QCOMPARE(userEditInvoice.totalDiscountVal().toString(precision), totalDiscountVal.toString(precision));
    QCOMPARE(userEditInvoice.totalGrossVal().toString(precision), totalGrossVal.toString(precision));

    InvoiceData invoiceWithID(userEditInvoice.invoiceData());
    invoiceWithID.setId(invoice.id()); //supplying missing field
    checkInvoice(mwp.database(), invoiceWithID);

    checkInvoice(mwp.database(), invoice);
}


void MainWindowTest::testCaseEditInvoice_data()
{
    QTest::addColumn<QList<CommodityData> >("lcd");
    QTest::addColumn<Counterparty_t>("counterparty");
    QTest::addColumn<InvoiceData>("invoice");
    QTest::addColumn<QList<int> >("netValIndices");
    QTest::addColumn<QList<DecVal> >("discountsPerCommodities");
    QTest::addColumn<Money_t>("totalNetVal");
    QTest::addColumn<Money_t>("totalDiscountVal");
    QTest::addColumn<Money_t>("totalGrossVal");

    QList<CommodityData> lcd;
    Counterparty_t counterparty;
    InvoiceData invoice;
    QList<int> netValIndices;
    Money_t totalNetVal, totalDiscountVal, totalGrossVal;
    QList<DecVal> discountsPerCommodities;

    SettingsGlobal s;
    Database db;
    QString invoiceNumber;
    const QStringList vatRates(s.value(s.keyName(s.VAT_RATES)).toString().split("|"));

    const int maxInvoices = qrand() % 20 + 3; //3-20
    const int base = 10;
    const DecVal onePercent(DecVal(1) / DecVal(100));
    int id_commodity = 1;
    for(int invNum = 0; invNum < maxInvoices; ++invNum)
    {
        const int maxCommod = qrand() % 20 + 1; //1-20 commodities per invoice

        lcd.clear();
        netValIndices.clear();
        discountsPerCommodities.clear();

        invoice.setDiscount(DecVal(invNum));
        totalNetVal = totalDiscountVal = totalGrossVal = Money_t(0);
        for(int commod = 0; commod < maxCommod; ++commod, ++id_commodity)
        {
            CommodityData cd;

            cd.setAbbrev(QString("abbrev_%1").arg(id_commodity));
            cd.setId(id_commodity);
            cd.setName(QString("name_%1").arg(id_commodity));
            cd.setNet1(Money_t(QString("%1%2%3").arg(id_commodity % 4).arg(s.decimalPointStr().at(0)).arg(11)));
            cd.setNet2(Money_t(QString("%1%2%3").arg(id_commodity % 4).arg(s.decimalPointStr().at(0)).arg(12)));
            cd.setNet3(Money_t(QString("%1%2%3").arg(id_commodity % 4).arg(s.decimalPointStr().at(0)).arg(13)));
            cd.setNet4(Money_t(QString("%1%2%3").arg(id_commodity % 4).arg(s.decimalPointStr().at(0)).arg(14)));
            cd.setPkwiu(QString("pkwiu_%1_%2").arg(invNum).arg(commod));
            cd.setQuantity(DecVal(QString("%1%2%3").arg(id_commodity).arg(s.decimalPointStr().at(0)).arg(356)));
            cd.setVat(DecVal(vatRates.at(commod % vatRates.size())));
            const CommodityTypeData::CommodityType type = CommodityTypeData::GOODS;
            cd.setTypeId(type + 1);
            const UnitData::Name unit = (UnitData::Name)(qrand() % (UnitData::PACKAGE - UnitData::UNIT + 1));
            cd.setUnitId(unit + 1);

            lcd.append(cd);

            const int netValIndex = id_commodity % 4;
            netValIndices.append(netValIndex);

            const DecVal discountPerCommodity(invNum % 100);
            discountsPerCommodities.append(discountPerCommodity);

            const Money_t netVal(QString("%1%2%3").arg(id_commodity % 4).arg(s.decimalPointStr().at(0)).arg(11 + id_commodity % 4));
            const DecVal quantity(DecVal(QString("%1%2%3").arg(id_commodity).arg(s.decimalPointStr().at(0)).arg(356)));
            const Money_t grossVal(netVal * (DecVal(1) + cd.vat() * onePercent));
            const Money_t discountVal(grossVal * discountPerCommodity * onePercent);
            totalNetVal += netVal * quantity;
            totalGrossVal += grossVal * quantity;
            totalDiscountVal += discountVal * quantity;
            totalGrossVal -= discountVal * quantity;
        }
        totalDiscountVal += totalGrossVal * invoice.discount() * onePercent;
        totalGrossVal -= totalGrossVal * invoice.discount() * onePercent;

        counterparty.setAccountNumber(QString("%1-%2-%3-%4-%5-%6-%7")
                .arg(qrand() % 100, 2, base, QChar('0'))
                .arg(qrand() % 10000, 4, base, QChar('0'))
                .arg(qrand() % 10000, 4, base, QChar('0'))
                .arg(qrand() % 10000, 4, base, QChar('0'))
                .arg(qrand() % 10000, 4, base, QChar('0'))
                .arg(qrand() % 10000, 4, base, QChar('0'))
                .arg(qrand() % 10000, 4, base, QChar('0')));
        counterparty.setCity(QString("city_%1").arg(invNum));
        const QStringList countries(db.modelCountry()->stringList());
        counterparty.setCountry(QString("%1").arg(countries.at(qrand() % (countries.size()))));
        counterparty.setEmail(QString("%1@test.pl").arg(invNum));
        counterparty.setId(invNum + 2LL); //sql starts from 1 + skip my own company (which is on the list of all counterparties)
        counterparty.setName(QString("name_%1").arg(invNum));
        counterparty.setPhone(QString("%1").arg(qrand(), 10, base, QChar('1')));
        counterparty.setPostalCode(QString("postalCode_%1").arg(invNum));
        counterparty.setStreet(QString("street_%1").arg(invNum));
        counterparty.setTaxIdent(QString("%1-%2-%3-%4")
                .arg(qrand()%1000, 3, base, QChar('0'))
                .arg(qrand()%1000, 3, base, QChar('0'))
                .arg(qrand()%100, 2, base, QChar('0'))
                .arg(qrand()%100, 2, base, QChar('0')));
        counterparty.setType(CounterpartyTypeData::name( (qrand() % 2)? CounterpartyTypeData::COMPANY : CounterpartyTypeData::OFFICE));
        counterparty.setWww(QString("http://www.invoice%1.com").arg(invNum));
        counterparty.setInvNumFormat("");

        invoiceNumber = db.modelInvoice()->generateInvoiceNumber(
                    *(InvoiceNumberFormat_t::Parse(s.value(s.DEFAULT_INV_NUM_FORMAT).toString()).get()),
                                         invoiceNumber,
                                         QDate::currentDate(),
                                         QDate::currentDate(),
                                         InvoiceTypeData::VAT);

        invoice.setId(invNum + 1);
        invoice.setAdditText(QString("addit_text_%1").arg(invNum));
        invoice.setCounterpartyID(invNum + 2);
        invoice.setCurrency(CurrencyData::PLN);
        invoice.setInvNumber(invoiceNumber);
        invoice.setIssuanceDate(QDate::currentDate());
        invoice.setPaymentDate(QDate::currentDate());
        invoice.setPaymentType(PaymentTypeData::CASH);
        invoice.setSellingDate(QDate::currentDate());
        invoice.setType(InvoiceTypeData::VAT);

        QTest::newRow(qPrintable(QString("%1").arg(invNum))) << lcd << counterparty << invoice << netValIndices
                                                             << discountsPerCommodities
                                                             << totalNetVal << totalDiscountVal << totalGrossVal;
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
    recCommod.setValue(CommodityFields::ID_COMMODITY, cd.id());
    recCommod.setValue(CommodityFields::NAME, cd.name());
    recCommod.setValue(CommodityFields::ABBREV, cd.abbrev());
    recCommod.setValue(CommodityFields::PKWIU, cd.pkwiu());
    recCommod.setValue(CommodityFields::TYPE_ID, cd.typeId());
    recCommod.setValue(CommodityFields::UNIT_ID, cd.unitId());
    recCommod.setValue(CommodityFields::NET1, cd.net1().toString());
    recCommod.setValue(CommodityFields::NET2, cd.net2().toString());
    recCommod.setValue(CommodityFields::NET3, cd.net3().toString());
    recCommod.setValue(CommodityFields::NET4, cd.net4().toString());
    recCommod.setValue(CommodityFields::VAT, cd.vat().toString()); //TODO: check special values (e.g. "zw.")
    recCommod.setValue(CommodityFields::QUANTITY, cd.quantity().toString());

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
    ret.setValue(CounterpartyFields::ACCOUNT_NAME, counterparty.accountNumber());
    ret.setValue(CounterpartyFields::COUNTRY, counterparty.country());
    ret.setValue(CounterpartyFields::LOCATION, counterparty.city());
    ret.setValue(CounterpartyFields::NAME, counterparty.name());
    ret.setValue(CounterpartyFields::POSTAL_CODE, counterparty.postalCode());
    ret.setValue(CounterpartyFields::PRIMARY_EMAIL, counterparty.email());
    ret.setValue(CounterpartyFields::PRIMARY_PHONE, counterparty.phone());
    ret.setValue(CounterpartyFields::STREET, counterparty.street());
    ret.setValue(CounterpartyFields::TAX_IDENT, counterparty.taxIdent());
    ret.setValue(CounterpartyFields::INV_NUM_FORMAT, counterparty.invNumFormat());

    if(counterparty.type() == CounterpartyTypeData::name(CounterpartyTypeData::COMPANY))
        ret.setValue(CounterpartyFields::TYPE_ID, (int)CounterpartyTypeData::COMPANY + 1);
    else
        ret.setValue(CounterpartyFields::TYPE_ID, (int)CounterpartyTypeData::OFFICE + 1);
    ret.setValue(CounterpartyFields::WWW, counterparty.www());
}


void MainWindowTest::convert(const QList<CommodityData> &commodities, const QList<DecVal> &discountPerCommodities,
                             const QList<int> &netValIndices, QList<CommodityVisualData> &ret) const
{
    for(int i = 0; i < commodities.size(); ++i)
    {
        CommodityVisualData cvd;
        cvd.setDiscount(discountPerCommodities.at(i));
        cvd.setID(commodities.at(i).id());
        cvd.setName(commodities.at(i).name());

        switch(netValIndices.at(i))
        {
        case 0:
            cvd.setNet(commodities.at(i).net1());
            break;
        case 1:
            cvd.setNet(commodities.at(i).net2());
            break;
        case 2:
            cvd.setNet(commodities.at(i).net3());
            break;
        case 3:
            cvd.setNet(commodities.at(i).net4());
            break;
        }

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


void MainWindowTest::checkCommodities(Database *db, const QList<CommodityData> &lcd)
{
    foreach(const CommodityData cd, lcd)
    {
        QSqlQuery query(db->modelCommodity()->query());
        query.exec(QString("SELECT id_commodity, name, abbreviation, pkwiu, type_id, unit_id, "
                           "net1, net2, net3, net4, vat, quantity FROM commodity WHERE id_commodity = %1").arg(cd.id()));
        if(!query.isActive())
        {
            const QString errMsg(QString("InvoiceDialogTest::checkCommodities(): SQL query failed. Reason: %1\nQuery: %2").arg(query.lastError().text()).arg(query.lastQuery()));
            QFAIL(qPrintable(errMsg));
        }
        QVERIFY(query.next());
        QCOMPARE(query.value(CommodityFields::ID_COMMODITY).toLongLong(), cd.id());
        QCOMPARE(query.value(CommodityFields::NAME).toString(), cd.name());
        QCOMPARE(query.value(CommodityFields::ABBREV).toString(), cd.abbrev());
        QCOMPARE(query.value(CommodityFields::PKWIU).toString(), cd.pkwiu());
        QCOMPARE(query.value(CommodityFields::TYPE_ID).toLongLong(),    cd.typeId());
        QCOMPARE(query.value(CommodityFields::UNIT_ID).toLongLong(),    cd.unitId());
        QCOMPARE(query.value(CommodityFields::NET1).toString(), cd.net1().toString());
        QCOMPARE(query.value(CommodityFields::NET2).toString(), cd.net2().toString());
        QCOMPARE(query.value(CommodityFields::NET3).toString(), cd.net3().toString());
        QCOMPARE(query.value(CommodityFields::NET4).toString(), cd.net4().toString());
        QCOMPARE(query.value(CommodityFields::VAT).toString(), cd.vat().toString());
        QCOMPARE(query.value(CommodityFields::QUANTITY).toString(), cd.quantity().toString());
    }
}



void MainWindowTest::checkCounterparty(Database *db, const Counterparty_t &counterparty)
{
    QSqlQuery query(db->modelCounterparty()->query());
    query.exec(QString("SELECT id_counterparty, name, type_id, country, location, postal_code, street, tax_ident, "
                       "account_name, www, primary_email, primary_phone, inv_number_format FROM counterparty "
                       "WHERE id_counterparty = (SELECT MAX(id_counterparty) FROM counterparty)"));
    if(!query.isActive())
    {
        const QString errMsg(QString("InvoiceDialogTest::checkCounterparty(): SQL query failed. Reason: %1\nQuery: %2")
                             .arg(query.lastError().text()).arg(query.lastQuery()));
        QFAIL(qPrintable(errMsg));
    }

    QVERIFY(query.next());
    QCOMPARE(query.value(CounterpartyFields::ID).toLongLong(), counterparty.id());
    QCOMPARE(query.value(CounterpartyFields::NAME).toString(), counterparty.name());
    QCOMPARE(CounterpartyTypeData::name((CounterpartyTypeData::CounterpartyType)(query.value(CounterpartyFields::TYPE_ID).toLongLong() - 1LL)),
             counterparty.type());
    QCOMPARE(query.value(CounterpartyFields::COUNTRY).toString(), counterparty.country());
    QCOMPARE(query.value(CounterpartyFields::LOCATION).toString(), counterparty.city());
    QCOMPARE(query.value(CounterpartyFields::POSTAL_CODE).toString(), counterparty.postalCode());
    QCOMPARE(query.value(CounterpartyFields::STREET).toString(), counterparty.street());
    QCOMPARE(query.value(CounterpartyFields::TAX_IDENT).toString(), counterparty.taxIdent());
    QCOMPARE(query.value(CounterpartyFields::ACCOUNT_NAME).toString(), counterparty.accountNumber());
    QCOMPARE(query.value(CounterpartyFields::WWW).toString(), counterparty.www());
    QCOMPARE(query.value(CounterpartyFields::PRIMARY_EMAIL).toString(), counterparty.email());
    QCOMPARE(query.value(CounterpartyFields::PRIMARY_PHONE).toString(), counterparty.phone());
    QCOMPARE(query.value(CounterpartyFields::INV_NUM_FORMAT).toString(), counterparty.invNumFormat());
}



void MainWindowTest::checkInvoice(Database *db, const InvoiceData &invData)
{
    QSqlQuery query(db->modelInvoice()->query());
    query.exec(QString("SELECT id_invoice, inv_number, selling_date, invoice_type.invoice_type, counterparty_id, "
                       "issuance_date, payment_date, payment_type.type AS payment_type, currency.code AS currency, additional_text, discount "
                       "FROM invoice "
                            "JOIN invoice_type ON invoice.type_id = invoice_type.id_invoice_type "
                            "JOIN  payment_type ON invoice.payment_id = payment_type.id_payment_type "
                            "JOIN currency ON invoice.currency_id = currency.id_currency "
                       "WHERE invoice.id_invoice = %1").arg(invData.id()));
    if(!query.isActive())
    {
        const QString errMsg(QString("InvoiceDialogTest::checkInvoice(): SQL query failed. Reason: %1\nQuery: %2")
                             .arg(query.lastError().text()).arg(query.lastQuery()));
        QFAIL(qPrintable(errMsg));
    }

    QVERIFY(query.next());
    QCOMPARE(query.value(InvoiceFields::ID_INVOICE).toLongLong(), invData.id());
    QCOMPARE(query.value(InvoiceFields::INV_NUMBER).toString(), invData.invNumber());
    QCOMPARE(query.value(InvoiceFields::SELLING_DATE).toDate(), invData.sellingDate());
    QCOMPARE(query.value(InvoiceFields::TYPE_ID).toString(), InvoiceTypeData::name(invData.type()));
    QCOMPARE(query.value(InvoiceFields::COUNTERPARTY_ID).toLongLong(), invData.counterpartyID());
    QCOMPARE(query.value(InvoiceFields::ISSUANCE_DATE).toDate(), invData.issuanceDate());
    QCOMPARE(query.value(InvoiceFields::PAYMENT_DATE).toDate(), invData.paymentDate());
    QCOMPARE(query.value(InvoiceFields::PAYMENT_ID).toString(), PaymentTypeData::name(invData.paymentType()));
    QCOMPARE(query.value(InvoiceFields::CURRENCY_ID).toString(), CurrencyData::codeName(invData.currency()));
    QCOMPARE(query.value(InvoiceFields::ADDIT_TEXT).toString(), invData.additText());
    QCOMPARE(query.value(InvoiceFields::DISCOUNT).toDouble(), invData.discount().toDouble());
}


Q_DECLARE_METATYPE(InvoiceData);

QTEST_MAIN(MainWindowTest)

#include "tst_mainwindowtest.moc"
