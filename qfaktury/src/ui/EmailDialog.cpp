#include <QContextMenuEvent>
#include <QMenu>
#include <QSqlRelationalDelegate>
#include <QSqlError>

#include "EmailDialog.h"
#include "ui_EmailDialog.h"
#include "Database.h"
#include "ModelEmail.h"
#include "EmailData.h"

EmailDialog::EmailDialog(QWidget *parent, Database *db, const QModelIndex &id_counterparty) :
    QDialog(parent), ui(new Ui::EmailDialog), db_(db)
{
    ui->setupUi(this);
    ui->tableView->setModel(db_->modelEmail());
    ui->tableView->hideColumn(EmailFields::ID_EMAIL);
    if(id_counterparty.isValid())
        db_->modelEmail()->setFilter(QString("`id_counterparty` = %1").arg(id_counterparty.data().toString()));
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
    db_->modelEmail()->select();

    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editList_(QModelIndex)));
}


EmailDialog::~EmailDialog()
{
    delete ui;
}




void EmailDialog::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction(ui->actionAddEmail);
    menu.addAction(ui->actionRemoveEmail);
    menu.exec(ui->tableView->mapToGlobal(event->pos()));
    event->accept();
}

void EmailDialog::editList_(QModelIndex index)
{
    ui->tableView->edit(index);
}


void EmailDialog::on_actionAddEmail_triggered()
{
    if(db_->modelEmail()->insertRow(db_->modelEmail()->rowCount()))
    {
        ui->tableView->setCurrentIndex(db_->modelEmail()->index(db_->modelEmail()->rowCount() - 1, 0));
        ui->tableView->selectionModel()->select(ui->tableView->currentIndex(), QItemSelectionModel::Rows);
        ui->tableView->edit(ui->tableView->currentIndex());
    }
    else
    {
        qDebug() << "EmailDialog::on_actionAddEmail_triggered()" << db_->modelEmail()->lastError().text();
    }
}

void EmailDialog::on_actionRemoveEmail_triggered()
{
    const QModelIndexList list(ui->tableView->selectionModel()->selectedIndexes());
    if(!list.empty())
        db_->modelEmail()->removeRow(list.at(0).row());
}
