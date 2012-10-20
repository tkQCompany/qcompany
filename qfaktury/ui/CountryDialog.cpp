#include "CountryDialog.h"
#include "ui_CountryDialog.h"

CountryDialog::CountryDialog(QWidget *parent, Database *db) :
    QDialog(parent), ui(new Ui::CountryDialog), db_(db)
{
    ui->setupUi(this);
    ui->listView->setModel(db_->modelCountry());
    ui->listView->setModelColumn(CountryFields::NAME);
}





CountryDialog::~CountryDialog()
{
    delete ui;
}
