#include <QLocale>

#include "DecVal.h"
#include "SettingsGlobal.h"

DecVal::DecVal(const double val): val_(val)
{
}


DecVal::DecVal(const QString valStr)
{
    const int base = 10;
    mpf_class valTmp;
    valTmp.set_str(valStr.toStdString(), base);
    val_ = valTmp;
    val_.canonicalize();
}


DecVal DecVal::decimalPart() const
{
    mpf_class valInt(val_);
    valInt = trunc(valInt);
    const mpf_class valFull(val_);
    const mpf_class valDecim(valFull - valInt);
    return DecVal(valDecim.get_d());
}


DecVal DecVal::integerPart() const
{
    return *this - decimalPart();
}


QString DecVal::removeTrailingZeros(const QString &numberStr)
{
    QString ret(numberStr);
    while(ret.endsWith(QChar('0')))
    {
          ret = ret.remove(ret.size() - 1, 1);
    }

    SettingsGlobal s;
    const QChar decimalPointChar(s.decimalPoint());
    if(ret.endsWith(decimalPointChar))
    {
        ret = ret.remove(ret.size() - 1, 1);
    }

    return ret;
}


double DecVal::toDouble() const
{
    return val_.get_d();
}


QString DecVal::toString(const int digits) const
{
    QLocale lc;
    lc.setNumberOptions(QLocale::OmitGroupSeparator);
    return lc.toString(val_.get_d(), 'f', digits);
}


DecVal& DecVal::operator+=(const DecVal &rhs)
{
    val_ += rhs.val_;
    return *this;
}


const DecVal  DecVal::operator+ (const DecVal &rhs) const
{
    DecVal ret(*this);
    ret += rhs;
    return ret;
}


DecVal& DecVal::operator*=(const DecVal &rhs)
{
    val_ *= rhs.val_;
    return *this;
}


const DecVal  DecVal::operator* (const DecVal &rhs) const
{
    DecVal ret(*this);
    ret *= rhs;
    return ret;
}


DecVal& DecVal::operator-=(const DecVal &rhs)
{
    val_ -= rhs.val_;
    return *this;
}


const DecVal  DecVal::operator- (const DecVal &rhs) const
{
    DecVal ret(*this);
    ret -= rhs;
    return ret;
}


const DecVal  DecVal::operator- () const
{
    DecVal ret(*this);
    ret.val_ = -ret.val_;
    return ret;
}


DecVal& DecVal::operator/=(const DecVal &rhs)
{
    val_ /= rhs.val_;
    return *this;
}


const DecVal  DecVal::operator/ (const DecVal &rhs) const
{
    DecVal ret(*this);
    ret /= rhs;
    return ret;
}


const DecVal  DecVal::operator% (const DecVal &rhs) const
{
    DecVal ret(*this);
    const mpf_class retF(ret.val_);
    const mpf_class rhsF(rhs.val_);
    mpz_class retZ(retF);
    const mpz_class rhsZ(rhsF);

    retZ %= rhsZ;
    ret.val_ = mpq_class(retZ);
    return ret;
}


bool DecVal::operator<(const DecVal &rhs) const
{
    return val_ < rhs.val_;
}


bool DecVal::operator>(const DecVal &rhs) const
{
    return val_ > rhs.val_;
}


bool DecVal::operator<=(const DecVal &rhs) const
{
    return val_ <= rhs.val_;
}

bool DecVal::operator>=(const DecVal &rhs) const
{
    return val_ >= rhs.val_;
}
