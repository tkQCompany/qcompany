#ifndef INVOICENUMBERFORMATDATA_H
#define INVOICENUMBERFORMATDATA_H

#include <QStringList>
#include <QDebug>

struct InvoiceNumberFormatData
{
    enum Separator {SLASH, BACKSLASH, HYPHEN};
    enum Field {NR, NR_Y, NR_M, NR_D, NR_W, NR_Q, INVOICE_TYPE, TEXT1, TEXT2, TEXT3, PERIOD_YEAR, PERIOD_MONTH, PERIOD_DAY, PERIOD_WEEK, PERIOD_QUARTER};

    static QChar SeparatorName(const int separator)
    {
        switch(separator)
        {
        case SLASH: return QChar('\\');
        case BACKSLASH: return QChar('/');
        case HYPHEN: return QChar('-');
        default:
            qDebug() << QString("Undefined separator in InvNumFormatData::SeparatorName: separator=%1").arg((int)separator);
            return QChar();
        }
    }

    static QString FieldName(const int field)
    {
        switch(field)
        {
        case NR: return QObject::trUtf8("{NR}");
        case NR_Y: return QObject::trUtf8("{NR_R}");
        case NR_M: return QObject::trUtf8("{NR_M}");
        case NR_D: return QObject::trUtf8("{NR_D}");
        case NR_W: return QObject::trUtf8("{NR_T}");
        case NR_Q: return QObject::trUtf8("{NR_K}");
        case INVOICE_TYPE: return QObject::trUtf8("{TYP_FAKTURY}");
        case TEXT1: return QObject::trUtf8("{TEKST1}");
        case TEXT2: return QObject::trUtf8("{TEKST2}");
        case TEXT3: return QObject::trUtf8("{TEKST3}");
        case PERIOD_YEAR: return QObject::trUtf8("{R}");
        case PERIOD_MONTH: return QObject::trUtf8("{M}");
        case PERIOD_DAY: return QObject::trUtf8("{D}");
        case PERIOD_WEEK: return QObject::trUtf8("{T}");
        case PERIOD_QUARTER: return QObject::trUtf8("{K}");
        default:
            qDebug() << QString("Undefined field in InvNumFormatData::FieldName: field=%1").arg((int)field);
            return QString();
        }
    }

    static int FieldID(const QString &field)
    {
        int ret = -1;
        for(int i = NR; i <= PERIOD_QUARTER; ++i)
        {
            if(field.compare(FieldName(i)) == 0)
            {
                ret = i;
                break;
            }
        }
        return ret;
    }



    static QString FieldDescription(const int field)
    {
        switch(field)
        {
        case NR: return QObject::trUtf8("Nr kolejny w całym okresie działania firmy");
        case NR_Y: return QObject::trUtf8("Nr kolejny w roku");
        case NR_M: return QObject::trUtf8("Nr kolejny w miesiącu");
        case NR_D: return QObject::trUtf8("Nr kolejny w dniu");
        case NR_W: return QObject::trUtf8("Nr kolejny w tygodniu");
        case NR_Q: return QObject::trUtf8("Nr kolejny w kwartale");
        case INVOICE_TYPE: return QObject::trUtf8("Typ faktury");
        case TEXT1: return QObject::trUtf8("Tekst 1");
        case TEXT2: return QObject::trUtf8("Tekst 2");
        case TEXT3: return QObject::trUtf8("Tekst 3");
        case PERIOD_YEAR: return QObject::trUtf8("Rok");
        case PERIOD_MONTH: return QObject::trUtf8("Miesiąc");
        case PERIOD_DAY: return QObject::trUtf8("Dzień");
        case PERIOD_WEEK: return QObject::trUtf8("Tydzień");
        case PERIOD_QUARTER: return QObject::trUtf8("Kwartał");
        default:
            qDebug() << QString("Undefined field in InvNumFormatData::FieldDescription: field=%1").arg((int)field);
            return QString();
        }
    }
};

#endif
