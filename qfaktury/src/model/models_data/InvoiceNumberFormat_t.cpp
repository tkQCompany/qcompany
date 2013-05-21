#include <QDebug>

#include "InvoiceNumberFormat_t.h"

InvoiceNumberFormat_t::InvoiceNumberFormat_t(QObject *parent) : QObject(parent)
{
}


bool InvoiceNumberFormat_t::append(const Field field, const SettingsGlobal &s)
{
    if(!list_.isEmpty())
    {
        const Field prevField = list_.at(list_.size() - 1).first;
        if( ((field < (Field)SEPARATOR_START) || (field > (Field)SEPARATOR_END)) && ((prevField < (Field)SEPARATOR_START) || (prevField > (Field)SEPARATOR_END)))
        {
            qDebug() << "InvoiceNumberFormat_t::append() can't allow two non-separate fields in a row. Field1: "
                     << FieldName(prevField) << ", field2: " << FieldName(field);
            return false;
        }
    }

    const QString regexpStr(fieldToRegexpStr(field, s));
    list_.append(std::make_pair<pairFirstType_, pairSecondType_>(field, regexpStr));
    regexp_.setPattern(regexp_.pattern() + regexpStr);
    return true;
}


QString InvoiceNumberFormat_t::cap(const QString &strIn, const int position) const
{
    regexp_.indexIn(strIn);
    const QString capStr(regexp_.cap(position+1));
    return capStr;
}


void InvoiceNumberFormat_t::clearAll()
{
    list_.clear();
    regexp_.setPattern("");
}


QList<InvoiceNumberFormat_t::Field> InvoiceNumberFormat_t::fieldList() const
{
    QList<Field> ret;
    std::pair<Field, QString> p;
    foreach(p, list_)
    {
        ret.append(p.first);
    }

    return ret;
}


QStringList InvoiceNumberFormat_t::fieldStrList() const
{
    QStringList ret;

    const QList<Field> fields(fieldList());
    foreach(Field f, fields)
    {
        ret.append(FieldName(f));
    }

    return ret;
}


QString InvoiceNumberFormat_t::FieldName(const InvoiceNumberFormat_t::Field field)
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
        qDebug() << QString("Undefined field in InvoiceNumberFormat_t::FieldName: field=%1").arg((int)field);
        return QString();
    }
}


InvoiceNumberFormat_t::Field InvoiceNumberFormat_t::FieldID(const QString &field)
{
    Field ret = NR;
    for(int i = NR; i <= HYPHEN; ++i)
    {
        const Field val = (Field)i;
        if(field.compare(FieldName(val)) == 0)
        {
            ret = val;
            break;
        }
    }
    return ret;
}


QString InvoiceNumberFormat_t::FieldDescription(const InvoiceNumberFormat_t::Field field)
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
        qDebug() << QString("Undefined field in InvoiceNumberFormat_t::FieldDescription: field=%1").arg((int)field);
        return QString();
    }
}


QString InvoiceNumberFormat_t::fieldToRegexpStr(const Field field, const SettingsGlobal &s)
{
    switch(field)
    {
    case NR:
    case NR_Y:
    case NR_M:
    case NR_D:
    case NR_Q:  return QString("(\\d+)");
    case INVOICE_TYPE:  return QString("(\\w+)");
    case TEXT1: return QString("(%1)").arg(s.value(s.TEXT1).toString());
    case TEXT2: return QString("(%1)").arg(s.value(s.TEXT2).toString());
    case TEXT3: return QString("(%1)").arg(s.value(s.TEXT3).toString());
    case PERIOD_YEAR:   return QString("(\\d{4})");
    case PERIOD_MONTH:  return QString("(\\d\\d\?)");
    case PERIOD_DAY:    return QString("(\\d\\d\?)");
    case PERIOD_QUARTER:    return QString("(\\d\\d\?)");
    case SLASH: return QString("(/)");
    case BACKSLASH: return QString("(\\\\)");
    case HYPHEN:    return QString("(-)");
    default:
        qDebug() << QString("Undefined field in InvoiceNumberFormat_t::fieldToRegexp: field=%1").arg((int)field);
        return QString();
    }
}


QString InvoiceNumberFormat_t::toRegexp(const QString& format)
{
    QString ret;
    SettingsGlobal s;

    foreach(const Field field, Parse(format)->fieldList())
    {
        ret += fieldToRegexpStr(field, s);
    }

    return ret;
}


QString InvoiceNumberFormat_t::toString() const
{
    QString ret;

    std::pair<Field, QString> p;
    foreach(p, list_)
    {
        ret.append(FieldName(p.first));
    }
    return ret;
}


std::auto_ptr<InvoiceNumberFormat_t> InvoiceNumberFormat_t::Parse(const QString &format)
{
    int from = 0, to = 0;
    const QChar left('{'), right('}');
    SettingsGlobal s;
    std::auto_ptr<InvoiceNumberFormat_t> ret(new InvoiceNumberFormat_t);

    if(!format.isEmpty())
    {
        while( (from = format.indexOf(left, from)) != -1)
        {
            if( (to = format.indexOf(right, from)) != -1)
            {
                ret->append(FieldID(format.mid(from, to - from + 1)), s);
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
                for(int i = SEPARATOR_START; i <= SEPARATOR_END; ++i)
                {
                    const Field val = (Field)i;
                    if(separator == FieldName(val))
                    {
                        ret->append(val, s);
                        break;
                    }
                }
            }
        }
    }
    return ret;
}
