#include <QString>
#include <QTest>

#include "ModelInvoice.h"

class ModelInvoiceTest : public QObject
{
    Q_OBJECT
    
public:
    ModelInvoiceTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseSamePeriodValues();
    void testCaseSamePeriodValues_data();
    void testCaseCheckAllFields();
    void testCaseCheckAllFields_data();

private:
    void generateCasesSamePeriodValues_(const size_t maxInvoicesPerDay, const size_t maxDays, const QDate &startDate, const int maxDaysWithoutInvoices);
    static bool isDayChanging_(const QDate &currentDate, const QDate &prevDate);
    static bool isMonthChanging_(const QDate &currentDate, const QDate &prevDate);
    static bool isQuarterChanging_(const QDate &currentDate, const QDate &prevDate);
    static bool isYearChanging_(const QDate &currentDate, const QDate &prevDate);

private:
    ModelInvoice *model_;
    InvoiceNumberFormat_t format_;
};

ModelInvoiceTest::ModelInvoiceTest()
{
}

void ModelInvoiceTest::initTestCase()
{
    model_ = new ModelInvoice(this);
    qsrand(QTime::currentTime().msec());
}

void ModelInvoiceTest::cleanupTestCase()
{
    delete model_;
}


void ModelInvoiceTest::testCaseSamePeriodValues()
{
    QFETCH(QString, desiredInvNum);
    QFETCH(QString, prevInvNum);
    QFETCH(QDate, issuanceDate);
    QFETCH(QDate, prevIssuanceDate);
    QCOMPARE(model_->generateInvoiceNumber(format_, prevInvNum, issuanceDate, prevIssuanceDate, InvoiceTypeData::VAT), desiredInvNum);
}


void ModelInvoiceTest::testCaseSamePeriodValues_data()
{
    generateCasesSamePeriodValues_(qrand() % 20 + 1, qrand() % 3000 + 500, QDate(2013, qrand() % 12 + 1, qrand() % 28 + 1), qrand() % 400 + 1);
}

void ModelInvoiceTest::testCaseCheckAllFields()
{
    QFETCH(QString, desiredInvNum);
    QFETCH(QString, prevInvNum);
    QFETCH(QDate, issuanceDate);
    QFETCH(QDate, prevIssuanceDate);
    QFETCH(InvoiceTypeData::Type, invoiceType);
    QString ret(model_->generateInvoiceNumber(format_, prevInvNum, issuanceDate, prevIssuanceDate, invoiceType));
    QCOMPARE(ret, desiredInvNum);
}

void ModelInvoiceTest::testCaseCheckAllFields_data()
{
    SettingsGlobal s;

    format_.clearAll();
    format_.append(InvoiceNumberFormat_t::NR, s);
    format_.append(InvoiceNumberFormat_t::SLASH, s);
    format_.append(InvoiceNumberFormat_t::NR_Y, s);
    format_.append(InvoiceNumberFormat_t::BACKSLASH, s);
    format_.append(InvoiceNumberFormat_t::NR_M, s);
    format_.append(InvoiceNumberFormat_t::BACKSLASH, s);
    format_.append(InvoiceNumberFormat_t::NR_D, s);
    format_.append(InvoiceNumberFormat_t::SLASH, s);
    format_.append(InvoiceNumberFormat_t::NR_Q, s);
    format_.append(InvoiceNumberFormat_t::HYPHEN, s);
    format_.append(InvoiceNumberFormat_t::INVOICE_TYPE, s);
    format_.append(InvoiceNumberFormat_t::HYPHEN, s);
    format_.append(InvoiceNumberFormat_t::TEXT1, s);
    format_.append(InvoiceNumberFormat_t::HYPHEN, s);
    format_.append(InvoiceNumberFormat_t::TEXT2, s);
    format_.append(InvoiceNumberFormat_t::HYPHEN, s);
    format_.append(InvoiceNumberFormat_t::TEXT3, s);
    format_.append(InvoiceNumberFormat_t::HYPHEN, s);
    format_.append(InvoiceNumberFormat_t::PERIOD_YEAR, s);
    format_.append(InvoiceNumberFormat_t::BACKSLASH, s);
    format_.append(InvoiceNumberFormat_t::PERIOD_MONTH, s);
    format_.append(InvoiceNumberFormat_t::SLASH, s);
    format_.append(InvoiceNumberFormat_t::PERIOD_DAY, s);
    format_.append(InvoiceNumberFormat_t::BACKSLASH, s);
    format_.append(InvoiceNumberFormat_t::HYPHEN, s);
    format_.append(InvoiceNumberFormat_t::PERIOD_QUARTER, s);
    format_.append(InvoiceNumberFormat_t::SLASH, s);
    format_.append(InvoiceNumberFormat_t::BACKSLASH, s);
    format_.append(InvoiceNumberFormat_t::HYPHEN, s);

    QTest::addColumn<QString>("desiredInvNum");
    QTest::addColumn<QString>("prevInvNum");
    QTest::addColumn<QDate>("issuanceDate");
    QTest::addColumn<QDate>("prevIssuanceDate");
    QTest::addColumn<InvoiceTypeData::Type>("invoiceType");

    int nr_y = 1, nr_m = 1, nr_d = 1, nr_q = 1;
    long nr = 1;
    QDate issuanceDate, prevIssuanceDate;
    QString desiredInvNum, prevInvNum;
    issuanceDate.setYMD(2011, 1, 1);
    const InvoiceTypeData::Type invoiceType = InvoiceTypeData::VAT;

    const size_t maxDays = 400; //the whole year + some days
    size_t invoicesPerDay = 0;
    const size_t maxInvoicesPerDay = 200;
    const int fieldWidth = 2;
    const int base = 10;
    const QChar padding('0');
    for(size_t i = 0; i < maxDays; ++i)
    {
        invoicesPerDay = qrand() % maxInvoicesPerDay;
        for(size_t inv = 0; inv < invoicesPerDay; ++inv)
        {
            desiredInvNum = QString("%1/%2\\%3\\%4/%5-%6-%7-%8-%9-%10\\%11/%12\\-%13/\\-").arg(nr).arg(nr_y).arg(nr_m).arg(nr_d).arg(nr_q)
                    .arg(InvoiceTypeData::shortName(invoiceType))
                    .arg(s.value(s.TEXT1).toString()).arg(s.value(s.TEXT2).toString()).arg(s.value(s.TEXT3).toString())
                    .arg(issuanceDate.year())
                    .arg(issuanceDate.month(), fieldWidth, base, padding)
                    .arg(issuanceDate.day(), fieldWidth, base, padding)
                    .arg(1+ (issuanceDate.month() - 1)/3, fieldWidth, base, padding);
            QTest::newRow((QString("%1").arg(nr)).toAscii())
                    << desiredInvNum
                    << prevInvNum
                    << issuanceDate
                    << prevIssuanceDate
                    << invoiceType;
            prevInvNum = desiredInvNum;
            prevIssuanceDate = issuanceDate;
            nr++;
            if(isYearChanging_(issuanceDate, prevIssuanceDate))
            {
                nr_y = 1;
            }
            else
            {
                nr_y++;
            }

            if(isMonthChanging_(issuanceDate, prevIssuanceDate))
            {
                nr_m = 1;
            }
            else
            {
                nr_m++;
            }

            if(isDayChanging_(issuanceDate, prevIssuanceDate))
            {
                nr_d = 1;
            }
            else
            {
                nr_d++;
            }

            if(isQuarterChanging_(issuanceDate, prevIssuanceDate))
            {
                nr_q = 1;
            }
            else
            {
                nr_q++;
            }
        }
        issuanceDate.addDays(1);
    }
}


void ModelInvoiceTest::generateCasesSamePeriodValues_(const size_t maxInvoicesPerDay, const size_t maxDays, const QDate &startDate,
                                                         const int maxDaysWithoutInvoices)
{
    SettingsGlobal s;

    format_.clearAll();
    format_.append(InvoiceNumberFormat_t::NR, s);
    format_.append(InvoiceNumberFormat_t::SLASH, s);
    format_.append(InvoiceNumberFormat_t::PERIOD_YEAR, s);
    format_.append(InvoiceNumberFormat_t::BACKSLASH, s);
    format_.append(InvoiceNumberFormat_t::PERIOD_MONTH, s);
    format_.append(InvoiceNumberFormat_t::SLASH, s);
    format_.append(InvoiceNumberFormat_t::PERIOD_DAY, s);
    format_.append(InvoiceNumberFormat_t::BACKSLASH, s);
    format_.append(InvoiceNumberFormat_t::PERIOD_QUARTER, s);
    format_.append(InvoiceNumberFormat_t::HYPHEN, s);

    QTest::addColumn<QString>("desiredInvNum");
    QTest::addColumn<QString>("prevInvNum");
    QTest::addColumn<QDate>("issuanceDate");
    QTest::addColumn<QDate>("prevIssuanceDate");

    long nr = 1;
    QDate currIssuanceDate, prevIssuanceDate;
    QString currInvNum, prevInvNum;
    currIssuanceDate.setYMD(startDate.year(), startDate.month(), startDate.day());
    size_t invoicesPerDay = 0;
    const int fieldWidth = 2;
    const int base = 10;
    const QChar padding('0');
    const QString formatStr("%1/%2\\%3/%4\\%5-");

    for(size_t day = 0; day < maxDays; ++day)
    {
        invoicesPerDay = qrand() % maxInvoicesPerDay;

        for(size_t inv = 0; inv < invoicesPerDay; ++inv)
        {
            currInvNum = QString(formatStr).arg(nr).arg(currIssuanceDate.year())
                    .arg(currIssuanceDate.month(), fieldWidth, base, padding)
                    .arg(currIssuanceDate.day(), fieldWidth, base, padding)
                    .arg(1 + (currIssuanceDate.month()-1)/3, fieldWidth, base, padding);

            QTest::newRow(QString("%1").arg(nr).toAscii())
                    << currInvNum
                    << prevInvNum
                    << currIssuanceDate
                    << prevIssuanceDate;
            prevInvNum = currInvNum;
            prevIssuanceDate = currIssuanceDate;
            ++nr;
        }
        currIssuanceDate = currIssuanceDate.addDays(qrand() % maxDaysWithoutInvoices);
    }
}



bool ModelInvoiceTest::isDayChanging_(const QDate &currentDate, const QDate &prevDate)
{
    if(currentDate.isValid() && prevDate.isValid())
    {
        return currentDate.day() != prevDate.day();
    }
    else
    {
        return true;
    }
}


bool ModelInvoiceTest::isMonthChanging_(const QDate &currentDate, const QDate &prevDate)
{
    if(currentDate.isValid() && prevDate.isValid())
    {
        return currentDate.month() != prevDate.month();
    }
    else
    {
        return true;
    }
}


bool ModelInvoiceTest::isQuarterChanging_(const QDate &currentDate, const QDate &prevDate)
{
    if(currentDate.isValid() && prevDate.isValid())
    {
        const int q1 = (currentDate.month() - 1) / 3;
        const int q2 = (prevDate.month() - 1) / 3;

        return q1 != q2;
    }
    else
    {
        return true;
    }
}


bool ModelInvoiceTest::isYearChanging_(const QDate &currentDate, const QDate &prevDate)
{
    if(currentDate.isValid() && prevDate.isValid())
    {
        return currentDate.year() != prevDate.year();
    }
    else
    {
        return true;
    }
}




QTEST_MAIN(ModelInvoiceTest)

#include "tst_modelinvoicetest.moc"
