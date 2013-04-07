#include <QProcess>

#include "MainWindow.h"
#include "CounterpartyDialog.h"
#include "CorrectiveInvoiceDialog.h"


/**
 * @brief Default constructor
 *
 * @param parent Pointer to Qt's parent widget
 */
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    setupUi(this);
    init_();
}


/**
 * @brief
 *
 */
void MainWindow::init_()
{
    SettingsGlobal s;

    retranslateUi_();

    if (firstRun_())
    {
        s.resetSettings();
        editMyCompanyInfo_();
    }
    else
    {
        dateEditFilterStart->setDisplayFormat(s.dateFormatExternal());
        dateEditFilterEnd->setDisplayFormat(s.dateFormatExternal());
    }

    dateEditFilterStart->setDate(QDate::currentDate());
    dateEditFilterEnd->setDate(QDate::currentDate());
    db_.modelInvoice()->setDataRange(QDate(), QDate());//full range

    tableViewCounterparties->setModel(db_.modelCounterparty());
    tableViewCounterparties->setItemDelegate(new QSqlRelationalDelegate(tableViewCounterparties));
    tableViewCounterparties->hideColumn(0);
    tableViewCounterparties->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    tableViewCommodities->setModel(db_.modelCommodity());
    tableViewCommodities->setItemDelegate(new QSqlRelationalDelegate(tableViewCommodities));
    tableViewCommodities->hideColumn(0);
    tableViewCommodities->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    tableViewInvoices->setModel(db_.modelInvoice());
    tableViewInvoices->setItemDelegate(new QSqlRelationalDelegate(tableViewInvoices));
    tableViewInvoices->hideColumn(0);
    tableViewInvoices->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    this->setWindowTitle(QString("%1 - %2").arg(qApp->applicationName()).arg(qApp->applicationVersion()));

    // connect slots
    connect(actionHelp_BugReport, SIGNAL (activated ()), this, SLOT(reportBug_()));
    connect(toolButtonApply, SIGNAL (clicked()), this, SLOT(reReadInvHistory_()));
    connect(actionProgram_CompanyInfo, SIGNAL(activated()), this, SLOT(editMyCompanyInfo_()));
    connect(actionProgram_Exit, SIGNAL(activated()), this, SLOT(close()));
    connect(actionCounterparties_Add, SIGNAL(activated()), this, SLOT(addCounterparty_()));
    connect(actionCounterparties_Remove, SIGNAL(activated()), this, SLOT(delCounterparty_()));
    connect(actionCounterparties_Edit, SIGNAL(activated()), this, SLOT(editCounterparty_()));
    connect(actionInvoices_InvoiceNew, SIGNAL(activated()), this, SLOT(newInvoice_()));
    connect(actionInvoices_Remove, SIGNAL(activated()), this, SLOT(delInvoice_()));
    connect(actionInvoices_Edit, SIGNAL(triggered()), this, SLOT(editInvoice_()));
    connect(actionInvoices_InvoiceDuplicate, SIGNAL(activated()), this, SLOT(newDuplicate_()));
    connect(actionInvoices_InvoiceGross, SIGNAL(activated()), this, SLOT(newGrossInvoice_()));
    connect(actionInvoices_Bill, SIGNAL(activated()), this, SLOT(newBill_()));
    connect(actionInvoices_InvoiceCorrective, SIGNAL(activated()), this, SLOT(newCorrection_()));
    connect(actionInvoices_InvoiceProForma, SIGNAL(activated()), this, SLOT(newProFormaInvoice_()));
    connect(actionCommodities_Add, SIGNAL(activated()), this, SLOT(addCommodity_()));
    connect(actionCommodities_Edit, SIGNAL(activated()), this, SLOT(editCommodity_()));
    connect(actionCommodities_Remove, SIGNAL(activated()), this, SLOT(delCommodity_()));
    connect(actionHelp_AboutQt, SIGNAL(activated()), this, SLOT(aboutQt_()));
    connect(actionHelp_About, SIGNAL(activated()), this, SLOT(about_()));
    connect(actionProgram_Settings, SIGNAL(activated()), this, SLOT(editSettings_()));
    connect(tabWidgetMain, SIGNAL(currentChanged(QWidget*)), this, SLOT(tabChanged_(QWidget*)));
    connect(actionHelp_Help, SIGNAL(activated()), this, SLOT(help_()));
    connect(tableViewInvoices, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editInvoice_()));
    connect(tableViewInvoices, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTableMenuHistory_(QPoint)));
    connect(tableViewCounterparties, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editCounterparty_()));
    connect(tableViewCounterparties, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTableMenuCounterparties_(QPoint)));
    connect(tableViewCommodities, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editCommodity_()));
    connect(tableViewCommodities, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTableMenuGoods_(QPoint)));

    connect(tableViewInvoices, SIGNAL(clicked(QModelIndex)), this, SLOT(mainUpdateStatus_(QModelIndex)));
    connect(tableViewCounterparties, SIGNAL(clicked(QModelIndex)), this, SLOT(mainUpdateStatus_(QModelIndex)));
    connect(tableViewCommodities, SIGNAL(clicked(QModelIndex)), this, SLOT(mainUpdateStatus_(QModelIndex)));

    tabChanged_(tabWidgetMain);
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
            this->menuPlugins->addAction(action);
            customActions[i] = path + allFiles[i];
        }
    }
    menuPlugins->addSeparator();
    menuPlugins->addAction(trUtf8("Informacje"), this, SLOT (pluginInfoSlot_()));
}



/**
 * @brief
 *
 * @param type
 */
void MainWindow::createInvoice_(const InvoiceTypeData::Type type)
{
    QScopedPointer<QDialog> invoice;
    switch(type)
    {
    case InvoiceTypeData::VAT:
    case InvoiceTypeData::PRO_FORMA:
        invoice.reset(new InvoiceDialog(this, &db_, type));
        break;
    case InvoiceTypeData::BILL:
        invoice.reset(new BillDialog(this, &db_));
        break;
    case InvoiceTypeData::GROSS:
        invoice.reset(new InvoiceGrossDialog(this, &db_, QModelIndex()));
        break;
    default:
        return;
    }

    if(!invoice.isNull())
    {
        if(QDialog::Accepted == invoice->exec())
        {
            tableViewInvoices->selectionModel()->setCurrentIndex(db_.modelInvoice()->index(db_.modelInvoice()->rowCount() - 1,
                                                                           InvoiceFields::ID_INVOICE), QItemSelectionModel::Rows | QItemSelectionModel::Select);
            dateEditFilterStart->setDate(QDate::currentDate()); //otherwise our new invoice couldn't be shown
            dateEditFilterEnd->setDate(QDate::currentDate());
        }
    }
}


/**
 * @brief
 *
 * @return bool
 */
bool MainWindow::firstRun_() const
{
    SettingsGlobal s;
    return s.value(s.FIRST_RUN, true).toBool();
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
    actionCounterparties_Edit->setEnabled(counterpartiesEditEnabled);
    actionCounterparties_Remove->setEnabled(counterpartiesRemoveEnable);
    actionCommodities_Edit->setEnabled(commoditiesEditEnabled);
    actionCommodities_Remove->setEnabled(commoditiesRemoveEnabled);
    actionInvoices_Edit->setEnabled(historyEditEnabled);
    actionInvoices_Remove->setEnabled(historyRemoveEnabled);
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
        switch (tabWidgetMain->currentIndex())
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
        if (tabWidgetMain->currentIndex() != tabWidgetMain->count() - 1)
        {
            tabWidgetMain->setCurrentIndex(tabWidgetMain->currentIndex() + 1);
        }
        else
        {
            tabWidgetMain->setCurrentIndex(0);
        }
    }
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_PageDown)
    {
        if (tabWidgetMain->currentIndex() != 0)
        {
            tabWidgetMain->setCurrentIndex(tabWidgetMain->currentIndex() - 1);
        }
        else
        {
            tabWidgetMain->setCurrentIndex(tabWidgetMain->count() - 1);
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
    db_.modelInvoice()->setDataRange(dateEditFilterStart->date(), dateEditFilterEnd->date());
}


void MainWindow::retranslateUi_()
{
    QTranslator appTranslator;
    SettingsGlobal s;
    appTranslator.load(QString("translations/qfaktury_") + s.value(s.LANG).toString());
    qApp->installTranslator(&appTranslator);
    retranslateUi(this);
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
    args += customActions[scriptId];
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
    QMenu menutableViewGoods(tableViewCommodities);
    menutableViewGoods.addAction(actionCommodities_Add);
    menutableViewGoods.addAction(actionCommodities_Remove);
    menutableViewGoods.addAction(actionCommodities_Edit);
    menutableViewGoods.exec(tableViewCommodities->mapToGlobal(p));
}



/**
 * @brief
 *
 * @param p
 */
void MainWindow::showTableMenuCounterparties_(const QPoint &p) const
{
    QMenu menuTableCounterparties(tableViewCounterparties);
    menuTableCounterparties.addAction(actionCounterparties_Add);
    menuTableCounterparties.addAction(actionCounterparties_Remove);
    menuTableCounterparties.addAction(actionCounterparties_Edit);
    menuTableCounterparties.exec(tableViewCounterparties->mapToGlobal(p));
}



/**
 * @brief
 *
 * @param p
 */
void MainWindow::showTableMenuHistory_(const QPoint &p) const
{
    QMenu menuTableInvoices(tableViewInvoices);
    menuTableInvoices.addAction(actionInvoices_InvoiceNew);
    menuTableInvoices.addAction(actionInvoices_InvoiceGross);
    menuTableInvoices.addAction(actionInvoices_Bill);
    menuTableInvoices.addSeparator();
    menuTableInvoices.addAction(actionInvoices_InvoiceProForma);
    menuTableInvoices.addAction(actionInvoices_InvoiceCorrective);
    menuTableInvoices.addAction(actionInvoices_InvoiceDuplicate);
    menuTableInvoices.addSeparator();
    menuTableInvoices.addAction(actionInvoices_Edit);
    menuTableInvoices.addAction(actionInvoices_Remove);
    menuTableInvoices.exec(tableViewInvoices->mapToGlobal(p));
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
void MainWindow::tabChanged_(QWidget * widget)
{
    // disable Edit and Remove actions _ONLY_
    switch (tabWidgetMain->indexOf(widget))
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
    const QModelIndexList list(tableViewInvoices->selectionModel()->selectedIndexes());
    if(list.size() <= 0)
    {
        QMessageBox::information(this, qApp->applicationName(), trUtf8("Żadna faktura nie jest wybrana.") + QChar(' ') + trUtf8("Nie można edytować."));
        return;
    }

    QSqlQuery query(db_.modelInvoiceType()->query());
    const qint64 invType = query.exec(QString("SELECT `id_invoice_type` FROM `invoice_type` WHERE `type` = '%1'")
                                .arg(db_.modelInvoice()->data(db_.modelInvoice()->index(list.at(0).row(),
                                InvoiceFields::TYPE_ID)).toString()));
    switch(invType)
    {
    case InvoiceTypeData::VAT:
    {
        InvoiceDialog dialog(this, &db_, InvoiceTypeData::VAT, list.at(0));
        dialog.exec();
    }
        break;
    case InvoiceTypeData::PRO_FORMA:
    {
        InvoiceDialog dialog(this, &db_, InvoiceTypeData::PRO_FORMA, list.at(0));
        dialog.exec();
    }
        break;
    case InvoiceTypeData::CORRECTIVE_VAT:
    {
        CorrectiveInvoiceDialog dialog(this, &db_, InvoiceTypeData::CORRECTIVE_VAT, list.at(0));
        dialog.exec();  // edit window shouln't return anything
    }
        break;
    case InvoiceTypeData::GROSS:
    {
        InvoiceGrossDialog dialog(this, &db_, QModelIndex());
        dialog.exec();
    }
        break;
    case InvoiceTypeData::CORRECTIVE_GROSS:
    {
        CorrectiveInvoiceGrossDialog dialog(this, &db_, list.at(0));
        dialog.exec(); // edit window shouln't return anything
    }
        break;
    case InvoiceTypeData::BILL:
    {
        BillDialog dialog(this, &db_);
        dialog.exec(); // edit window shouln't return anything
    }
        break;
    default:
        qDebug("MainWindow::editInvoice() switch: unexpected value: %lld", invType);
        break;
    }
}


/**
 * @brief
 *
 */
void MainWindow::delInvoice_()
{
    if(tableViewInvoices->selectionModel()->selectedRows().length() != 1)
    {
        QMessageBox::information(this, qApp->applicationName(), trUtf8("Żadna faktura nie jest wybrana.") + QChar(' ') + trUtf8("Nie można usuwać."));
    }
    else
    {
        if (QMessageBox::warning(this, QString("%1 - %2").arg(qApp->applicationName()).arg(qApp->applicationVersion()), trUtf8("Czy na pewno chcesz usunąć tę fakturę?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            const QString title("Usuwanie faktury");
            if(db_.modelInvoice()->removeRow(tableViewInvoices->currentIndex().row()))
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
void MainWindow::editMyCompanyInfo_()
{
    db_.modelCounterparty()->setOnlyMyCompanyVisible(true);
    db_.modelCounterpartyType()->setMyCompanyVisible(true, true);

    CounterpartyDialog dialog(this, &db_, db_.modelCounterparty()->index(0, CounterpartyFields::ID), true);
    //dialog.pushButtonEditTypeList->setEnabled(false);
    //dialog.setWindowTitle(trUtf8("Moja firma"));
    if (dialog.exec() == QDialog::Accepted)
    {
        const QString title("Edycja mojej firmy");
        if(!db_.modelCounterparty()->submitAll())
        {
            QMessageBox::warning(this, title, db_.modelCounterparty()->lastError().text());
        }
        else
        {
            QMessageBox::information(this, title, trUtf8("Edycja danych firmy zakończyła się sukcesem."));
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
    CounterpartyDialog dialog(this, &db_);
    if (dialog.exec() == QDialog::Accepted)
    {
        if(db_.modelCounterparty()->submitAll())
        {
            tableViewCounterparties->selectionModel()->setCurrentIndex(db_.modelCounterparty()->index(db_.modelCounterparty()->rowCount() - 1,
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
    if(tableViewCounterparties->selectionModel()->selectedRows().length() != 1)
    {
        QMessageBox::information(this, trUtf8("Niewybrany wiersz"), trUtf8("Wybierz tylko jednego kontrahenta do skasowania"));
    }
    else
    {
        if (QMessageBox::warning(this, qApp->applicationName(), trUtf8("Czy na pewno chcesz usunąć kontrahenta: %1?")
                                 .arg(db_.modelCounterparty()->index(tableViewCounterparties->currentIndex().row(), CounterpartyFields::NAME).data().toString()),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            const QString title("Usuwanie kontrahenta");
            if(db_.modelCounterparty()->removeRow(tableViewCounterparties->currentIndex().row()))
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
    if(tableViewCounterparties->selectionModel()->selectedRows().length() != 1)
    {
        QMessageBox::information(this, trUtf8("Niewybrany wiersz"), trUtf8("Wybierz tylko jednego kontrahenta do edycji"));
    }
    else
    {
        const QModelIndexList list(tableViewCounterparties->selectionModel()->selectedRows(CounterpartyFields::ID));
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
    const QModelIndexList list(tableViewInvoices->selectionModel()->selectedIndexes());
    if(list.size() <= 0)
    {
        QMessageBox::information(this, qApp->applicationName(), trUtf8("Żadna faktura nie jest wybrana.") + QChar(' ') + trUtf8("Nie można edytować."));
        return;
    }

    const QString invTypeStr(db_.modelInvoice()->data(db_.modelInvoice()->
                             index(list.at(0).row(), InvoiceFields::TYPE_ID)).
                             toString());
    const int invType = InvoiceTypeData::StringToInvoiceType(invTypeStr);
    switch(invType)
    {
    case InvoiceTypeData::VAT:
    {
        CorrectiveInvoiceDialog dialog(this, &db_, InvoiceTypeData::VAT);
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
    const QModelIndexList list(tableViewInvoices->selectionModel()->selectedIndexes());
    if(list.size() <= 0)
    {
        QMessageBox::information(this, qApp->applicationName(), trUtf8("Żadna faktura nie jest wybrana.") + QChar(' ') + trUtf8("Nie można edytować."));
        return;
    }

    const QString invTypeStr(db_.modelInvoice()->data(db_.modelInvoice()->
                             index(list.at(0).row(), InvoiceFields::TYPE_ID)).
                             toString());
    const int invType = InvoiceTypeData::StringToInvoiceType(invTypeStr);
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
    CommodityDialog dialog(this, &db_);
    if (dialog.exec() == QDialog::Accepted)
    {
        if(db_.modelCommodity()->submitAll())
        {
            tableViewCommodities->selectionModel()->setCurrentIndex(db_.modelCommodity()->index(db_.modelCommodity()->rowCount() - 1,
                                                                               CommodityFields::ID), QItemSelectionModel::Rows | QItemSelectionModel::Select);
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
    if(tableViewCommodities->selectionModel()->selectedRows().length() != 1)
    {
        QMessageBox::information(this, trUtf8("Niewybrany wiersz"), trUtf8("Wybierz tylko jeden towar do skasowania"));
    }
    else
    {
        if (QMessageBox::warning(this, qApp->applicationName(), trUtf8("Czy na pewno chcesz usunąć towar: %1?")
                                 .arg(db_.modelCommodity()->index(tableViewCommodities->currentIndex().row(),
                                 CommodityFields::NAME).data().toString()), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            const QString title("Usuwanie towaru");
            if(db_.modelCommodity()->removeRows(tableViewCommodities->currentIndex().row(), 1))
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
    if(tableViewCommodities->selectionModel()->selectedRows().length() != 1)
    {
        QMessageBox::information(this, trUtf8("Niewybrany wiersz"), trUtf8("Wybierz tylko jeden towar do edycji"));
    }
    else
    {
        const QModelIndex index = tableViewCommodities->selectionModel()->selectedRows(CommodityFields::ID).at(0);
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
