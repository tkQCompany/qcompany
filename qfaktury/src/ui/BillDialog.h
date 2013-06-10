#ifndef BILLDIALOG_H
#define BILLDIALOG_H

#include <QDialog>

class Database;

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

protected:
    class BillDialogImpl;
    BillDialogImpl *pImpl_;
};

#endif
