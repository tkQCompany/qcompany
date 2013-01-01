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
    DuplicateDialog(QWidget *, Database *db);
public slots:
    /**
     * @brief
     *
     */
    void init_();
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
    virtual void makeInvoiceHeader(const bool, const bool, const bool);
private:
    QDateEdit *duplicateDate; /**< TODO */

};
#endif
