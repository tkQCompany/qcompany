#include "CounterpartyTypeDialog.h"
#include "ui_CounterpartyTypeDialog.h"

CounterpartyTypeDialog::CounterpartyTypeDialog(QWidget *parent, Database *db) :
    QDialog(parent),
    ui(new Ui::CounterpartyTypeDialog), db_(db)
{
    ui->setupUi(this);
    db_->modelCounterpartyType()->setMyCompanyVisibility(false);
    ui->listView->setModel(db_->modelCounterpartyType());
    ui->listView->setModelColumn(CounterpartyTypeFields::TYPE);

}

CounterpartyTypeDialog::~CounterpartyTypeDialog()
{
    delete ui;
}
