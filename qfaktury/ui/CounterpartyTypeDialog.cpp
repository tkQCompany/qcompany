#include "CounterpartyTypeDialog.h"
#include "ui_CounterpartyTypeDialog.h"

CounterpartyTypeDialog::CounterpartyTypeDialog(QWidget *parent, Database *db) :
    QDialog(parent),
    ui(new Ui::CounterpartyTypeDialog), db_(db)
{
    ui->setupUi(this);
    db_->modelCounterpartyType()->setFilter(QString("id_counterparty_type != %1").arg(CounterpartyTypeData::MY_COMPANY));
    db_->modelCounterpartyType()->select();
    ui->listView->setModel(db_->modelCounterpartyType());
    ui->listView->setModelColumn(CounterpartyTypeFields::TYPE);

}

CounterpartyTypeDialog::~CounterpartyTypeDialog()
{
    db_->modelCounterpartyType()->setFilter("");
    db_->modelCounterpartyType()->select();
    delete ui;
}
