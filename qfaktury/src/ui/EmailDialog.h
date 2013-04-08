#ifndef EMAILDIALOG_H
#define EMAILDIALOG_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
class EmailDialog;
}


class Database;


/**
 * @brief
 *
 */
class EmailDialog : public QDialog
{
    Q_OBJECT
    
public:
    /**
     * @brief
     *
     * @param parent
     * @param db
     * @param id_counterparty
     */
    explicit EmailDialog(QWidget *parent, Database *db, const QModelIndex &id_counterparty = QModelIndex());
    /**
     * @brief
     *
     */
    ~EmailDialog();
    
private slots:
    /**
     * @brief
     *
     * @param event
     */
    void contextMenuEvent(QContextMenuEvent *event);
    /**
     * @brief
     *
     * @param index
     */
    void editList_(QModelIndex index);
    /**
     * @brief
     *
     */
    void on_actionAddEmail_triggered();
    /**
     * @brief
     *
     */
    void on_actionRemoveEmail_triggered();

private:
    Ui::EmailDialog *ui; /**< TODO */
    Database *db_; /**< TODO */
};

#endif // EMAILDIALOG_H
