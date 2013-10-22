#include <QTest>
#include <QThread>

#include "../TestsCommon.h"
#include "Database.h"
#include "SettingsGlobal.h"
#include "CommodityTypeData.h"
#include "PaymentTypeData.h"
#include "InvoiceDialogPublic.h"
#include "UnitData.h"
#include "GuiUserAddCommodity.h"
#include "GuiUserAddNewCommodity.h"
#include "GuiUserAddCounterparty.h"
#include "ModelCommodity.h"
#include "ModelCountry.h"
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
    QFETCH(QList<CommodityVisualData>, lcvd);
    QFETCH(Counterparty_t, counterparty);

    Database db;
    InvoiceDialogPublic invD(0, &db, InvoiceTypeData::VAT);
    QThread threadNewCommodity, threadCommodity, threadCounterparty;
    qRegisterMetaType<QItemSelection>("QItemSelection");
    foreach(const CommodityVisualData cvd, lcvd)
    {
        GuiUserAddNewCommodity userAddNewCommod(&invD, &cvd);
        startUserThread(&userAddNewCommod, &threadNewCommodity, invD.ui()->pushButtonAddNewCommodity);
        while(threadNewCommodity.isRunning())
        {
            QTest::qSleep(100);
        }
        db.modelCommodity()->submitAll();

        GuiUserAddCommodity userAddCommod(&invD, &cvd);
        startUserThread(&userAddCommod, &threadCommodity, invD.ui()->pushButtonAddCommodity);
        while(threadCommodity.isRunning())
        {
            QTest::qSleep(100);
        }
    }

    QCOMPARE(invD.ui()->tableWidgetCommodities->rowCount(), lcvd.size());

    GuiUserAddCounterparty userAddNewCounterp(&invD, &counterparty);
    startUserThread(&userAddNewCounterp, &threadCounterparty, invD.ui()->pushButtonAddCounterparty);
    while(threadCounterparty.isRunning())
    {
        QTest::qSleep(100);
    }
    invD.ui()->comboBoxCounterparties->setCurrentIndex(0);

    QVERIFY(invD.ui()->comboBoxCounterparties->currentText() == counterparty.name);

    QTest::mouseClick(invD.ui()->pushButtonSave, Qt::LeftButton);
    QTest::mouseClick(invD.ui()->pushButtonClose, Qt::LeftButton);
}


void InvoiceDialogTest::testGUI_AddDeleteCommodities_data()
{
    QTest::addColumn<QList<CommodityVisualData> >("lcvd");
    QTest::addColumn<Counterparty_t>("counterparty");

    QList<CommodityVisualData> lcvd;
    Counterparty_t counterparty;

    SettingsGlobal s;
    Database db;
    QStringList vatRates(s.value(s.keyName(s.VAT_RATES)).toString().split("|"));

    const int maxRows = /*qrand() % 2*/ + 1; //1-200
    const int base = 10;
    for(int row = 0; row < maxRows; ++row)
    {
        const int maxCommod = /*qrand() % 2 +*/ 1; //1-200 commodities per invoice
        for(int commod = 0; commod < maxCommod; ++commod)
        {
            CommodityVisualData cvd;
            cvd.discount = QString("discount_%1_%2").arg(row).arg(commod);
            cvd.name = QString("name_%1_%2").arg(row).arg(commod);
            cvd.net = QString("net_%1_%2").arg(row).arg(commod);
            cvd.pkwiu = QString("pkwiu_%1_%2").arg(row).arg(commod);
            cvd.quantity = QString("%1").arg(qrand() % 1000 + 1);

            const CommodityTypeData::CommodityType type = (qrand()%2 == 0 ? CommodityTypeData::GOODS : CommodityTypeData::SERVICES);
            cvd.type = QString(CommodityTypeData::name(type));

            UnitData::Name unit = UnitData::Name(qrand() % 7 + 1);
            cvd.unit = QString(UnitData::name(unit));
            cvd.vat = vatRates[qrand() % vatRates.size()];

            lcvd.append(cvd);
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

        QTest::newRow(qPrintable(QString("%1").arg(row))) << lcvd << counterparty;
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
