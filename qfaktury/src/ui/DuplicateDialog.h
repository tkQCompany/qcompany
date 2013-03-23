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
class DuplicateDialog: public QDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param
 * @param db
 */
    DuplicateDialog(QDialog *parent = 0);
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
    Ui::InvoiceDialog *ui_;
    QDateEdit *duplicateDate; /**< TODO */

};
#endif
