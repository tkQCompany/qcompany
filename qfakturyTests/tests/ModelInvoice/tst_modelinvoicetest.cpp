#include <QString>
#include <QTest>

#include "../TestsCommon.h"
#include "ModelInvoice.h"

class ModelInvoiceTest : public QObject
{
    Q_OBJECT
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseSamePeriodValues();
    void testCaseSamePeriodValues_data();
    void testCaseCheckAllFields();
    void testCaseCheckAllFields_data();
    void test_compatibilityWithOldGenerateInvoiceNumber();
    void test_compatibilityWithOldGenerateInvoiceNumber_data();

private:
    void generateCasesSamePeriodValues_(const size_t maxInvoicesPerDay, const size_t maxDays, const QDate &startDate, const int maxDaysWithoutInvoices);
    QString generateInvoiceNumberOldVer(InvoiceTypeData::Type invType, bool day, bool month, bool year, bool shortYear);
    QString numbersCount(int in, int x);
    static bool isDayChanging_(const QDate &currentDate, const QDate &prevDate);
    static bool isMonthChanging_(const QDate &currentDate, const QDate &prevDate);
    static bool isQuarterChanging_(const QDate &currentDate, const QDate &prevDate);
    static bool isYearChanging_(const QDate &currentDate, const QDate &prevDate);

private:
    ModelInvoice *model_;
    InvoiceNumberFormat_t format_;
};



void ModelInvoiceTest::initTestCase()
{
    TestsCommon::setAppData();
    SettingsGlobal s;
    s.setFirstRun(true);

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
    format_.append(InvoiceNumberFormat_t::HYPHEN, s);
    format_.append(InvoiceNumberFormat_t::PERIOD_SHORT_YEAR, s);
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
    issuanceDate.setDate(2011, 1, 1);
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
            desiredInvNum = QString("%1/%2\\%3\\%4/%5-%6-%7-%8-%9-%10-%11\\%12/%13\\-%14/\\-").arg(nr).arg(nr_y).arg(nr_m).arg(nr_d).arg(nr_q)
                    .arg(InvoiceTypeData::shortName(invoiceType))
                    .arg(s.value(s.TEXT1).toString()).arg(s.value(s.TEXT2).toString()).arg(s.value(s.TEXT3).toString())
                    .arg(issuanceDate.year())
                    .arg(issuanceDate.year() % 100, fieldWidth, base, padding)
                    .arg(issuanceDate.month(), fieldWidth, base, padding)
                    .arg(issuanceDate.day(), fieldWidth, base, padding)
                    .arg(1+ (issuanceDate.month() - 1)/3, fieldWidth, base, padding);
            QTest::newRow((QString("%1").arg(nr)).toLatin1())
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
    currIssuanceDate.setDate(startDate.year(), startDate.month(), startDate.day());
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

            QTest::newRow(QString("%1").arg(nr).toLatin1())
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


void ModelInvoiceTest::test_compatibilityWithOldGenerateInvoiceNumber()
{
    SettingsGlobal s;

    QFETCH(QStringList, format);
    QFETCH(QDate, issuanceDate);
    QFETCH(bool, isVAT);
    QFETCH(QString, invoiceNum);

    InvoiceNumberFormat_t fmt;
    foreach(const QString field, format)
    {
        fmt.append(InvoiceNumberFormat_t::FieldID(field), s);
    }

    QCOMPARE(model_->simulateConsecutiveInvoiceNumbers(fmt, issuanceDate,
                                                                  isVAT ? InvoiceTypeData::VAT: InvoiceTypeData::PRO_FORMA, 1).at(0), invoiceNum);
}


void ModelInvoiceTest::test_compatibilityWithOldGenerateInvoiceNumber_data()
{
    QTest::addColumn<QStringList>("format");
    QTest::addColumn<QDate>("issuanceDate");
    QTest::addColumn<bool>("isVAT");
    QTest::addColumn<QString>("invoiceNum");

    for(unsigned int state = 0; state < 32; ++state)//32 is a number of all possible combinations of input of generateInvoiceNumberOldVer()
    {
        InvoiceTypeData::Type invType = ( state & 16 ? InvoiceTypeData::VAT : InvoiceTypeData::PRO_FORMA);
        bool day = (state & 8);
        bool month = (state & 4);
        bool year = (state & 2);
        bool shortYear = (state & 1);
        QStringList formatList;
        formatList.append(InvoiceNumberFormat_t::FieldName(InvoiceNumberFormat_t::NR));
        if(day)
        {
            formatList.append(InvoiceNumberFormat_t::FieldName(InvoiceNumberFormat_t::SLASH));
            formatList.append(InvoiceNumberFormat_t::FieldName(InvoiceNumberFormat_t::PERIOD_DAY));
        }

        if(month)
        {
            formatList.append(InvoiceNumberFormat_t::FieldName(InvoiceNumberFormat_t::SLASH));
            formatList.append(InvoiceNumberFormat_t::FieldName(InvoiceNumberFormat_t::PERIOD_MONTH));
        }

        if(year)
        {
            formatList.append(InvoiceNumberFormat_t::FieldName(InvoiceNumberFormat_t::SLASH));
            if(shortYear)
                formatList.append(InvoiceNumberFormat_t::FieldName(InvoiceNumberFormat_t::PERIOD_SHORT_YEAR));
            else
                formatList.append(InvoiceNumberFormat_t::FieldName(InvoiceNumberFormat_t::PERIOD_YEAR));
        }

        QTest::newRow(qPrintable(QString("%1%2%3%4%5")
                                 .arg(int(invType == InvoiceTypeData::VAT))
                                 .arg((int)day)
                                 .arg((int)month)
                                 .arg((int)year)
                                 .arg((int)shortYear)))
                << formatList << QDate::currentDate() << (invType == InvoiceTypeData::VAT)
                                                << generateInvoiceNumberOldVer(invType, day, month, year, shortYear);
    }
}


QString ModelInvoiceTest::generateInvoiceNumberOldVer(InvoiceTypeData::Type invType, bool day, bool month, bool year, bool shortYear)
{//old code - for checking compatibility with previous versions
    QString tmp, prefix, suffix;
    SettingsGlobal s;

    if(invType == InvoiceTypeData::PRO_FORMA)
    {
        tmp = s.value("fpro").toString();
    }
    else
    {
        tmp = s.value("fvat").toString();
    }

    prefix = s.value("prefix").toString(); //empty by default

    QStringList one1 = tmp.split("/");
    one1[0] = one1[0].remove(prefix);

    int nr = one1[0].toInt() + 1;
    QString lastInvoice = prefix + numbersCount(nr, 0);

    if(day)//if (s.value("day") .toBool())
        lastInvoice += "/" + QDate::currentDate().toString("dd");

    if(month)//if (sett().value("month") .toBool())
        lastInvoice += "/" + QDate::currentDate().toString("MM");

    if(year) {//if (s.value("year") .toBool()) {
        if(shortYear) //if (!s.value("shortYear") .toBool())
            lastInvoice += "/" + QDate::currentDate().toString("yy");
        else
            lastInvoice += "/" + QDate::currentDate().toString("yyyy");
    }
    suffix = s.value("sufix").toString(); //empty by default
    lastInvoice += suffix;

    return lastInvoice;
}


QString ModelInvoiceTest::numbersCount(int in, int x) {//old code - for checking compatibility with previous versions
    SettingsGlobal s;
    QString tmp2, tmp = s.numberToString(in);
    tmp2 = "";
    int incr = x - tmp.length();
    for (int i = 0; i < incr; ++i)
        tmp2 += "0";
    return tmp2 + tmp;
}

QTEST_MAIN(ModelInvoiceTest)

#include "tst_modelinvoicetest.moc"
