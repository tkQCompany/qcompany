/*
 * KorektaBrutto.h
 *
 *  Created on: Apr 3, 2009
 *      Author: moux
 */

#ifndef CORRECTIVEINVOICEGROSSDIALOG_H
#define CORRECTIVEINVOICEGROSSDIALOG_H

#include <QDialog>
#include <QModelIndex>

class Database;

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

protected:
    class CorrectiveInvoiceGrossDialogImpl;
    CorrectiveInvoiceGrossDialogImpl *pImpl_;
};

#endif
