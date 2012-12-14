#ifndef COMMODITYLISTDIALOG_H
#define COMMODITYLISTDIALOG_H

#include <QMessageBox>
#include <QScopedPointer>
#include <QDataWidgetMapper>

#include "ui_CommodityListDialog.h"
#include "../src/model/Database.h"

/**
 * @brief
 *
 */
class CommodityListDialog: public QDialog, public Ui::CommodityListDialog
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
    Database *db_; /**< TODO */
    QDataWidgetMapper widgetMapper_; /**< TODO */
    double netVal_; /**< TODO */
};
#endif
