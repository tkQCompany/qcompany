/*
 * FakturaBrutto.h
 *
 *  Created on: Apr 2, 2009
 *      Author: TPIELECH
 */

#ifndef INVOICEGROSSDIALOG_H
#define INVOICEGROSSDIALOG_H

#include <QDialog>
#include <QModelIndex>

class Database;

/**
 * @brief
 *
 */
class InvoiceGrossDialog: public QDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param
 * @param db
 */
    InvoiceGrossDialog(QWidget *, Database *db, const QModelIndex &idInvoice = QModelIndex());
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
