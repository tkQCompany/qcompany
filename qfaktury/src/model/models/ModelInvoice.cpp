#include "ModelInvoice.h"
#include <QDebug>

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

    switch(section)
    {
    case InvoiceFields::ID_INVOICE:
        return trUtf8("L.p.");
    case InvoiceFields::INV_NUMBER:
        return trUtf8("Numer faktury");
    case InvoiceFields::SELLING_DATE:
        return trUtf8("Data sprzedaży");
    case InvoiceFields::TYPE_ID:
        return trUtf8("Rodzaj faktury");
    case InvoiceFields::COUNTERPARTY_ID:
        return trUtf8("Nazwa kontrahenta");
    case InvoiceFields::ISSUANCE_DATE:
        return trUtf8("Data wydania");
    case InvoiceFields::PAYMENT_DATE:
        return trUtf8("Data płatności");
    case InvoiceFields::PAYMENT_ID:
        return trUtf8("Rodzaj płatności");
    case InvoiceFields::CURRENCY_ID:
        return trUtf8("Waluta");
    case InvoiceFields::ADDIT_TEXT:
        return trUtf8("Tekst dodatkowy");
    case InvoiceFields::DISCOUNT:
        return trUtf8("Zniżka");

    default:
        qDebug() << QString("File: %1, line: %2 - Unknown header of invoices' table: %3").arg(__FILE__).arg(__LINE__).arg(section);
        return QVariant();
    }
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
QString ModelInvoice::generateInvoiceNumber(const QString &format, const QDate &issuanceDate, const int invoiceType) const
{
    QString ret;
    SettingsGlobal s;
    const QVector<int> parse(InvoiceNumberFormatData::Parse(format));
    for(int i = 0; i < parse.size(); ++i)
    {
        switch(parse.at(i))
        {
        case InvoiceNumberFormatData::NR:
            ret += QString("%1").arg(this->rowCount()); //the "rowCount()" includes the newly added empty row
            break;
        case InvoiceNumberFormatData::NR_Y:
            ret += QString("%1").arg(1);
            break;
        case InvoiceNumberFormatData::NR_M:
            ret += QString("%1").arg(1);
            break;
        case InvoiceNumberFormatData::NR_D:
            ret += QString("%1").arg(1);
            break;
        case InvoiceNumberFormatData::NR_Q:
            ret += QString("%1").arg(1);
            break;
        case InvoiceNumberFormatData::INVOICE_TYPE:
            ret += InvoiceTypeData::InvoiceTypeToString(invoiceType);
            break;
        case InvoiceNumberFormatData::TEXT1:
            ret += s.value(s.keyName(s.TEXT1)).toString();
            break;
        case InvoiceNumberFormatData::TEXT2:
            ret += s.value(s.keyName(s.TEXT2)).toString();
            break;
        case InvoiceNumberFormatData::TEXT3:
            ret += s.value(s.keyName(s.TEXT3)).toString();
            break;
        case InvoiceNumberFormatData::PERIOD_YEAR:
            ret += issuanceDate.toString("yyyy");
            break;
        case InvoiceNumberFormatData::PERIOD_MONTH:
            ret += issuanceDate.toString("MM");
            break;
        case InvoiceNumberFormatData::PERIOD_DAY:
            ret += issuanceDate.toString("dd");
            break;
        case InvoiceNumberFormatData::PERIOD_QUARTER:
            ret += QString("%1").arg( (issuanceDate.month() - 1)/3 + 1); //1-4
            break;
        case InvoiceNumberFormatData::SLASH:
        case InvoiceNumberFormatData::BACKSLASH:
        case InvoiceNumberFormatData::HYPHEN:
            ret += InvoiceNumberFormatData::FieldName(parse.at(i));
            break;
        }

    }
    return ret;
}
