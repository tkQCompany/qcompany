#include <QStringList>
#include <QDebug>

#include "Money_t.h"
#include "SettingsGlobal.h"


Money_t::Money_t()
{
    SettingsGlobal s;
    init_(s.numberToString(0.0, 'g', 2), true);
}

Money_t::Money_t(const double val)
{
    SettingsGlobal s;
    init_(s.numberToString(val, 'g', 2), true);
}


Money_t::Money_t(const QString &val)
{
    init_(val, true);
}


void Money_t::setValue(const double val)
{
    SettingsGlobal s;
    init_(s.numberToString(val, 'g', 2), false);
}


void Money_t::setValue(const QString &val)
{
    init_(val, false);
}


Money_t& Money_t::operator+=(const Money_t &rhs)
{
    if(currency_ == rhs.currency_)
        value_ += rhs.value_;
    return *this;
}

Money_t Money_t::operator+ (const Money_t &rhs)
{
    Money_t ret(*this);
    ret += rhs; //Money_t::operator+=()
    return ret;
}


Money_t& Money_t::operator*=(const double rhs)
{
    value_ *= rhs;
    return *this;
}


Money_t  Money_t::operator* (const double rhs)
{
    Money_t ret(*this);
    ret *= rhs; //Money_t::operator*=()
    return ret;
}


short Money_t::digit_(const val_t &num, const size_t index) const
{
    const std::string str(num.get_str());
    const short ret = (str.size() > index)? str.at(index) - '0' : 0;
    return ret;
}

void Money_t::init_(const QString &val, const bool setDefaultCurrency)
{
    const int base = 10;
    if(setDefaultCurrency)
        setDefaultCurrency_();

    mpf_class valTmp;
    valTmp.set_str(val.toStdString(), base);

    value_ = valTmp;
    value_.canonicalize();
}

void Money_t::setDefaultCurrency_()
{
    SettingsGlobal s;
    currency_ = (CurrencyData::Currencies)s.value(s.keyName(s.DEFAULT_CURRENCY)).toInt();
}


QString Money_t::verballyPL1_999(const val_t &val) const
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
    if(val >= val_t(1) && val <= val_t(999))
    {
        if(val >= val_t(100))
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
        else if(val >= val_t(10))
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
    if(value_ >= val_t("1000000000000000000000"))
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

    mpz_class divisorInt("1000000000000000000"), divisionInt, valTmp(value_);
    const mpq_class valDec(100 * (value_ - mpq_class(valTmp))); //decimal part

    for(size_t i = 0; i < unitsSize; ++i, divisorInt /= 1000)
    {
        if(valTmp >= divisorInt)
        {
            divisionInt = trunc(mpf_class((valTmp / divisorInt)));
            if(divisionInt != 1)
            {
                const val_t leftPart(divisionInt);
                ret += verballyPL1_999(leftPart);
                const short digit = digit_(leftPart, leftPart.get_str().size() - 1);
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

            valTmp = mpz_class(valTmp) % divisorInt;
        }
    }

    if(valTmp >= divisorInt) //if valTmp >= 1 && < 1000
    {
        divisionInt = trunc(mpf_class((valTmp / divisorInt)));
        if(divisionInt != 1)
        {
            const val_t leftPart(divisionInt);
            ret += verballyPL1_999(leftPart);
        }
        else
        {
            ret += QString("jeden ");
        }
        valTmp = mpz_class(valTmp) % divisorInt;
    }



    if(ret.isEmpty())
    {
        ret += "zero ";
    }

    mpz_class lastDigit(value_);
    lastDigit %= 10;
    switch(lastDigit.get_ui())
    {
        case 1:
        if(mpz_class(value_) == 1)
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

    ret += QString("%1/100 %2").arg(valDec.get_d(), fieldWidth, format, precision, fillChar)
            .arg(CurrencyData::codeName(currency_));

    return ret;
}


QString Money_t::toString() const
{
    SettingsGlobal s;
    return s.numberToString(value_.get_d(), 'f', 2);
}
