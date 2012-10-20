/*
 * CustomPayment.h
 *
 *  Created on: Apr 4, 2009
 *      Author: moux
 */

#ifndef CUSTOMPAYMENTDIALOG_H
#define CUSTOMPAYMENTDIALOG_H

#include <QMessageBox>

#include "../SettingsGlobal.h"
#include "../CustomPaymData.h"
#include "ui_CustomPaymentDialog.h"

/**
 * @brief
 *
 */
class CustomPaymentDialog: public QDialog, public Ui::CustomPaymentDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param parent
 */
    CustomPaymentDialog(QWidget *parent = 0);
    /**
     * @brief
     *
     */
    virtual ~CustomPaymentDialog();
    /**
     * @brief
     *
     * @param
     */
    void setInvoiceAmount(const double &);
public:
    CustomPaymData *custPaymData; /**< TODO */
private slots:
    /**
     * @brief
     *
     */
    void okClicked();
    /**
     * @brief
     *
     */
    void doubleSpinBoxAmount1Changed();
    /**
     * @brief
     *
     */
    void doubleSpinBoxAmount2Changed();
private:
    /**
     * @brief
     *
     */
    void init();
    /**
     * @brief
     *
     * @return bool
     */
    bool validateForm();
private:
    double invoiceAmount; /**< TODO */
};

#endif
