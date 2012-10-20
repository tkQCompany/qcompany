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
    case InvoiceDataFields::ID:
        return trUtf8("L.p.");
    case InvoiceDataFields::INV_NUMBER:
        return trUtf8("Numer faktury");
    case InvoiceDataFields::SELLING_DATE:
        return trUtf8("Data sprzedaży");
    case InvoiceDataFields::TYPE_ID:
        return trUtf8("Rodzaj faktury");
    case InvoiceDataFields::COUNTERPARTY_ID:
        return trUtf8("Nazwa kontrahenta");
    case InvoiceDataFields::ISSUANCE_DATE:
        return trUtf8("Data wydania");
    case InvoiceDataFields::PAYMENT_DATE:
        return trUtf8("Data płatności");
    case InvoiceDataFields::PAYMENT_ID:
        return trUtf8("Rodzaj płatności");
    case InvoiceDataFields::CURRENCY_ID:
        return trUtf8("Waluta");
    case InvoiceDataFields::ADDIT_TEXT:
        return trUtf8("Tekst dodatkowy");
    case InvoiceDataFields::DISCOUNT:
        return trUtf8("Zniżka");

    default:
        qDebug() << QString("File: %1, line: %2 - Unknown header of invoices' table: %3").arg(__FILE__).arg(__LINE__).arg(section);
        return QVariant();
    }
}
