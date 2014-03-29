#ifndef DIALOGWITHCOUNTERPARTYDIALOG_H
#define DIALOGWITHCOUNTERPARTYDIALOG_H

#include "DialogWithPublicDialog.h"

class CounterpartyDialogPublic;

class DialogWithCounterpartyDialog : public DialogWithPublicDialog
{
public:    
    virtual CounterpartyDialogPublic* counterpartyDialogPublic() const = 0;
};

#endif // DIALOGWITHCOUNTERPARTYDIALOG_H
