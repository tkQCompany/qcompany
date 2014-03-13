#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QRegExp>

#include "ModelInvoice.h"
#include "InvoiceData.h"
#include "SettingsGlobal.h"


ModelInvoice::ModelInvoice(QObject *parent) :
    QSqlRelationalTableModel(parent, QSqlDatabase::database())
{
    setTable("invoice");
}


QVariant ModelInvoice::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation != Qt::Horizontal)
        return QVariant();
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    return InvoiceData::header((InvoiceFields::Fields)section);
}

QStringList ModelInvoice::simulateConsecutiveInvoiceNumbers(const InvoiceNumberFormat_t &invoiceNumFormat,
                                                            const QDate &firstIssuanceDate,
                                                            const InvoiceTypeData::Type invoiceType,
                                                            const int invNumCounts) const
{
    QStringList retList;
    QDate issuanceDate(firstIssuanceDate);
    retList.append(""); //temporarily - for the algorithm below
    for(int i = 1; i <= invNumCounts; ++i)
    {
        retList.append(generateInvoiceNumber(invoiceNumFormat, retList.at(i-1), issuanceDate, issuanceDate.addDays(-1), invoiceType));
        issuanceDate = issuanceDate.addDays(1);
    }
    retList.pop_front(); //removing empty string
    return retList;
}


/**
 * @brief Generate the new number for newly created invoice
 *
 * Problems related to generating numbers for invoices:
 * 1) In the period chosen by the owner (month, quarter, year, whole company's life), numbers should be consecutive (with the exception in point 5 below).
 * 2) At the new year, numbering should start from 1.
 * 3) The chosen period should refer to issuance dates, not dates of sell or payment (because of possible problems when the year is changing).
 * 4) No correction of the invoice number is possible; it's forbidden by the law.
 * 5) Must be possible to introduce separate numbering scheme for different counterparties.
 * 6) Must be possible to introduce separate numbering scheme for different checkouts
 * 7) Should be possible to take missing invoice numbers into consideration. Example: we have invoices 1, 2, 3, then gap and then: 5, 6, 7, etc.
 * 8) The format of an invoice number should be stored somewhere (because it must be clear to everyone and see point 5 above). E.g. in the number itself or in the database
 * @return QString The new invoice number
 */
QString ModelInvoice::generateInvoiceNumber(const InvoiceNumberFormat_t& invoiceNumFormat, const QString &prevInvNum,
                                            const QDate &issuanceDate, const QDate &prevIssuanceDate,
                                            const InvoiceTypeData::Type invoiceType) const
{
    QString ret;
    SettingsGlobal s;

    if(invoiceNumFormat.isEmpty())
        return ret;

    const QList<InvoiceNumberFormat_t::Field> list(invoiceNumFormat.fieldList());
    for(int i = 0; i < list.size(); ++i)
    {
        const InvoiceNumberFormat_t::Field f = list.at(i);
        switch(f)
        {
        case InvoiceNumberFormat_t::NR:
        case InvoiceNumberFormat_t::NR_Y:
        case InvoiceNumberFormat_t::NR_M:
        case InvoiceNumberFormat_t::NR_D:
        case InvoiceNumberFormat_t::NR_Q:
            ret += QString("%1").arg(increaseNumber_(invoiceNumFormat, prevInvNum, issuanceDate, prevIssuanceDate, f, i));
            break;
        case InvoiceNumberFormat_t::INVOICE_TYPE:
            ret += InvoiceTypeData::shortName(invoiceType);
            break;
        case InvoiceNumberFormat_t::TEXT1:
            ret += s.value(s.TEXT1).toString();
            break;
        case InvoiceNumberFormat_t::TEXT2:
            ret += s.value(s.TEXT2).toString();
            break;
        case InvoiceNumberFormat_t::TEXT3:
            ret += s.value(s.TEXT3).toString();
            break;
        case InvoiceNumberFormat_t::PERIOD_YEAR:
            ret += issuanceDate.toString("yyyy");
            break;
        case InvoiceNumberFormat_t::PERIOD_SHORT_YEAR:
            ret += issuanceDate.toString("yy");
            break;
        case InvoiceNumberFormat_t::PERIOD_MONTH:
            ret += issuanceDate.toString("MM");
            break;
        case InvoiceNumberFormat_t::PERIOD_DAY:
            ret += issuanceDate.toString("dd");
            break;
        case InvoiceNumberFormat_t::PERIOD_QUARTER:
            ret += QString("0%1").arg( (issuanceDate.month() - 1)/3 + 1); //output range 01-04
            break;
        case InvoiceNumberFormat_t::SLASH:
        case InvoiceNumberFormat_t::BACKSLASH:
        case InvoiceNumberFormat_t::HYPHEN:
            ret += InvoiceNumberFormat_t::FieldName(f);
            break;
        }
    }
    return ret;
}


QString ModelInvoice::getInvoiceNumberFormat(const QString &counterpartyName) const
{
    SettingsGlobal s;
    QString ret = s.value(s.DEFAULT_INV_NUM_FORMAT).toString();

    if(!counterpartyName.isEmpty())
    {
        database().transaction();
        QSqlQuery q(query());
        q.exec(QString("SELECT inv_number_format FROM 'counterparty' WHERE name = \"%1\"").arg(counterpartyName));
        if(q.isActive())
        {
            if(q.next())
            {
                if(!q.value(0).toString().isEmpty())
                {
                    ret = q.value(0).toString();
                }
            }
        }
        else
        {
            qDebug() << "ModelInvoice::getInvoiceNumberFormat(): " << q.lastError().text();
        }
        database().commit();
    }
    return ret;
}


void ModelInvoice::setDataRange(const QDate &from, const QDate &to)
{
    if(from.isValid() && to.isValid())
    {
        const QString dateFormat("yyyy-MM-dd");
        setFilter(QString("issuance_date <= \"%1\" AND issuance_date >= \"%2\"")
                  .arg(to.toString(dateFormat))
                  .arg(from.toString(dateFormat)));
    }
    else
    {
        setFilter("");
    }
}


std::auto_ptr<ModelInvoice::DBData> ModelInvoice::getLastExistingNumberDateFromDB(const bool defaultInvNumFormat,
                                                                                    const QString &counterpartyName) const
{
    std::auto_ptr<ModelInvoice::DBData> ret(new ModelInvoice::DBData());
    if( (!defaultInvNumFormat) && (counterpartyName.isEmpty() || counterpartyName.isNull()))
    {
        qDebug("ModelInvoice::getLastExistingNumberDateFromDB_(): Counterparty's name is not supplied.");
        return ret;
    }

    QSqlQuery q(this->query());

    this->database().transaction();
    if(defaultInvNumFormat)
    {
        const QString sql("SELECT MAX(inv_number), MAX(issuance_date) FROM invoice JOIN counterparty ON invoice.counterparty_id=counterparty.id_counterparty");
        if(!q.exec(sql))
        {
            qDebug("ModelInvoice::getLastExistingNumberDateFromDB(): First SQL query wasn't successful");
        }
    }
    else
    {
        const QString sql("SELECT inv_number, issuance_date FROM 'invoice' WHERE id_invoice = (SELECT MAX(id_invoice) FROM 'invoice' WHERE counterparty_id = (SELECT id_counterparty FROM 'counterparty' WHERE name = \"%1\"))");
        if(!q.exec(sql.arg(counterpartyName)))
        {
            qDebug("ModelInvoice::getLastExistingNumberDateFromDB(): Second SQL query wasn't successful");
        }
    }

    if(q.isActive())
    {
        if(q.next())
        {
            ret->invNumStr = q.value(0).toString();
            ret->gotIssuanceDate = q.value(1).toDate();
        }
    }
    else
    {
        qDebug() << QString("ModelInvoice::getLastExistingNumberDateFromDB_(): SQL error detected in line %1: %2")
                    .arg(__LINE__).arg(q.lastError().text());
    }
    this->database().commit();

    return ret;
}



long ModelInvoice::increaseNumber_(const InvoiceNumberFormat_t& invoiceNumFormat, const QString &prevInvNum, const QDate &issuanceDate, const QDate &prevIssuanceDate, const InvoiceNumberFormat_t::Field periodId, const int position)
{
    const QString capStr(invoiceNumFormat.cap(prevInvNum, position));
    switch(periodId)
    {
    case InvoiceNumberFormat_t::NR:
        return capStr.toLong() + 1L;
    case InvoiceNumberFormat_t::NR_Y:
        if(issuanceDate.year() == prevIssuanceDate.year())
        {
            return capStr.toLong() + 1L;
        }
        break;
    case InvoiceNumberFormat_t::NR_M:
        if(issuanceDate.year() == prevIssuanceDate.year() &&
                issuanceDate.month() == prevIssuanceDate.month())
        {
            return capStr.toLong() + 1L;
        }
        break;
    case InvoiceNumberFormat_t::NR_D:
        if(issuanceDate.year() == prevIssuanceDate.year() &&
                issuanceDate.month() == prevIssuanceDate.month() &&
                issuanceDate.day() == prevIssuanceDate.day())
        {
            return capStr.toLong() + 1L;
        }
        break;
    case InvoiceNumberFormat_t::NR_Q:
        if(issuanceDate.year() == prevIssuanceDate.year() &&
                issuanceDate.month()/4 == prevIssuanceDate.month()/4)
        {
            return capStr.toLong() + 1L;
        }
        break;
    default:
        qDebug() << "Unexpected period in ModelInvoice::increaseNumber_(): " << (int)periodId;
    }
    return 1L;
}
