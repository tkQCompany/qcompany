#include <QStringList>

#include "Money_t.h"
#include "SettingsGlobal.h"


Money_t::Money_t()
{
    QLocale lc;
    lc.setNumberOptions(QLocale::OmitGroupSeparator);
    init_(lc.toString(0.0, 'f', 2), true);
}

Money_t::Money_t(const int val)
{
    QLocale lc;
    lc.setNumberOptions(QLocale::OmitGroupSeparator);
    init_(lc.toString(val), true);
}


Money_t::Money_t(const QString &val)
{
    init_(val, true);
}


void Money_t::setValue(const int val)
{
    QLocale lc;
    lc.setNumberOptions(QLocale::OmitGroupSeparator);
    init_(lc.toString(val), false);
}


void Money_t::setValue(const QString &val)
{
    init_(val, false);
}


void Money_t::init_(const QString &val, const bool setDefaultCurrency)
{
    qRegisterMetaTypeStreamOperators<Money_t>("Money_t");
    if(setDefaultCurrency)
        setDefaultCurrency_();

    value_ = DecVal(val);
}


Money_t& Money_t::operator+=(const Money_t &rhs)
{
    if(currency_ == rhs.currency_)
        value_ += rhs.value_;
    return *this;
}


const Money_t Money_t::operator+ (const Money_t &rhs) const
{
    Money_t ret(*this);
    ret += rhs; //Money_t::operator+=()
    return ret;
}


Money_t& Money_t::operator*=(const DecVal &rhs)
{
    value_ *= rhs;
    return *this;
}


const Money_t  Money_t::operator* (const DecVal &rhs) const
{
    Money_t ret(*this);
    ret *= rhs; //Money_t::operator*=()
    return ret;
}


Money_t& Money_t::operator-=(const Money_t &rhs)
{
    if(currency_ == rhs.currency_)
        value_ -= rhs.value_;
    return *this;
}


const Money_t  Money_t::operator- (const Money_t &rhs) const
{
    Money_t ret(*this);
    ret -= rhs; //Money_t::operator-=()
    return ret;
}


Money_t& Money_t::operator/=(const DecVal &rhs)
{
    value_ /= rhs;
    return *this;
}


const Money_t  Money_t::operator/ (const DecVal &rhs) const
{
    Money_t ret(*this);
    ret /= rhs; //Money_t::operator/=()
    return ret;
}


bool Money_t::operator<(const Money_t &rhs) const
{
    return value_ < rhs.value_;
}


bool Money_t::operator>(const Money_t &rhs) const
{
    return value_ > rhs.value_;
}


bool Money_t::operator<=(const Money_t &rhs) const
{
    return value_ <= rhs.value_;
}


bool Money_t::operator>=(const Money_t &rhs) const
{
    return value_ >= rhs.value_;
}


short Money_t::digit_(const DecVal &num, const int index) const
{
    const QString str(DecVal::removeTrailingZeros(num.toString()));
    const short ret = (str.size() > index)? str.at(index).toLatin1() - '0' : 0;
    return ret;
}



void Money_t::setDefaultCurrency_()
{
    SettingsGlobal s;
    currency_ = (CurrencyData::Currencies)s.value(s.DEFAULT_CURRENCY).toInt();
}


QString Money_t::verballyPL1_999(const DecVal &val) const
{
    QString ret;
    static const QStringList pl100_900(QStringList() << "sto " << "dwieście " << "trzysta " << "czterysta " << "pięćset "
                                << "sześćset " << "siedemset " << "osiemset " << "dziewięćset ");
    static const QStringList pl20_90(QStringList() << "dwadzieścia " << "trzydzieści " << "czterdzieści " << "pięćdziesiąt "
                              << "sześćdziesiąt " << "siedemdziesiąt " << "osiemdziesiąt " << "dziewięćdziesiąt ");
    static const QStringList pl2_19(QStringList() << "jeden " << "dwa " << "trzy " << "cztery " << "pięć " << "sześć "
                             << "siedem " << "osiem " << "dziewięć " << "dziesięć " << "jedenaście " << "dwanaście "
                             << "trzynaście " << "czternaście " << "piętnaście " << "szesnaście " << "siedemnaście "
                             << "osiemnaście " << "dziewiętnaście ");
    if(val >= DecVal(1) && val <= DecVal(999))
    {
        if(val >= DecVal(100))
        {
            const short dozens = digit_(val, 1);
            const short ones = digit_(val, 2);
            ret += pl100_900.at(digit_(val, 0) - 1);
            if(dozens >= 2)
            {
                ret += pl20_90.at(dozens - 2);
                if(ones != 0)
                {
                    ret += pl2_19.at(ones - 1);
                }
            } else if(dozens != 0 || ones != 0)
            {
                ret += pl2_19.at(10 * dozens + ones - 1);
            }
        }
        else if(val >= DecVal(10))
        {
            const short dozens = digit_(val, 0);
            const short ones = digit_(val, 1);
            if(dozens >= 2)
            {
                ret += pl20_90.at(dozens - 2);
                if(ones != 0)
                {
                    ret += pl2_19.at(ones - 1);
                }
            }
            else
            {
                ret += pl2_19.at(10 * dozens + ones - 1);
            }
        }
        else
        {
            const short ones = digit_(val, 0);
            ret += pl2_19.at(ones - 1);
        }
    }

    return ret;
}


/**
 * @brief Changes stored decimal value to its polish verbal form (with code of the current currency). Rules (in polish): http://obcyjezykpolski.strefa.pl/?md=archive&id=190
 * @return Polish verbal form of a stored decimal value (with currency's code)
 */
QString Money_t::verballyPL() const
{
    QString ret;
    if(value_ >= DecVal("1000000000000000000000"))
    {
        return ret;
    }

    const size_t unitsSize = 6;
    struct
    {
        QString name1;
        QString name2;
        QString name3;
    } units[unitsSize];

    units[0].name1 = QString("tryliony ");
    units[0].name2 = QString("trylionów ");
    units[0].name3 = QString("jeden trylion ");

    units[1].name1 = QString("biliardy ");
    units[1].name2 = QString("biliardów ");
    units[1].name3 = QString("jeden biliard ");

    units[2].name1 = QString("biliony ");
    units[2].name2 = QString("bilionów ");
    units[2].name3 = QString("jeden bilion ");

    units[3].name1 = QString("miliardy ");
    units[3].name2 = QString("miliardów ");
    units[3].name3 = QString("jeden miliard ");

    units[4].name1 = QString("miliony ");
    units[4].name2 = QString("milionów ");
    units[4].name3 = QString("jeden milion ");

    units[5].name1 = QString("tysiące ");
    units[5].name2 = QString("tysięcy ");
    units[5].name3 = QString("tysiąc ");

    DecVal divisorInt("1000000000000000000"), divisionInt, valTmp(value_);

    for(size_t i = 0; i < unitsSize; ++i, divisorInt /= DecVal(1000))
    {
        if(valTmp >= divisorInt)
        {
            divisionInt = (valTmp / divisorInt).integerPart();
            if(divisionInt != DecVal(1))
            {
                const QString leftPartStr(DecVal::removeTrailingZeros(divisionInt.toString()));
                const DecVal leftPart(leftPartStr);
                ret += verballyPL1_999(leftPart);
                const short digit = digit_(leftPart, leftPartStr.size() - 1);
                if(digit >= 2 && digit <= 4)
                {
                    ret += units[i].name1;
                }
                else
                {
                    ret += units[i].name2;
                }
            }
            else
            {
                ret += units[i].name3;
            }

            valTmp = valTmp % divisorInt;
        }
    }

    if(valTmp >= divisorInt) //if valTmp >= 1 && < 1000
    {
        divisionInt = (valTmp / divisorInt).integerPart();
        if(divisionInt != DecVal(1))
        {
            const DecVal leftPart(qPrintable(divisionInt.toString()));
            ret += verballyPL1_999(leftPart);
        }
        else
        {
            ret += QString("jeden ");
        }
        valTmp = valTmp % divisorInt;
    }



    if(ret.isEmpty())
    {
        ret += "zero ";
    }

    const DecVal lastDigit(value_ % DecVal(10));
    switch((int)lastDigit.toDouble())
    {
        case 1:
        if(int(value_.toDouble()) == 1)
        {
            ret += "złoty ";
        }
        else
        {
            ret += "złotych ";
        }
        break;
    case 2:
    case 3:
    case 4:
        ret += "złote ";
        break;
    default:
        ret += "złotych ";
    }

    const int fieldWidth = 2;
    const char format = 'f';
    const int precision = 0;
    const QChar fillChar(QChar('0'));

    const DecVal valDecimal100(DecVal(100) * value_.decimalPart()); //100 * decimal part
    ret += QString("%1/100 %2").arg(valDecimal100.toDouble(), fieldWidth, format, precision, fillChar)
            .arg(CurrencyData::codeName(currency_));

    return ret;
}


QString Money_t::toString(const int digits) const
{
    return value_.toString(digits);
}
