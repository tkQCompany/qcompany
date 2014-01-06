#ifndef GUIUSERADDCOUNTERPARTY_H
#define GUIUSERADDCOUNTERPARTY_H

#include "GuiUser.h"
#include "Counterparty_t.h"

class InvoiceDialogPublic;
struct Counterparty_t;
class QComboBox;

class GuiUserAddCounterparty : public GuiUser
{
    Q_OBJECT
public:
    explicit GuiUserAddCounterparty(InvoiceDialogPublic *idp, Counterparty_t *counterparty, QObject *parent = 0);
    
public slots:
    void process();

signals:
    void setComboBoxIndex(int);

private:
    void postText_(QObject *obj, const QString &str);
    void postComboBoxIndex_(QComboBox *obj, const int index);

private:
    const InvoiceDialogPublic *idp_;
    const Counterparty_t counterparty_;    
};

#endif // GUIUSERADDCOUNTERPARTY_H
