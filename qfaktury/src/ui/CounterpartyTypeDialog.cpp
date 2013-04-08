#include <QMessageBox>

#include "CounterpartyTypeDialog.h"
#include "ui_CounterpartyTypeDialog.h"
#include "Database.h"
#include "ModelCounterpartyType.h"
#include "CounterpartyTypeData.h"


CounterpartyTypeDialog::CounterpartyTypeDialog(QWidget *parent, Database *db) :
    QDialog(parent),
    ui(new Ui::CounterpartyTypeDialog), db_(db),
    actionAddType_(new QAction(this)), actionDelType_(new QAction(this))
{
    ui->setupUi(this);

    ui->listViewTypes->setModel(db_->modelCounterpartyType());
    ui->listViewTypes->setModelColumn(CounterpartyTypeFields::TYPE);

    connect(ui->pushButtonAdd, SIGNAL(clicked()), this, SLOT(addType_()));
    connect(ui->pushButtonDel, SIGNAL(clicked()), this, SLOT(delType_()));
}

CounterpartyTypeDialog::~CounterpartyTypeDialog()
{
    delete ui;
}

bool CounterpartyTypeDialog::addType_()
{
    const QString title(trUtf8("Dodawanie typu"));
    if(ui->lineEditType->text().isEmpty())
    {
        QMessageBox::information(this, title,
                                 trUtf8("Wpisz typ, pole jest puste."));
    }
    else
    {
        if(db_->modelCounterpartyType()->addType(ui->lineEditType->text()))
        {
            QMessageBox::information(this, title,
                                     trUtf8("Dodawanie typu zakończyło się sukcesem."));
            return true;
        }
    }
    return false;
}

bool CounterpartyTypeDialog::delType_()
{
    const QString title(trUtf8("Usuwanie typu"));
    if(!ui->listViewTypes->currentIndex().isValid())
    {
        QMessageBox::information(this, title,
                                 trUtf8("Wybierz typ do usunięcia."));
    }
    else
    {
        if(db_->modelCounterpartyType()->delType(ui->listViewTypes->currentIndex().data().toString()))
        {
            QMessageBox::information(this, title,
                                     trUtf8("Usunięcie typu zakończyło się sukcesem."));
            return true;
        }
    }
    return false;
}
