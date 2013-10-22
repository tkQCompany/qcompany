#include <QStringList>

#include "Money_t.h"
#include "QDecDouble.hh"
#include "QDecPacked.hh"

Money_t::Money_t()
{
    SettingsGlobal s;
    currency_ = s.value(s.keyName(s.DEFAULT_CURRENCY)).value<CurrencyData::Currencies>();
    value_.fromInt32(0);
    context_.setDefault();
}


Money_t::Money_t(const QString &val)
{
    Money_t();
    value_.fromString(val.toLatin1(), &context_);
}

short Money_t::digit_(const QDecNumber &num, const int index) const
{
    return num.toString().at(index) - '0';
}


QString Money_t::verballyPL1_999(const QDecNumber &val) const
{
    QString ret;
    if(val >= QDecNumber(1) && val <= QDecNumber(999))
    {
        const QStringList pl100_900(QStringList() << "sto " << "dwieście " << "trzysta " << "czterysta " << "pięćset "
                                    << "sześćset " << "siedemset " << "osiemset " << "dziewięćset ");
        const QStringList pl20_90(QStringList() << "dwadzieścia " << "trzydzieści " << "czterdzieści " << "pięćdziesiąt "
                                  << "sześćdziesiąt " << "siedemdziesiąt " << "osiemdziesiąt " << "dziewięćdziesiąt ");
        const QStringList pl2_19(QStringList() << "jeden " << "dwa " << "trzy " << "cztery " << "pięć " << "sześć "
                                 << "siedem " << "osiem " << "dziewięć " << "dziesięć " << "jedenaście " << "dwanaście "
                                 << "trzynaście " << "czternaście " << "piętnaście " << "szesnaście " << "siedemnaście "
                                 << "osiemnaście " << "dziewiętnaście ");

        if(val >= QDecNumber(100))
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
        else if(val >= QDecNumber(10))
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
    if(value_ >= QDecNumber("1e21"))
    {
        return ret;
    }

    QDecNumber valTmp(value_), divisor("1e18"), divisionInt;

    if(valTmp >= divisor)
    {
        divisionInt = valTmp.divideInteger(divisor);
        if(divisionInt != QDecNumber(1))
        {
            const QDecNumber leftPart(divisionInt);
            ret += verballyPL1_999(leftPart);
            const short digit = digit_(leftPart, 2);
            if(digit >= 2 && digit <= 4)
            {
                ret += QString("tryliony ");
            }
            else
            {
                ret += QString("trylionów ");
            }
        }
        else
        {
            ret += QString("jeden trylion ");
        }

        valTmp %= divisor;
    }

    divisor /= QDecNumber(1000);

    if(valTmp >= divisor)
    {
        divisionInt = valTmp.divideInteger(divisor);
        if(divisionInt != QDecNumber(1))
        {
            const QDecNumber leftPart(divisionInt);
            ret += verballyPL1_999(leftPart);
            const short digit = digit_(leftPart, 2);
            if(digit >= 2 && digit <= 4)
            {
                ret += QString("biliardy ");
            }
            else
            {
                ret += QString("biliardów ");
            }
        }
        else
        {
            ret += QString("jeden biliard ");
        }
        valTmp %= divisor;
    }

    divisor /= QDecNumber(1000);

    if(valTmp >= divisor)
    {
        divisionInt = valTmp.divideInteger(divisor);
        if(divisionInt != QDecNumber(1))
        {
            const QDecNumber leftPart(divisionInt);
            ret += verballyPL1_999(leftPart);
            const short digit = digit_(leftPart, 2);
            if(digit >= 2 && digit <= 4)
            {
                ret += QString("biliony ");
            }
            else
            {
                ret += QString("bilionów ");
            }
        }
        else
        {
            ret += QString("jeden bilion ");
        }
        valTmp %= divisor;
    }

    divisor /= QDecNumber(1000);

    if(valTmp >= divisor)
    {
        divisionInt = valTmp.divideInteger(divisor);
        if(divisionInt != QDecNumber(1))
        {
            const QDecNumber leftPart(divisionInt);
            ret += verballyPL1_999(leftPart);
            const short digit = digit_(leftPart, 2);
            if(digit >= 2 && digit <= 4)
            {
                ret += QString("miliardy ");
            }
            else
            {
                ret += QString("miliardów ");
            }
        }
        else
        {
            ret += QString("jeden miliard ");
        }
        valTmp %= divisor;
    }

    divisor /= QDecNumber(1000);

    if(valTmp >= divisor)
    {
        divisionInt = valTmp.divideInteger(divisor);
        if(divisionInt != QDecNumber(1))
        {
            const QDecNumber leftPart(divisionInt);
            ret += verballyPL1_999(leftPart);
            const short digit = digit_(leftPart, 2);
            if(digit >= 2 && digit <= 4)
            {
                ret += QString("miliony ");
            }
            else
            {
                ret += QString("milionów ");
            }
        }
        else
        {
            ret += QString("jeden milion ");
        }
        valTmp %= divisor;
    }

    divisor /= QDecNumber(1000);

    if(valTmp >= divisor)
    {
        divisionInt = valTmp.divideInteger(divisor);
        if(divisionInt != QDecNumber(1))
        {
            const QDecNumber leftPart(divisionInt);
            ret += verballyPL1_999(leftPart);
            const short digit = digit_(leftPart, leftPart.toString().size() - 1);
            if(digit >= 2 && digit <= 4)
            {
                ret += QString("tysiące ");
            }
            else
            {
                ret += QString("tysięcy ");
            }
        }
        else
        {
            ret += QString("tysiąc ");
        }
        valTmp %= divisor;
    }

    divisor /= QDecNumber(1000);

    if(valTmp >= divisor)
    {
        divisionInt = valTmp.divideInteger(divisor);
        if(divisionInt != QDecNumber(1))
        {
            const QDecNumber leftPart(divisionInt);
            ret += verballyPL1_999(leftPart);
        }
        else
        {
            ret += QString("jeden ");
        }
        valTmp %= divisor;
    }

    const int radix = 10;
    const int fieldWidth = 2;
    const QChar fillChar(QChar('0'));
    QDecDouble dbl;
    dbl.fromQDecNumber(valTmp * QDecNumber(100));
    if(ret.isEmpty())
    {
        ret += "zero ";
    }

    switch(digit_(value_, value_.divideInteger(QDecNumber(1)).toString().size() - 1))
    {
        case 1:
        if((value_ % QDecNumber(1)) == QDecNumber(0))
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

    ret += QString("%1/100 %2").arg((int)dbl.toDouble(), fieldWidth, radix, fillChar)
            .arg(CurrencyData::codeName(currency_));

    return ret;
}
