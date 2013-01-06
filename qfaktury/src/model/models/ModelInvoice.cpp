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
