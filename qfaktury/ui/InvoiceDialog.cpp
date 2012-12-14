#include "InvoiceDialog.h"


/** Constructor
 */
/**
 * @brief
 *
 * @param parent
 * @param db
 * @param id_edit
 */
InvoiceDialog::InvoiceDialog(QWidget *parent, Database *db, const QModelIndex &id_edit) : QDialog(parent), db_(db)
{
    setupUi(this);
    init();

    if(id_edit.isValid())
    {
        mapper_.setCurrentIndex(id_edit.row());
        fillTableCommodity_(db_->commodities(id_edit.data().toLongLong()));
    }
    else
    {
        db_->modelInvoice()->insertRow(db_->modelInvoice()->rowCount());
        mapper_.toLast();
        lineEditInvNumber->setText(generateInvoiceNumber());
        setInitialComboBoxIndexes_();
        if(!sett().value ("addText").toString().isEmpty())
        {
            lineEditAdditionalText->setText(sett().value("addText").toString());
        }
    }
}


/**
 * @brief
 *
 */
InvoiceDialog::~InvoiceDialog()
{
    db_->modelInvoice()->revertAll();
}


/** Init method
 */
/**
 * @brief
 *
 */
void InvoiceDialog::init()
{
    // set all the dates to todays date -> could be logical date :)
    dateEditDateOfSell->setDate(QDate::currentDate());
    dateEditDateOfIssuance->setDate(QDate::currentDate());
    dateEditDayOfPayment->setDate(QDate::currentDate());

    // connects
    connect(pushButtonAddCommodity, SIGNAL(clicked()), this, SLOT(addCommodity()));
    connect(pushButtonMoreInfo, SIGNAL(clicked()), this, SLOT(counterpartyMoreInfo_()));
    connect(pushButtonRemoveCommodity, SIGNAL(clicked()), this, SLOT(delCommodity_()));
    connect(pushButtonEditCommodity, SIGNAL(clicked()), this, SLOT(editCommodity_()));
    connect(pushButtonClose, SIGNAL(clicked()), this, SLOT(canQuit()));
    connect(pushButtonSave, SIGNAL(clicked()), this, SLOT(saveInvoice()));
    connect(pushButtonPrint, SIGNAL(clicked()), this, SLOT(makeInvoice()));
    connect(tableWidgetCommodities, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editCommodity_()));
    connect(tableWidgetCommodities, SIGNAL(itemActivated(QTableWidgetItem *)), this, SLOT(tableActivated_(QTableWidgetItem *)));
    connect(tableWidgetCommodities, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(tableActivated_(QTableWidgetItem *)));
    connect(lineEditAdditionalText, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
    connect(comboBoxPayment, SIGNAL(currentIndexChanged (QString)), this, SLOT(payTextChanged(QString)));
    connect(comboBoxCurrency, SIGNAL(currentIndexChanged (QString)), this, SLOT(textChanged(QString)));
    connect(dateEditDateOfSell, SIGNAL(dateChanged(QDate)), this, SLOT(dateChanged_(QDate)));
    connect(dateEditDateOfIssuance, SIGNAL(dateChanged(QDate)), this, SLOT(dateChanged_(QDate)));
    connect(dateEditDayOfPayment, SIGNAL(dateChanged(QDate)), this, SLOT(dateChanged_(QDate)));
    connect(spinBoxDiscount, SIGNAL(valueChanged(int)), this, SLOT(discountChange()));
    connect(checkBoxDiscount, SIGNAL(stateChanged(int)), this, SLOT(discountConstChange()));
    connect(pushButtonAddCounterparty, SIGNAL(clicked()), this, SLOT(counterpartyAdd_()));

    pushButtonRemoveCommodity->setEnabled(false);
    pushButtonEditCommodity->setEnabled(false);
    dateEditDayOfPayment->setEnabled(false);
    spinBoxDiscount->setEnabled(false);

    for(int i = InvoiceTypeData::VAT; i <= InvoiceTypeData::BILL; ++i)
    {
        comboBoxInvoiceType->addItem(InvoiceTypeData::InvoiceTypeToString(i));
        comboBoxInvoiceType->setItemData(i, InvoiceTypeData::InvoiceTypeToString(i));
    }

    comboBoxCounterparties->setModel(db_->modelCounterparty());
    comboBoxCounterparties->setModelColumn(CounterpartyFields::NAME);
    comboBoxCounterparties->setInsertPolicy(QComboBox::InsertAtBottom);
    comboBoxPayment->setModel(db_->modelPaymentType());
    comboBoxPayment->setModelColumn(PaymentTypeFields::TYPE);
    comboBoxCurrency->setModel(db_->modelCurrency());
    comboBoxCurrency->setModelColumn(CurrencyFields::NAME);

    mapper_.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper_.setItemDelegate(new QSqlRelationalDelegate(this));
    mapper_.setModel(db_->modelInvoice());
    mapper_.addMapping(lineEditInvNumber, InvoiceDataFields::INV_NUMBER);
    mapper_.addMapping(dateEditDateOfSell, InvoiceDataFields::SELLING_DATE);
    mapper_.addMapping(comboBoxInvoiceType, InvoiceDataFields::TYPE_ID);
    mapper_.addMapping(comboBoxCounterparties, InvoiceDataFields::COUNTERPARTY_ID, "currentIndex");
    mapper_.addMapping(dateEditDateOfIssuance, InvoiceDataFields::ISSUANCE_DATE);
    mapper_.addMapping(dateEditDayOfPayment, InvoiceDataFields::PAYMENT_DATE);
    mapper_.addMapping(comboBoxPayment, InvoiceDataFields::PAYMENT_ID);
    mapper_.addMapping(comboBoxCurrency, InvoiceDataFields::CURRENCY_ID);
    mapper_.addMapping(lineEditAdditionalText, InvoiceDataFields::ADDIT_TEXT);
    mapper_.addMapping(spinBoxDiscount, InvoiceDataFields::DISCOUNT);

    unsaved = false;
}


// ---- SLOTS START  --//////////////////////////////////////////////////////////////////////////////////

/**
 * @brief
 *
 */
void InvoiceDialog::counterpartyAdd_()
{
    CounterpartyDialog dialog(this, db_);
    if (dialog.exec() == QDialog::Accepted)
    {
        if(db_->modelCounterparty()->submitAll())
        {
            comboBoxCounterparties->setCurrentIndex(db_->modelCounterparty()->rowCount() - 1);
        }
        else
        {
            QMessageBox::critical(this, trUtf8("Błąd dodawania kontrahenta"), db_->modelCounterparty()->lastError().text());
        }
    }
}


/** Slot getCustomer
 *  Opens window with the list of the customers
 */
/**
 * @brief
 *
 */
void InvoiceDialog::counterpartyMoreInfo_()
{
    if(comboBoxCounterparties->currentIndex() != -1)
    {
        CounterpartyInfoDialog dialog(this, db_,
                                      comboBoxCounterparties->model()->index(comboBoxCounterparties->currentIndex(),
                                                                             CounterpartyFields::ID));
        dialog.exec();
    }
}

/** Slot
 *  Enables and disables rebatValue field
 */
/**
 * @brief
 *
 */
void InvoiceDialog::discountConstChange()
{
    if (checkBoxDiscount->isChecked())
    {
        spinBoxDiscount->setEnabled(true);
        pushButtonSave->setEnabled(true);
        calculateSum();
    }
    else
    {
        spinBoxDiscount->setValue(0);
        spinBoxDiscount->setEnabled(false);
    }
}


/**
 * @brief Used to recalculate discount when spinBox arrows are pressed.
 *
 */
void InvoiceDialog::discountChange()
{
    calculateSum();
    pushButtonSave->setEnabled(true);
    unsaved = true;
}



/**
 * @brief
 *
 */
void InvoiceDialog::addCommodity()
{
    CommodityListDialog dialog(this, db_);
    if (dialog.exec() == QDialog::Accepted)
    {
        const int rowNum = tableWidgetCommodities->rowCount() == 0 ? 0 : tableWidgetCommodities->rowCount() - 1;
        tableWidgetCommodities->insertRow(rowNum);

        tableWidgetCommodities->setColumnCount(CommodityVisualFields::DISCOUNT - CommodityVisualFields::ID + 1);

        QStringList headers;
        for(int i = CommodityVisualFields::ID; i <= CommodityVisualFields::DISCOUNT; ++i)
        {
            tableWidgetCommodities->setItem(rowNum, i, new QTableWidgetItem(dialog.ret.field(i)));
            headers.push_back(CommodityVisualData::header(i));
        }

        tableWidgetCommodities->hideColumn(CommodityVisualFields::ID);
        tableWidgetCommodities->setHorizontalHeaderLabels(headers);
        tableWidgetCommodities->resizeColumnsToContents();

        pushButtonSave->setEnabled(true);
        unsaved = true;
        calculateSum();
    }
}




/**
 * @brief Removes the commodity from the list
 *
 */
void InvoiceDialog::delCommodity_()
{
    tableWidgetCommodities->removeRow(tableWidgetCommodities->currentRow());
    calculateSum();
    pushButtonSave->setEnabled(true);
    unsaved = true;
}




/**
 * @brief Allows to edit selected product. Opens ChangeAmountDialog window.
 *
 */
void InvoiceDialog::editCommodity_()
{
    // we can only modify quantity
    ChangeAmountDialog changeAmount(this);
    changeAmount.lineEditCommodityName->setText(tableWidgetCommodities-> item(tableWidgetCommodities->currentRow(), 1)->text());
    changeAmount.lineEditCommodityCode->setText(tableWidgetCommodities-> item(tableWidgetCommodities->currentRow(), 2)->text());
    changeAmount.spinBoxCommodityAmount->setValue(tableWidgetCommodities-> item(tableWidgetCommodities->currentRow(), 4)->text().toInt());
    if (changeAmount.exec() == QDialog::Accepted)
    {
        const int currentRow = tableWidgetCommodities->currentRow();
        tableWidgetCommodities->item(currentRow, CommodityVisualFields::QUANTITY)->setText(changeAmount.spinBoxCommodityAmount->cleanText());
        pushButtonSave->setEnabled(true);
        unsaved = true;
        calculateSum();
    }
}





/**
 * @brief Asks the user if save the invoice
 *
 */
void InvoiceDialog::canQuit()
{
    // canClose == false -> data changed
    if (!unsaved || QMessageBox::warning(this, qApp->applicationName(), trUtf8("Dane zostały zmienione, czy chcesz zapisać?"),
                                         QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)
            == QMessageBox::No)
    {
        reject();
    }
    else
    {
        saveInvoice();
        if (!saveFailed)
        {
            accept();
        }
    }
}

/** Slot tableActivated
 *  Enable/Disable buttons
 */
/**
 * @brief
 *
 * @param item
 */
void InvoiceDialog::tableActivated_(QTableWidgetItem * item)
{
    if (item != NULL || !item->text().isEmpty())
    {
        pushButtonRemoveCommodity->setEnabled(true);
        pushButtonEditCommodity->setEnabled(true);
    }
}

/** Slot payTextChanged
 *  Activates buttons
 */
/**
 * @brief
 *
 * @param text
 */
void InvoiceDialog::payTextChanged(QString text)
{
    if (comboBoxPayment->currentIndex() > 0)
    {
        dateEditDayOfPayment->setEnabled(true);
    }
    else
    {
        dateEditDayOfPayment->setEnabled(false);
    }

    if (comboBoxPayment->currentIndex() == comboBoxPayment->count() - 1)
    {
        if (sett().stringToDouble(labelSumGrossVal->text()) == 0)
        {
            QMessageBox::warning(this, qApp->applicationName(), trUtf8("Taki sposób płatności nie może zostać wybrany, ponieważ kwota do zapłaty wynosi 0."));
            comboBoxPayment->setCurrentIndex(0);
            return;
        }
        CustomPaymentDialog cp;
        cp.setInvoiceAmount(sett().stringToDouble(labelSumGrossVal->text()));
        if (cp.exec() ==  QDialog::Accepted)
        {
            custPaymData = cp.custPaymData;
            dateEditDayOfPayment->setEnabled(false);
        }
        else
        {
            comboBoxPayment->setCurrentIndex(0);
        }
    }

    textChanged(text);
}

/** Slot textChanged
 *  Activates buttons
 */
/**
 * @brief
 *
 * @param QString
 */
void InvoiceDialog::textChanged(QString) {
    pushButtonSave->setEnabled(true);
    unsaved = true;
}

/** Slot dateChanged
 *  Activates buttons
 */
/**
 * @brief
 *
 * @param QDate
 */
void InvoiceDialog::dateChanged_(QDate ) {
    pushButtonSave->setEnabled(true);
    unsaved = true;
}


/** Slot saveInvoice
 *  Generates Invoice XML
 */
/**
 * @brief
 *
 * @return bool
 */
bool InvoiceDialog::saveInvoice()
{
    if (!validateForm_()) return false;

    bool result = mapper_.submit();
    if(result)
    {
        result = db_->invoiceWithCommoditiesInsertTransact(getInvoiceData_(), getCommoditiesVisualData_());
        if(!result)
        {
            qDebug() << "InvoiceDialog::saveInvoice() - lastError: " << db_->modelInvoice()->lastError().text();
            pushButtonSave->setEnabled(false);
            pushButtonRemoveCommodity->setEnabled(false);
            pushButtonEditCommodity->setEnabled(false);
        }
    }
    else
    {
        qDebug() << "InvoiceDialog::saveInvoice() - mapper_submit - lastError: " << db_->modelInvoice()->lastError().text();
        saveFailed = true;
        unsaved = true;
    }

    return result;
}

/** Slot makeInvoice
 *  Generate html with invoice and show Print Preview dialog
 */
/**
 * @brief
 *
 */
void InvoiceDialog::makeInvoice()
{
    if(!validateForm_())
        return;

     //if (fraStrList.isEmpty()) {
    // for perfomance reasons could be done... but not now TODO

    invStrList.clear();
    makeInvoiceHeaderHTML();

    makeInvoiceHeader(true, false, true);
    makeInvoiceBody();
    makeInvoiceProducts();
    makeInvoiceSumm();
    makeInvoiceSummAll();
    makeInvoiceFooter();

    const int numberOfCopies = sett().value("numberOfCopies", 2).toInt();
    for (int i = 1; i <= numberOfCopies; i++)
    {
        // print copy
        makeInvoiceHeader(true, true, false);
        makeInvoiceBody();
        makeInvoiceProducts();
        makeInvoiceSumm();
        makeInvoiceSummAll();
        makeInvoiceFooter();
    }

    makeInvoiceFooterHtml();
    print();
    pushButtonSave->setFocus();
}

/** Slot print
 *  Helper slot used to display print preview
 */
/**
 * @brief
 *
 * @param printer
 */
void InvoiceDialog::printSlot_(QPrinter *printer) const
{
    QTextDocument doc(InvoiceTypeData::InvoiceTypeToString(invType));

    QString s;
    QStringListIterator listIter(invStrList);
    while(listIter.hasNext())
    {
        s += listIter.next() + "\n";
    }

    QFile file(sett().getWorkingDir() + "/invoice.html");
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);

        listIter.toFront();
        while(listIter.hasNext())
        {
            stream << listIter.next() << "\n";
        }

        file.close();
    }

    doc.setHtml(s);
    doc.print(printer);
}

/** Slot
 *  Print slot
 */
/**
 * @brief
 *
 */
void InvoiceDialog::print()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    preview.setWindowFlags(Qt::Window);
    preview.setWindowTitle(trUtf8("%1 - Podgląd wydruku").arg(comboBoxInvoiceType->currentText()));

    connect(&preview, SIGNAL(paintRequested(QPrinter *)), this, SLOT(printSlot_(QPrinter *)));
    if (preview.exec() == QDialog::Accepted)
    {
    }
}

// ---- SLOTS END --//////////////////////////////////////////////////////////////////////////////////
// ******************************** XML Helpers START **********************************************
/** Check if data on the form is correct
 */
/**
 * @brief
 *
 * @return bool
 */
bool InvoiceDialog::validateForm_()
{
    if(lineEditInvNumber->text().isEmpty())
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Brak numeru kolejnego faktury"),
                              QMessageBox::Ok);
        saveFailed = true;
        return false;
    }

    if(comboBoxInvoiceType->currentIndex() == -1)
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Wybierz typ faktury."),
                              QMessageBox::Ok);
        saveFailed = true;
        return false;
    }

    if (comboBoxCounterparties->currentIndex() == -1)
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Wybierz kontrahenta."),
                              QMessageBox::Ok);
        saveFailed = true;
        return false;
    }

    if (tableWidgetCommodities->rowCount() == 0)
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Wybierz towary lub usługi do sprzedania."),
                              QMessageBox::Ok);
        saveFailed = true;
        return false;
    }


    if(comboBoxPayment->currentIndex() == -1)
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Wybierz rodzaj płatności."),
                              QMessageBox::Ok);
        saveFailed = true;
        return false;
    }


    if(comboBoxCurrency->currentIndex() == -1)
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Wybierz walutę rozliczeniową."),
                              QMessageBox::Ok);
        saveFailed = true;
        return false;
    }

    return true;
}

// ******************************** XML Helpers END **********************************************

// Generate Invoice HTML methods --- START ---

/**
 * @brief
 *
 */
void InvoiceDialog::makeInvoiceHeaderHTML() {
    invStrList += "<html><head>";
    invStrList += "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"; //@TODO
    invStrList += "<meta name=\"creator\" value=\"http://www.e-linux.pl\" />";
    invStrList += "</head>";

    if (windowTitle().right(3) == "VAT") {
        invStrList += "<title>"+ InvoiceTypeData::InvoiceTypeToString(invType)  +"</title>";
    } else {
        invStrList += "<title>" + InvoiceTypeData::InvoiceTypeToString(invType) + "</title>";
    }

    invStrList += "<style type=\"text/css\"> ";
    QFile file(sett().getTemplate());
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        QString line;
        while (!stream.atEnd()) {
            line = stream.readLine();
            invStrList += line;
        }
        file.close();
    } else {
        qWarning() << "Could not open CSS file: " << file.fileName();
    }
    invStrList += "</style>";
    invStrList += "<body>";
}


/**
 * @brief
 *
 * @param sellDate
 * @param breakPage
 * @param original
 */
void InvoiceDialog::makeInvoiceHeader(const bool sellDate, const bool breakPage, const bool original) {
    QString breakPageStr = "class=\"page_break\"";
    if (breakPage == false) breakPageStr = "";

    invStrList += "<table comment=\"headar table\" width=\"100%\" border=\"0\"" + breakPageStr + ">";
    invStrList += "<tr>";
    invStrList += "<td width=\"60%\" align=\"center\" valign=\"bottom\">";
    invStrList += "<span class=\"stamp\">";
    const QString logo = sett().value("logo").toString();
    if (logo != "") {
        invStrList += "<img src=\"" + logo + "\" width=\"100\" " + " height=\"100\"+ >";
    } else {
        invStrList += trUtf8("Pieczęć wystawcy");
    }
    invStrList += "</span>";
    invStrList += "</td>";

    invStrList += "<td align=\"right\">";
    invStrList += "<span style=\"font-size:12pt; font-weight:600\">";
    invStrList += InvoiceTypeData::InvoiceTypeToString(invType) + "<br/>";
    invStrList += trUtf8("Nr: ") + lineEditInvNumber->text() + "<br></span>";
    invStrList += "<span class=\"dates\">" + trUtf8("Data wystawienia: ")
            + dateEditDateOfIssuance->date().toString(sett().getDateFormat()) + "<br>";

    if (sellDate)
        invStrList += trUtf8("Data sprzedaży: ")
                + dateEditDateOfSell->date().toString(sett().getDateFormat())
                + "<br>";
    invStrList += "</span></td><td width=\"3%\">&nbsp;</td>";
    invStrList += "</tr>";
    invStrList += "<tr>";
    invStrList += "<td colspan=\"2\" align=\"right\" valign=\"top\"><br>";
    if (original) {
        invStrList += trUtf8("ORYGINAŁ");
    } else {
        invStrList += trUtf8("KOPIA");
    }
    invStrList += "<br></td><td width=\"3%\">&nbsp;</td>";
    invStrList += "</tr>";
    invStrList += "</table>";
    invStrList += "<hr>";
    invStrList += "</td></tr>";
}


/**
 * @brief
 *
 */
void InvoiceDialog::makeInvoiceBody() {
    invStrList += "<tr><td>";

    invStrList += "<table width=\"100%\" border=\"0\">";
    invStrList += "<tr class=\"persons\">";
    invStrList += "<td width=\"20\">&nbsp;</td>";
    invStrList += "<td width=\"48%\"> ";
    invStrList += trUtf8("Sprzedawca:")+"<br>";
    QSettings userSettings("elinux", "user");

    sett().beginGroup("printpos");
    if (sett().value("usernazwa").toBool())
        invStrList += userSettings.value("name").toString() + "<br>";
    if (sett().value("useradres").toBool())
        invStrList += userSettings.value("address").toString() + "<br>"; // trUtf8("Ul. ") +
    if (sett().value("usermiejscowosc").toBool())
        invStrList += userSettings.value("zip").toString() + " ";
    if (sett().value("usermiejscowosc").toBool())
        invStrList += userSettings.value("city").toString() + "<br>";
    if (sett().value("usernip").toBool())
        invStrList += trUtf8("NIP: ") + userSettings.value("tic").toString() + "<br>";
    if (sett().value("userkonto").toBool())
        invStrList += trUtf8("Nr konta: ")
                + userSettings.value("account").toString().replace("-", " ") + "<br>";
    sett().endGroup();

    invStrList += "</td>";
    invStrList += "<td width=\"20\">&nbsp;</td>";
    invStrList += "<td width=\"48%\">";
    invStrList += trUtf8("Nabywca:") + "<br>" + comboBoxCounterparties->currentText().replace(",", "<br>") + "<br>";
    invStrList += "</td>";
    invStrList += "</tr>";
    invStrList += "</table>";

    invStrList += "<hr>";
    invStrList += "</td></tr>";
}

/**
 * @brief
 *
 */
void InvoiceDialog::makeInvoiceProductsHeader() {
    int currentPercent = 0;
    invStrList += "<tr align=\"center\" valign=\"middle\" class=\"productsHeader\" >";

    if (sett().value("faktury_pozycje/Lp").toBool()) {
        currentPercent = 3;
        invStrList += "<td align=\"center\" width=\""+ sett().numberToString(currentPercent) + "%\">" + trUtf8("Lp.") + "</td>";
        currentPercent = 0;
    } else {
        currentPercent += 3;
    }
    if (sett().value("faktury_pozycje/Nazwa").toBool()) {
        currentPercent += 25;
        invStrList += "<td align=\"center\" width=\""+ sett().numberToString(currentPercent) + "%\">" + trUtf8("Nazwa") + "</td>";
        currentPercent = 0;
    } else {
        currentPercent += 25;
    }
    if (sett().value("faktury_pozycje/Kod").toBool()) {
        currentPercent += 7;
        invStrList += "<td align=\"center\" width=\""+ sett().numberToString(currentPercent) + "%\">" + trUtf8("Kod") + "</td>";
        currentPercent = 0;
    } else {
        currentPercent += 7;
    }
    if (sett().value("faktury_pozycje/pkwiu").toBool()) {
        currentPercent += 7;
        invStrList += "<td align=\"center\" width=\""+ sett().numberToString(currentPercent) + "%\">" + trUtf8("PKWiU") + "</td>";
        currentPercent = 0;
    } else {
        currentPercent += 7;
    }
    if (sett().value("faktury_pozycje/ilosc").toBool()) {
        invStrList += "<td align=\"center\" width=\""+ sett().numberToString(7) + "%\">" + trUtf8("Ilość") + "</td>";
    } else {
        currentPercent += 7;
    }
    if (sett().value("faktury_pozycje/jm").toBool()) {
        invStrList += "<td align=\"center\" width=\""+ sett().numberToString(3) + "%\">" + trUtf8("jm.") + "</td>";
    } else {
        currentPercent += 3;
    }
    if (sett().value("faktury_pozycje/cenajedn").toBool()) {
        currentPercent += 7;
        invStrList += "<td align=\"center\" width=\""+ sett().numberToString(currentPercent) + "%\">" + trUtf8("Cena jdn.") + "</td>";
        currentPercent = 0;
    } else {
        currentPercent += 7;
    }
    if (sett().value("faktury_pozycje/wartnetto").toBool()) {
        currentPercent += 7;
        invStrList += "<td align=\"center\" width=\""+ sett().numberToString(currentPercent) + "%\">" + trUtf8("Wartość Netto") + "</td>";
        currentPercent = 0;
    } else {
        currentPercent += 7;
    }
    if (sett().value("faktury_pozycje/rabatperc").toBool()) {
        invStrList += "<td align=\"center\" width=\""+ sett().numberToString(3) + "%\">" + trUtf8("Rabat %") + "</td>";
    } else {
        currentPercent += 3;
    }
    if (sett().value("faktury_pozycje/rabatval").toBool()) {
        currentPercent += 3;
        invStrList += "<td align=\"center\" width=\""+ sett().numberToString(currentPercent) + "%\">" + trUtf8("Rabat Wartość") + "</td>";
        currentPercent = 0;
    } else {
        currentPercent += 3;
    }
    if (sett().value("faktury_pozycje/nettoafter").toBool()) {
        currentPercent += 7;
        invStrList += "<td align=\"center\" width=\""+ sett().numberToString(currentPercent) + "%\">" + trUtf8("Netto po rabacie") + "</td>";
        currentPercent = 0;
    } else {
        currentPercent += 7;
    }
    if (sett().value("faktury_pozycje/vatval").toBool()) {
        invStrList += "<td align=\"center\" width=\""+ sett().numberToString(7) + "%\">" + trUtf8("Stawka VAT") + "</td>";
    } else {
        currentPercent += 7;
    }
    if (sett().value("faktury_pozycje/vatprice").toBool()) {
        currentPercent += 7;
        invStrList += "<td align=\"center\" width=\""+ sett().numberToString(currentPercent) + "%\">" + trUtf8("Kwota Vat") + "</td>";
        currentPercent = 0;
    } else {
        currentPercent += 7;
    }
    if (sett().value("faktury_pozycje/bruttoval").toBool()) {
        currentPercent += 7;
        invStrList += "<td align=\"center\" width=\""+ sett().numberToString(currentPercent) + "%\">" + trUtf8("Wartość Brutto") + "</td>";
        currentPercent = 0;
    } else {
        currentPercent += 7;
    }
    invStrList += "</tr>";

}

/**
 * @brief
 *
 */
void InvoiceDialog::makeInvoiceProducts() {
    invStrList += "<tr><td>";
    invStrList += "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\" width=\"100%\">";

    makeInvoiceProductsHeader();

    for (int i = 0; i < tableWidgetCommodities->rowCount(); ++i)
    {
        invStrList += "<tr valign=\"middle\" align=\"center\" class=\"products\">";
        // lp, nazwa, kod, pkwiu, ilosc, jm, rabat, cena jm., netto, vat, brutto
        if (sett().value("faktury_pozycje/Lp").toBool())
            invStrList += "<td align=\"center\" >" + sett().numberToString(i + 1) + "</td>";
        if (sett().value("faktury_pozycje/Nazwa") .toBool())
            invStrList += "<td align=\"left\">" + tableWidgetCommodities->item(i, 1)->text() + "</td>";
        if (sett().value("faktury_pozycje/Kod") .toBool())
            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, 2)->text() + "</td>";
        if (sett().value("faktury_pozycje/pkwiu") .toBool())
            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, 3)->text() + "</td>";
        if (sett().value("faktury_pozycje/ilosc") .toBool())
            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, 4)->text() + "</td>";
        if (sett().value("faktury_pozycje/jm") .toBool())
            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, 5)->text() + "</td>";
        if (sett().value("faktury_pozycje/cenajedn") .toBool())
            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, 7)->text() + "</td>";
        const double discountVal = sett().stringToDouble(tableWidgetCommodities->item(i, 8)->text()) *
                (tableWidgetCommodities->item(i, 6)->text().toDouble() * 0.01);
        const double nettMinusDisc = sett().stringToDouble(tableWidgetCommodities->item(i, 8)->text()) - discountVal;
        if (sett().value("faktury_pozycje/wartnetto") .toBool())
            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, 8)->text()	+ "</td>"; // netto
        if (sett().value("faktury_pozycje/rabatperc") .toBool())
            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, 6)->text()	+ "%</td>"; // rabat
        if (sett().value("faktury_pozycje/rabatval") .toBool())
            invStrList += "<td align=\"center\" >" + sett().numberToString(discountVal, 'f',  2)	+ "</td>";
        if (sett().value("faktury_pozycje/nettoafter") .toBool())
            invStrList += "<td align=\"center\" >" + sett().numberToString(nettMinusDisc, 'f', 2) + "</td>";
        if (sett().value("faktury_pozycje/vatval") .toBool())
            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, 9)->text()	+ "%</td>";
        const double vatPrice = sett().stringToDouble(tableWidgetCommodities->item(i, 10)->text())
                - sett().stringToDouble(tableWidgetCommodities->item(i, 8)->text()); // brutt-nett :)
        if (sett().value("faktury_pozycje/vatprice") .toBool())
            invStrList += "<td align=\"center\" >" + sett().numberToString(vatPrice, 'f', 2) + "</td>";
        if (sett().value("faktury_pozycje/bruttoval") .toBool())
            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, 10)->text() + "</td>";
        invStrList += "</tr>";
    }

    invStrList += "</table>";
}

/**
 * @brief
 *
 */
void InvoiceDialog::makeInvoiceSumm() {
    const double vatPrice = sett().stringToDouble(labelSumGrossVal->text()) - sett().stringToDouble(labelSumNetVal->text());
    invStrList += "<br><table width=\"100%\" border=\"0\" cellpadding=\"5\">";
    invStrList += "<tr class=\"productsSumHeader\" valign=\"middle\">";
    invStrList += "<td width=\"67%\" align=\"center\">&nbsp;</td>";
    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Wartość Netto") + "</td>"; // netto
    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Kwota VAT") + "</td>";// vat
    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Wartość Brutto") + "</td>"; // brutto
    invStrList += "</tr><tr class=\"productsSum\">";
    invStrList += "<td align=\"right\">" + trUtf8("Razem:") + "</td>";
    invStrList += "<td align=\"center\">" + labelSumNetVal->text() + "</td>"; // netto
    invStrList += "<td align=\"center\">" + sett().numberToString(vatPrice, 'f', 2) + "</td>";// vat
    invStrList += "<td align=\"center\">" + labelSumGrossVal->text() + "</td>"; // brutto
    invStrList += "</tr>";
    invStrList += "</table><br><br>";
}

/**
 * @brief
 *
 */
void InvoiceDialog::makeInvoiceSummAll() {
    invStrList += "</td></tr>"; // closing products row
    invStrList += "<tr comment=\"podsumowanie\"><td>";

    invStrList += "<table width=\"100%\" border=\"0\">";
    invStrList += "<tr class=\"summary\">";
    invStrList += "<td width=\"3%\">&nbsp;</td>";
    invStrList += "<td width=\"48%\"><span style=\"toPay\">";
    invStrList += trUtf8("Do zapłaty: ") + labelSumGrossVal->text() + " "
            + comboBoxCurrency->currentText() + "</span><br>";

    invStrList += trUtf8("słownie:")
            + ConvertAmount::convertPL(labelSumGrossVal->text(), comboBoxCurrency->currentText()) + "<br>";
    //QString paym1 = sett().value("paym1").toString();
    if (comboBoxPayment->currentIndex() == 0) {
        invStrList += trUtf8("forma płatności: ") + comboBoxPayment->currentText() + "<br><b>";
        invStrList += trUtf8("Zapłacono gotówką") + "<br>";
    } else if ((comboBoxPayment->currentIndex() == comboBoxPayment->count() -1) && (custPaymData != NULL)) {
        invStrList += "<span style=\"toPay\">";
        invStrList += QString(trUtf8("Zapłacono: ") + custPaymData->payment1 + ": "
                              +  sett().numberToString(custPaymData->amount1) + " " + comboBoxCurrency->currentText() + " "
                              + custPaymData->date1.toString(sett().getDateFormat()) + "<br>");
        invStrList += QString(trUtf8("Zaległości: ") + custPaymData->payment2 + ": "
                              +  sett().numberToString(custPaymData->amount2) + " " + comboBoxCurrency->currentText() + " "
                              + custPaymData->date2.toString(sett().getDateFormat()));
        invStrList += "</span>";
    }  else {
        invStrList += trUtf8("forma płatności: ") + comboBoxPayment->currentText() + "<br><b>";
        invStrList += "<span style=\"payDate\">";
        invStrList += trUtf8("termin płatności: ")
                + dateEditDayOfPayment->date().toString(sett().getDateFormat())	+ "<br>";
        invStrList += "</span>";
    }

    invStrList += "</b><br><br>";
    invStrList += "<span class=\"additionalText\">"	+ lineEditAdditionalText->text() + "</span>";
    invStrList += "</td>";
    invStrList += "<td width=\"3%\">&nbsp;</td>";
    invStrList += "<td width=\"48%\" valign=\"top\">";

    invStrList += "<table width=\"90%\" border=\"0\">";
    invStrList += "<tr class=\"stawkiHeader\"><td colspan=\"4\">";
    invStrList += trUtf8("Ogółem stawkami:");
    invStrList += "</td></tr>";
    invStrList += getGroupedSums();
    invStrList += "<tr>";
    invStrList += "<td>&nbsp;</td>"; // netto
    invStrList += "<td>&nbsp;</td>"; // stawka
    invStrList += "<td>&nbsp;</td>"; // podatek
    invStrList += "<td>&nbsp;</td>"; // brutto
    invStrList += "</tr>";
    invStrList += "</table>";

    invStrList += "</td>";
    invStrList += "</tr>";
    invStrList += "</table>";

    invStrList += "</td></tr>";
}

/**
 * @brief
 *
 */
void InvoiceDialog::makeInvoiceFooter() {
    invStrList += "<tr comment=\"podpis\" align=\"center\"><td>";
    invStrList += "<br><br><br><br>";

    invStrList += "<table width=\"80%\" border=\"0\">";
    invStrList += "<tr>";
    invStrList += "<td width=\"3%\">&nbsp;</td>";
    invStrList += "<td width=\"43%\" align=\"center\"> ";
    invStrList += "<hr width=\"100%\" noshade=\"noshade\" color=\"black\" />";
    invStrList += "</td>";
    invStrList += "<td width=\"7%\">&nbsp;</td>";
    invStrList += "<td width=\"3%\">&nbsp;</td>";
    invStrList += "<td width=\"43%\" align=\"center\"> ";
    invStrList += "<hr width=\"100%\" noshade=\"noshade\" color=\"black\" />";
    invStrList += "</td>";
    invStrList += "</tr>";
    invStrList += "<tr class=\"signature\">";
    invStrList += "<td width=\"3%\">&nbsp;</td>";
    invStrList += "<td width=\"43%\" align=\"center\"> ";
    invStrList += trUtf8("Imię i nazwisko osoby upoważnionej")
            + "<br>" + trUtf8(" do wystawienia faktury VAT");
    invStrList += "</td>";
    invStrList += "<td width=\"7%\">&nbsp;</td>";
    invStrList += "<td width=\"3%\">&nbsp;</td>";
    invStrList += "<td width=\"43%\" align=\"center\">";
    invStrList += trUtf8("Imię i nazwisko osoby upoważnionej")
            + "<br>" + trUtf8(" do odbioru faktury VAT");
    invStrList += "</td>";
    invStrList += "</tr>";
    invStrList += "</table>";

    invStrList += "</td></tr>";
    invStrList += "</table>";

}

/**
 * @brief
 *
 */
void InvoiceDialog::makeInvoiceFooterHtml() {
    invStrList += "</body>";
    invStrList += "</html>";
}

/** getGroupedSums
 *  Groups prices by VAT
 */
/**
 * @brief
 *
 * @return QString
 */
QString InvoiceDialog::getGroupedSums() {
    QStringList out;
    const QStringList rates = sett().value("stawki").toString().split("|");
    QMap<int, double> rateNet;
    QMap<int, double> rateVat;
    QMap<int, double> ratesGross;
    // every currency in int value is equal vat currency

    const int ssize = rates.size();

    // lp, nazwa, kod, pkwiu, ilosc, jm, rabat, cena jm., netto, vat, brutto
    for (int i = 0; i < tableWidgetCommodities->rowCount(); ++i)
    {
        for (int y = 0; y < ssize; ++y)
        {
            if (rates[y] == tableWidgetCommodities->item(i, 9)->text())
            {
                rateNet[y] += sett().stringToDouble(tableWidgetCommodities->item(i, 8)->text());
                ratesGross[y] += sett().stringToDouble(tableWidgetCommodities->item(i, 10)->text());
                rateVat[y] = ratesGross[y] - rateNet[y];
            }
            else
            {
                rateNet[y] += 0;
                ratesGross[y] += 0;
                rateVat[y] += 0;
            }
        }
    }

    for (int y = 0; y < ssize; ++y)
    {
        out += "<tr class=\"stawki\">";
        out += "<td width=\"30%\">" + sett().numberToString(rateNet[y], 'f', 2) + "</td>"; // netto
        out += "<td width=\"10%\">" + rates[y] + "</td>"; // stawka
        out += "<td width=\"30%\">" + sett().numberToString(rateVat[y], 'f', 2) + "</td>"; // podatek
        out += "<td width=\"30%\">" + sett().numberToString(ratesGross[y], 'f', 2) + "</td>"; // brutto
        out += "</tr>";

    }
    return out.join(" ");
}
// Generate Invoice HTML methods --- END ---


/** Sets the editability
 */
/**
 * @brief
 *
 * @param isAllowed
 */
void InvoiceDialog::setIsEditAllowed(const bool isAllowed) {
    isEdit = true;
    lineEditInvNumber->setEnabled(isAllowed);
    dateEditDateOfSell->setEnabled(isAllowed);
    dateEditDateOfIssuance->setEnabled(isAllowed);
    tableWidgetCommodities->setEnabled(isAllowed);
    spinBoxDiscount->setEnabled(isAllowed);
    labelDiscount1->setEnabled(isAllowed);
    comboBoxPayment->setEnabled(isAllowed);
    dateEditDayOfPayment->setEnabled(isAllowed);
    lineEditAdditionalText->setEnabled(isAllowed);
    pushButtonAddCommodity->setEnabled(isAllowed);
    pushButtonRemoveCommodity->setEnabled(isAllowed);
    pushButtonEditCommodity->setEnabled(isAllowed);
    checkBoxDiscount->setEnabled(isAllowed);
    comboBoxCounterparties->setEnabled(isAllowed);
    comboBoxCurrency->setEnabled(isAllowed);
    pushButtonSave->setEnabled(isAllowed);
    pushButtonAddCounterparty->setEnabled(isAllowed);
    dateEditDayOfPayment->setEnabled(isAllowed);
    if (!isAllowed || spinBoxDiscount->value() == 0) {
        checkBoxDiscount->setChecked(false);
        spinBoxDiscount->setEnabled(false);
    } else {
        checkBoxDiscount->setChecked(true);
        spinBoxDiscount->setEnabled(true);
    }
    if (isAllowed && (comboBoxPayment->currentIndex() > 0)) {
        dateEditDayOfPayment->setEnabled(true);
    } else {
        dateEditDayOfPayment->setEnabled(false);
    }
}



/** Calculate sum
 */
/**
 * @brief
 *
 */
void InvoiceDialog::calculateSum()
{
    netTotal = discountTotal = grossTotal = 0.0;

    for (int i = 0; i < tableWidgetCommodities->rowCount(); ++i)
    {
        const double quantity = sett().stringToDouble(tableWidgetCommodities->item(i, CommodityVisualFields::QUANTITY)->text());
        const double netVal = sett().stringToDouble(tableWidgetCommodities->item(i, CommodityVisualFields::NET)->text());
        const double vatRate = 0.01 * sett().stringToDouble(tableWidgetCommodities->item(i, CommodityVisualFields::VAT)->text());
        const double discountRate = 0.01 * sett().stringToDouble(tableWidgetCommodities->item(i, CommodityVisualFields::DISCOUNT)->text());

        const double grossVal = netVal + netVal * vatRate;
        const double discountValue = discountRate * grossVal;

        netTotal += netVal * quantity;
        discountTotal += discountValue * quantity;
        grossTotal += (grossVal - discountValue) * quantity;
    }

    labelSumNetVal->setText(sett().numberToString(netTotal, 'f', 2));
    labelDiscountVal->setText(sett().numberToString(discountTotal, 'f', 2));
    labelSumGrossVal->setText(sett().numberToString(grossTotal, 'f', 2));
}


/**
 * @brief Generate the new number for newly created invoice
 *
 * @return QString The new invoice number
 */
QString InvoiceDialog::generateInvoiceNumber() const //TODO: introduce restarting numbering every new year
{
//    QString tmp, prefix, suffix;

//    if(comboBoxInvoiceType->currentIndex() == InvoiceTypeData::PRO_FORMA)
//    {
//        tmp = sett().value("fpro").toString();
//    }
//    else
//    {
//        tmp = sett().value("fvat").toString();
//    }

//    prefix = sett().value("prefix").toString();

//    QStringList one1 = tmp.split("/");
//    one1[0] = one1[0].remove(prefix);

//    int nr = one1[0].toInt() + 1;
//    QString lastInvoice = prefix + numbersCount(nr,
//                                        sett().value("chars_in_symbol").toInt());

//    if (sett().value("day") .toBool())
//        lastInvoice += "/" + QDate::currentDate().toString("dd");

//    if (sett().value("month") .toBool())
//        lastInvoice += "/" + QDate::currentDate().toString("MM");

//    if (sett().value("year") .toBool()) {
//        if (!sett().value("shortYear") .toBool())
//            lastInvoice += "/" + QDate::currentDate().toString("yy");
//        else
//            lastInvoice += "/" + QDate::currentDate().toString("yyyy");
//    }
//    suffix = sett().value("sufix").toString();
//    lastInvoice += suffix;

//    return lastInvoice;
    return QDate::currentDate().toString("yyyy/MM/dd") + QString("/%1").arg(db_->modelInvoice()->rowCount()); //the "rowCount()" includes the newly added empty row
}




/**
 * @brief
 *
 * @param commodities
 */
void InvoiceDialog::fillTableCommodity_(const QList<CommodityVisualData> &commodities)
{
    tableWidgetCommodities->setColumnCount(CommodityVisualFields::DISCOUNT - CommodityVisualFields::ID + 1);
    tableWidgetCommodities->setRowCount(commodities.size());
    for(int r = 0; r < commodities.size(); ++r)
    {
        for(int c = CommodityVisualFields::ID; c <= CommodityVisualFields::DISCOUNT; ++c)
        {
            tableWidgetCommodities->setItem(r, c, new QTableWidgetItem(commodities.at(r).field(c)));
        }
    }

    QStringList headers;
    for(int c = CommodityVisualFields::ID; c <= CommodityVisualFields::DISCOUNT; ++c)
    {
        headers.append(CommodityVisualData::header(c));
    }
    tableWidgetCommodities->setHorizontalHeaderLabels(headers);
    tableWidgetCommodities->hideColumn(CommodityVisualFields::ID);
    tableWidgetCommodities->resizeColumnsToContents();
}


/**
 * @brief
 *
 * @return InvoiceData
 */
InvoiceData InvoiceDialog::getInvoiceData_() const
{
    InvoiceData ret;
    ret.additText = lineEditAdditionalText->text();

    ret.counterpartyID = db_->modelCounterparty()->data(db_->modelCounterparty()->index(comboBoxCounterparties->currentIndex(),
                                                                                        CounterpartyFields::ID)).toLongLong();

    ret.currencyID = comboBoxCurrency->currentIndex() + 1;
    ret.discount = spinBoxDiscount->value();
    ret.invNumber = lineEditInvNumber->text();
    ret.issuanceDate = dateEditDateOfIssuance->date();
    ret.paymentDate = dateEditDayOfPayment->date();
    ret.paymentID = comboBoxPayment->currentIndex() + 1;
    ret.sellingDate = dateEditDateOfSell->date();
    ret.typeID = comboBoxInvoiceType->currentIndex() + 1;
    return ret;
}


/**
 * @brief
 *
 * @return QList<CommodityVisualData>
 */
QList<CommodityVisualData> InvoiceDialog::getCommoditiesVisualData_() const
{
    QList<CommodityVisualData> ret;

    for(int r = 0; r < tableWidgetCommodities->rowCount(); ++r)
    {
        CommodityVisualData d;
        for(int c = CommodityVisualFields::ID; c <= CommodityVisualFields::DISCOUNT; ++c)
        {
            d.setField(c, tableWidgetCommodities->item(r, c)->data(Qt::DisplayRole));
        }
        ret.append(d);
    }

    return ret;
}


/**
 * @brief
 *
 */
void InvoiceDialog::setInitialComboBoxIndexes_()
{
    comboBoxInvoiceType->setCurrentIndex(InvoiceTypeData::VAT - 1);
    comboBoxPayment->setCurrentIndex(PaymentTypeData::CASH - 1);
    comboBoxCurrency->setCurrentIndex(CurrencyData::PLN - 1); //TODO: i18n
}
