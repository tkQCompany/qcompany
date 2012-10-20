/*
 * Rachunek.h
 *
 *  Created on: Apr 2, 2009
 *      Author: moux
 */

#ifndef BILLDIALOG_H
#define BILLDIALOG_H

#include "InvoiceDialog.h"
#include "../ConvertAmount.h"
#include "../SettingsGlobal.h"


/**
 * @brief
 *
 */
class BillDialog: public InvoiceDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param
 * @param db
 */
    BillDialog(QWidget*, Database *db);
    /**
     * @brief
     *
     */
    void init();
};

#endif
