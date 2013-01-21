#ifndef INVOICENUMBERFORMATDATA_H
#define INVOICENUMBERFORMATDATA_H

#include <QStringList>
#include <QDebug>

struct InvoiceNumberFormatData
{
    enum ChosenPeriod {WEEK, MONTH, QUARTER, YEAR, WHOLE_LIFE};
    enum Separator {SLASH, BACKSLASH, HYPHEN};
    enum Field {NUMBER, INVOICE_TYPE, TEXT1, TEXT2, TEXT3, F_YEAR, F_MONTH, F_DAY, F_WEEK, F_QUARTER};

    static QString ChosenPeriodName(const int period)
    {
        switch(period)
        {
        case WEEK: return QObject::trUtf8("Tydzień");
        case MONTH: return QObject::trUtf8("Miesiąc");
        case QUARTER: return QObject::trUtf8("Kwartał");
        case YEAR: return QObject::trUtf8("Rok");
        case WHOLE_LIFE: return QObject::trUtf8("Cały okres działalności firmy");
        default:
            qDebug() << QString("Undefined period in InvNumFormatData::ChosenPeriodName: period=%1").arg((int)period);
            return QString();
        }
    }

    static QString SeparatorName(const int separator)
    {
        switch(separator)
        {
        case SLASH: return QObject::trUtf8("\\");
        case BACKSLASH: return QObject::trUtf8("/");
        case HYPHEN: return QObject::trUtf8("-");
        default:
            qDebug() << QString("Undefined period in InvNumFormatData::SeparatorName: separator=%1").arg((int)separator);
            return QString();
        }
    }

    static QString FieldName(const int field)
    {
        switch(field)
        {
        case NUMBER: return QObject::trUtf8("Numer kolejny w wybranym okresie");
        case INVOICE_TYPE: return QObject::trUtf8("Typ faktury");
        case TEXT1: return QObject::trUtf8("Tekst 1");
        case TEXT2: return QObject::trUtf8("Tekst 2");
        case TEXT3: return QObject::trUtf8("Tekst 3");
        case F_YEAR: return QObject::trUtf8("Rok");
        case F_MONTH: return QObject::trUtf8("Miesiąc");
        case F_DAY: return QObject::trUtf8("Dzień");
        case F_WEEK: return QObject::trUtf8("Tydzień");
        case F_QUARTER: return QObject::trUtf8("Kwartał");
        default:
            qDebug() << QString("Undefined period in InvNumFormatData::FieldName: field=%1").arg((int)field);
            return QString();
        }
    }
};

#endif
