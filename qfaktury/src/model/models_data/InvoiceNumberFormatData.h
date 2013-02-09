#ifndef INVOICENUMBERFORMATDATA_H
#define INVOICENUMBERFORMATDATA_H

#include <QStringList>
#include <QDebug>
#include <SettingsGlobal.h>

struct InvoiceNumberFormatData
{
    enum Field {NR, NR_Y, NR_M, NR_D, NR_Q, INVOICE_TYPE, TEXT1, TEXT2, TEXT3,
                PERIOD_YEAR, PERIOD_MONTH, PERIOD_DAY, PERIOD_QUARTER};
    enum Separator {SLASH = PERIOD_QUARTER + 1, BACKSLASH, HYPHEN};

    static QString FieldName(const int field)
    {
        switch(field)
        {
        case NR: return QObject::trUtf8("{NR}");
        case NR_Y: return QObject::trUtf8("{NR_R}");
        case NR_M: return QObject::trUtf8("{NR_M}");
        case NR_D: return QObject::trUtf8("{NR_D}");
        case NR_Q: return QObject::trUtf8("{NR_K}");
        case INVOICE_TYPE: return QObject::trUtf8("{TYP_FAKTURY}");
        case TEXT1: return QObject::trUtf8("{TEKST1}");
        case TEXT2: return QObject::trUtf8("{TEKST2}");
        case TEXT3: return QObject::trUtf8("{TEKST3}");
        case PERIOD_YEAR: return QObject::trUtf8("{R}");
        case PERIOD_MONTH: return QObject::trUtf8("{M}");
        case PERIOD_DAY: return QObject::trUtf8("{D}");
        case PERIOD_QUARTER: return QObject::trUtf8("{K}");
        case SLASH: return QString("/");
        case BACKSLASH: return QString("\\");
        case HYPHEN: return QString("-");
        default:
            qDebug() << QString("Undefined field in InvNumFormatData::FieldName: field=%1").arg((int)field);
            return QString();
        }
    }

    static int FieldID(const QString &field)
    {
        int ret = -1;
        for(int i = NR; i <= HYPHEN; ++i)
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
        case NR_Q: return QObject::trUtf8("Nr kolejny w kwartale");
        case INVOICE_TYPE: return QObject::trUtf8("Typ faktury");
        case TEXT1: return QObject::trUtf8("Tekst 1");
        case TEXT2: return QObject::trUtf8("Tekst 2");
        case TEXT3: return QObject::trUtf8("Tekst 3");
        case PERIOD_YEAR: return QObject::trUtf8("Rok");
        case PERIOD_MONTH: return QObject::trUtf8("Miesiąc");
        case PERIOD_DAY: return QObject::trUtf8("Dzień");
        case PERIOD_QUARTER: return QObject::trUtf8("Kwartał");
        case SLASH: return QObject::trUtf8("Separator - prawy ukośnik");
        case BACKSLASH: return QObject::trUtf8("Separator - lewy ukośnik");
        case HYPHEN: return QObject::trUtf8("Separator - łącznik");
        default:
            qDebug() << QString("Undefined field in InvNumFormatData::FieldDescription: field=%1").arg((int)field);
            return QString();
        }
    }

    static QString toRegexp(const QString& format)
    {
        QString ret;
        SettingsGlobal s;

        const QVector<int> tokens(Parse(format));
        foreach(int field, tokens)
        {
            switch(field)
            {
            case NR:
            case NR_Y:
            case NR_M:
            case NR_D:
            case NR_Q:
                ret += QString("(\\d+)");
                break;
            case INVOICE_TYPE:
                ret += QString("(\\w+)");
                break;
            case TEXT1:
                ret += QString("(%1)").arg(s.value(s.keyName(s.TEXT1)).toString());
                break;
            case TEXT2:
                ret += QString("(%1)").arg(s.value(s.keyName(s.TEXT2)).toString());
                break;
            case TEXT3:
                ret += QString("(%1)").arg(s.value(s.keyName(s.TEXT3)).toString());
                break;
            case PERIOD_YEAR:
                ret += QString("(\\d{4})");
                break;
            case PERIOD_MONTH:
                ret += QString("(\\d\\d?)");
                break;
            case PERIOD_DAY:
                ret += QString("(\\d\\d?)");
                break;
            case PERIOD_QUARTER:
                ret += QString("(\\d\\d?)");
                break;
            case SLASH:
                ret += QString("(/)");
                break;
            case BACKSLASH:
                ret += QString("(\\\\)");
                break;
            case HYPHEN:
                ret += QString("(-)");
                break;
            default:
                qDebug() << QString("Undefined field in InvNumFormatData::toRegexp: field=%1").arg((int)field);
                return QString();
            }
        }

        return ret;
    }

    static QVector<int> Parse(const QString &format)
    {
        int from = 0, to = 0;
        const QChar left('{'), right('}');
        QVector<int> ret;

        if(!format.isEmpty())
        {
            while( (from = format.indexOf(left, from)) != -1)
            {
                if( (to = format.indexOf(right, from)) != -1)
                {
                    ret.append(FieldID(format.mid(from, to - from + 1)));
                    from = to + 1;
                    if(from >= format.count())
                        break;
                }
                else
                {
                    break;
                }

                if(format.at(from) != left)
                {//it should be a separator now. If not, then it's an error
                    to = format.indexOf(left, from);
                    if(to == -1)
                    {
                        to = format.count() - 1;
                    }

                    const QString separator(format.at(from));
                    for(int i = InvoiceNumberFormatData::SLASH; i <= InvoiceNumberFormatData::HYPHEN; ++i)
                    {
                        if(separator == FieldName(i))
                        {
                            ret.append(i);
                            break;
                        }
                    }
                }
            }
        }
        return ret;
    }
};

#endif
