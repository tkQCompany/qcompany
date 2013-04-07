#ifndef INVOICEDIALOG_H
#define INVOICEDIALOG_H

#include <QDialog>
#include <QModelIndex>

#include <memory>

#include "InvoiceTypeData.h"
#include "InvoiceDialogImpl.h"

class CustomPaymData;
class Database;

/**
 * @brief
 *
 */
class InvoiceDialog: public QDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param parent
 * @param db
 * @param id_edit
 */
    InvoiceDialog(QWidget *parent, Database *db, InvoiceTypeData::Type invoiceType = InvoiceTypeData::VAT, const QModelIndex &idEdit = QModelIndex(), const bool newPImpl = true);
    /**
     * @brief
     *
     */
    virtual ~InvoiceDialog();

protected:
    virtual void setPImpl(InvoiceDialogImpl *pImpl);

private:
    InvoiceDialogImpl *pImpl_;
};
#endif
