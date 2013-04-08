/*
 *
 * ConvertAmount.cpp
 *
 *  Created on: Mar 16, 2009
 *      Author: tpielech
 */
#include <QStringList>

#include "ConvertAmount.h"
#include "SettingsGlobal.h"

QString ConvertAmount::convertPL(QString input, const QString &currency)
{
    SettingsGlobal s;

    QString in(input.remove(s.tPointStr()));
    const QString tmp(in.remove(in.indexOf(s.decimalPointStr()), 3));

    QString hundreds(tmp.right(3));
    QString thousands;
    if( (tmp.length() >= 4) && (tmp.length() <= 6))
    {
        thousands = tmp.left(tmp.length() - 3);
    }

    const QStringList ones(QStringList() << QObject::trUtf8("jeden") << QObject::trUtf8("dwa")
            << QObject::trUtf8("trzy") << QObject::trUtf8("cztery")
            << QObject::trUtf8("pięć") << QObject::trUtf8("sześć")
            << QObject::trUtf8("siedem") << QObject::trUtf8("osiem")
            << QObject::trUtf8("dziewięć"));

    const QStringList dozens(QStringList() << QObject::trUtf8("jedenaście") << QObject::trUtf8("dwanaście")
           << QObject::trUtf8("trzynaście") << QObject::trUtf8("czternaście")
           << QObject::trUtf8("piętnaście") << QObject::trUtf8("szesnaście")
           << QObject::trUtf8("siedemnaście")
           << QObject::trUtf8("osiemnaście") << QObject::trUtf8("dziewiętnaście")
                             << QObject::trUtf8("dziesięć"));

    const QStringList tens(QStringList() << QObject::trUtf8("dziesięć")
                           << QObject::trUtf8("dwadzieścia")
                           << QObject::trUtf8("trzydzieści")
          << QObject::trUtf8("czterdzieści")
          << QObject::trUtf8("pięćdziesiąt")
          << QObject::trUtf8("sześdziesiąt") << QObject::trUtf8(
                 "siedemdziesiąt") << QObject::trUtf8("osiemdziesiąt")
          << QObject::trUtf8("dziewięćdziesiąt"));

    const QStringList hundredsNames(QStringList() << QObject::trUtf8("sto") << QObject::trUtf8("dwieście")
        << QObject::trUtf8("trzysta") << QObject::trUtf8("czterysta")
        << QObject::trUtf8("pięćset") << QObject::trUtf8("sześćset")
        << QObject::trUtf8("siedemset") << QObject::trUtf8("osiemset")
        << QObject::trUtf8("dziewięćset"));

    // ******************************************************************
    while (thousands.length() < 3)
        thousands = QChar('0') + thousands;

    QString out;
    const QChar space(' ');
    if(0 != thousands.left(1).toInt())
    {
        out += space + hundredsNames[thousands.left(1).toInt() - 1];
    }

    const int mid1000 = thousands.mid(1, 1).toInt();
    const int digit = thousands.right(1).toInt();
    if (mid1000 == 1)
    {
        if(digit == 0)
        {
            out += space + dozens[digit - 1];
        }
        else
        {
            out += space + dozens[9];
        }
    }
    else
    {
        if((mid1000 >= 2) && (mid1000 <= 9))
        {
            out += space + tens[mid1000 - 2];
        }
        if((digit >= 1) && (digit <= 9))
        {
            out += QObject::trUtf8(" ") + ones[digit - 1];
        }
    }

    if (out.length() > 2)
        out += QObject::trUtf8(" tys.");

    //***************************************************************
    while (hundreds.length() < 3)
        hundreds.prepend(QChar('0'));

    if(0 != hundreds.left(1).toInt())
    {
        out += space + hundredsNames[hundreds.left(1).toInt()];
    }

    const int mid100 = hundreds.mid(1, 1).toInt();
    if(mid100 != 0)
    {
        out += space + tens[mid100 - 2]; //było mid100 - 2
    }

    const int right100 = hundreds.right(1).toInt();
    if(mid100 == 1)
    {
        if(right100 == 0)
        {
            out += space + dozens[9];
        }
        else
        {
            out += space + dozens[right100 - 1];
        }
    }
    else
    {
        if(right100 == 0)
        {
            out += space;
        }
        else
        {
            out += space + ones[right100 - 1];
        }
    }

    const QString fraction = input.remove(0, input.indexOf(s.decimalPointStr()) + 1);
    return (out + space + fraction + QObject::trUtf8("/100 ") + currency).trimmed();
}
