#ifndef COUNTERPARTYINFODIALOG_H
#define COUNTERPARTYINFODIALOG_H

#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QDebug>
#include <QDataWidgetMapper>

#include "../SettingsGlobal.h"
#include "ui_CounterpartyInfoDialog.h"
#include "../model/Database.h"

/**
 * @brief
 *
 */
class CounterpartyInfoDialog: public QDialog, public Ui::CounterpartyInfoDialog
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
    CounterpartyInfoDialog(QWidget *parent, Database *db, const QModelIndex &id_edit);

private slots:
    /**
     * @brief
     *
     */
    void openCustomerWWW();

private:
    /**
     * @brief
     *
     */
    void init();
private:
    Database *db_; /**< TODO */
    QDataWidgetMapper *widgetMapper_; /**< TODO */
};
#endif
