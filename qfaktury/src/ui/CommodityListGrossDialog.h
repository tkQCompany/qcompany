/*
 * TowaryBruttoLista.h
 *
 *  Created on: Apr 3, 2009
 *      Author: TPIELECH
 */

#ifndef COMMODITYLISTGROSSDIALOG_H
#define COMMODITYLISTGROSSDIALOG_H

#include "CommodityListDialog.h"
#include "../model/models/ModelCommodity.h"

/**
 * @brief
 *
 */
class CommodityListGrossDialog: public QDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param
 * @param db
 */
    CommodityListGrossDialog(QDialog *parent = 0);
    ~CommodityListGrossDialog();
private slots:
    /**
     * @brief
     *
     */
    void updateNetVal();
    /**
     * @brief
     *
     */
    void doAccept();
private:
    Ui::CommodityListDialog *ui_;
};

#endif
