#include <QTest>
#include <QThread>
#include <QSqlQuery>
#include <QSqlError>

#include "../TestsCommon.h"
#include "Database.h"
#include "SettingsGlobal.h"
#include "CommodityData.h"
#include "CommodityTypeData.h"
#include "PaymentTypeData.h"
#include "InvoiceDialogPublic.h"
#include "UnitData.h"
#include "GuiUserAddCommodity.h"
#include "GuiUserAddNewCommodity.h"
#include "GuiUserAddCounterparty.h"
#include "ModelCommodity.h"
#include "ModelCountry.h"
#include "ModelInvoice.h"
#include "CounterpartyTypeData.h"
#include "Counterparty_t.h"


class InvoiceDialogTest : public QObject
{
    Q_OBJECT
    
private Q_SLOTS:
    void initTestCase();
    void testGUI_InitialState();
    void testGUI_AddDeleteCommodities();
    void testGUI_AddDeleteCommodities_data();

private:
    void startUserThread(GuiUser *guiUser, QThread *thread, QPushButton *buttonStart) const;
};


void InvoiceDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
    SettingsGlobal s;
    s.setFirstRun(true);
    qsrand(0);
}



void InvoiceDialogTest::testGUI_InitialState()
{
    SettingsGlobal s;
    Database db;
    InvoiceDialogPublic invoiceDialog(0, &db, InvoiceTypeData::VAT);

    QCOMPARE(invoiceDialog.windowTitle(), trUtf8("Nowy dokument - %1 [*]").arg(InvoiceTypeData::name(InvoiceTypeData::VAT)));
    QVERIFY( ! invoiceDialog.ui()->lineEditInvNumFormat->text().isEmpty());
    QCOMPARE(invoiceDialog.ui()->dateEditDateOfIssuance->date(), QDate::currentDate());
    QCOMPARE(invoiceDialog.ui()->comboBoxInvoiceType->currentText(), InvoiceTypeData::name(InvoiceTypeData::VAT));
    QVERIFY( ! invoiceDialog.ui()->lineEditInvNumber->text().isEmpty());

    QCOMPARE(invoiceDialog.ui()->tableWidgetCommodities->rowCount(), 0);
    const int maxCols = invoiceDialog.ui()->tableWidgetCommodities->columnCount();
    QVERIFY(invoiceDialog.ui()->tableWidgetCommodities->horizontalHeaderItem(0));
    QCOMPARE(maxCols, CommodityVisualFields::DISCOUNT - CommodityVisualFields::ID + 1);

    for(int header = 0; header < maxCols; ++header)
    {
        QCOMPARE(invoiceDialog.ui()->tableWidgetCommodities->horizontalHeaderItem(header)->text(), CommodityVisualData::header(header));
    }

    QCOMPARE(invoiceDialog.ui()->checkBoxDiscount->isChecked(), false);
    QCOMPARE(invoiceDialog.ui()->spinBoxDiscount->value(), 0);

    QCOMPARE(invoiceDialog.ui()->dateEditDateOfSell->date(), QDate::currentDate());
    QCOMPARE(invoiceDialog.ui()->dateEditDayOfPayment->date(), QDate::currentDate());
    QCOMPARE(invoiceDialog.ui()->tableWidgetCommodities->rowCount(), 0);
    QVERIFY(invoiceDialog.ui()->comboBoxReasonOfCorrection->currentText().isEmpty());
    QCOMPARE(invoiceDialog.ui()->comboBoxPayment->currentText(), PaymentTypeData::name(PaymentTypeData::CASH));
    QCOMPARE(invoiceDialog.ui()->comboBoxCurrency->currentText(), s.value(s.DEFAULT_CURRENCY).toString());

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

    Database db;
    InvoiceDialogPublic invD(0, &db, invoiceType);
    QThread threadNewCommodity, threadCommodity, threadCounterparty;
    qRegisterMetaType<QItemSelection>("QItemSelection");
    foreach(const CommodityData cd, lcd)
    {
        GuiUserAddNewCommodity userAddNewCommod(&invD, &cd);
        startUserThread(&userAddNewCommod, &threadNewCommodity, invD.ui()->pushButtonAddNewCommodity);
        while(threadNewCommodity.isRunning())
        {
            QTest::qSleep(100);
        }
        QVERIFY(db.modelCommodity()->submitAll());

        QSqlQuery query(db.modelCommodity()->query());
        query.exec(QString("SELECT name, abbreviation, pkwiu, type_id, unit_id, net1, net2, net3, net4, vat, quantity FROM commodity WHERE id_commodity = (SELECT MAX(id_commodity) FROM commodity)"));
        if(!query.isActive())
        {
            qDebug() << "InvoiceDialogTest::testGUI_AddDeleteCommodities(): first SQL query failed. Reason" << query.lastError().text() << "\nQuery: " << query.lastQuery();
        }
        QVERIFY(query.isActive());
        QVERIFY(query.next());
        QCOMPARE(query.value(0).toString(), cd.field(CommodityFields::NAME).toString());
        QCOMPARE(query.value(1).toString(), cd.field(CommodityFields::ABBREV).toString());
        QCOMPARE(query.value(2).toString(), cd.field(CommodityFields::PKWIU).toString());
        QCOMPARE(query.value(3).toInt() - 1, cd.field(CommodityFields::TYPE_ID).toInt());
        QCOMPARE(query.value(4).toInt() - 1, cd.field(CommodityFields::UNIT_ID).toInt());
        QCOMPARE(query.value(5).toString(), cd.field(CommodityFields::NET1).toString());
        QCOMPARE(query.value(6).toString(), cd.field(CommodityFields::NET2).toString());
        QCOMPARE(query.value(7).toString(), cd.field(CommodityFields::NET3).toString());
        QCOMPARE(query.value(8).toString(), cd.field(CommodityFields::NET4).toString());
        QCOMPARE(query.value(9).toString(), cd.field(CommodityFields::VAT).toString());
        QCOMPARE(query.value(10).toString(), cd.field(CommodityFields::QUANTITY).toString());

        if(QTest::currentTestFailed())
        {
            break;
        }

        GuiUserAddCommodity userAddCommod(&invD, &cd);
        startUserThread(&userAddCommod, &threadCommodity, invD.ui()->pushButtonAddCommodity);
        while(threadCommodity.isRunning())
        {
            QTest::qSleep(100);
        }
    }

    QCOMPARE(invD.ui()->tableWidgetCommodities->rowCount(), lcd.size());

    GuiUserAddCounterparty userAddNewCounterp(&invD, &counterparty);
    startUserThread(&userAddNewCounterp, &threadCounterparty, invD.ui()->pushButtonAddCounterparty);
    while(threadCounterparty.isRunning())
    {
        QTest::qSleep(100);
    }

    QVERIFY(invD.ui()->comboBoxCounterparties->findText(counterparty.name) != -1);

    QTest::mouseClick(invD.ui()->pushButtonSave, Qt::LeftButton);
    QTest::mouseClick(invD.ui()->pushButtonClose, Qt::LeftButton);

//    QSqlQuery query(db.modelInvoice()->query());
//    query.exec(QString("SELECT inv_number FROM invoice WHERE counterparty_id = (SELECT id_counterparty FROM counterparty WHERE name = %1)").arg(counterparty.name));
//    if(!query.isActive())
//    {
//        qDebug() << "InvoiceDialogTest::testGUI_AddDeleteCommodities(): first SQL query failed. Reason" << query.lastError().text() << "\nQuery: " << query.lastQuery();
//    }
//    QVERIFY(query.isActive());
//    QVERIFY(query.next());
//    QVERIFY(query.value(0).toString() == invoiceNumber);

//    query.exec(QString("SELECT invoice_type FROM invoice JOIN invoice_type ON invoice.type_id = invoice_type.id_invoice_type WHERE invoice.counterparty_id = (SELECT id_counterparty FROM counterparty WHERE name = %1)").arg(counterparty.name));
//    if(!query.isActive())
//    {
//        qDebug() << "InvoiceDialogTest::testGUI_AddDeleteCommodities(): second SQL query failed. Reason" << query.lastError().text() << "\nQuery: " << query.lastQuery();
//    }
//    QVERIFY(query.isActive());
//    QVERIFY(query.next());
//    QVERIFY(query.value(0).toString() == InvoiceTypeData::name(invoiceType));
}


void InvoiceDialogTest::testGUI_AddDeleteCommodities_data()
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

    const int maxRows = qrand() % 5 + 1; //1-200
    const int base = 10;
    for(int row = 0; row < maxRows; ++row)
    {
        const int maxCommod = qrand() % 10 + 1; //1-200 commodities per invoice
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
                    *(InvoiceNumberFormat_t::Parse(s.value(s.DEFAULT_INV_NUM_FORMAT).toString()).release()),
                                         invNum,
                                         QDate::currentDate(),
                                         QDate::currentDate(),
                                         invType);

        QTest::newRow(qPrintable(QString("%1").arg(row))) << lcd << counterparty << invNum << invType;
    }
}


void InvoiceDialogTest::startUserThread(GuiUser *guiUser, QThread *thread, QPushButton *buttonStart) const
{

    guiUser->moveToThread(thread);
    connect(thread, SIGNAL(started()), guiUser, SLOT(process()));
    connect(guiUser, SIGNAL(finished()), thread, SLOT(quit()));
    thread->start();
    QTest::mouseClick(buttonStart, Qt::LeftButton);
}


QTEST_MAIN(InvoiceDialogTest)

#include "tst_invoicedialogtest.moc"
