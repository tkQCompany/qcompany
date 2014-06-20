#ifndef COMMODITYLISTDIALOG_H
#define COMMODITYLISTDIALOG_H

#include <QDialog>
#include <QDoubleValidator>

#include "CommodityVisualData.h"
#include "Money_t.h"

namespace Ui
{
    class CommodityListDialog;
}


class Database;
class QDataWidgetMapper;


/**
 * @brief
 *
 */
class CommodityListDialog: public QDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param parent
 * @param db
 */
    CommodityListDialog(QWidget *parent, Database *db);
    /**
     * @brief
     *
     */
    virtual ~CommodityListDialog();
public:
    CommodityVisualData ret; /**< TODO */

protected slots:
    /**
     * @brief
     *
     */
    virtual void doAccept();
    /**
     * @brief
     *
     */
    virtual void updateLabelNetVal();
    /**
     * @brief
     *
     * @param int
     */
    void comboBoxCommoditiesChanged(int);
    /**
     * @brief
     *
     * @param int
     */
    void netPriceChanged();

protected:
    /**
     * @brief
     *
     */
    void init();

protected:
    Ui::CommodityListDialog *ui;
    Database *db; /**< TODO */
    QDataWidgetMapper *widgetMapper; /**< TODO */
    QDoubleValidator validator;
    Money_t netVal; /**< TODO */
};
#endif
