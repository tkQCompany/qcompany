#ifndef COUNTERPARTYTYPEDIALOG_H
#define COUNTERPARTYTYPEDIALOG_H

#include <QDialog>

#include "../src/model/Database.h"

namespace Ui {
class CounterpartyTypeDialog;
}

/**
 * @brief
 *
 */
class CounterpartyTypeDialog : public QDialog
{
    Q_OBJECT
    
public:
    /**
     * @brief
     *
     * @param parent
     * @param db
     */
    explicit CounterpartyTypeDialog(QWidget *parent, Database *db);
    /**
     * @brief
     *
     */
    ~CounterpartyTypeDialog();
    
private:
    Ui::CounterpartyTypeDialog *ui; /**< TODO */
    Database *db_; /**< TODO */
};

#endif // COUNTERPARTYTYPEDIALOG_H
