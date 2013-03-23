#include "ChangeAmountDialog.h"
#include "ui_ChangeAmountDialog.h"

ChangeAmountDialog::ChangeAmountDialog(QWidget *parent):
    QDialog(parent), ui_(new Ui::ChangeAmountDialog)
{
    ui_->setupUi(this);
}


ChangeAmountDialog::~ChangeAmountDialog()
{
    delete ui_;
}
