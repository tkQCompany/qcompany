#include "MainWindow.h"



/**
 * @brief Default constructor
 *
 * @param parent Pointer to Qt's parent widget
 */
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    setupUi(this);
    init();
}


/**
 * @brief Saves settings on exit
 *
 */
MainWindow::~MainWindow()
{
    saveAllSettings();
}


/**
 * @brief
 *
 */
void MainWindow::init()
{
    if (firstRun())
    {
        sett().resetSettings();
    }
    else
    {
        setupDir();
        dateEditFilterStart->setDisplayFormat(sett().getDateFormat());
        dateEditFilterStart->setDate(sett().getValueAsDate("filtrStart"));
        dateEditFilterEnd->setDisplayFormat(sett().getDateFormat());
        dateEditFilterEnd->setDate(sett().getValueAsDate("filtrEnd"));
    }

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
    connect(actionHelp_BugReport, SIGNAL (activated ()), this, SLOT(reportBug()));
    //connect(toolButtonApply, SIGNAL (clicked()), this, SLOT(rereadHistory()));
    connect(actionProgram_CompanyInfo, SIGNAL(activated()), this, SLOT(editCompanyInfo()));
    connect(actionProgram_Exit, SIGNAL(activated()), this, SLOT(close()));
    connect(actionCounterparties_Add, SIGNAL(activated()), this, SLOT(addCounterparty()));
    connect(actionCounterparties_Remove, SIGNAL(activated()), this, SLOT(delCounterparty()));
    connect(actionCounterparties_Edit, SIGNAL(activated()), this, SLOT(editCounterparty()));
    connect(actionInvoices_InvoiceNew, SIGNAL(activated()), this, SLOT(newInvoice()));
    connect(actionInvoices_Remove, SIGNAL(activated()), this, SLOT(delInvoice()));
    connect(actionInvoices_Edit, SIGNAL(triggered()), this, SLOT(editInvoice()));
    connect(actionInvoices_InvoiceDuplicate, SIGNAL(activated()), this, SLOT(newDuplicate()));
    connect(actionInvoices_InvoiceGross, SIGNAL(activated()), this, SLOT(newGrossInvoice()));
    connect(actionInvoices_Bill, SIGNAL(activated()), this, SLOT(newBill()));
    connect(actionInvoices_InvoiceCorrective, SIGNAL(activated()), this, SLOT(newCorrection()));
    connect(actionInvoices_InvoiceProForma, SIGNAL(activated()), this, SLOT(newProFormaInvoice()));
    connect(actionCommodities_Add, SIGNAL(activated()), this, SLOT(addCommodity()));
    connect(actionCommodities_Edit, SIGNAL(activated()), this, SLOT(editCommodity()));
    connect(actionCommodities_Remove, SIGNAL(activated()), this, SLOT(delCommodity()));
    connect(actionHelp_AboutQt, SIGNAL(activated()), this, SLOT(aboutQt()));
    connect(actionHelp_About, SIGNAL(activated()), this, SLOT(about()));
    connect(actionProgram_Settings, SIGNAL(activated()), this, SLOT(editSettings()));
    connect(tabWidgetMain, SIGNAL(currentChanged(QWidget*)), this, SLOT(tabChanged(QWidget*)));
    connect(actionHelp_Help, SIGNAL(activated()), this, SLOT(help()));
    connect(tableViewInvoices, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editInvoice()));
    connect(tableViewInvoices, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTableMenuHistory(QPoint)));
    connect(tableViewCounterparties, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editCounterparty()));
    connect(tableViewCounterparties, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTableMenuCounterparties(QPoint)));
    connect(tableViewCommodities, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editCommodity()));
    connect(tableViewCommodities, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTableMenuGoods(QPoint)));

    connect(tableViewInvoices, SIGNAL(clicked(QModelIndex)), this, SLOT(mainUpdateStatus(QModelIndex)));
    connect(tableViewCounterparties, SIGNAL(clicked(QModelIndex)), this, SLOT(mainUpdateStatus(QModelIndex)));
    connect(tableViewCommodities, SIGNAL(clicked(QModelIndex)), this, SLOT(mainUpdateStatus(QModelIndex)));

    tabChanged(tabWidgetMain);
    loadPlugins();
}



/**
 * @brief
 *
 */
void MainWindow::loadPlugins()
{
    QDir allFiles;
    const QString path = sett().getWorkingDir() + "/plugins/";
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
            connect(action, SIGNAL(triggered()), this, SLOT (pluginSlot()));
            this->menuPlugins->addAction(action);
            customActions[i] = path + allFiles[i];
        }
    }
    menuPlugins->addSeparator();
    menuPlugins->addAction(trUtf8("Informacje"), this, SLOT (pluginInfoSlot()));
}



/**
 * @brief
 *
 * @param type
 */
void MainWindow::createInvoice(const InvoiceTypeData::Type type)
{
    QScopedPointer<InvoiceDialog> invoice;
    switch(type)
    {
    case InvoiceTypeData::VAT:
        invoice.reset(new InvoiceDialog(this, &db_));
        break;
    case InvoiceTypeData::PRO_FORMA:
        invoice.reset(new InvoiceDialog(this, &db_));
        //invoice->proForma = true;
        invoice->setWindowTitle(trUtf8("Faktura Pro Forma"));
        //invoice->pushButtonBackClick();
        break;
    case InvoiceTypeData::BILL:
        invoice.reset(new BillDialog(this, &db_));
        invoice->init();
        break;
    case InvoiceTypeData::CORRECTIVE_VAT:
    case InvoiceTypeData::CORRECTIVE_GROSS:
        if(type == InvoiceTypeData::CORRECTIVE_VAT)
        {
            invoice.reset(new CorrectiveInvoiceDialog(this, &db_));
        }
        else
        {
            invoice.reset(new CorrectiveInvoiceGrossDialog(this, &db_));
        }
        invoice->init();
        //invoice->readData(tableViewInvoices->item(tableViewInvoices->selectedItems()[0]->row(), 0)->text(), type);
        invoice->setWindowTitle(trUtf8("Nowa korekta"));
        break;
    case InvoiceTypeData::GROSS:
        invoice.reset(new InvoiceGrossDialog(this, &db_));
        break;
    }

    if(!invoice.isNull())
    {
        invoice->exec();
        tableViewInvoices->selectionModel()->setCurrentIndex(db_.modelInvoice()->index(db_.modelInvoice()->rowCount() - 1,
                                                                           InvoiceDataFields::ID), QItemSelectionModel::Rows | QItemSelectionModel::Select);
    }
}


/**
 * @brief
 *
 * @return bool
 */
bool MainWindow::firstRun()
{
    const bool ok = sett().value("firstrun", true).toBool();
    sett().checkSettings();
    if(ok)
    {
        // set dates for filter
        dateEditFilterStart->setDate(QDate::currentDate());
        dateEditFilterEnd->setDate(QDate::currentDate());
    }
    return ok;
}



/**
 * @brief
 *
 */
void MainWindow::saveColumnWidth()
{
    sett().setValue("towCol0", tableViewCommodities->columnWidth(CMF::ID));
    sett().setValue("towCol1", tableViewCommodities->columnWidth(CMF::NAME));
    sett().setValue("towCol2", tableViewCommodities->columnWidth(CMF::ABBREV));
    //sett().setValue("towCol3", tableViewCommodities->columnWidth(CMF::POSTAL_CODE));
    sett().setValue("towCol4", tableViewCommodities->columnWidth(CMF::PKWIU));
    sett().setValue("towCol5", tableViewCommodities->columnWidth(CMF::TYPE_ID));
    sett().setValue("towCol6", tableViewCommodities->columnWidth(CMF::UNIT_ID));
    sett().setValue("towCol7", tableViewCommodities->columnWidth(CMF::NET1));
    sett().setValue("towCol8", tableViewCommodities->columnWidth(CMF::NET2));
    sett().setValue("towCol9", tableViewCommodities->columnWidth(CMF::NET3));
    sett().setValue("towCol10", tableViewCommodities->columnWidth(CMF::NET4));
    sett().setValue("towCol11", tableViewCommodities->columnWidth(CMF::VAT));

    //sett().setValue("histCol0", tableViewInvoices->columnWidth(IVF::FILE_NAME));
    sett().setValue("histCol1", tableViewInvoices->columnWidth(IVF::INV_NUMBER));
    sett().setValue("histCol2", tableViewInvoices->columnWidth(IVF::SELLING_DATE));
    sett().setValue("histCol3", tableViewInvoices->columnWidth(IVF::TYPE_ID));
    sett().setValue("histCol4", tableViewInvoices->columnWidth(IVF::COUNTERPARTY_ID));
    //sett().setValue("histCol5", tableViewInvoices->columnWidth(IVF::COUNTERPARTY_TAX_ID));

    sett().setValue("custCol0", tableViewCounterparties->columnWidth(CPF::NAME));
    sett().setValue("custCol1", tableViewCounterparties->columnWidth(CPF::TYPE_ID));
    sett().setValue("custCol2", tableViewCounterparties->columnWidth(CPF::LOCATION));
    sett().setValue("custCol3", tableViewCounterparties->columnWidth(CPF::ADDRESS));
    //sett().setValue("custCol4", tableViewCounterparties->columnWidth(CPF::PHONE_ID));
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
void MainWindow::setActions(const bool counterpartiesEditEnabled, const bool counterpartiesRemoveEnable,
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





/**
 * @brief
 *
 */
void MainWindow::saveAllSettings()
{
    // save filtr
    sett().setValue("filtrStart", dateEditFilterStart->text());
    sett().setValue("filtrEnd", dateEditFilterEnd->text());

    saveColumnWidth();

    // save unsaved
    sett().sync();
}



/**
 * @brief
 *
 */
void MainWindow::setupDir() const
{
    const QString workingDir(sett().getWorkingDir());
    const QDir dir(workingDir);
    if (!dir.exists())
    {
        dir.mkdir(workingDir);
    }

    if (!dir.exists(workingDir + sett().getDataDir()))
    {
        dir.mkdir(workingDir + sett().getDataDir());
    }
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
            editInvoice();
            break;
        case 1:
            editCounterparty();
            break;
        case 2:
            editCommodity();
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
void MainWindow::pluginInfoSlot()
{
    QMessageBox::information(this, qApp->applicationName(),
                             trUtf8("To menu służy do obsługi pluginów pythona,\nnp. archiwizacji danych, generowania raportów etc.\n\n"
                                    "Skrypty Pythona sa czytane z folderu \"~/elinux/plugins/\"."));
}



/**
 * @brief
 *
 */
void MainWindow::pluginSlot()
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
void MainWindow::showTableMenuGoods(const QPoint &p) const
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
void MainWindow::showTableMenuCounterparties(const QPoint &p) const
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
void MainWindow::showTableMenuHistory(const QPoint &p) const
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
void MainWindow::mainUpdateStatus(QModelIndex index)
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
void MainWindow::tabChanged(QWidget * widget)
{
    // disable Edit and Remove actions _ONLY_
    switch (tabWidgetMain->indexOf(widget))
    {
    case 1: //counterparties
        setActions(true, true, false, false, false, false);
        break;
    case 2: //commodities
        setActions(false, false, true, true, false, false);
        break;
    default:// invoices
        setActions(false, false, false, false, true, true);
        break;
    }
}



/**
 * @brief
 *
 */
void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this, QString("%1 - %2").arg(qApp->applicationName()).arg(qApp->applicationVersion()));
}


/**
 * @brief
 *
 */
void MainWindow::about()
{
    QMessageBox::about(
                this,
                trUtf8("O programie"),
                trUtf8("Program do wystawiania faktur.\n\n ") + QString("%1 - %2").arg(qApp->applicationName()).arg(qApp->applicationVersion()) +
                trUtf8("\n\nKoordynator projektu:\n\tGrzegorz Rękawek\t\t\n\nProgramiści: \n\tTomasz Pielech\n\t") +
                trUtf8("Rafał Rusin http://people.apache.org/~rr/\n\tSławomir Patyk \n\nIkony:\n\tDariusz Arciszewski\n\n") +
                trUtf8("UWAGA!!!\n") +
                trUtf8("Ten program komputerowy dostarczany jest przez autora w formie \"takiej, jaki jest\". ") +
                trUtf8("Autor nie udziela żadnej gwarancji oraz rękojmi, że program będzie działał ") +
                trUtf8("prawidłowo, jest odpowiedniej jakości oraz że spełni oczekiwania ") +
                trUtf8("użytkownika. Autor nie odpowiada za jakiekolwiek straty wynikające z użytkowania ") +
                trUtf8("programu, w tym utratą spodziewanych korzyści, danych, informacji ") +
                trUtf8("gospodarczych lub koszt urządzeń lub programów zastępczych."));
}


/**
 * @brief
 *
 */
void MainWindow::editInvoice()
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
                                InvoiceDataFields::TYPE_ID)).toString()));
    switch(invType)
    {
    case InvoiceTypeData::VAT:
    case InvoiceTypeData::PRO_FORMA:
    {
        InvoiceDialog dialog(this, &db_, list.at(0));
        //InvoiceData::InvoiceType type = InvoiceData::PRO_FORMA;
        //if (tableViewInvoices->item(row, IVF::TYPE)->text() == InvoiceData::InvoiceTypeToString(InvoiceData::VAT))
        //{
        //    type = InvoiceData::VAT;
        //}
        //dialog.readData(tableViewInvoices->item(row, IVF::FILE_NAME)->text(), type);
        if (dialog.exec() == QDialog::Accepted)
        {

        }
    }
        break;
    case InvoiceTypeData::CORRECTIVE_VAT:
    {
        CorrectiveInvoiceDialog dialog(this, &db_);
        dialog.init(/*true*/);
        //dialog.readCorrData(tableViewInvoices->item(row, IVF::FILE_NAME)->text());
        if (dialog.exec() == QDialog::Accepted)
        {
            // edit window shouln't return anything
        }
    }
        break;
    case InvoiceTypeData::GROSS:
    {
        InvoiceGrossDialog dialog(this, &db_);
        //dialog.readData(tableViewInvoices->item(row, IVF::FILE_NAME)->text(), InvoiceData::GROSS);
        if (dialog.exec() == QDialog::Accepted)
        {
        }
    }
        break;
    case InvoiceTypeData::CORRECTIVE_GROSS:
    {
        CorrectiveInvoiceGrossDialog dialog(this, &db_);
        dialog.init(/*true*/);
        //dialog.readCorrData(tableViewInvoices->item(row, IVF::FILE_NAME)->text());
        if (dialog.exec() == QDialog::Accepted)
        {
            // edit window shouln't return anything
        }
    }
        break;
    case InvoiceTypeData::BILL:
    {
        BillDialog dialog(this, &db_);
        //dialog.readData(tableViewInvoices->item(row, IVF::FILE_NAME)->text(), InvoiceData::BILL);
        dialog.init(/*false*/);
        dialog.setWindowTitle(trUtf8("Edytuje Rachunek"));
        if (dialog.exec() == QDialog::Accepted)
        {
            // edit window shouln't return anything
        }
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
void MainWindow::delInvoice()
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
void MainWindow::editCompanyInfo()
{
    db_.modelCounterparty()->setFilter(QString("type_id = %1").arg(CounterpartyTypeData::MY_COMPANY));
    db_.modelCounterparty()->select();
    db_.modelCounterpartyType()->setFilter(QString("id_counterparty_type = %1").arg(CounterpartyTypeData::MY_COMPANY));
    db_.modelCounterpartyType()->select();

    CounterpartyDialog dialog(this, &db_, db_.modelCounterparty()->index(0, CounterpartyFields::ID));
    dialog.pushButtonEditTypeList->setEnabled(false);
    dialog.setWindowTitle(trUtf8("Moja firma"));
    if (dialog.exec() == QDialog::Accepted)
    {
        if(!db_.modelCounterparty()->submitAll())
        {
            QMessageBox::critical(this, trUtf8("Edycja mojej firmy"), db_.modelCounterparty()->lastError().text());
        }
    }

    db_.modelCounterparty()->setFilter(QString("type_id != %1").arg(CounterpartyTypeData::MY_COMPANY));
    db_.modelCounterparty()->select();
    db_.modelCounterpartyType()->setFilter(QString("id_counterparty_type != %1").arg(CounterpartyTypeData::MY_COMPANY));
    db_.modelCounterpartyType()->select();
}



/**
 * @brief
 *
 */
void MainWindow::editSettings()
{
    SettingsDialog settWindow(this);
    settWindow.exec();
}



/**
 * @brief
 *
 */
void MainWindow::addCounterparty()
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
        }
    }
}



/**
 * @brief
 *
 */
void MainWindow::delCounterparty()
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
void MainWindow::editCounterparty()
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
void MainWindow::newInvoice()
{
    createInvoice(InvoiceTypeData::VAT);
}



/**
 * @brief
 *
 */
void MainWindow::newBill()
{
    createInvoice(InvoiceTypeData::BILL);
}



/**
 * @brief
 *
 */
void MainWindow::newGrossInvoice()
{
    createInvoice(InvoiceTypeData::GROSS);
}



/**
 * @brief
 *
 */
void MainWindow::newProFormaInvoice()
{
    createInvoice(InvoiceTypeData::PRO_FORMA);
}


/**
 * @brief
 *
 */
void MainWindow::newCorrection()
{
    /*if(!areTableItemsSelected(tableViewInvoices, trUtf8("Żadna faktura nie jest wybrana.") + QChar(' ') +
                              trUtf8("Wybierz fakturę, do której chcesz wystawić korektę.")))
        return;

    const int row = tableViewInvoices->selectedItems()[0]->row();

    const QStringList invTypes(QStringList() << InvoiceData::InvoiceTypeToString(InvoiceData::VAT)
                               << InvoiceData::InvoiceTypeToString(InvoiceData::GROSS));
    if (invTypes.contains(tableViewInvoices->item(row, IVF::TYPE)->text()))
    {

        if (tableViewInvoices->item(row, IVF::TYPE)->text().contains(InvoiceData::InvoiceTypeToString(InvoiceData::VAT)))
        {
            createInvoice(InvoiceData::CORRECTIVE_VAT);
        }
        else
        {
            createInvoice(InvoiceData::CORRECTIVE_GROSS);
        }
    }
    else
    {
        QMessageBox::information(this, qApp->applicationName(), trUtf8("Do faktury typu %1 nie wystawiamy korekt.")
                                 .arg(tableViewInvoices->item(row, IVF::TYPE)->text()), QMessageBox::Ok);
    }*/
}


/**
 * @brief
 *
 */
void MainWindow::newDuplicate()
{
    /*if(!areTableItemsSelected(tableViewInvoices, trUtf8("Żadna faktura nie jest wybrana.") + QChar(' ') +
                              trUtf8("Wybierz fakturę, do której chcesz wystawić duplikat.")))
        return;

    const int row = tableViewInvoices->selectedItems()[0]->row();

    // types of invoices for which it's ok to issue a duplicate
    const QStringList invTypes(QStringList() << InvoiceData::InvoiceTypeToString(InvoiceData::VAT)
                               << InvoiceData::InvoiceTypeToString(InvoiceData::GROSS));

    if (invTypes.contains(tableViewInvoices->item(row, IVF::TYPE)->text()))
    {
        DuplicateDialog dupWindow(this, model);
        dupWindow.readData(tableViewInvoices->item(row, IVF::FILE_NAME)->text(), InvoiceData::VAT);
        dupWindow.setWindowTitle(trUtf8("Nowy duplikat"));
        dupWindow.init();
        if (dupWindow.exec() == QDialog::Accepted)
        {
            // not saving duplicate
        }
    }
    else
    {
        QMessageBox::information(this, qApp->applicationName(), trUtf8("Do faktury typu %1 nie wystawiamy duplikatów.")
                                 .arg(tableViewInvoices->item(row, IVF::TYPE)->text()), QMessageBox::Ok);
    }*/
}


/**
 * @brief
 *
 */
void MainWindow::addCommodity()
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
void MainWindow::delCommodity()
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
void MainWindow::editCommodity()
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
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::question(this, trUtf8("Potwierdź"),
                              trUtf8("Czy chcesz wyjść z programu?"), QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::Yes) == QMessageBox::Yes)
    {
        saveAllSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}


/**
 * @brief
 *
 */
void MainWindow::help() const
{
    QDesktopServices::openUrl(QUrl("http://www.e-linux.pl/"));
}


/**
 * @brief
 *
 */
void MainWindow::reportBug() const
{
    QDesktopServices::openUrl(QUrl("https://sourceforge.net/tracker2/?func=add&group_id=154610&atid=792471"));
}

// ----------------------------------------  SLOTS ---------------------------------//
