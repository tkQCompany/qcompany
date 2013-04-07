/*
 * Duplicate.h
 *
 *  Created on: Mar 31, 2009
 *      Author: moux
 */

#ifndef DUPLICATEDIALOG_H
#define DUPLICATEDIALOG_H

#include <QDateEdit>

#include "InvoiceDialog.h"


/**
 * @brief
 *
 */
class DuplicateDialog: public InvoiceDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param
 * @param db
 */
    DuplicateDialog(QWidget *parent, Database *db, InvoiceTypeData::Type invoiceType, const QModelIndex &idInvoice = QModelIndex());
    ~DuplicateDialog();
public slots:
    /**
     * @brief
     *
     */
    virtual void canQuit();
protected:
    /**
     * @brief
     *
     * @param bool
     * @param bool
     * @param bool
     */
    //virtual void makeInvoiceHeader(const bool, const bool, const bool);
private:
    /**
     * @brief
     *
     */
    void init_();

private:
    class DuplicateDialogImpl;
    DuplicateDialogImpl *pImpl_;

    QDateEdit *duplicateDate_; /**< TODO */
};
#endif
