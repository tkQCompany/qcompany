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
class CorrectiveInvoiceGrossDialog: public CorrectiveInvoiceDialog
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
    /**
     * @brief
     *
     * @return QString
     */
    QString getInvoiceTypeAndSaveNr();

};

#endif
