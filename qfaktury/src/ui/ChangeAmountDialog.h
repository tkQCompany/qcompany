#ifndef CHANGEAMOUNTDIALOG_H
#define CHANGEAMOUNTDIALOG_H

#include <QDialog>

namespace Ui
{
class ChangeAmountDialog;
}

/**
 * @brief
 *
 */
class ChangeAmountDialog: public QDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param
 */
    explicit ChangeAmountDialog(QWidget *);
    ~ChangeAmountDialog();
private:
    Ui::ChangeAmountDialog *ui_;
};
#endif
