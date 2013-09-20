#ifndef COMMODITYDIALOG_H
#define COMMODITYDIALOG_H

#include <QModelIndex>
#include <QDoubleValidator>
#include <QDataWidgetMapper>

#include "SettingsDialog.h"


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

protected slots:
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

    void servicesCantBeCounted(const QString &name);
private:
    /**
     * @brief
     *
     */
    void init_();
protected:
    Ui::CommodityDialog *ui;
    Database *db; /**< TODO */
    QDataWidgetMapper mapper; /**< TODO */
    QDoubleValidator validator; /**< TODO */
};
#endif
