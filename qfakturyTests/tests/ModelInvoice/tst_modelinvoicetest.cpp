#include <QString>
#include <QtTest>

#include "ModelInvoice.h"

class ModelInvoiceTest : public QObject
{
    Q_OBJECT
    
public:
    ModelInvoiceTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseOneShotGeneration();
    void testCaseOneShotGeneration_data();

private:
    ModelInvoice *model_;
    QDate issuanceDate_;
};

ModelInvoiceTest::ModelInvoiceTest()
{
}

void ModelInvoiceTest::initTestCase()
{
    model_ = new ModelInvoice(this);
}

void ModelInvoiceTest::cleanupTestCase()
{
    delete model_;
}

void ModelInvoiceTest::testCaseOneShotGeneration()
{
    QFETCH(QString, invNum);
    //QFETCH(InvoiceNumberFormat_t, format);
    QFETCH(QString, prevInvNum);
    QFETCH(QDate, issuanceDate);
    QFETCH(InvoiceTypeData::Type, invoiceType);
    //QVERIFY(invNum == model_->generateInvoiceNumber(format, prevInvNum, issuanceDate, invoiceType));
}

void ModelInvoiceTest::testCaseOneShotGeneration_data()
{
    issuanceDate_.setYMD(2011, 1, 1);

    QTest::addColumn<QString>("invNum");
    //QTest::addColumn<InvoiceNumberFormat_t>("format");
    QTest::addColumn<QString>("prevInvNum");
    QTest::addColumn<QDate>("issuanceDate");
    QTest::addColumn<InvoiceTypeData::Type>("invoiceType");

    InvoiceNumberFormat_t format;
    format.append(InvoiceNumberFormat_t::NR);
    format.append(InvoiceNumberFormat_t::SLASH);
    format.append(InvoiceNumberFormat_t::NR_Y);
    format.append(InvoiceNumberFormat_t::BACKSLASH);
    format.append(InvoiceNumberFormat_t::NR_M);
    format.append(InvoiceNumberFormat_t::NR_D);
    format.append(InvoiceNumberFormat_t::NR_Q);
    format.append(InvoiceNumberFormat_t::INVOICE_TYPE);
    format.append(InvoiceNumberFormat_t::TEXT1);
    format.append(InvoiceNumberFormat_t::TEXT2);
    format.append(InvoiceNumberFormat_t::TEXT3);
    format.append(InvoiceNumberFormat_t::PERIOD_YEAR);
    format.append(InvoiceNumberFormat_t::PERIOD_MONTH);
    format.append(InvoiceNumberFormat_t::PERIOD_DAY);
    format.append(InvoiceNumberFormat_t::PERIOD_QUARTER);
    format.append(InvoiceNumberFormat_t::SLASH);
    format.append(InvoiceNumberFormat_t::BACKSLASH);
    format.append(InvoiceNumberFormat_t::HYPHEN);

    InvoiceTypeData::Type invoiceType = InvoiceTypeData::VAT;

    const size_t maxDays = 365 + 50; //the whole year + some days
    for(size_t i = 0; i < maxDays; ++i)
    {
        QTest::newRow((QString("%1").arg(i)).toAscii())
                //<< format
                << QString("1/")
                << issuanceDate_
                << invoiceType;
    }
}

QTEST_MAIN(ModelInvoiceTest)

#include "tst_modelinvoicetest.moc"
