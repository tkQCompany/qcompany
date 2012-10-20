#ifndef PHONEDIALOG_H
#define PHONEDIALOG_H

#include <QDialog>
#include <QMenu>
#include <QContextMenuEvent>
#include <QSqlRelationalDelegate>

#include "../src/model/Database.h"

namespace Ui {
class PhoneDialog;
}

/**
 * @brief
 *
 */
class PhoneDialog : public QDialog
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
    explicit PhoneDialog(QWidget *parent, Database *db, const QModelIndex &id_counterparty);
    /**
     * @brief
     *
     */
    ~PhoneDialog();

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
    void on_actionAddPhone_triggered();
    /**
     * @brief
     *
     */
    void on_actionRemovePhone_triggered();

private:
    Ui::PhoneDialog *ui; /**< TODO */
    Database *db_; /**< TODO */
};

#endif // PHONEDIALOG_H
