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

QTEST_APPLESS_MAIN(InvoiceComposerTest)

#include "tst_invoicecomposertest.moc"
