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

/**
 * @brief
 *
 */
class CorrectiveInvoiceGrossDialog: public QDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param parent
 * @param db
 */
    CorrectiveInvoiceGrossDialog(QWidget *parent, Database *db);
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
    Ui::InvoiceDialog *ui_;
    Database *db_;
};

#endif
