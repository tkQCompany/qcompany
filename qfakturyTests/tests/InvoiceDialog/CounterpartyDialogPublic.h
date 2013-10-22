#ifndef COUNTERPARTYDIALOGPUBLIC_H
#define COUNTERPARTYDIALOGPUBLIC_H

#include "CounterpartyDialog.h"

class CounterpartyDialogPublic : public CounterpartyDialog
{
    Q_OBJECT
public:
    CounterpartyDialogPublic(QWidget *parent, Database *db, const QModelIndex &id = QModelIndex(), const bool myCompany = false);
    Ui::CounterpartyDialog* ui();
};

#endif // COUNTERPARTYDIALOGPUBLIC_H
