/*
 * KorektaBrutto.h
 *
 *  Created on: Apr 3, 2009
 *      Author: moux
 */

#ifndef CORRECTIVEINVOICEGROSSDIALOG_H
#define CORRECTIVEINVOICEGROSSDIALOG_H

#include "CommodityListGrossDialog.h"
#include "CorrectiveInvoiceDialog.h"
#include "InvoiceDialog.h"

/**
 * @brief
 *
 */
class CorrectiveInvoiceGrossDialog: public InvoiceDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param parent
 * @param db
 */
    CorrectiveInvoiceGrossDialog(QWidget *parent, Database *db, const QModelIndex &idInvoice);
    ~CorrectiveInvoiceGrossDialog();
    /**
     * @brief
     *
     */
    void addCommodity();
protected:
    /**
     * @brief
     *
     * @param int
     */
    void calculateOneDiscount(const int);
private:
    void init_();

private:
    class CorrectiveInvoiceGrossDialogImpl;
    CorrectiveInvoiceGrossDialogImpl *pImpl_;
};

#endif
