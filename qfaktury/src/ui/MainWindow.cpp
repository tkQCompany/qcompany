#include <QProcess>
#include <QSqlRelationalDelegate>
#include <QSqlQuery>
#include <QSqlError>
#include <QKeyEvent>
#include <QTranslator>
#include <QDesktopServices>
#include <QDir>
#include <QMessageBox>
#include <QUrl>
#include <QTextStream>
#include <QDebug>

#include "MainWindow.h"
#include "CounterpartyDialog.h"
#include "InvoiceGrossDialog.h"
#include "CorrectiveInvoiceDialog.h"
#include "CorrectiveInvoiceGrossDialog.h"
#include "DuplicateDialog.h"
#include "BillDialog.h"
#include "SettingsDialog.h"
#include "CommodityDialog.h"
#include "ModelInvoice.h"
#include "ModelCounterparty.h"
#include "ModelCommodity.h"
#include "ModelInvoiceType.h"
#include "ModelCounterpartyType.h"

/**
 * @brief Default constructor
 *
 * @param parent Pointer to Qt's parent widget
 */
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui_(new Ui::MainWindow())
{
    ui_->setupUi(this);
    init_();
}

MainWindow::~MainWindow()
{
    if(commodityDialogPtr_)
        delete commodityDialogPtr_;

    if(counterpartyDialogPtr_)
        delete counterpartyDialogPtr_;

    if(invoiceDialogPtr_)
        delete invoiceDialogPtr_;

    delete ui_;
}


/**
 * @brief
 *
 */
void MainWindow::init_()
{
    SettingsGlobal s;

    retranslateUi_();

    ui_->dateEditFilterStart->setDisplayFormat(s.dateFormatExternal());
    ui_->dateEditFilterEnd->setDisplayFormat(s.dateFormatExternal());
    ui_->dateEditFilterStart->setDate(QDate::currentDate());
    ui_->dateEditFilterEnd->setDate(QDate::currentDate());
    db_.modelInvoice()->setDataRange(QDate(), QDate());//full range

    ui_->tableViewCounterparties->setModel(db_.modelCounterparty());
    ui_->tableViewCounterparties->setItemDelegate(new QSqlRelationalDelegate(ui_->tableViewCounterparties));
    ui_->tableViewCounterparties->hideColumn(0);

    ui_->tableViewCommodities->setModel(db_.modelCommodity());
    ui_->tableViewCommodities->setItemDelegate(new QSqlRelationalDelegate(ui_->tableViewCommodities));
    ui_->tableViewCommodities->hideColumn(0);

    ui_->tableViewInvoices->setModel(db_.modelInvoice());
    ui_->tableViewInvoices->setItemDelegate(new QSqlRelationalDelegate(ui_->tableViewInvoices));
    ui_->tableViewInvoices->hideColumn(0);

    #if QT_VERSION < 0x050000
    ui->tableViewCounterparties->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewCommodities->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewInvoices->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    #else
    ui_->tableViewCounterparties->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui_->tableViewCommodities->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui_->tableViewInvoices->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    #endif

    ui_->tableViewCommodities->horizontalHeader()->setStretchLastSection(true);
    ui_->tableViewCounterparties->horizontalHeader()->setStretchLastSection(true);
    ui_->tableViewInvoices->horizontalHeader()->setStretchLastSection(true);

    this->setWindowTitle(QString("%1 - %2").arg(qApp->applicationName()).arg(qApp->applicationVersion()));

    // connect slots
    connect(ui_->actionHelp_BugReport, SIGNAL(triggered()), this, SLOT(reportBug_()));
    connect(ui_->toolButtonApply, SIGNAL (clicked()), this, SLOT(reReadInvHistory_()));
    connect(ui_->actionProgram_CompanyInfo, SIGNAL(triggered()), this, SLOT(editMyCompanyInfo()));
    connect(ui_->actionProgram_Exit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui_->actionCounterparties_Add, SIGNAL(triggered()), this, SLOT(addCounterparty_()));
    connect(ui_->actionCounterparties_Remove, SIGNAL(triggered()), this, SLOT(delCounterparty_()));
    connect(ui_->actionCounterparties_Edit, SIGNAL(triggered()), this, SLOT(editCounterparty_()));
    connect(ui_->actionInvoices_InvoiceNew, SIGNAL(triggered()), this, SLOT(newInvoice_()));
    connect(ui_->actionInvoices_Remove, SIGNAL(triggered()), this, SLOT(delInvoice_()));
    connect(ui_->actionInvoices_Edit, SIGNAL(triggered()), this, SLOT(editInvoice_()));
    connect(ui_->actionInvoices_InvoiceDuplicate, SIGNAL(triggered()), this, SLOT(newDuplicate_()));
    connect(ui_->actionInvoices_InvoiceGross, SIGNAL(triggered()), this, SLOT(newGrossInvoice_()));
    connect(ui_->actionInvoices_Bill, SIGNAL(triggered()), this, SLOT(newBill_()));
    connect(ui_->actionInvoices_InvoiceCorrective, SIGNAL(triggered()), this, SLOT(newCorrection_()));
    connect(ui_->actionInvoices_InvoiceProForma, SIGNAL(triggered()), this, SLOT(newProFormaInvoice_()));
    connect(ui_->actionCommodities_Add, SIGNAL(triggered()), this, SLOT(addCommodity_()));
    connect(ui_->actionCommodities_Edit, SIGNAL(triggered()), this, SLOT(editCommodity_()));
    connect(ui_->actionCommodities_Remove, SIGNAL(triggered()), this, SLOT(delCommodity_()));
    connect(ui_->actionHelp_AboutQt, SIGNAL(triggered()), this, SLOT(aboutQt_()));
    connect(ui_->actionHelp_About, SIGNAL(triggered()), this, SLOT(about_()));
    connect(ui_->actionProgram_Settings, SIGNAL(triggered()), this, SLOT(editSettings_()));
    connect(ui_->tabWidgetMain, SIGNAL(currentChanged(int)), this, SLOT(tabChanged_(int)));
    connect(ui_->actionHelp_Help, SIGNAL(triggered()), this, SLOT(help_()));
    connect(ui_->tableViewInvoices, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editInvoice_()));
    connect(ui_->tableViewInvoices, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTableMenuHistory_(QPoint)));
    connect(ui_->tableViewCounterparties, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editCounterparty_()));
    connect(ui_->tableViewCounterparties, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTableMenuCounterparties_(QPoint)));
    connect(ui_->tableViewCommodities, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editCommodity_()));
    connect(ui_->tableViewCommodities, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTableMenuGoods_(QPoint)));

    connect(ui_->tableViewInvoices, SIGNAL(clicked(QModelIndex)), this, SLOT(mainUpdateStatus_(QModelIndex)));
    connect(ui_->tableViewCounterparties, SIGNAL(clicked(QModelIndex)), this, SLOT(mainUpdateStatus_(QModelIndex)));
    connect(ui_->tableViewCommodities, SIGNAL(clicked(QModelIndex)), this, SLOT(mainUpdateStatus_(QModelIndex)));

    tabChanged_(ui_->tabWidgetMain->currentIndex());
    loadPlugins();
}



/**
 * @brief
 *
 */
void MainWindow::loadPlugins()
{
    QDir allFiles;

    const QString path("/plugins/");
    allFiles.setPath(path);
    allFiles.setFilter(QDir::Files);
    QStringList filters;
    filters << "*.py" << "*.Py" << "*.PY" << "*.pY";
    allFiles.setNameFilters(filters);
    const QStringList files(allFiles.entryList());
    const int max = files.count();
    for (int i = 0; i < max; ++i)
    {
        QFile skrypt(path + allFiles[i]);
        if (!skrypt.open(QIODevice::ReadOnly))
        {
            return;
        }
        else
        {
            QTextStream t(&skrypt);
            t.readLine();
            QAction *action = new QAction(t.readLine().remove ("# "), this);
            action->setData(QVariant(i));
            connect(action, SIGNAL(triggered()), this, SLOT (pluginSlot_()));
            ui_->menuPlugins->addAction(action);
            customActions_[i] = path + allFiles[i];
        }
    }
    ui_->menuPlugins->addSeparator();
    ui_->menuPlugins->addAction(trUtf8("Informacje"), this, SLOT (pluginInfoSlot_()));
}



/**
 * @brief
 *
 * @param type
 */
void MainWindow::createInvoice_(const InvoiceTypeData::Type type)
{
    switch(type)
    {
    case InvoiceTypeData::VAT:
    case InvoiceTypeData::PRO_FORMA:
        invoiceDialogPtr_ = new InvoiceDialog(this, &db_, type);
        break;
    case InvoiceTypeData::BILL:
        invoiceDialogPtr_ = new BillDialog(this, &db_);
        break;
    case InvoiceTypeData::GROSS:
        invoiceDialogPtr_ = new InvoiceGrossDialog(this, &db_);
        break;
    default:
        return;
    }

    if(invoiceDialogPtr_)
    {
        if(QDialog::Accepted == invoiceDialogPtr_->exec())
        {
            ui_->tableViewInvoices->selectionModel()->setCurrentIndex(db_.modelInvoice()->index(db_.modelInvoice()->rowCount() - 1,
                                                                           InvoiceFields::ID_INVOICE), QItemSelectionModel::Rows | QItemSelectionModel::Select);
            ui_->dateEditFilterStart->setDate(QDate::currentDate()); //otherwise our new invoice couldn't be shown
            ui_->dateEditFilterEnd->setDate(QDate::currentDate());
        }
        db_.modelInvoice()->revertAll();
    }
}


Database* MainWindow::database()
{
    return &db_;
}


/**
 * @brief
 *
 * @param counterpartiesEditEnabled
 * @param counterpartiesRemoveEnable
 * @param commoditiesEditEnabled
 * @param commoditiesRemoveEnabled
 * @param historyEditEnabled
 * @param historyRemoveEnabled
 */
void MainWindow::setActions_(const bool counterpartiesEditEnabled, const bool counterpartiesRemoveEnable,
                            const bool commoditiesEditEnabled, const bool commoditiesRemoveEnabled,
                            const bool historyEditEnabled, const bool historyRemoveEnabled)
{
    ui_->actionCounterparties_Edit->setEnabled(counterpartiesEditEnabled);
    ui_->actionCounterparties_Remove->setEnabled(counterpartiesRemoveEnable);
    ui_->actionCommodities_Edit->setEnabled(commoditiesEditEnabled);
    ui_->actionCommodities_Remove->setEnabled(commoditiesRemoveEnabled);
    ui_->actionInvoices_Edit->setEnabled(historyEditEnabled);
    ui_->actionInvoices_Remove->setEnabled(historyRemoveEnabled);
}



// ----------------------------------------  SLOTS ---------------------------------//

/**
 * @brief
 *
 * @param event
 */
void MainWindow::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Return)
    {
        switch (ui_->tabWidgetMain->currentIndex())
        {
        case 0:
            editInvoice_();
            break;
        case 1:
            editCounterparty_();
            break;
        case 2:
            editCommodity_();
            break;
        }
    }
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_PageUp)
    {
        if (ui_->tabWidgetMain->currentIndex() != ui_->tabWidgetMain->count() - 1)
        {
            ui_->tabWidgetMain->setCurrentIndex(ui_->tabWidgetMain->currentIndex() + 1);
        }
        else
        {
            ui_->tabWidgetMain->setCurrentIndex(0);
        }
    }
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_PageDown)
    {
        if (ui_->tabWidgetMain->currentIndex() != 0)
        {
            ui_->tabWidgetMain->setCurrentIndex(ui_->tabWidgetMain->currentIndex() - 1);
        }
        else
        {
            ui_->tabWidgetMain->setCurrentIndex(ui_->tabWidgetMain->count() - 1);
        }
    }
}



/**
 * @brief
 *
 */
void MainWindow::pluginInfoSlot_()
{
    QMessageBox::information(this, qApp->applicationName(),
                             trUtf8("To menu służy do obsługi pluginów pythona,\nnp. archiwizacji danych, generowania raportów etc.\n\n"
                                    "Skrypty Pythona sa czytane z folderu \"~/elinux/plugins/\"."));
}


void MainWindow::reReadInvHistory_()
{
    db_.modelInvoice()->setDataRange(ui_->dateEditFilterStart->date(), ui_->dateEditFilterEnd->date());
}


void MainWindow::retranslateUi_()
{
    QTranslator appTranslator;
    SettingsGlobal s;
    appTranslator.load(QString(":/res/translations/qfaktury_") + s.value(s.LANG).toString());
    qApp->installTranslator(&appTranslator);
    ui_->retranslateUi(this);
}


/**
 * @brief
 *
 */
void MainWindow::pluginSlot_()
{
    const QAction *a = static_cast<QAction *> (this->sender());

    const int scriptId = a->data().toInt();

    QStringList args;
    args += customActions_[scriptId];
    args += QString("%1").arg(winId());

    QProcess cmd(this);
    const QString program("python");
    cmd.start(program, args);
    if (!cmd.waitForStarted())
    {
        QMessageBox::information(this, qApp->applicationName(), trUtf8("Uruchomienie się nie powiodło."), QMessageBox::Ok);
    }
}



/**
 * @brief
 *
 * @param p
 */
void MainWindow::showTableMenuGoods_(const QPoint &p) const
{
    QMenu menutableViewGoods(ui_->tableViewCommodities);
    menutableViewGoods.addAction(ui_->actionCommodities_Add);
    menutableViewGoods.addAction(ui_->actionCommodities_Remove);
    menutableViewGoods.addAction(ui_->actionCommodities_Edit);
    menutableViewGoods.exec(ui_->tableViewCommodities->mapToGlobal(p));
}



/**
 * @brief
 *
 * @param p
 */
void MainWindow::showTableMenuCounterparties_(const QPoint &p) const
{
    QMenu menuTableCounterparties(ui_->tableViewCounterparties);
    menuTableCounterparties.addAction(ui_->actionCounterparties_Add);
    menuTableCounterparties.addAction(ui_->actionCounterparties_Remove);
    menuTableCounterparties.addAction(ui_->actionCounterparties_Edit);
    menuTableCounterparties.exec(ui_->tableViewCounterparties->mapToGlobal(p));
}



/**
 * @brief
 *
 * @param p
 */
void MainWindow::showTableMenuHistory_(const QPoint &p) const
{
    QMenu menuTableInvoices(ui_->tableViewInvoices);
    menuTableInvoices.addAction(ui_->actionInvoices_InvoiceNew);
    menuTableInvoices.addAction(ui_->actionInvoices_InvoiceGross);
    menuTableInvoices.addAction(ui_->actionInvoices_Bill);
    menuTableInvoices.addSeparator();
    menuTableInvoices.addAction(ui_->actionInvoices_InvoiceProForma);
    menuTableInvoices.addAction(ui_->actionInvoices_InvoiceCorrective);
    menuTableInvoices.addAction(ui_->actionInvoices_InvoiceDuplicate);
    menuTableInvoices.addSeparator();
    menuTableInvoices.addAction(ui_->actionInvoices_Edit);
    menuTableInvoices.addAction(ui_->actionInvoices_Remove);
    menuTableInvoices.exec(ui_->tableViewInvoices->mapToGlobal(p));
}



/**
 * @brief
 *
 * @param index
 */
void MainWindow::mainUpdateStatus_(QModelIndex index)
{
    const QString message(QString("%1 : %2, %3 : %4, %5 : %6")
                          .arg(index.model()->headerData(1, Qt::Horizontal).toString())
                          .arg(index.model()->data(index.model()->index(index.row(), 1)).toString())
                          .arg(index.model()->headerData(2, Qt::Horizontal).toString())
                          .arg(index.model()->data(index.model()->index(index.row(), 2)).toString())
                          .arg(index.model()->headerData(3, Qt::Horizontal).toString())
                          .arg(index.model()->data(index.model()->index(index.row(), 3)).toString()));

    this->QMainWindow::statusBar()->showMessage(trUtf8("Wybrana pozycja: %1").arg(message));

}

 //Slot which enables/disables menu. It's possible to add/remove goods/customers
  // only if this is the current tab.

/**
 * @brief
 *
 * @param widget
 */
void MainWindow::tabChanged_(int index)
{
    // disable Edit and Remove actions _ONLY_
    switch (index)
    {
    case 1: //counterparties
        setActions_(true, true, false, false, false, false);
        break;
    case 2: //commodities
        setActions_(false, false, true, true, false, false);
        break;
    default:// invoices
        setActions_(false, false, false, false, true, true);
        break;
    }
}



/**
 * @brief
 *
 */
void MainWindow::aboutQt_()
{
    QMessageBox::aboutQt(this, QString("%1 - %2").arg(qApp->applicationName()).arg(qApp->applicationVersion()));
}


/**
 * @brief
 *
 */
void MainWindow::about_()
{
    const QString msgAbout(trUtf8("Program do wystawiania faktur.\n\n%1 - %2\n\n"
                                  "Koordynator projektu:\n\tGrzegorz Rękawek\t\t\n\n"
                                  "Programiści: \n\tTomasz Pielech"
                                  "\n\tRafał Rusin http://people.apache.org/~rr/"
                                  "\n\tSławomir Patyk"
                                  "\n\tTomasz Krzal"
                                  "\n\nIkony:"
                                  "\n\tDariusz Arciszewski\n\n"
                                  "UWAGA!!!\n"
                                  "Ten program komputerowy dostarczany jest przez autora w formie \"takiej, jaki jest\". "
                                  "Autor nie udziela żadnej gwarancji oraz rękojmi, że program będzie działał "
                                  "prawidłowo, jest odpowiedniej jakości oraz że spełni oczekiwania "
                                  "użytkownika. Autor nie odpowiada za jakiekolwiek straty wynikające z użytkowania "
                                  "programu, w tym utraty spodziewanych korzyści, danych, informacji "
                                  "gospodarczych lub koszt urządzeń lub programów zastępczych.")
                           .arg(qApp->applicationName()).arg(qApp->applicationVersion()));
    QMessageBox::about(this, trUtf8("O programie"), msgAbout);
}


/**
 * @brief
 *
 */
void MainWindow::editInvoice_()
{
    const QModelIndexList list(ui_->tableViewInvoices->selectionModel()->selectedIndexes());
    if(list.size() <= 0)
    {
        QMessageBox::information(this, qApp->applicationName(), trUtf8("Żadna faktura nie jest wybrana.") + QChar(' ') + trUtf8("Nie można edytować."));
        return;
    }

    const int firstInvoice = 0;
    const QModelIndex indexSel(list.at(firstInvoice));
    const int invType = db_.modelInvoice()->data(db_.modelInvoice()->index(indexSel.row(), InvoiceFields::TYPE_ID)).toInt();

    switch(invType)
    {
    case InvoiceTypeData::VAT:
    {
        invoiceDialogPtr_ = new InvoiceDialog(this, &db_, InvoiceTypeData::VAT, indexSel);
        invoiceDialogPtr_->exec();
    }
        break;
    case InvoiceTypeData::PRO_FORMA:
    {
        invoiceDialogPtr_ = new InvoiceDialog(this, &db_, InvoiceTypeData::PRO_FORMA, indexSel);
        invoiceDialogPtr_->exec();
    }
        break;
    case InvoiceTypeData::CORRECTIVE_VAT:
    {
        invoiceDialogPtr_ = new CorrectiveInvoiceDialog(this, &db_, InvoiceTypeData::CORRECTIVE_VAT, indexSel);
        invoiceDialogPtr_->exec();  // edit window shouln't return anything
    }
        break;
    case InvoiceTypeData::GROSS:
    {
        invoiceDialogPtr_ = new InvoiceGrossDialog(this, &db_, QModelIndex());
        invoiceDialogPtr_->exec();
    }
        break;
    case InvoiceTypeData::CORRECTIVE_GROSS:
    {
        invoiceDialogPtr_ = new CorrectiveInvoiceGrossDialog(this, &db_, indexSel);
        invoiceDialogPtr_->exec(); // edit window shouln't return anything
    }
        break;
    case InvoiceTypeData::BILL:
    {
        invoiceDialogPtr_ = new BillDialog(this, &db_);
        invoiceDialogPtr_->exec(); // edit window shouln't return anything
    }
        break;
    default:
        qDebug("MainWindow::editInvoice() switch: unexpected value: %d", invType);
        break;
    }
}


/**
 * @brief
 *
 */
void MainWindow::delInvoice_()
{
    if(ui_->tableViewInvoices->selectionModel()->selectedRows().length() != 1)
    {
        QMessageBox::information(this, qApp->applicationName(), trUtf8("Żadna faktura nie jest wybrana.") + QChar(' ') + trUtf8("Nie można usuwać."));
    }
    else
    {
        if (QMessageBox::warning(this, QString("%1 - %2").arg(qApp->applicationName()).arg(qApp->applicationVersion()), trUtf8("Czy na pewno chcesz usunąć tę fakturę?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            const QString title("Usuwanie faktury");
            if(db_.modelInvoice()->removeRow(ui_->tableViewInvoices->currentIndex().row()))
            {
                QMessageBox::information(this, title, trUtf8("Wybrana faktura została usunięta"));
                db_.modelInvoice()->submitAll();
            }
            else
            {
                QMessageBox::warning(this, title, trUtf8("Wybrana faktura NIE została usunięta"));
            }
        }
    }
}



/**
 * @brief
 *
 */
void MainWindow::editMyCompanyInfo()
{
    db_.modelCounterparty()->setOnlyMyCompanyVisible(true);
    db_.modelCounterpartyType()->setMyCompanyVisible(true, true);

    CounterpartyDialog dialog(this, &db_, db_.modelCounterparty()->index(0, CounterpartyFields::ID), true);
    const QString title("Edycja mojej firmy");
    dialog.setWindowTitle(title);
    if (dialog.exec() == QDialog::Accepted)
    {
        if(db_.modelCounterparty()->submitAll())
        {
            QMessageBox::information(this, title, trUtf8("Edycja danych firmy zakończyła się sukcesem."));
        }
        else
        {
            QMessageBox::warning(this, title, db_.modelCounterparty()->lastError().text());
        }
    }

    db_.modelCounterparty()->setOnlyMyCompanyVisible(false);
    db_.modelCounterpartyType()->setMyCompanyVisible(false);
}



/**
 * @brief
 *
 */
void MainWindow::editSettings_()
{
    SettingsDialog settWindow(this, &db_);
    if(QDialog::Accepted == settWindow.exec())
    {
        retranslateUi_();
    }
}



/**
 * @brief
 *
 */
void MainWindow::addCounterparty_()
{
    QScopedPointer<CounterpartyDialog> dialog(new CounterpartyDialog(this, &db_));
    counterpartyDialogPtr_ = dialog.data();
    if (dialog->exec() == QDialog::Accepted)
    {
        if(db_.modelCounterparty()->submitAll())
        {
            ui_->tableViewCounterparties->selectionModel()->setCurrentIndex(db_.modelCounterparty()->index(db_.modelCounterparty()->rowCount() - 1,
                                                                               CounterpartyFields::ID), QItemSelectionModel::Rows | QItemSelectionModel::Select);
        }
        else
        {
            QMessageBox::critical(this, trUtf8("Błąd w dodawaniu kontrahenta"), db_.modelCounterparty()->lastError().text());
            db_.modelCounterparty()->revertAll();
        }
    }
}



/**
 * @brief
 *
 */
void MainWindow::delCounterparty_()
{
    if(ui_->tableViewCounterparties->selectionModel()->selectedRows().length() != 1)
    {
        QMessageBox::information(this, trUtf8("Niewybrany wiersz"), trUtf8("Wybierz tylko jednego kontrahenta do skasowania"));
    }
    else
    {
        if (QMessageBox::warning(this, qApp->applicationName(), trUtf8("Czy na pewno chcesz usunąć kontrahenta: %1?")
                                 .arg(db_.modelCounterparty()->index(ui_->tableViewCounterparties->currentIndex().row(), CounterpartyFields::NAME).data().toString()),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            const QString title("Usuwanie kontrahenta");
            if(db_.modelCounterparty()->removeRow(ui_->tableViewCounterparties->currentIndex().row()))
            {
                QMessageBox::information(this, title, trUtf8("Wybrany kontrahent został usunięty"));
                db_.modelCounterparty()->submitAll();
            }
            else
            {
                QMessageBox::warning(this, title, trUtf8("Wybrany kontrahent NIE został usunięty"));
            }
        }
    }
}




/**
 * @brief
 *
 */
void MainWindow::editCounterparty_()
{
    if(ui_->tableViewCounterparties->selectionModel()->selectedRows().length() != 1)
    {
        QMessageBox::information(this, trUtf8("Niewybrany wiersz"), trUtf8("Wybierz tylko jednego kontrahenta do edycji"));
    }
    else
    {
        const QModelIndexList list(ui_->tableViewCounterparties->selectionModel()->selectedRows(CounterpartyFields::ID));
        if(list.size() > 0)
        {
            CounterpartyDialog dialog(this, &db_, list.at(0));
            const QString errorTitle(trUtf8("Błąd edycji kontrahenta"));
            if (dialog.exec() == QDialog::Accepted)
            {
                if(db_.modelCounterparty()->submitAll())
                {
                    QMessageBox::information(this, trUtf8("Edycja kontrahenta"), trUtf8("Edycja kontrahenta zakończyło się sukcesem"));
                }
                else
                {
                    QMessageBox::critical(this, errorTitle, db_.modelCounterparty()->lastError().text());
                }
            }
        }
    }
}



/**
 * @brief
 *
 */
void MainWindow::newInvoice_()
{
    createInvoice_(InvoiceTypeData::VAT);
}



/**
 * @brief
 *
 */
void MainWindow::newBill_()
{
    createInvoice_(InvoiceTypeData::BILL);
}



/**
 * @brief
 *
 */
void MainWindow::newGrossInvoice_()
{
    createInvoice_(InvoiceTypeData::GROSS);
}



/**
 * @brief
 *
 */
void MainWindow::newProFormaInvoice_()
{
    createInvoice_(InvoiceTypeData::PRO_FORMA);
}


/**
 * @brief
 *
 */
void MainWindow::newCorrection_()
{//TODO: remove redundancy here and in edit* methods (hint: areTableItemsSelected())
    const QModelIndexList list(ui_->tableViewInvoices->selectionModel()->selectedIndexes());
    if(list.size() <= 0)
    {
        QMessageBox::information(this, qApp->applicationName(), trUtf8("Żadna faktura nie jest wybrana.") + QChar(' ') + trUtf8("Nie można edytować."));
        return;
    }

    const QString invTypeStr(db_.modelInvoice()->data(db_.modelInvoice()->
                             index(list.at(0).row(), InvoiceFields::TYPE_ID)).
                             toString());
    const int invType = InvoiceTypeData::stringToInvoiceType(invTypeStr);
    switch(invType)
    {
    case InvoiceTypeData::VAT:
    {
        CorrectiveInvoiceDialog dialog(this, &db_, InvoiceTypeData::CORRECTIVE_VAT);
        dialog.exec();
    }
        break;
    case InvoiceTypeData::GROSS:
    {
        CorrectiveInvoiceGrossDialog dialog(this, &db_, db_.modelInvoice()->
                            index(list.at(0).row(), InvoiceFields::ID_INVOICE));
        dialog.exec();
    }
        break;
    default:
        QMessageBox::information(this, qApp->applicationName(),
                                 trUtf8("Do faktury typu %1 nie wystawiamy korekt.")
                                 .arg(invTypeStr),
                                 QMessageBox::Ok);
        break;
    }
}


/**
 * @brief
 *
 */
void MainWindow::newDuplicate_()
{//TODO: remove redundancy here and in edit* methods (hint: areTableItemsSelected())
    const QModelIndexList list(ui_->tableViewInvoices->selectionModel()->selectedIndexes());
    if(list.size() <= 0)
    {
        QMessageBox::information(this, qApp->applicationName(), trUtf8("Żadna faktura nie jest wybrana.") + QChar(' ') + trUtf8("Nie można edytować."));
        return;
    }

    const QString invTypeStr(db_.modelInvoice()->data(db_.modelInvoice()->
                             index(list.at(0).row(), InvoiceFields::TYPE_ID)).
                             toString());
    const int invType = InvoiceTypeData::stringToInvoiceType(invTypeStr);
    switch(invType)
    {
    case InvoiceTypeData::VAT:
    {
        DuplicateDialog dialog(this, &db_, InvoiceTypeData::VAT);
        dialog.exec(); // not saving duplicates
    }
        break;
    case InvoiceTypeData::GROSS:
    {
        DuplicateDialog dialog(this, &db_, InvoiceTypeData::GROSS);
        dialog.exec(); // not saving duplicates
    }
        break;
    default:
        QMessageBox::information(this, qApp->applicationName(),
                                 trUtf8("Do faktury typu %1 nie wystawiamy duplikatów.")
                                 .arg(invTypeStr),
                                 QMessageBox::Ok);
        break;
    }
}


/**
 * @brief
 *
 */
void MainWindow::addCommodity_()
{
    QScopedPointer<CommodityDialog> dialog(new CommodityDialog(this, &db_));
    commodityDialogPtr_ = dialog.data();
    if (dialog->exec() == QDialog::Accepted)
    {
        if(db_.modelCommodity()->submitAll())
        {
            ui_->tableViewCommodities->selectionModel()->setCurrentIndex(db_.modelCommodity()->index(db_.modelCommodity()->rowCount() - 1,
                                                                               CommodityFields::ID_COMMODITY), QItemSelectionModel::Rows | QItemSelectionModel::Select);
        }
        else
        {
            QMessageBox::warning(this, trUtf8("Błąd w dodawaniu towaru"), db_.modelCommodity()->lastError().text());
        }
    }
}


/**
 * @brief
 *
 */
void MainWindow::delCommodity_()
{
    if(ui_->tableViewCommodities->selectionModel()->selectedRows().length() != 1)
    {
        QMessageBox::information(this, trUtf8("Niewybrany wiersz"), trUtf8("Wybierz tylko jeden towar do skasowania"));
    }
    else
    {
        if (QMessageBox::warning(this, qApp->applicationName(), trUtf8("Czy na pewno chcesz usunąć towar: %1?")
                                 .arg(db_.modelCommodity()->index(ui_->tableViewCommodities->currentIndex().row(),
                                 CommodityFields::NAME).data().toString()), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            const QString title("Usuwanie towaru");
            if(db_.modelCommodity()->removeRows(ui_->tableViewCommodities->currentIndex().row(), 1))
            {
                qDebug() << "Sql error: " << db_.modelCommodity()->lastError().text();
                db_.modelCommodity()->submitAll();
                QMessageBox::information(this, title, trUtf8("Wybrany towar został usunięty"));
            }
            else
            {
                db_.modelCommodity()->revertAll();
                QMessageBox::warning(this, title, trUtf8("Wybrany towar NIE został usunięty"));
            }
        }
    }
}


/**
 * @brief
 *
 */
void MainWindow::editCommodity_()
{
    if(ui_->tableViewCommodities->selectionModel()->selectedRows().length() != 1)
    {
        QMessageBox::information(this, trUtf8("Niewybrany wiersz"), trUtf8("Wybierz tylko jeden towar do edycji"));
    }
    else
    {
        const QModelIndex index = ui_->tableViewCommodities->selectionModel()->selectedRows(CommodityFields::ID_COMMODITY).at(0);
        CommodityDialog dialog(this, &db_, index);
        const QString errorTitle(trUtf8("Błąd edycji towaru"));
        if (dialog.exec() == QDialog::Accepted)
        {
            if(!db_.modelCommodity()->submitAll())
            {
                QMessageBox::warning(this, errorTitle, db_.modelCommodity()->lastError().text());
                db_.modelCommodity()->revertAll();
            }
        }
    }
}


/**
 * @brief
 *
 */
void MainWindow::help_() const
{
    QDesktopServices::openUrl(QUrl("http://www.e-linux.pl/"));
}


/**
 * @brief
 *
 */
void MainWindow::reportBug_() const
{
    QDesktopServices::openUrl(QUrl("https://sourceforge.net/tracker2/?func=add&group_id=154610&atid=792471"));
}
