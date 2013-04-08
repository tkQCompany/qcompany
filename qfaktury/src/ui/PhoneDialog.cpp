#include <QSqlRelationalDelegate>
#include <QSqlError>
#include <QMenu>
#include <QContextMenuEvent>

#include "PhoneDialog.h"
#include "ui_PhoneDialog.h"
#include "Database.h"
#include "ModelPhone.h"
#include "PhoneData.h"

PhoneDialog::PhoneDialog(QWidget *parent, Database *db, const QModelIndex &id_counterparty) :
    QDialog(parent), ui(new Ui::PhoneDialog), db_(db)
{
    ui->setupUi(this);
    if(id_counterparty.isValid())
    {
        db_->modelPhone()->setIDCounterparty(id_counterparty.data().toString());
    }

    ui->tableView->setModel(db_->modelPhone());
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(PhoneFields::ID_PHONE);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
    db_->modelPhone()->select();

    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editList_(QModelIndex)));
}

PhoneDialog::~PhoneDialog()
{
    delete ui;
}


void PhoneDialog::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(ui->actionAddPhone);
    menu.addAction(ui->actionRemovePhone);
    menu.exec(ui->tableView->mapToGlobal(event->pos()));
    event->accept();
}


void PhoneDialog::editList_(QModelIndex index)
{
    ui->tableView->edit(index);
}

void PhoneDialog::on_actionAddPhone_triggered()
{
    if(db_->modelPhone()->insertRow(db_->modelPhone()->rowCount()))
    {
        ui->tableView->setCurrentIndex(db_->modelPhone()->index(db_->modelPhone()->rowCount() - 1, 0));
        ui->tableView->selectionModel()->select(ui->tableView->currentIndex(), QItemSelectionModel::Rows);
        ui->tableView->edit(ui->tableView->currentIndex());
    }
    else
    {
        qDebug() << "PhoneDialog::on_actionAddPhone_triggered()" << db_->modelPhone()->lastError().text();
    }
}

void PhoneDialog::on_actionRemovePhone_triggered()
{
    const QModelIndexList list(ui->tableView->selectionModel()->selectedIndexes());
    if(!list.empty())
        db_->modelPhone()->removeRow(list.at(0).row());
}
