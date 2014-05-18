#ifndef DECVAL_H
#define DECVAL_H

#include <QString>
#include <QMetaType>

#include <gmpxx.h>

class DecVal;

inline bool operator==(const DecVal &lhs, const DecVal &rhs);
inline bool operator!=(const DecVal &lhs, const DecVal &rhs);

class DecVal
{
    friend bool operator==(const DecVal &lhs, const DecVal &rhs);
    friend bool operator!=(const DecVal &lhs, const DecVal &rhs);
public:
    explicit DecVal(const double val = 0.0);
    explicit DecVal(const QString valStr);

    DecVal decimalPart() const;
    DecVal integerPart() const;
    static QString removeTrailingZeros(const QString &numberStr);
    double toDouble() const;
    QString toString(const int digits = 6) const;

    DecVal& operator+=(const DecVal &rhs);
    const DecVal  operator+ (const DecVal &rhs) const;
    DecVal& operator*=(const DecVal &rhs);
    const DecVal  operator* (const DecVal &rhs) const;
    DecVal& operator-=(const DecVal &rhs);
    const DecVal  operator- (const DecVal &rhs) const;
    const DecVal  operator- () const;
    DecVal& operator/=(const DecVal &rhs);
    const DecVal  operator/ (const DecVal &rhs) const;
    const DecVal  operator% (const DecVal &rhs) const;

    bool operator<(const DecVal &rhs) const;
    bool operator>(const DecVal &rhs) const;
    bool operator<=(const DecVal &rhs) const;
    bool operator>=(const DecVal &rhs) const;

private:
    mpq_class val_;
};


inline bool operator==(const DecVal &lhs, const DecVal &rhs)
{
    return (lhs.val_ == rhs.val_);
}


inline bool operator!=(const DecVal &lhs, const DecVal &rhs)
{
    return !(lhs == rhs);
}


Q_DECLARE_METATYPE(DecVal)

#endif // DECVAL_H
