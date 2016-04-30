#include <QString>
#include <QtTest>
#include <QDomDocument>

//#include <iostream>

#define private public

#include "InvoiceComposer.h"
#include "SettingsGlobal.h"

class InvoiceComposerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testCaseDefault();
    void testCaseCheckLanguage();
    void testCaseCheckLanguage_data();
    void testCaseCheckInvoiceType();
    void testCaseCheckInvoiceType_data();
};


void InvoiceComposerTest::testCaseDefault()
{
    InvoiceComposer ic;
    QVERIFY(ic.getInvoiceHtml().isEmpty());
}

void InvoiceComposerTest::testCaseCheckLanguage()
{
    QFETCH(QString, language);

    SettingsGlobal s;
    s.setValue(s.keyName(s.LANG), language);

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
    QVERIFY(doc.documentElement().hasAttributes());
    const auto attribs = doc.documentElement().attributes();
    QCOMPARE(attribs.namedItem("xml:lang").toAttr().value(), language);
    QCOMPARE(attribs.namedItem("lang").toAttr().value(), language);
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

QTEST_APPLESS_MAIN(InvoiceComposerTest)

#include "tst_invoicecomposertest.moc"
