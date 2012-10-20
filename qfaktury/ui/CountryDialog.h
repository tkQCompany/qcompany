#ifndef COUNTRYDIALOG_H
#define COUNTRYDIALOG_H

#include <QDialog>

#include "../src/model/Database.h"

namespace Ui {
class CountryDialog;
}

/**
 * @brief
 *
 */
class CountryDialog : public QDialog
{
    Q_OBJECT
    
public:
    /**
     * @brief
     *
     * @param parent
     * @param db
     */
    explicit CountryDialog(QWidget *parent, Database *db);
    /**
     * @brief
     *
     */
    ~CountryDialog();
    
private:
    Ui::CountryDialog *ui; /**< TODO */
    Database *db_; /**< TODO */
};

#endif // COUNTRYDIALOG_H
