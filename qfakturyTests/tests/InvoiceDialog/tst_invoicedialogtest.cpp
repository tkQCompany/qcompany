#include <QTest>
#include <QThread>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidget>
#include <QTime>
#include <QMap>
#include <QDebug>

#include "Database.h"
#include "SettingsGlobal.h"
#include "CommodityData.h"
#include "CommodityTypeData.h"
#include "PaymentTypeData.h"
#include "UnitData.h"
#include "../../TestsCommon/TestsCommon.h"
#include "../../TestsCommon/InvoiceDialogPublic.h"
#include "../../TestsCommon/GuiUserAddCommodity.h"
#include "../../TestsCommon/GuiUserAddNewCommodity.h"
#include "../../TestsCommon/GuiUserAddCounterparty.h"
#include "../../TestsCommon/Counterparty_t.h"
#include "../../TestsCommon/InvoiceSums.h"
#include "ModelCommodity.h"
#include "ModelCounterparty.h"
#include "ModelCountry.h"
#include "ModelInvoice.h"
#include "ModelInvoiceWithCommodities.h"
#include "ModelVat.h"
#include "Money_t.h"
#include "CounterpartyTypeData.h"


class InvoiceDialogTest : public QObject
{
    Q_OBJECT
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testGUI_InitialState();
    void testGUI_AddDeleteCommodities();
    void testGUI_AddDeleteCommodities_data();

private:
    void addExistingCommodityInThread(InvoiceDialogPublic *idp, const CommodityData &cd, const int netValIndex, const DecVal &discount);
    void addCounterpartyInThread(InvoiceDialogPublic *idp, const Counterparty_t &counterparty);
    void addNewCommodityInThread(InvoiceDialogPublic *idp, const CommodityData &cd);
    void checkTableCommoditiesWidget(const QTableWidget *tableWidgetCommodities, const QList<CommodityData> &lcd, const int invoiceIndex,
                                     const QList<int> &netValIndices, const QList<DecVal> &discountsPerCommodities,
                                     const int precision = 2, const int precisionQuantity = 3);
    void checkTableInvoiceCommodity(const int invoiceIndex, const QList<CommodityData> &lcd, const QList<int> &netValIndices,
                                    const QList<DecVal> &discountsPerCommodities, const int precision = 2);
    void checkLastInsertedCommodity(const CommodityData &cd);
    void startUserThread(GuiUser *guiUser, QThread *thread, QPushButton *buttonStart) const;

    CommodityData createNewCommodity(const int invoiceIndex, const int commodityIndex) const;
    Counterparty_t createNewCounterparty(const int invoiceIndex) const;

private:
    Database *db_;
};


void InvoiceDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    SettingsGlobal s;
    s.setFirstRun(true);
    qsrand(QTime::currentTime().msec());
    TestsCommon::removeDBFile();
    db_ = new Database();
}


void InvoiceDialogTest::cleanupTestCase()
{
    delete db_;
}


void InvoiceDialogTest::testGUI_InitialState()
{
    SettingsGlobal s;
    InvoiceDialogPublic invoiceDialog(0, db_, InvoiceTypeData::VAT);

    QCOMPARE(invoiceDialog.windowTitle(), trUtf8("Nowy dokument - %1 [*]").arg(InvoiceTypeData::name(InvoiceTypeData::VAT)));
    QVERIFY( ! invoiceDialog.ui()->lineEditInvNumFormat->text().isEmpty());
    QCOMPARE(invoiceDialog.ui()->dateEditDateOfIssuance->date(), QDate::currentDate());
    QCOMPARE(invoiceDialog.ui()->comboBoxInvoiceType->currentText(), InvoiceTypeData::name(InvoiceTypeData::VAT));
    QVERIFY( ! invoiceDialog.ui()->lineEditInvNumber->text().isEmpty());
    QVERIFY( ! invoiceDialog.ui()->pushButtonMoreInfo->isEnabled());

    QCOMPARE(invoiceDialog.ui()->tableWidgetCommodities->rowCount(), 0);
    const int maxCols = invoiceDialog.ui()->tableWidgetCommodities->columnCount();
    QVERIFY(invoiceDialog.ui()->tableWidgetCommodities->horizontalHeaderItem(0));
    QCOMPARE(maxCols, CommodityVisualFields::DISCOUNT - CommodityVisualFields::ID + 1);

    for(int header = 0; header < maxCols; ++header)
    {
        QCOMPARE(invoiceDialog.ui()->tableWidgetCommodities->horizontalHeaderItem(header)->text(), CommodityVisualData::header((CommodityVisualFields::Field)header));
    }

    QCOMPARE(invoiceDialog.ui()->checkBoxDiscount->isChecked(), false);
    QCOMPARE(invoiceDialog.ui()->spinBoxDiscount->value(), 0);

    QCOMPARE(invoiceDialog.ui()->dateEditDateOfSell->date(), QDate::currentDate());
    QCOMPARE(invoiceDialog.ui()->dateEditDayOfPayment->date(), QDate::currentDate());
    QCOMPARE(invoiceDialog.ui()->tableWidgetCommodities->rowCount(), 0);
    QVERIFY(invoiceDialog.ui()->comboBoxReasonOfCorrection->currentText().isEmpty());
    QCOMPARE(invoiceDialog.ui()->comboBoxPayment->currentText(), PaymentTypeData::name(PaymentTypeData::CASH));
    QCOMPARE(invoiceDialog.ui()->comboBoxCurrency->currentText(), CurrencyData::codeName((CurrencyData::Currencies)s.value(s.DEFAULT_CURRENCY).value<int>()));

    QLocale locale;
    QCOMPARE(invoiceDialog.ui()->labelSumNetVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(invoiceDialog.ui()->labelDiscountVal->text(), locale.toString(0.0, 'f', 2));
    QCOMPARE(invoiceDialog.ui()->labelSumGrossVal->text(), locale.toString(0.0, 'f', 2));

    QVERIFY(invoiceDialog.ui()->lineEditAdditionalText->text() == s.value(s.ADDIT_TEXT).toString());
}



void InvoiceDialogTest::testGUI_AddDeleteCommodities()
{
    QFETCH(QList<CommodityData>, lcd);
    QFETCH(Counterparty_t, counterparty);
    QFETCH(QString, invoiceNumber);
    QFETCH(InvoiceTypeData::Type, invoiceType);
    QFETCH(QList<int>, netValIndices);
    QFETCH(QList<DecVal>, discountsPerCommodities);
    QFETCH(DecVal, discountPerInvoice);
    QFETCH(Money_t, totalNetVal);
    QFETCH(Money_t, totalDiscountVal);
    QFETCH(Money_t, totalGrossVal);

    InvoiceDialogPublic invD(0, db_, invoiceType);
    invD.show();
    invD.ui()->spinBoxDiscount->setValue((int)discountPerInvoice.toDouble());
    const int invoiceIndex(QString(QTest::currentDataTag()).toInt());
    for(int i = 0; i < lcd.size(); ++i)
    {
        addNewCommodityInThread(&invD, lcd.at(i));
        QVERIFY(db_->modelCommodity()->submitAll());
        checkLastInsertedCommodity(lcd.at(i));

        addExistingCommodityInThread(&invD, lcd.at(i), netValIndices.at(i), discountsPerCommodities.at(i));
    }

    checkTableCommoditiesWidget(invD.ui()->tableWidgetCommodities, lcd, invoiceIndex, netValIndices, discountsPerCommodities);

    const int precision = 2;
    QCOMPARE(invD.ui()->labelSumNetVal->text(), totalNetVal.toString(precision));
    QCOMPARE(invD.ui()->labelDiscountVal->text(), totalDiscountVal.toString(precision));
    QCOMPARE(invD.ui()->labelSumGrossVal->text(), totalGrossVal.toString(precision));

    QCOMPARE(invD.ui()->tableWidgetCommodities->rowCount(), lcd.size());

    addCounterpartyInThread(&invD, counterparty);
    QVERIFY(db_->modelCounterparty()->submitAll());

    QVERIFY(invD.ui()->pushButtonMoreInfo->isEnabled());

    const int indNewCounterparty = invD.ui()->comboBoxCounterparties->findText(counterparty.name());
    QVERIFY(indNewCounterparty != -1);
    invD.ui()->comboBoxCounterparties->setCurrentIndex(indNewCounterparty);
    QCOMPARE(invD.ui()->comboBoxCounterparties->currentIndex(), indNewCounterparty);

    QTest::mouseClick(invD.ui()->pushButtonSave, Qt::LeftButton);
    QTest::mouseClick(invD.ui()->pushButtonClose, Qt::LeftButton);

    QVERIFY(db_->modelInvoice()->submitAll());

    for(int i = 0; i < lcd.size(); ++i)
    {
        QSqlQuery queryCommod(db_->modelCommodity()->query());
        queryCommod.exec(QString("SELECT quantity FROM commodity WHERE name = '%1'").arg(lcd.at(i).name()));
        if(!queryCommod.isActive())
        {
            qDebug() << "InvoiceDialogTest::testGUI_AddDeleteCommodities_data(): first SQL query failed. Reason" << queryCommod.lastError().text() << "\nQuery: " << queryCommod.lastQuery();
        }
        QVERIFY(queryCommod.isActive());
        QVERIFY(queryCommod.next());
        QCOMPARE(queryCommod.value(0).value<DecVal>().toString().toDouble(), 0.0);
    }

    QSqlQuery queryInvoice(db_->modelInvoice()->query());
    queryInvoice.exec(QString("SELECT inv_number FROM invoice WHERE counterparty_id = (SELECT id_counterparty FROM counterparty WHERE name = '%1')").arg(counterparty.name()));
    if(!queryInvoice.isActive())
    {
        qDebug() << "InvoiceDialogTest::testGUI_AddDeleteCommodities(): second SQL query failed. Reason" << queryInvoice.lastError().text() << "\nQuery: " << queryInvoice.lastQuery();
    }
    QVERIFY(queryInvoice.isActive());
    QVERIFY(queryInvoice.next());
    QCOMPARE(queryInvoice.value(0).toString(), invoiceNumber);

    queryInvoice.exec(QString("SELECT invoice_type FROM invoice JOIN invoice_type ON invoice.type_id = invoice_type.id_invoice_type "
                              "WHERE invoice.counterparty_id = (SELECT id_counterparty FROM counterparty WHERE name = '%1')")
                      .arg(counterparty.name()));
    if(!queryInvoice.isActive())
    {
        qDebug() << "InvoiceDialogTest::testGUI_AddDeleteCommodities(): third SQL query failed. Reason" << queryInvoice.lastError().text() << "\nQuery: " << queryInvoice.lastQuery();
    }
    QVERIFY(queryInvoice.isActive());
    QVERIFY(queryInvoice.next());
    QCOMPARE(queryInvoice.value(0).toString(), InvoiceTypeData::name(invoiceType));

    checkTableInvoiceCommodity(invoiceIndex, lcd, netValIndices, discountsPerCommodities);
}


void InvoiceDialogTest::testGUI_AddDeleteCommodities_data()
{
    QTest::addColumn<QList<CommodityData> >("lcd");
    QTest::addColumn<Counterparty_t>("counterparty");
    QTest::addColumn<QString>("invoiceNumber");
    QTest::addColumn<InvoiceTypeData::Type>("invoiceType");
    QTest::addColumn<QList<int> >("netValIndices");
    QTest::addColumn<QList<DecVal> >("discountsPerCommodities");
    QTest::addColumn<DecVal>("discountPerInvoice");
    QTest::addColumn<Money_t>("totalNetVal");
    QTest::addColumn<Money_t>("totalDiscountVal");
    QTest::addColumn<Money_t>("totalGrossVal");

    QList<CommodityData> lcd;
    QString invNum;
    InvoiceTypeData::Type invType;
    QList<int> netValIndices;
    Money_t totalNetVal, totalDiscountVal, totalGrossVal;
    QList<DecVal> discountsPerCommodities;
    DecVal discountPerInvoice;

    SettingsGlobal s;

    const int maxInvoices = qrand() % 4 + 2;
    for(int invoice = 0; invoice < maxInvoices; ++invoice)
    {
        const int maxCommodPerInvoice = qrand() % 2 + 2;

        lcd.clear();
        netValIndices.clear();
        discountsPerCommodities.clear();

        discountPerInvoice = DecVal(invoice);
        totalNetVal = totalDiscountVal = totalGrossVal = Money_t(0);
        for(int commod = 0; commod < maxCommodPerInvoice; ++commod)
        {
            const CommodityData cd = createNewCommodity(invoice, commod);
            lcd.append(cd);

            const int netValIndex = qrand() % 4;
            netValIndices.append(netValIndex);
            const DecVal discountPerCommodity(10 * invoice + commod);
            discountsPerCommodities.append(discountPerCommodity);

            const Money_t netVal(InvoiceSums::computeNetVal(cd, netValIndex));
            totalNetVal += netVal;
            totalDiscountVal += InvoiceSums::computeDiscountVal(cd, netVal, discountPerCommodity, discountPerInvoice);
            totalGrossVal += InvoiceSums::computeGrossVal(cd, netVal);
        }

        invType = InvoiceTypeData::VAT;

        invNum = db_->modelInvoice()->generateInvoiceNumber(
                    *(InvoiceNumberFormat_t::Parse(s.value(s.DEFAULT_INV_NUM_FORMAT).toString()).get()),
                                         invNum,
                                         QDate::currentDate(),
                                         QDate::currentDate(),
                                         invType);

        totalGrossVal -= totalDiscountVal;

        QTest::newRow(qPrintable(QString("%1").arg(invoice)))
                << lcd
                << createNewCounterparty(invoice)
                << invNum
                << invType
                << netValIndices
                << discountsPerCommodities
                << discountPerInvoice
                << totalNetVal
                << totalDiscountVal
                << totalGrossVal;
    }
}


void InvoiceDialogTest::addExistingCommodityInThread(InvoiceDialogPublic *idp, const CommodityData &cd, const int netValIndex, const DecVal &discount)
{
    GuiUserAddCommodity userAddCommod(idp, cd, netValIndex, discount);
    QThread threadCommodity;
    threadCommodity.setObjectName("threadCommodity");
    startUserThread(&userAddCommod, &threadCommodity, idp->ui()->pushButtonAddCommodity);
    while(!threadCommodity.isFinished())
    {
        QTest::qWait(200);
    }
}


void InvoiceDialogTest::addCounterpartyInThread(InvoiceDialogPublic *idp, const Counterparty_t &counterparty)
{
    GuiUserAddCounterparty userAddNewCounterp(idp, counterparty);
    QThread threadCounterparty;
    threadCounterparty.setObjectName("threadCounterparty");
    startUserThread(&userAddNewCounterp, &threadCounterparty, idp->ui()->pushButtonAddCounterparty);
    while(!threadCounterparty.isFinished())
    {
        QTest::qWait(200);
    }
}


void InvoiceDialogTest::addNewCommodityInThread(InvoiceDialogPublic *idp, const CommodityData &cd)
{
    GuiUserAddNewCommodity userAddNewCommod(idp, cd);
    QThread threadNewCommodity;
    threadNewCommodity.setObjectName("threadNewCommodity");
    startUserThread(&userAddNewCommod, &threadNewCommodity, idp->ui()->pushButtonAddNewCommodity);
    while(!threadNewCommodity.isFinished())
    {
        QTest::qWait(200);
    }
}


void InvoiceDialogTest::checkTableCommoditiesWidget(const QTableWidget *tableWidgetCommodities, const QList<CommodityData> &lcd,
                                                    const int invoiceIndex, const QList<int> &netValIndices, const QList<DecVal> &discountsPerCommodities,
                                                    const int precision, const int precisionQuantity)
{
    SettingsGlobal s;
    QTableWidgetItem *item = 0;
    for(int i = 0; i < lcd.size(); ++i)
    {
        item = tableWidgetCommodities->item(i, CommodityVisualFields::ID);
        QVERIFY(item != NULL);
        QCOMPARE(item->text().toLongLong(), lcd.at(i).id());

        item = tableWidgetCommodities->item(i, CommodityVisualFields::NAME);
        QVERIFY(item != NULL);
        QCOMPARE(item->text(), lcd.at(i).name());
        QCOMPARE(item->text(), QString("name_%1_%2").arg(invoiceIndex).arg(i));

        item = tableWidgetCommodities->item(i, CommodityVisualFields::QUANTITY);
        QVERIFY(item != NULL);
        QCOMPARE(item->text(), lcd.at(i).quantity().toString(precisionQuantity));
        QCOMPARE(item->text(), DecVal(1000.123 + 10 * invoiceIndex + i).toString(precisionQuantity));

        item = tableWidgetCommodities->item(i, CommodityVisualFields::UNIT);
        QVERIFY(item != NULL);
        QCOMPARE(item->text(), UnitData::name((UnitData::Name)lcd.at(i).unitId()));

        item = tableWidgetCommodities->item(i, CommodityVisualFields::PKWIU);
        QVERIFY(item != NULL);
        QCOMPARE(item->text(), lcd.at(i).pkwiu());
        QCOMPARE(item->text(), QString("pkwiu_%1_%2").arg(invoiceIndex).arg(i));

        item = tableWidgetCommodities->item(i, CommodityVisualFields::NET);
        QVERIFY(item != NULL);
        switch(netValIndices.at(i))
        {
        case 0:
            QCOMPARE(item->text(), lcd.at(i).net1().toString(precision));
            QCOMPARE(item->text(), QString("%1%2%3").arg(i).arg(s.decimalPoint()).arg(11));
            break;
        case 1:
            QCOMPARE(item->text(), lcd.at(i).net2().toString(precision));
            QCOMPARE(item->text(), QString("%1%2%3").arg(i).arg(s.decimalPoint()).arg(12));
            break;
        case 2:
            QCOMPARE(item->text(), lcd.at(i).net3().toString(precision));
            QCOMPARE(item->text(), QString("%1%2%3").arg(i).arg(s.decimalPoint()).arg(13));
            break;
        case 3:
            QCOMPARE(item->text(), lcd.at(i).net4().toString(precision));
            QCOMPARE(item->text(), QString("%1%2%3").arg(i).arg(s.decimalPoint()).arg(14));
            break;
        default:
            QFAIL("Unexpected case value in the switch");
            break;
        }

        item = tableWidgetCommodities->item(i, CommodityVisualFields::TYPE);
        QVERIFY(item != NULL);
        QCOMPARE(item->text(), CommodityTypeData::name((CommodityTypeData::CommodityType)lcd.at(i).typeId()));

        item = tableWidgetCommodities->item(i, CommodityVisualFields::VAT);
        QVERIFY(item != NULL);
        QCOMPARE(item->text(), DecVal::removeTrailingZeros(lcd.at(i).vat().toString()));

        item = tableWidgetCommodities->item(i, CommodityVisualFields::DISCOUNT);
        QVERIFY(item != NULL);
        QCOMPARE(item->text(), discountsPerCommodities.at(i).toString(precision));
        QCOMPARE(item->text(), DecVal(10 * invoiceIndex + i).toString(precision));
    }
}



void InvoiceDialogTest::checkTableInvoiceCommodity(const int invoiceIndex, const QList<CommodityData> &lcd,
                                                   const QList<int> &netValIndices, const QList<DecVal> &discountsPerCommodities,
                                                   const int precision)
{
    SettingsGlobal s;

    QSqlQuery queryTableInvoiceCommodity(db_->modelInvoiceWithCommodities()->query());
    queryTableInvoiceCommodity.exec(QString("SELECT commodity.id_commodity, commodity.name, table_invoice_commodity.quantity, unit.unit_name, commodity.pkwiu,table_invoice_commodity.net, commodity.vat, commodity_type.type, table_invoice_commodity.discount "
                                            "FROM commodity INNER JOIN table_invoice_commodity ON commodity.id_commodity = table_invoice_commodity.commodity_id "
                                            "INNER JOIN unit ON unit.id_unit = commodity.unit_id "
                                            "INNER JOIN commodity_type ON commodity_type.id_commodity_type = commodity.type_id "
                                            "WHERE table_invoice_commodity.invoice_id = %1 ORDER BY commodity.id_commodity").arg(invoiceIndex + 1));
    if(!queryTableInvoiceCommodity.isActive())
    {
        qDebug() << "InvoiceDialogTest::testGUI_AddDeleteCommodities(): fourth SQL query failed. Reason" << queryTableInvoiceCommodity.lastError().text() << "\nQuery: " << queryTableInvoiceCommodity.lastQuery();
    }
    QVERIFY(queryTableInvoiceCommodity.isActive());

    for(int i = 0; i < lcd.size(); ++i)
    {
        QVERIFY(queryTableInvoiceCommodity.next());

        QCOMPARE(queryTableInvoiceCommodity.value(0).toLongLong(), lcd.at(i).id());
        QCOMPARE(queryTableInvoiceCommodity.value(1).toString(), lcd.at(i).name());
        QCOMPARE(queryTableInvoiceCommodity.value(2).toDouble(), lcd.at(i).quantity().toDouble());
        QCOMPARE(queryTableInvoiceCommodity.value(3).toString(), UnitData::name((UnitData::Name) lcd.at(i).unitId()));
        QCOMPARE(queryTableInvoiceCommodity.value(4).toString(), lcd.at(i).pkwiu());

        Money_t net;
        switch(netValIndices.at(i))
        {
        case 0:
            net = lcd.at(i).net1();
            break;
        case 1:
            net = lcd.at(i).net2();
            break;
        case 2:
            net = lcd.at(i).net3();
            break;
        case 3:
            net = lcd.at(i).net4();
            break;
        default:
            QFAIL("Unexpected case value in the switch");
            break;
        }
        QCOMPARE(s.numberToString(queryTableInvoiceCommodity.value(5).toDouble(), 'f', precision), net.toString(precision));

        const QString vatStr(DecVal::removeTrailingZeros(lcd.at(i).vat().toString()));
        QCOMPARE(queryTableInvoiceCommodity.value(6).toString(), vatStr);

        QCOMPARE(queryTableInvoiceCommodity.value(7).toString(), CommodityTypeData::name((CommodityTypeData::CommodityType)lcd.at(i).typeId()));
        QCOMPARE(queryTableInvoiceCommodity.value(8).toDouble(), discountsPerCommodities.at(i).toDouble());
    }
}



void InvoiceDialogTest::checkLastInsertedCommodity(const CommodityData &cd)
{
    QSqlQuery query(db_->modelCommodity()->query());
    query.exec(QString("SELECT name, abbreviation, pkwiu, type_id, unit_id, net1, net2, net3, net4, vat, quantity FROM commodity WHERE id_commodity = (SELECT MAX(id_commodity) FROM commodity)"));
    if(!query.isActive())
    {
        const QString errMsg(QString("InvoiceDialogTest::testGUI_AddDeleteCommodities(): first SQL query failed. Reason: %1\nQuery: %2").arg(query.lastError().text()).arg(query.lastQuery()));
        QFAIL(qPrintable(errMsg));
    }

    const int precision = 2;
    const int precisionQuantity = 3;

    QVERIFY(query.next());
    QCOMPARE(query.value(0).toString(), cd.name());
    QCOMPARE(query.value(1).toString(), cd.abbrev());
    QCOMPARE(query.value(2).toString(), cd.pkwiu());
    QCOMPARE(query.value(3).toInt() - 1, cd.typeId()); //-1 because SQL starts from 1
    QCOMPARE(query.value(4).toInt() - 1, cd.unitId()); //-1 because SQL starts from 1
    QCOMPARE(query.value(5).toString(), cd.net1().toString(precision));
    QCOMPARE(query.value(6).toString(), cd.net2().toString(precision));
    QCOMPARE(query.value(7).toString(), cd.net3().toString(precision));
    QCOMPARE(query.value(8).toString(), cd.net4().toString(precision));
    QCOMPARE(query.value(9).toString(), DecVal::removeTrailingZeros(cd.vat().toString()));

    SettingsGlobal s;
    QCOMPARE(DecVal(query.value(10).toDouble()).toString(precisionQuantity), cd.quantity().toString(precisionQuantity));
}


void InvoiceDialogTest::startUserThread(GuiUser *guiUser, QThread *thread, QPushButton *buttonStart) const
{
    guiUser->moveToThread(thread);
    connect(thread, SIGNAL(started()), guiUser, SLOT(process()));
    connect(guiUser, SIGNAL(finished()), thread, SLOT(quit()));
    thread->start();
    QTest::mouseClick(buttonStart, Qt::LeftButton);
}


CommodityData InvoiceDialogTest::createNewCommodity(const int invoiceIndex, const int commodityIndex) const
{
    CommodityData cd;
    SettingsGlobal s;
    const QStringList vatRates(db_->modelVat()->listVAT());
    static long long commodityIDNum = 1LL;//SQL starts from 1

    cd.setAbbrev(QString("abbrev_%1").arg(invoiceIndex));
    cd.setId(commodityIDNum++);
    cd.setName(QString("name_%1_%2").arg(invoiceIndex).arg(commodityIndex));
    cd.setNet1(Money_t(QString("%1%2%3").arg(commodityIndex).arg(s.decimalPoint()).arg(11)));
    cd.setNet2(Money_t(QString("%1%2%3").arg(commodityIndex).arg(s.decimalPoint()).arg(12)));
    cd.setNet3(Money_t(QString("%1%2%3").arg(commodityIndex).arg(s.decimalPoint()).arg(13)));
    cd.setNet4(Money_t(QString("%1%2%3").arg(commodityIndex).arg(s.decimalPoint()).arg(14)));
    cd.setPkwiu(QString("pkwiu_%1_%2").arg(invoiceIndex).arg(commodityIndex));
    cd.setQuantity(DecVal(1000.123 + 10 * invoiceIndex + commodityIndex));

    const double vatRate(vatRates.at(qrand() % vatRates.size()).toDouble());
    cd.setVat(DecVal(vatRate));

    const CommodityTypeData::CommodityType type = CommodityTypeData::GOODS;
    cd.setTypeId(type);

    const UnitData::Name unit = UnitData::Name(qrand() % (UnitData::PACKAGE - UnitData::UNIT + 1));
    cd.setUnitId(unit);

    return cd;
}


Counterparty_t InvoiceDialogTest::createNewCounterparty(const int invoiceIndex) const
{
    const int base = 10;
    Counterparty_t counterparty;

    counterparty.setAccountNumber(QString("%1-%2-%3-%4-%5-%6-%7")
            .arg(qrand() % 100, 2, base, QChar('0'))
            .arg(qrand() % 10000, 4, base, QChar('0'))
            .arg(qrand() % 10000, 4, base, QChar('0'))
            .arg(qrand() % 10000, 4, base, QChar('0'))
            .arg(qrand() % 10000, 4, base, QChar('0'))
            .arg(qrand() % 10000, 4, base, QChar('0'))
            .arg(qrand() % 10000, 4, base, QChar('0')));
    counterparty.setCity(QString("city_%1").arg(invoiceIndex));

    const QStringList countries(db_->modelCountry()->stringList());
    counterparty.setCountry(QString("%1").arg(countries.at(qrand() % (countries.size()))));

    counterparty.setEmail(QString("%1@test.pl").arg(invoiceIndex));
    counterparty.setName(QString("name_%1").arg(invoiceIndex));
    counterparty.setPhone(QString("%1").arg(qrand(), 10, base, QChar('1')));
    counterparty.setPostalCode(QString("postalCode_%1").arg(invoiceIndex));
    counterparty.setStreet(QString("street_%1").arg(invoiceIndex));
    counterparty.setTaxIdent(QString("%1-%2-%3-%4")
            .arg(qrand()%1000, 3, base, QChar('0'))
            .arg(qrand()%1000, 3, base, QChar('0'))
            .arg(qrand()%100, 2, base, QChar('0'))
            .arg(qrand()%100, 2, base, QChar('0')));
    counterparty.setType(CounterpartyTypeData::name( (qrand() % 2)? CounterpartyTypeData::COMPANY : CounterpartyTypeData::OFFICE));
    counterparty.setWww(QString("http://www.row%1.com").arg(invoiceIndex));

    return counterparty;
}

QTEST_MAIN(InvoiceDialogTest)

#include "tst_invoicedialogtest.moc"
