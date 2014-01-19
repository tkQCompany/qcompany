#ifndef GUIUSERADDCOUNTERPARTY_H
#define GUIUSERADDCOUNTERPARTY_H

#include "GuiUser.h"
#include "Counterparty_t.h"

struct Counterparty_t;
class DialogWithCounterpartyDialog;
class QComboBox;

class GuiUserAddCounterparty : public GuiUser
{
    Q_OBJECT
public:
    explicit GuiUserAddCounterparty(DialogWithCounterpartyDialog *d, Counterparty_t *counterparty, QObject *parent = 0);
    
public slots:
    void process();

signals:
    void setComboBoxIndex(int);

private:
    void postText_(QObject *obj, const QString &str);
    void postComboBoxIndex_(QComboBox *obj, const int index);

private:
    DialogWithCounterpartyDialog *dialog_;
    const Counterparty_t counterparty_;    
};

#endif // GUIUSERADDCOUNTERPARTY_H
