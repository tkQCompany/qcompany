#ifndef DIALOGWITHINVOICEDIALOG_H
#define DIALOGWITHINVOICEDIALOG_H

#include "DialogWithPublicDialog.h"

class InvoiceDialogPublic;

class DialogWithInvoiceDialog : public DialogWithPublicDialog
{
public:
    virtual InvoiceDialogPublic* invoiceDialogPublic() const = 0;
};

#endif // DIALOGWITHINVOICEDIALOG_H
