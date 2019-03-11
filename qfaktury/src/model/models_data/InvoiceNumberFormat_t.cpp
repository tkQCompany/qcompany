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
        if( ((field < static_cast<Field>(SEPARATOR_START)) || (field > static_cast<Field>(SEPARATOR_END)))
                && ((prevField < static_cast<Field>(SEPARATOR_START)) || (prevField > static_cast<Field>(SEPARATOR_END))))
        {
            qDebug() << "InvoiceNumberFormat_t::append() can't allow two non-separate fields in a row. Field1: "
                     << FieldName(prevField) << ", field2: " << FieldName(field);
            return false;
        }
    }

    const QString regexpStr(fieldToRegexpStr(field, s));
    list_.append(std::make_pair(field, regexpStr));
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
    QString ret;

    switch(field)
    {
    case NR: ret = QObject::trUtf8("{NR}");
        break;
    case NR_Y: ret = QObject::trUtf8("{NR_R}");
        break;
    case NR_M: ret = QObject::trUtf8("{NR_M}");
        break;
    case NR_D: ret = QObject::trUtf8("{NR_D}");
        break;
    case NR_Q: ret = QObject::trUtf8("{NR_K}");
        break;
    case INVOICE_TYPE: ret = QObject::trUtf8("{TYP_FAKTURY}");
        break;
    case TEXT1: ret = QObject::trUtf8("{TEKST1}");
        break;
    case TEXT2: ret = QObject::trUtf8("{TEKST2}");
        break;
    case TEXT3: ret = QObject::trUtf8("{TEKST3}");
        break;
    case PERIOD_YEAR: ret = QObject::trUtf8("{R}");
        break;
    case PERIOD_SHORT_YEAR: ret = QObject::trUtf8("{KR}");
        break;
    case PERIOD_MONTH: ret = QObject::trUtf8("{M}");
        break;
    case PERIOD_DAY: ret = QObject::trUtf8("{D}");
        break;
    case PERIOD_QUARTER: ret = QObject::trUtf8("{K}");
        break;
    case SLASH: ret = QString("/");
        break;
    case BACKSLASH: ret = QString("\\");
        break;
    case HYPHEN: ret = QString("-");
        break;
    }

    return ret;
}


InvoiceNumberFormat_t::Field InvoiceNumberFormat_t::FieldID(const QString &field)
{
    Field ret = NR;
    for(int i = NR; i <= HYPHEN; ++i)
    {
        const Field val = static_cast<Field>(i);
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
    QString ret;

    switch(field)
    {
    case NR: ret = QObject::trUtf8("Nr kolejny w całym okresie działania firmy");
        break;
    case NR_Y: ret = QObject::trUtf8("Nr kolejny w roku");
        break;
    case NR_M: ret = QObject::trUtf8("Nr kolejny w miesiącu");
        break;
    case NR_D: ret = QObject::trUtf8("Nr kolejny w dniu");
        break;
    case NR_Q: ret = QObject::trUtf8("Nr kolejny w kwartale");
        break;
    case INVOICE_TYPE: ret = QObject::trUtf8("Typ faktury");
        break;
    case TEXT1: ret = QObject::trUtf8("Tekst 1");
        break;
    case TEXT2: ret = QObject::trUtf8("Tekst 2");
        break;
    case TEXT3: ret = QObject::trUtf8("Tekst 3");
        break;
    case PERIOD_YEAR: ret = QObject::trUtf8("Rok (RRRR)");
        break;
    case PERIOD_SHORT_YEAR: ret = QObject::trUtf8("Rok (RR)");
        break;
    case PERIOD_MONTH: ret = QObject::trUtf8("Miesiąc");
        break;
    case PERIOD_DAY: ret = QObject::trUtf8("Dzień");
        break;
    case PERIOD_QUARTER: ret = QObject::trUtf8("Kwartał");
        break;
    case SLASH: ret = QObject::trUtf8("Separator - prawy ukośnik");
        break;
    case BACKSLASH: ret = QObject::trUtf8("Separator - lewy ukośnik");
        break;
    case HYPHEN: ret = QObject::trUtf8("Separator - łącznik");
        break;
    }

    return ret;
}


QString InvoiceNumberFormat_t::fieldToRegexpStr(const Field field, const SettingsGlobal &s)
{
    QString ret;

    switch(field)
    {
    case NR:
    case NR_Y:
    case NR_M:
    case NR_D:
    case NR_Q:  ret = QString("(\\d+)");
        break;
    case INVOICE_TYPE:  ret = QString("(\\w+)");
        break;
    case TEXT1: ret = QString("(%1)").arg(s.value(s.TEXT1).toString());
        break;
    case TEXT2: ret = QString("(%1)").arg(s.value(s.TEXT2).toString());
        break;
    case TEXT3: ret = QString("(%1)").arg(s.value(s.TEXT3).toString());
        break;
    case PERIOD_YEAR:   ret = QString("(\\d{4})");
        break;
    case PERIOD_SHORT_YEAR:   ret = QString("(\\d{2})");
        break;
    case PERIOD_MONTH:  ret = QString("(\\d\\d\?)");
        break;
    case PERIOD_DAY:    ret = QString("(\\d\\d\?)");
        break;
    case PERIOD_QUARTER:    ret = QString("(\\d\\d\?)");
        break;
    case SLASH: ret = QString("(/)");
        break;
    case BACKSLASH: ret = QString("(\\\\)");
        break;
    case HYPHEN:    ret = QString("(-)");
        break;
    }

    return ret;
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


std::unique_ptr<InvoiceNumberFormat_t> InvoiceNumberFormat_t::Parse(const QString &format)
{
    int from = 0, to = 0;
    const QChar left('{'), right('}');
    SettingsGlobal s;
    std::unique_ptr<InvoiceNumberFormat_t> ret(new InvoiceNumberFormat_t);

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
                    const Field val = static_cast<Field>(i);
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
