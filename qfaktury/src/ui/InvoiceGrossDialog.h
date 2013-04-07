/*
 * FakturaBrutto.h
 *
 *  Created on: Apr 2, 2009
 *      Author: TPIELECH
 */

#ifndef INVOICEGROSSDIALOG_H
#define INVOICEGROSSDIALOG_H

#include "CommodityListGrossDialog.h"
#include "InvoiceDialog.h"


/**
 * @brief
 *
 */
class InvoiceGrossDialog: public InvoiceDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param
 * @param db
 */
    InvoiceGrossDialog(QWidget *, Database *db, const QModelIndex &idInvoice);
    ~InvoiceGrossDialog();
public slots:
    /**
     * @brief
     *
     */
    void addCommodity();
protected:
    /**
     * @brief
     *
     * @param i
     */
    void calculateOneDiscount(const int i);

private:
    class InvoiceGrossDialogImpl;
    InvoiceGrossDialogImpl *pImpl_;
};


#endif
