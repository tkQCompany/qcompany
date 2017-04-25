#include <QString>
#include <QtTest>
#include <QDomDocument>

#define private public

#include "../../TestsCommon/TestsCommon.h"
#include "InvoiceComposer.h"
#include "SettingsGlobal.h"

class InvoiceComposerTest : public QObject
{
    Q_OBJECT

private:
    QDomElement findChildElementById(const QString &attributeValue, const QString &elemName, const QDomElement &parent);

private Q_SLOTS:
    void initTestCase();
    void testCaseDefault();
    void testCaseCheckLanguage();
    void testCaseCheckLanguage_data();
    void testCaseCheckInvoiceType();
    void testCaseCheckInvoiceType_data();
    void testCaseCheckStyle();
    void testCaseCheckLogo();
    void testCaseCheckInvoiceHeader();
    void testCaseCheckInvoiceHeader_data();
    void testCaseCheckSeller();
    void testCaseCheckSeller_data();
    void testCaseCheckBuyer();
    void testCaseCheckBuyer_data();
    void testCaseCheckProducts();
    void testCaseCheckProducts_data();
};


void InvoiceComposerTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
    SettingsGlobal s;
    s.resetSettings();
    s.setFirstRun(false);
}

QDomElement InvoiceComposerTest::findChildElementById(const QString &attributeValue, const QString &elemName, const QDomElement &parent)
{
    QDomElement ret = parent.firstChildElement(elemName);
    while(!ret.isNull() && (ret.attribute("id") != attributeValue) )
    {
        ret = ret.nextSiblingElement(elemName);
    }

    if(ret.attribute("id") != attributeValue)
        return QDomElement();
    else
        return ret;
}


void InvoiceComposerTest::testCaseDefault()
{
    InvoiceComposer ic;
    QVERIFY(ic.getInvoiceHtml().isEmpty());
}

void InvoiceComposerTest::testCaseCheckLanguage()
{
    QFETCH(QString, language);

    SettingsGlobal s;
    s.setValue(s.LANG, language);

    //filling invoice
    InvoiceComposer ic;
    InvoiceData id;
    Money_t val;
    DecVal quantity;
    CounterpartyData cd;
    CommodityVisualData cvd;
    QList<CommodityVisualData> lcvd;
    lcvd.append(cvd);
    ic.setData(id, val, val, quantity, cd, cd, lcvd);

    //checking
    QDomDocument doc;
    QString errorMsg;
    int errorLine = -1, errorColumn = -1;
    QVERIFY2(doc.setContent(ic.getInvoiceHtml(), &errorMsg, &errorLine, &errorColumn),
             (errorMsg + QString(", errorLine = %1, errorColumn = %2")
              .arg(errorLine).arg(errorColumn))
             .toStdString().c_str());
    QVERIFY(doc.documentElement().hasAttributes());
    const auto rootAttribs = doc.documentElement().attributes();
    QCOMPARE(rootAttribs.namedItem("xml:lang").toAttr().value(), language);
    QCOMPARE(rootAttribs.namedItem("lang").toAttr().value(), language);
}

void InvoiceComposerTest::testCaseCheckLanguage_data()
{
    QTest::addColumn<QString>("language");
    QTest::newRow("pl") << QString("pl");
    QTest::newRow("en") << QString("en");
    QTest::newRow("de") << QString("de");
}


void InvoiceComposerTest::testCaseCheckInvoiceType()
{
    QFETCH(InvoiceTypeData::Type, invoiceType);

    //filling invoice
    InvoiceComposer ic;
    InvoiceData id;
    id.setType(invoiceType);

    Money_t val;
    DecVal quantity;
    CounterpartyData cd;
    CommodityVisualData cvd;
    QList<CommodityVisualData> lcvd;
    lcvd.append(cvd);
    ic.setData(id, val, val, quantity, cd, cd, lcvd);

    //checking
    QDomDocument doc;
    QString errorMsg;
    int errorLine = -1, errorColumn = -1;
    //std::cerr << ic.getInvoiceHtml().toStdString() << std::endl;
    QVERIFY2(doc.setContent(ic.getInvoiceHtml(), &errorMsg, &errorLine, &errorColumn),
             (errorMsg + QString(", errorLine = %1, errorColumn = %2")
              .arg(errorLine).arg(errorColumn))
             .toStdString().c_str());
    const auto titleElem = doc.documentElement().firstChildElement().firstChildElement("title");
    QCOMPARE(titleElem.text(), InvoiceTypeData::name(invoiceType));
}

void InvoiceComposerTest::testCaseCheckInvoiceType_data()
{
    QTest::addColumn<InvoiceTypeData::Type>("invoiceType");
    QTest::newRow("InvoiceTypeData::BILL") << InvoiceTypeData::BILL;
    QTest::newRow("InvoiceTypeData::CORRECTIVE_GROSS") << InvoiceTypeData::CORRECTIVE_GROSS;
    QTest::newRow("InvoiceTypeData::CORRECTIVE_VAT") << InvoiceTypeData::CORRECTIVE_VAT;
    QTest::newRow("InvoiceTypeData::GROSS") << InvoiceTypeData::GROSS;
    QTest::newRow("InvoiceTypeData::PRO_FORMA") << InvoiceTypeData::PRO_FORMA;
    QTest::newRow("InvoiceTypeData::VAT") << InvoiceTypeData::VAT;
}


void InvoiceComposerTest::testCaseCheckStyle()
{
    //filling invoice
    InvoiceComposer ic;
    InvoiceData id;
    Money_t val;
    DecVal quantity;
    CounterpartyData cd;
    CommodityVisualData cvd;
    QList<CommodityVisualData> lcvd;
    lcvd.append(cvd);
    ic.setData(id, val, val, quantity, cd, cd, lcvd);

    //checking
    QDomDocument doc;
    QString errorMsg;
    int errorLine = -1, errorColumn = -1;
    //std::cerr << ic.getInvoiceHtml().toStdString() << std::endl;
    QVERIFY2(doc.setContent(ic.getInvoiceHtml(), &errorMsg, &errorLine, &errorColumn),
             (errorMsg + QString(", errorLine = %1, errorColumn = %2")
              .arg(errorLine).arg(errorColumn))
             .toStdString().c_str());
    const auto styleElem = doc.documentElement().firstChildElement().firstChildElement("style");
    QVERIFY(!styleElem.isNull());
}

void InvoiceComposerTest::testCaseCheckLogo()
{
    //filling invoice
    SettingsGlobal s;
    s.setValue(s.LOGO_PATH, "logo.png");

    InvoiceComposer ic;
    InvoiceData id;
    Money_t val;
    DecVal quantity;
    CounterpartyData cd;
    CommodityVisualData cvd;
    QList<CommodityVisualData> lcvd;
    lcvd.append(cvd);
    ic.setData(id, val, val, quantity, cd, cd, lcvd);

    //checking
    QDomDocument doc;
    QString errorMsg;
    int errorLine = -1, errorColumn = -1;
    //std::cerr << ic.getInvoiceHtml().toStdString() << std::endl;
    QVERIFY2(doc.setContent(ic.getInvoiceHtml(), &errorMsg, &errorLine, &errorColumn),
             (errorMsg + QString(", errorLine = %1, errorColumn = %2")
              .arg(errorLine).arg(errorColumn))
             .toStdString().c_str());
    const auto logoElem = doc.documentElement().firstChildElement("body").firstChildElement("div").firstChildElement("img");
    QVERIFY(!logoElem.isNull());
    QCOMPARE(logoElem.attribute("src"), QString("logo.png"));
}


void InvoiceComposerTest::testCaseCheckInvoiceHeader()
{
    QFETCH(bool, displayInvoiceNum);
    QFETCH(bool, displayIssuanceDate);
    QFETCH(bool, displaySellingDate);
    QFETCH(bool, displayOriginalCopy);
    QFETCH(QString, invoiceNumber);
    QFETCH(QDate, issuanceDate);
    QFETCH(QDate, sellingDate);
    QFETCH(bool, original);

    SettingsGlobal s;
    s.setValue(s.DISPLAY_INVOICE_NUMBER, displayInvoiceNum);
    s.setValue(s.DISPLAY_ISSUANCE_DATE, displayIssuanceDate);
    s.setValue(s.DISPLAY_SELLING_DATE, displaySellingDate);
    s.setValue(s.DISPLAY_ORIGINAL_COPY, displayOriginalCopy);

    //filling invoice
    InvoiceComposer ic;

    InvoiceData id;
    id.setInvNumber(invoiceNumber);
    id.setIssuanceDate(issuanceDate);
    id.setSellingDate(sellingDate);

    Money_t val;
    DecVal quantity;
    CounterpartyData cd;
    CommodityVisualData cvd;
    QList<CommodityVisualData> lcvd;
    lcvd.append(cvd);
    ic.setData(id, val, val, quantity, cd, cd, lcvd);

    //checking
    QDomDocument doc;
    QString errorMsg;
    int errorLine = -1, errorColumn = -1;

    QVERIFY2(doc.setContent(ic.getInvoiceHtml(), &errorMsg, &errorLine, &errorColumn),
             (errorMsg + QString(", errorLine = %1, errorColumn = %2")
              .arg(errorLine).arg(errorColumn))
             .toStdString().c_str());
    const auto elemInvoiceHeader = doc.documentElement().firstChildElement("body").firstChildElement("div").nextSiblingElement();
    QVERIFY(!elemInvoiceHeader.isNull());
    QCOMPARE(elemInvoiceHeader.attribute("id"), QString("invoice_header"));

    const auto elemInvNumber = findChildElementById("invoice_number", "li", elemInvoiceHeader.firstChildElement("ul"));
    const auto elemIssuanceDate = findChildElementById("issuance_date", "li", elemInvoiceHeader.firstChildElement("ul"));
    const auto elemSellingDate = findChildElementById("selling_date", "li", elemInvoiceHeader.firstChildElement("ul"));
    const auto elemOrigCopy = findChildElementById("original_or_copy", "li", elemInvoiceHeader.firstChildElement("ul"));

    QCOMPARE(!elemInvNumber.isNull(), displayInvoiceNum);
    QCOMPARE(!elemIssuanceDate.isNull(), displayIssuanceDate);
    QCOMPARE(!elemSellingDate.isNull(), displaySellingDate);
    QCOMPARE(!elemOrigCopy.isNull(), displayOriginalCopy);

    if(displayInvoiceNum)
    {
        QVERIFY(elemInvNumber.text().contains(invoiceNumber));
    }
    else
    {
        QVERIFY(elemInvNumber.text().isEmpty());
    }

    if(displayIssuanceDate)
    {
        QVERIFY(elemIssuanceDate.text().contains(issuanceDate.toString(s.dateFormatExternal())));
    }
    else
    {
        QVERIFY(elemIssuanceDate.text().isEmpty());
    }

    if(displaySellingDate)
    {
        QVERIFY(elemSellingDate.text().contains(sellingDate.toString(s.dateFormatExternal())));
    }
    else
    {
        QVERIFY(elemSellingDate.text().isEmpty());
    }

    if(displayOriginalCopy)
    {
        QVERIFY(elemOrigCopy.text().contains(original ? QObject::trUtf8("ORYGINAŁ") : QObject::trUtf8("KOPIA")));
    }
    else
    {
        QVERIFY(elemOrigCopy.text().isEmpty());
    }
}

void InvoiceComposerTest::testCaseCheckInvoiceHeader_data()
{
    QTest::addColumn<bool>("displayInvoiceNum");
    QTest::addColumn<bool>("displayIssuanceDate");
    QTest::addColumn<bool>("displaySellingDate");
    QTest::addColumn<bool>("displayOriginalCopy");
    QTest::addColumn<QString>("invoiceNumber");
    QTest::addColumn<QDate>("issuanceDate");
    QTest::addColumn<QDate>("sellingDate");
    QTest::addColumn<bool>("original");

    QTest::newRow("0000") << false << false << false << false << QString("") << QDate(2001, 1, 1) << QDate(2001, 1, 2) << false;
    QTest::newRow("0001") << false << false << false << true << QString("") << QDate(2001, 1, 2) << QDate(2001, 1, 3) << true;
    QTest::newRow("0010") << false << false << true << false << QString("") << QDate(2001, 1, 3) << QDate(2001, 1, 4) << false;
    QTest::newRow("0011") << false << false << true << true << QString("") << QDate(2001, 1, 4) << QDate(2001, 1, 5) << true;
    QTest::newRow("0100") << false << true << false << false << QString("") << QDate(2001, 1, 6) << QDate(2001, 1, 7) << false;
    QTest::newRow("0101") << false << true << false << true << QString("") << QDate(2001, 1, 8) << QDate(2001, 1, 9) << true;
    QTest::newRow("0110") << false << true << true << false << QString("") << QDate(2001, 1, 9) << QDate(2001, 1, 10) << false;
    QTest::newRow("0111") << false << true << true << true << QString("") << QDate(2001, 1, 10) << QDate(2001, 1, 11) << true;

    QTest::newRow("1000") << true << false << false << false << QString("invNum1000") << QDate(2002, 1, 1) << QDate(2002, 1, 2) << false;
    QTest::newRow("1001") << true << false << false << true << QString("invNum1001") << QDate(2002, 1, 2) << QDate(2002, 1, 3) << true;
    QTest::newRow("1010") << true << false << true << false << QString("invNum1010") << QDate(2002, 1, 3) << QDate(2002, 1, 4) << false;
    QTest::newRow("1011") << true << false << true << true << QString("invNum1011") << QDate(2002, 1, 4) << QDate(2002, 1, 5) << true;
    QTest::newRow("1100") << true << true << false << false << QString("invNum1100") << QDate(2002, 1, 6) << QDate(2002, 1, 7) << false;
    QTest::newRow("1101") << true << true << false << true << QString("invNum1101") << QDate(2002, 1, 8) << QDate(2002, 1, 9) << true;
    QTest::newRow("1110") << true << true << true << false << QString("invNum1110") << QDate(2002, 1, 9) << QDate(2002, 1, 10) << false;
    QTest::newRow("1111") << true << true << true << true << QString("invNum1111") << QDate(2002, 1, 10) << QDate(2002, 1, 11) << true;
}


void InvoiceComposerTest::testCaseCheckSeller()
{
    QFETCH(bool, displaySellerName);
    QFETCH(bool, displaySellerStreet);
    QFETCH(bool, displaySellerLocation);
    QFETCH(bool, displaySellerTaxId);
    QFETCH(bool, displaySellerAccount);
    QFETCH(QString, sellerName);
    QFETCH(QString, sellerStreet);
    QFETCH(QString, sellerLocation);
    QFETCH(QString, sellerTaxId);
    QFETCH(QString, sellerAccount);

    SettingsGlobal s;
    s.setValue(s.DISPLAY_SELLER_NAME, displaySellerName);
    s.setValue(s.DISPLAY_SELLER_STREET, displaySellerStreet);
    s.setValue(s.DISPLAY_SELLER_LOCATION, displaySellerLocation);
    s.setValue(s.DISPLAY_SELLER_TAXID, displaySellerTaxId);
    s.setValue(s.DISPLAY_SELLER_ACCOUNT, displaySellerAccount);


    //filling invoice
    InvoiceComposer ic;

    InvoiceData id;
    Money_t val;
    DecVal quantity;
    CounterpartyData cd;
    cd.setName(sellerName);
    cd.setStreet(sellerStreet);
    cd.setLocation(sellerLocation);
    cd.setTax_ident(sellerTaxId);
    cd.setAccountName(sellerAccount);
    CommodityVisualData cvd;
    QList<CommodityVisualData> lcvd;
    lcvd.append(cvd);
    ic.setData(id, val, val, quantity, cd, cd, lcvd);

    //checking
    QDomDocument doc;
    QString errorMsg;
    int errorLine = -1, errorColumn = -1;

    QVERIFY2(doc.setContent(ic.getInvoiceHtml(), &errorMsg, &errorLine, &errorColumn),
             (errorMsg + QString(", errorLine = %1, errorColumn = %2")
              .arg(errorLine).arg(errorColumn))
             .toStdString().c_str());
    const auto elemSeller = doc.documentElement().firstChildElement("body").firstChildElement("div").nextSiblingElement().nextSiblingElement().firstChildElement();
    QVERIFY(!elemSeller.isNull());
    QCOMPARE(elemSeller.attribute("id"), QString("seller"));

    const auto elemSellerName = findChildElementById("seller_name", "li", elemSeller.firstChildElement("ul"));
    const auto elemSellerStreet = findChildElementById("seller_street", "li", elemSeller.firstChildElement("ul"));
    const auto elemSellerLocation = findChildElementById("seller_location", "li", elemSeller.firstChildElement("ul"));
    const auto elemSellerTaxId = findChildElementById("seller_tax_id", "li", elemSeller.firstChildElement("ul"));
    const auto elemSellerAccount = findChildElementById("seller_account", "li", elemSeller.firstChildElement("ul"));

    QCOMPARE(!elemSellerName.isNull(), displaySellerName);
    QCOMPARE(!elemSellerStreet.isNull(), displaySellerStreet);
    QCOMPARE(!elemSellerLocation.isNull(), displaySellerLocation);
    QCOMPARE(!elemSellerTaxId.isNull(), displaySellerTaxId);
    QCOMPARE(!elemSellerAccount.isNull(), displaySellerAccount);

    if(displaySellerName)
    {
        QVERIFY(elemSellerName.text().contains(sellerName));
    }
    else
    {
        QVERIFY(elemSellerName.text().isEmpty());
    }

    if(displaySellerStreet)
    {
        QVERIFY(elemSellerStreet.text().contains(sellerStreet));
    }
    else
    {
        QVERIFY(elemSellerStreet.text().isEmpty());
    }

    if(displaySellerLocation)
    {
        QVERIFY(elemSellerLocation.text().contains(sellerLocation));
    }
    else
    {
        QVERIFY(elemSellerLocation.text().isEmpty());
    }

    if(displaySellerTaxId)
    {
        QVERIFY(elemSellerTaxId.text().contains(sellerTaxId));
    }
    else
    {
        QVERIFY(elemSellerTaxId.text().isEmpty());
    }

    if(displaySellerAccount)
    {
        QVERIFY(elemSellerAccount.text().contains(sellerAccount));
    }
    else
    {
        QVERIFY(elemSellerAccount.text().isEmpty());
    }
}


void InvoiceComposerTest::testCaseCheckSeller_data()
{
    QTest::addColumn<bool>("displaySellerName");
    QTest::addColumn<bool>("displaySellerStreet");
    QTest::addColumn<bool>("displaySellerLocation");
    QTest::addColumn<bool>("displaySellerTaxId");
    QTest::addColumn<bool>("displaySellerAccount");
    QTest::addColumn<QString>("sellerName");
    QTest::addColumn<QString>("sellerStreet");
    QTest::addColumn<QString>("sellerLocation");
    QTest::addColumn<QString>("sellerTaxId");
    QTest::addColumn<QString>("sellerAccount");

    QTest::newRow("00000") << false << false << false << false << false << QString("name0") << QString("street0") << QString("location0") << QString("taxId0") << QString("account0");
    QTest::newRow("00001") << false << false << false << false << true  << QString("name1") << QString("street1") << QString("location1") << QString("taxId1") << QString("account1");
    QTest::newRow("00010") << false << false << false << true  << false << QString("name2") << QString("street2") << QString("location2") << QString("taxId2") << QString("account2");
    QTest::newRow("00011") << false << false << false << true  << true  << QString("name3") << QString("street3") << QString("location3") << QString("taxId3") << QString("account3");
    QTest::newRow("00100") << false << false << true  << false << false << QString("name4") << QString("street4") << QString("location4") << QString("taxId4") << QString("account4");
    QTest::newRow("00101") << false << false << true  << false << true  << QString("name5") << QString("street5") << QString("location5") << QString("taxId5") << QString("account5");
    QTest::newRow("00110") << false << false << true  << true  << false << QString("name6") << QString("street6") << QString("location6") << QString("taxId6") << QString("account6");
    QTest::newRow("00111") << false << false << true  << true  << true  << QString("name7") << QString("street7") << QString("location7") << QString("taxId7") << QString("account7");
    QTest::newRow("01000") << false << true  << false << false << false << QString("name8") << QString("street8") << QString("location8") << QString("taxId8") << QString("account8");
    QTest::newRow("01001") << false << true  << false << false << true  << QString("name9") << QString("street9") << QString("location9") << QString("taxId9") << QString("account9");
    QTest::newRow("01010") << false << true  << false << true  << false << QString("name10") << QString("street10") << QString("location10") << QString("taxId10") << QString("account10");
    QTest::newRow("01011") << false << true  << false << true  << true  << QString("name11") << QString("street11") << QString("location11") << QString("taxId11") << QString("account11");
    QTest::newRow("01100") << false << true  << true  << false << false << QString("name12") << QString("street12") << QString("location12") << QString("taxId12") << QString("account12");
    QTest::newRow("01101") << false << true  << true  << false << true  << QString("name13") << QString("street13") << QString("location13") << QString("taxId13") << QString("account13");
    QTest::newRow("01110") << false << true  << true  << true  << false << QString("name14") << QString("street14") << QString("location14") << QString("taxId14") << QString("account14");
    QTest::newRow("01111") << false << true  << true  << true  << true  << QString("name15") << QString("street15") << QString("location15") << QString("taxId15") << QString("account15");
    QTest::newRow("10000") << true  << false << false << false << false << QString("name16") << QString("street16") << QString("location16") << QString("taxId16") << QString("account16");
    QTest::newRow("10001") << true  << false << false << false << true  << QString("name17") << QString("street17") << QString("location17") << QString("taxId17") << QString("account17");
    QTest::newRow("10010") << true  << false << false << true  << false << QString("name18") << QString("street18") << QString("location18") << QString("taxId18") << QString("account18");
    QTest::newRow("10011") << true  << false << false << true  << true  << QString("name19") << QString("street19") << QString("location19") << QString("taxId19") << QString("account19");
    QTest::newRow("10100") << true  << false << true  << false << false << QString("name20") << QString("street20") << QString("location20") << QString("taxId20") << QString("account20");
    QTest::newRow("10101") << true  << false << true  << false << true  << QString("name21") << QString("street21") << QString("location21") << QString("taxId21") << QString("account21");
    QTest::newRow("10110") << true  << false << true  << true  << false << QString("name22") << QString("street22") << QString("location22") << QString("taxId22") << QString("account22");
    QTest::newRow("10111") << true  << false << true  << true  << true  << QString("name23") << QString("street23") << QString("location23") << QString("taxId23") << QString("account23");
    QTest::newRow("11000") << true  << true  << false << false << false << QString("name24") << QString("street24") << QString("location24") << QString("taxId24") << QString("account24");
    QTest::newRow("11001") << true  << true  << false << false << true  << QString("name25") << QString("street25") << QString("location25") << QString("taxId25") << QString("account25");
    QTest::newRow("11010") << true  << true  << false << true  << false << QString("name26") << QString("street26") << QString("location26") << QString("taxId26") << QString("account26");
    QTest::newRow("11011") << true  << true  << false << true  << true  << QString("name27") << QString("street27") << QString("location27") << QString("taxId27") << QString("account27");
    QTest::newRow("11100") << true  << true  << true  << false << false << QString("name28") << QString("street28") << QString("location28") << QString("taxId28") << QString("account28");
    QTest::newRow("11101") << true  << true  << true  << false << true  << QString("name29") << QString("street29") << QString("location29") << QString("taxId29") << QString("account29");
    QTest::newRow("11110") << true  << true  << true  << true  << false << QString("name30") << QString("street30") << QString("location30") << QString("taxId30") << QString("account30");
    QTest::newRow("11111") << true  << true  << true  << true  << true  << QString("name31") << QString("street31") << QString("location31") << QString("taxId31") << QString("account31");

}

void InvoiceComposerTest::testCaseCheckBuyer()
{
    QFETCH(bool, displayBuyerName);
    QFETCH(bool, displayBuyerStreet);
    QFETCH(bool, displayBuyerLocation);
    QFETCH(bool, displayBuyerTaxId);
    QFETCH(bool, displayBuyerAccount);
    QFETCH(QString, buyerName);
    QFETCH(QString, buyerStreet);
    QFETCH(QString, buyerLocation);
    QFETCH(QString, buyerTaxId);
    QFETCH(QString, buyerAccount);

    SettingsGlobal s;
    s.setValue(s.DISPLAY_BUYER_NAME, displayBuyerName);
    s.setValue(s.DISPLAY_BUYER_STREET, displayBuyerStreet);
    s.setValue(s.DISPLAY_BUYER_LOCATION, displayBuyerLocation);
    s.setValue(s.DISPLAY_BUYER_TAXID, displayBuyerTaxId);
    s.setValue(s.DISPLAY_BUYER_ACCOUNT, displayBuyerAccount);


    //filling invoice
    InvoiceComposer ic;

    InvoiceData id;
    Money_t val;
    DecVal quantity;
    CounterpartyData cd;
    cd.setName(buyerName);
    cd.setStreet(buyerStreet);
    cd.setLocation(buyerLocation);
    cd.setTax_ident(buyerTaxId);
    cd.setAccountName(buyerAccount);
    CommodityVisualData cvd;
    QList<CommodityVisualData> lcvd;
    lcvd.append(cvd);
    ic.setData(id, val, val, quantity, cd, cd, lcvd);

    //checking
    QDomDocument doc;
    QString errorMsg;
    int errorLine = -1, errorColumn = -1;

    QVERIFY2(doc.setContent(ic.getInvoiceHtml(), &errorMsg, &errorLine, &errorColumn),
             (errorMsg + QString(", errorLine = %1, errorColumn = %2")
              .arg(errorLine).arg(errorColumn))
             .toStdString().c_str());
    const auto elemBuyer = doc.documentElement().firstChildElement("body").firstChildElement("div").nextSiblingElement().nextSiblingElement().firstChildElement().nextSiblingElement();
    QVERIFY(!elemBuyer.isNull());
    QCOMPARE(elemBuyer.attribute("id"), QString("buyer"));

    const auto elemBuyerName = findChildElementById("buyer_name", "li", elemBuyer.firstChildElement("ul"));
    const auto elemBuyerStreet = findChildElementById("buyer_street", "li", elemBuyer.firstChildElement("ul"));
    const auto elemBuyerLocation = findChildElementById("buyer_location", "li", elemBuyer.firstChildElement("ul"));
    const auto elemBuyerTaxId = findChildElementById("buyer_tax_id", "li", elemBuyer.firstChildElement("ul"));
    const auto elemBuyerAccount = findChildElementById("buyer_account", "li", elemBuyer.firstChildElement("ul"));

    QCOMPARE(!elemBuyerName.isNull(), displayBuyerName);
    QCOMPARE(!elemBuyerStreet.isNull(), displayBuyerStreet);
    QCOMPARE(!elemBuyerLocation.isNull(), displayBuyerLocation);
    QCOMPARE(!elemBuyerTaxId.isNull(), displayBuyerTaxId);
    QCOMPARE(!elemBuyerAccount.isNull(), displayBuyerAccount);

    if(displayBuyerName)
    {
        QVERIFY(elemBuyerName.text().contains(buyerName));
    }
    else
    {
        QVERIFY(elemBuyerName.text().isEmpty());
    }

    if(displayBuyerStreet)
    {
        QVERIFY(elemBuyerStreet.text().contains(buyerStreet));
    }
    else
    {
        QVERIFY(elemBuyerStreet.text().isEmpty());
    }

    if(displayBuyerLocation)
    {
        QVERIFY(elemBuyerLocation.text().contains(buyerLocation));
    }
    else
    {
        QVERIFY(elemBuyerLocation.text().isEmpty());
    }

    if(displayBuyerTaxId)
    {
        QVERIFY(elemBuyerTaxId.text().contains(buyerTaxId));
    }
    else
    {
        QVERIFY(elemBuyerTaxId.text().isEmpty());
    }

    if(displayBuyerAccount)
    {
        QVERIFY(elemBuyerAccount.text().contains(buyerAccount));
    }
    else
    {
        QVERIFY(elemBuyerAccount.text().isEmpty());
    }
}

void InvoiceComposerTest::testCaseCheckBuyer_data()
{
    QTest::addColumn<bool>("displayBuyerName");
    QTest::addColumn<bool>("displayBuyerStreet");
    QTest::addColumn<bool>("displayBuyerLocation");
    QTest::addColumn<bool>("displayBuyerTaxId");
    QTest::addColumn<bool>("displayBuyerAccount");
    QTest::addColumn<QString>("buyerName");
    QTest::addColumn<QString>("buyerStreet");
    QTest::addColumn<QString>("buyerLocation");
    QTest::addColumn<QString>("buyerTaxId");
    QTest::addColumn<QString>("buyerAccount");

    QTest::newRow("00000") << false << false << false << false << false << QString("name0") << QString("street0") << QString("location0") << QString("taxId0") << QString("account0");
    QTest::newRow("00001") << false << false << false << false << true  << QString("name1") << QString("street1") << QString("location1") << QString("taxId1") << QString("account1");
    QTest::newRow("00010") << false << false << false << true  << false << QString("name2") << QString("street2") << QString("location2") << QString("taxId2") << QString("account2");
    QTest::newRow("00011") << false << false << false << true  << true  << QString("name3") << QString("street3") << QString("location3") << QString("taxId3") << QString("account3");
    QTest::newRow("00100") << false << false << true  << false << false << QString("name4") << QString("street4") << QString("location4") << QString("taxId4") << QString("account4");
    QTest::newRow("00101") << false << false << true  << false << true  << QString("name5") << QString("street5") << QString("location5") << QString("taxId5") << QString("account5");
    QTest::newRow("00110") << false << false << true  << true  << false << QString("name6") << QString("street6") << QString("location6") << QString("taxId6") << QString("account6");
    QTest::newRow("00111") << false << false << true  << true  << true  << QString("name7") << QString("street7") << QString("location7") << QString("taxId7") << QString("account7");
    QTest::newRow("01000") << false << true  << false << false << false << QString("name8") << QString("street8") << QString("location8") << QString("taxId8") << QString("account8");
    QTest::newRow("01001") << false << true  << false << false << true  << QString("name9") << QString("street9") << QString("location9") << QString("taxId9") << QString("account9");
    QTest::newRow("01010") << false << true  << false << true  << false << QString("name10") << QString("street10") << QString("location10") << QString("taxId10") << QString("account10");
    QTest::newRow("01011") << false << true  << false << true  << true  << QString("name11") << QString("street11") << QString("location11") << QString("taxId11") << QString("account11");
    QTest::newRow("01100") << false << true  << true  << false << false << QString("name12") << QString("street12") << QString("location12") << QString("taxId12") << QString("account12");
    QTest::newRow("01101") << false << true  << true  << false << true  << QString("name13") << QString("street13") << QString("location13") << QString("taxId13") << QString("account13");
    QTest::newRow("01110") << false << true  << true  << true  << false << QString("name14") << QString("street14") << QString("location14") << QString("taxId14") << QString("account14");
    QTest::newRow("01111") << false << true  << true  << true  << true  << QString("name15") << QString("street15") << QString("location15") << QString("taxId15") << QString("account15");
    QTest::newRow("10000") << true  << false << false << false << false << QString("name16") << QString("street16") << QString("location16") << QString("taxId16") << QString("account16");
    QTest::newRow("10001") << true  << false << false << false << true  << QString("name17") << QString("street17") << QString("location17") << QString("taxId17") << QString("account17");
    QTest::newRow("10010") << true  << false << false << true  << false << QString("name18") << QString("street18") << QString("location18") << QString("taxId18") << QString("account18");
    QTest::newRow("10011") << true  << false << false << true  << true  << QString("name19") << QString("street19") << QString("location19") << QString("taxId19") << QString("account19");
    QTest::newRow("10100") << true  << false << true  << false << false << QString("name20") << QString("street20") << QString("location20") << QString("taxId20") << QString("account20");
    QTest::newRow("10101") << true  << false << true  << false << true  << QString("name21") << QString("street21") << QString("location21") << QString("taxId21") << QString("account21");
    QTest::newRow("10110") << true  << false << true  << true  << false << QString("name22") << QString("street22") << QString("location22") << QString("taxId22") << QString("account22");
    QTest::newRow("10111") << true  << false << true  << true  << true  << QString("name23") << QString("street23") << QString("location23") << QString("taxId23") << QString("account23");
    QTest::newRow("11000") << true  << true  << false << false << false << QString("name24") << QString("street24") << QString("location24") << QString("taxId24") << QString("account24");
    QTest::newRow("11001") << true  << true  << false << false << true  << QString("name25") << QString("street25") << QString("location25") << QString("taxId25") << QString("account25");
    QTest::newRow("11010") << true  << true  << false << true  << false << QString("name26") << QString("street26") << QString("location26") << QString("taxId26") << QString("account26");
    QTest::newRow("11011") << true  << true  << false << true  << true  << QString("name27") << QString("street27") << QString("location27") << QString("taxId27") << QString("account27");
    QTest::newRow("11100") << true  << true  << true  << false << false << QString("name28") << QString("street28") << QString("location28") << QString("taxId28") << QString("account28");
    QTest::newRow("11101") << true  << true  << true  << false << true  << QString("name29") << QString("street29") << QString("location29") << QString("taxId29") << QString("account29");
    QTest::newRow("11110") << true  << true  << true  << true  << false << QString("name30") << QString("street30") << QString("location30") << QString("taxId30") << QString("account30");
    QTest::newRow("11111") << true  << true  << true  << true  << true  << QString("name31") << QString("street31") << QString("location31") << QString("taxId31") << QString("account31");
}


void InvoiceComposerTest::testCaseCheckProducts()
{
    QFETCH(QList<CommodityVisualData>, products);

    //filling invoice
    InvoiceComposer ic;

    InvoiceData id;
    Money_t val;
    DecVal quantity;
    CounterpartyData cd;

    ic.setData(id, val, val, quantity, cd, cd, products);


    //checking
    QDomDocument doc;
    QString errorMsg;
    int errorLine = -1, errorColumn = -1;

    QVERIFY2(doc.setContent(ic.getInvoiceHtml(), &errorMsg, &errorLine, &errorColumn),
             (errorMsg + QString(", errorLine = %1, errorColumn = %2")
              .arg(errorLine).arg(errorColumn))
             .toStdString().c_str());
    const auto elemProducts = doc.documentElement().firstChildElement("body").firstChildElement("div").nextSiblingElement().nextSiblingElement().nextSiblingElement();
    QVERIFY(!elemProducts.isNull());
    QCOMPARE(elemProducts.attribute("id"), QString("products"));

    auto elemCurrentRow = elemProducts.firstChildElement("table").firstChildElement("tr");
    QVERIFY(!elemCurrentRow.isNull());

    for(const CommodityVisualData product: products)
    {
        elemCurrentRow = elemCurrentRow.nextSiblingElement("tr");
        QVERIFY(!elemCurrentRow.isNull());

        const auto elemProductsId =         findChildElementById("prod_id", "td",       elemCurrentRow);
        const auto elemProductsName =       findChildElementById("prod_name", "td",     elemCurrentRow);
        const auto elemProductsPkwiu =      findChildElementById("prod_pkwiu", "td",    elemCurrentRow);
        const auto elemProductsQuantity =   findChildElementById("prod_quantity", "td", elemCurrentRow);
        const auto elemProductsUnit =       findChildElementById("prod_unit", "td",     elemCurrentRow);
        const auto elemProductsNetVal =     findChildElementById("prod_net_value", "td",  elemCurrentRow);
        const auto elemProductsDiscount =   findChildElementById("prod_discount", "td", elemCurrentRow);
        const auto elemProductsVatVal =     findChildElementById("prod_vat_value", "td",  elemCurrentRow);

        QCOMPARE(elemProductsId.text().toLongLong(),    product.ID());
        QCOMPARE(elemProductsName.text(),               product.name());
        QCOMPARE(elemProductsPkwiu.text(),              product.pkwiu());
        QCOMPARE(elemProductsQuantity.text(),           product.quantity().toString(3));
        QCOMPARE(elemProductsUnit.text(),               product.unit());
        QCOMPARE(elemProductsNetVal.text(),             product.net().toString(2));
        QCOMPARE(elemProductsDiscount.text(),           product.discount().toString(2));
        QCOMPARE(elemProductsVatVal.text(),             product.vat().toString(2));
    }
}


void InvoiceComposerTest::testCaseCheckProducts_data()
{
    QTest::addColumn<QList<CommodityVisualData>>("products");

    CommodityVisualData cvd1, cvd2;
    QList<CommodityVisualData> oneProduct, twoProducts;

    cvd1.setID(1LL);
    cvd1.setName("2");
    cvd1.setPkwiu("3");
    cvd1.setQuantity(DecVal(4.0));
    cvd1.setUnit("5");
    cvd1.setNet(Money_t(6));
    cvd1.setDiscount(DecVal(7));
    cvd1.setVat(DecVal(8));

    cvd2.setID(9LL);
    cvd2.setName("10");
    cvd2.setPkwiu("11");
    cvd2.setQuantity(DecVal(12.0));
    cvd2.setUnit("13");
    cvd2.setNet(Money_t(14));
    cvd2.setDiscount(DecVal(15));
    cvd2.setVat(DecVal(16));

    oneProduct.append(cvd1);
    twoProducts.append(cvd1);
    twoProducts.append(cvd2);

    QTest::newRow("1") << oneProduct;
    QTest::newRow("2") << twoProducts;
}


QTEST_APPLESS_MAIN(InvoiceComposerTest)

#include "tst_invoicecomposertest.moc"
