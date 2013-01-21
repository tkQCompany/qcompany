#include "ModelCounterparty.h"

ModelCounterparty::ModelCounterparty(QObject *parent) :
    QSqlRelationalTableModel(parent, QSqlDatabase::database())
{
    setTable("counterparty");
}


QVariant ModelCounterparty::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical)
        return QVariant();

    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    switch(section)
    {
    case CounterpartyFields::ID:
        return trUtf8("L.p.");
    case CounterpartyFields::NAME:
        return trUtf8("Nazwa");
    case CounterpartyFields::TYPE_ID:
        return trUtf8("Firma/Urząd");
    case CounterpartyFields::COUNTRY_ID:
        return trUtf8("Kraj");
    case CounterpartyFields::LOCATION:
        return trUtf8("Miejscowość");
    case CounterpartyFields::POSTAL_CODE:
        return trUtf8("Kod pocztowy");
    case CounterpartyFields::STREET:
        return trUtf8("Ulica");
    case CounterpartyFields::TAX_IDENT:
        return trUtf8("NIP");
    case CounterpartyFields::ACCOUNT_NAME:
        return trUtf8("Numer konta");
    case CounterpartyFields::WWW:
        return trUtf8("WWW");
    case CounterpartyFields::PRIMARY_EMAIL:
        return trUtf8("Główny e-mail");
    case CounterpartyFields::PRIMARY_PHONE:
        return trUtf8("Główny telefon");
    case CounterpartyFields::INV_FORMAT:
        return trUtf8("Format numeru faktury");

    default:
        qDebug() << QString("File: %1, line: %2 - Unknown header of counterparties' table: %3").arg(__FILE__).arg(__LINE__).arg(section);
        return QVariant();
    }
}
