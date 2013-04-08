#ifndef COUNTERPARTYTYPEDIALOG_H
#define COUNTERPARTYTYPEDIALOG_H

#include <QDialog>

namespace Ui {
class CounterpartyTypeDialog;
}


class Database;


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

private slots:
    bool addType_();
    bool delType_();
    
private:
    Ui::CounterpartyTypeDialog *ui; /**< TODO */
    Database *db_; /**< TODO */
    QAction *actionAddType_, *actionDelType_;
};

#endif // COUNTERPARTYTYPEDIALOG_H
