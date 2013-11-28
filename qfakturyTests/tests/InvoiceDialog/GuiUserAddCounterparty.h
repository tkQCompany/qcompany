#ifndef GUIUSERADDCOUNTERPARTY_H
#define GUIUSERADDCOUNTERPARTY_H

#include "GuiUser.h"
#include "Counterparty_t.h"

class InvoiceDialogPublic;
struct Counterparty_t;

class GuiUserAddCounterparty : public GuiUser
{
    Q_OBJECT
public:
    explicit GuiUserAddCounterparty(InvoiceDialogPublic *idp, Counterparty_t *counterparty, QObject *parent = 0);
    
public slots:
    void process();

private:
    InvoiceDialogPublic *idp_;
    Counterparty_t *counterparty_;
};

#endif // GUIUSERADDCOUNTERPARTY_H
