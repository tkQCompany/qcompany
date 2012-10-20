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
class CommodityListGrossDialog: public CommodityListDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param
 * @param db
 */
    CommodityListGrossDialog(QWidget *, Database *db);
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
};

#endif
