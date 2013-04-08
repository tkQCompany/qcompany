#ifndef COMMODITYLISTDIALOG_H
#define COMMODITYLISTDIALOG_H

#include <QDialog>
#include <QDataWidgetMapper>

#include "CommodityVisualData.h"

namespace Ui
{
    class CommodityListDialog;
}


class Database;


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
private slots:
    /**
     * @brief
     *
     */
    virtual void doAccept();
    /**
     * @brief
     *
     */
    virtual void updateNetVal();
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
    void comboBoxChosenNetPriceChanged(const int);

protected:
    /**
     * @brief
     *
     */
    void init();

protected:
    Ui::CommodityListDialog *ui;
    Database *db; /**< TODO */
    QDataWidgetMapper widgetMapper; /**< TODO */
    double netVal; /**< TODO */
};
#endif
