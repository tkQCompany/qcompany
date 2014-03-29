#include "CounterpartyDialogPublic.h"

CounterpartyDialogPublic::CounterpartyDialogPublic(QWidget *parent, Database *db, const QModelIndex &id, const bool myCompany)
    :CounterpartyDialog(parent, db, id, myCompany)
{
}

Ui::CounterpartyDialog* CounterpartyDialogPublic::ui()
{
    return CounterpartyDialog::ui;
}
