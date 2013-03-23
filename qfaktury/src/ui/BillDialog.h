#ifndef BILLDIALOG_H
#define BILLDIALOG_H

#include "../src/ConvertAmount.h"
#include "../src/SettingsGlobal.h"
#include "Database.h"

namespace Ui
{
class InvoiceDialog;
}

/**
 * @brief
 *
 */
class BillDialog: public QDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param
 * @param db
 */
    explicit BillDialog(QWidget *parent, Database *db);
    ~BillDialog();

private:
    /**
     * @brief
     *
     */
    void init_();

private:
    Ui::InvoiceDialog *ui_;
    Database *db_;
};

#endif
