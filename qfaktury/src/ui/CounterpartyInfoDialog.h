#ifndef COUNTERPARTYINFODIALOG_H
#define COUNTERPARTYINFODIALOG_H

#include <QDialog>
#include <QModelIndex>

namespace Ui
{
    class CounterpartyInfoDialog;
}


class Database;
class QDataWidgetMapper;


/**
 * @brief
 *
 */
class CounterpartyInfoDialog: public QDialog
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
    ~CounterpartyInfoDialog();

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
    Ui::CounterpartyInfoDialog *ui_;
    Database *db_; /**< TODO */
    QDataWidgetMapper *widgetMapper_; /**< TODO */
};
#endif
