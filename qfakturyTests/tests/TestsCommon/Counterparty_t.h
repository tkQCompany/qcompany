#ifndef COUNTERPARTY_T_H
#define COUNTERPARTY_T_H

#include <QString>
#include <QMetaType>

struct Counterparty_t
{
    QString name;
    QString type;
    QString country;
    QString city;
    QString postalCode;
    QString street;
    QString taxID;
    QString accountNumber;
    QString www;
    QString email;
    QString phone;
    QString invNumFormat;
};

Q_DECLARE_METATYPE(Counterparty_t)

#endif // COUNTERPARTY_T_H
