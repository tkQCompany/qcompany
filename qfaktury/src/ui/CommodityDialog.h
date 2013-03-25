#ifndef COMMODITYDIALOG_H
#define COMMODITYDIALOG_H

#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>

#include "SettingsDialog.h"
#include "../model/Database.h"


namespace Ui
{
class CommodityDialog;
}

/**
 * @brief
 *
 */
class CommodityDialog: public QDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param parent
 * @param db
 * @param id_edit
 */
    CommodityDialog(QWidget *parent, Database *db, const QModelIndex &id_edit = QModelIndex());
    /**
     * @brief
     *
     */
    ~CommodityDialog();

private slots:
    /**
     * @brief
     *
     */
    void okClick();
    /**
     * @brief
     *
     */
    void pkwiuGet();
    /**
     * @brief
     *
     * @param suffix
     */
    void addSuffix(const QString &suffix);
private:
    /**
     * @brief
     *
     */
    void init();
private:
    Ui::CommodityDialog *ui_;
    Database *db_; /**< TODO */
    QDataWidgetMapper *mapper_; /**< TODO */
    QDoubleValidator validator_; /**< TODO */
};
#endif