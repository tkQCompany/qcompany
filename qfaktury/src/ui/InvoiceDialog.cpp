#include "InvoiceDialog.h"
#include "ui_InvoiceDialog.h"


/**
 * @brief
 *
 * @param parent
 * @param db
 * @param id_edit
 */
InvoiceDialog::InvoiceDialog(QWidget *parent, Database *db, const int invoiceType, const QModelIndex &id_edit) :
    QDialog(parent), db_(db), ui_(new Ui::InvoiceDialog)
{
    ui_->setupUi(this);
    init_();

    if(id_edit.isValid())
    {
        mapper_.setCurrentIndex(id_edit.row());
        fillTableCommodity_(db_->commodities(id_edit.data().toLongLong()));
        ui_->pushButtonSave->setEnabled(false);
    }
    else
    {
        db_->modelInvoice()->insertRow(db_->modelInvoice()->rowCount());
        mapper_.toLast();

        SettingsGlobal s;
        const QString invoiceTypeStr(InvoiceTypeData::name(invoiceType));
        setInitialComboBoxIndexes_(invoiceTypeStr, PaymentTypeData::name(PaymentTypeData::CASH), s.value(s.DEFAULT_CURRENCY).toString());
        setWindowTitle(invoiceTypeStr);

        updateInvoiceNumber();

        const QString additText(s.value(s.ADDIT_TEXT).toString());
        if(!additText.isEmpty())
        {
            ui_->lineEditAdditionalText->setText(additText);
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
    delete ui_;
}


/** Init method
 */
/**
 * @brief
 *
 */
void InvoiceDialog::init_()
{
    ui_->dateEditDateOfSell->setDate(QDate::currentDate());
    ui_->dateEditDateOfIssuance->setDate(QDate::currentDate());
    ui_->dateEditDayOfPayment->setDate(QDate::currentDate());

    connect(ui_->pushButtonAddCommodity, SIGNAL(clicked()), this, SLOT(addCommodity()));
    connect(ui_->pushButtonMoreInfo, SIGNAL(clicked()), this, SLOT(counterpartyMoreInfo_()));
    connect(ui_->pushButtonRemoveCommodity, SIGNAL(clicked()), this, SLOT(delCommodity_()));
    connect(ui_->pushButtonEditCommodity, SIGNAL(clicked()), this, SLOT(editCommodity_()));
    connect(ui_->pushButtonClose, SIGNAL(clicked()), this, SLOT(canQuit()));
    connect(ui_->pushButtonSave, SIGNAL(clicked()), this, SLOT(saveInvoice()));
    connect(ui_->pushButtonPrint, SIGNAL(clicked()), this, SLOT(printInvoice()));
    connect(ui_->pushButtonAddCounterparty, SIGNAL(clicked()), this, SLOT(counterpartyAdd_()));
    connect(ui_->tableWidgetCommodities, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editCommodity_()));
    connect(ui_->tableWidgetCommodities, SIGNAL(itemActivated(QTableWidgetItem *)), this, SLOT(tableActivated_(QTableWidgetItem *)));
    connect(ui_->tableWidgetCommodities, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(tableActivated_(QTableWidgetItem *)));
    connect(ui_->lineEditAdditionalText, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
    connect(ui_->lineEditInvNumFormat, SIGNAL(textChanged(QString)), this, SLOT(updateInvoiceNumber()));
    connect(ui_->comboBoxPayment, SIGNAL(currentIndexChanged (QString)), this, SLOT(payTextChanged(QString)));
    connect(ui_->comboBoxCurrency, SIGNAL(currentIndexChanged (QString)), this, SLOT(textChanged(QString)));
    connect(ui_->comboBoxCounterparties, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateInvoiceNumberFormat()));
    connect(ui_->comboBoxInvoiceType, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateInvoiceNumberFormat()));
    connect(ui_->dateEditDateOfSell, SIGNAL(dateChanged(QDate)), this, SLOT(dateChanged_(QDate)));
    connect(ui_->dateEditDateOfIssuance, SIGNAL(dateChanged(QDate)), this, SLOT(dateChanged_(QDate)));
    connect(ui_->dateEditDateOfIssuance, SIGNAL(dateChanged(QDate)), this, SLOT(updateInvoiceNumberFormat()));
    connect(ui_->dateEditDayOfPayment, SIGNAL(dateChanged(QDate)), this, SLOT(dateChanged_(QDate)));
    connect(ui_->spinBoxDiscount, SIGNAL(valueChanged(int)), this, SLOT(discountChange()));
    connect(ui_->checkBoxDiscount, SIGNAL(stateChanged(int)), this, SLOT(discountConstChange()));

    ui_->pushButtonRemoveCommodity->setEnabled(false);
    ui_->pushButtonEditCommodity->setEnabled(false);
    ui_->dateEditDayOfPayment->setEnabled(false);
    ui_->spinBoxDiscount->setEnabled(false);

    ui_->comboBoxInvoiceType->setModel(db_->modelInvoiceType());
    ui_->comboBoxInvoiceType->setModelColumn(InvoiceTypeFields::TYPE);

    ui_->comboBoxCounterparties->setModel(db_->modelCounterparty());
    ui_->comboBoxCounterparties->setModelColumn(CounterpartyFields::NAME);
    ui_->comboBoxCounterparties->setInsertPolicy(QComboBox::InsertAtBottom);
    ui_->comboBoxPayment->setModel(db_->modelPaymentType());
    ui_->comboBoxPayment->setModelColumn(PaymentTypeFields::TYPE);
    ui_->comboBoxCurrency->setModel(db_->modelCurrency());
    ui_->comboBoxCurrency->setModelColumn(CurrencyFields::CODE);

    mapper_.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper_.setItemDelegate(new QSqlRelationalDelegate(this));
    mapper_.setModel(db_->modelInvoice());
    mapper_.addMapping(ui_->lineEditInvNumber, InvoiceFields::INV_NUMBER);
    mapper_.addMapping(ui_->dateEditDateOfSell, InvoiceFields::SELLING_DATE);
    mapper_.addMapping(ui_->comboBoxInvoiceType, InvoiceFields::TYPE_ID, "currentIndex");
    mapper_.addMapping(ui_->comboBoxCounterparties, InvoiceFields::COUNTERPARTY_ID, "currentIndex");
    mapper_.addMapping(ui_->dateEditDateOfIssuance, InvoiceFields::ISSUANCE_DATE);
    mapper_.addMapping(ui_->dateEditDayOfPayment, InvoiceFields::PAYMENT_DATE);
    mapper_.addMapping(ui_->comboBoxPayment, InvoiceFields::PAYMENT_ID);
    mapper_.addMapping(ui_->comboBoxCurrency, InvoiceFields::CURRENCY_ID, "currentIndex");
    mapper_.addMapping(ui_->lineEditAdditionalText, InvoiceFields::ADDIT_TEXT);
    mapper_.addMapping(ui_->spinBoxDiscount, InvoiceFields::DISCOUNT);

    unsaved = false;
    retranslateUi_();
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
            ui_->comboBoxCounterparties->setCurrentIndex(db_->modelCounterparty()->rowCount() - 1);
        }
        else
        {
            QMessageBox::warning(this, trUtf8("Błąd dodawania kontrahenta"), db_->modelCounterparty()->lastError().text());
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
    if(ui_->comboBoxCounterparties->currentIndex() != -1)
    {
        CounterpartyInfoDialog dialog(this, db_, ui_->comboBoxCounterparties->
                                      model()->index(ui_->comboBoxCounterparties->
                                            currentIndex(), CounterpartyFields::ID));
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
    if(ui_->checkBoxDiscount->isChecked())
    {
        ui_->spinBoxDiscount->setEnabled(true);
        ui_->pushButtonSave->setEnabled(true);
        calculateSum();
    }
    else
    {
        ui_->spinBoxDiscount->setValue(0);
        ui_->spinBoxDiscount->setEnabled(false);
    }
}


void InvoiceDialog::updateInvoiceNumber()
{
    if( (!ui_->lineEditInvNumFormat->text().isEmpty()) &&
            ui_->dateEditDateOfIssuance->date().isValid() &&
            (ui_->comboBoxInvoiceType->currentIndex() != -1) &&
            (ui_->comboBoxCounterparties->currentIndex() != -1))
    {
        genInvoiceNumber_(ui_->lineEditInvNumFormat->text(),
                          ui_->dateEditDateOfIssuance->date(),
                          InvoiceTypeData::StringToInvoiceType(ui_->comboBoxInvoiceType->currentText()),
                          ui_->comboBoxCounterparties->currentText());
    }
}


void InvoiceDialog::updateInvoiceNumberFormat()
{
    ui_->lineEditInvNumFormat->setText(db_->modelInvoice()->
                                       getInvoiceNumberFormat(ui_->
                                        comboBoxCounterparties->currentText()));
}

/**
 * @brief Used to recalculate discount when spinBox arrows are pressed.
 *
 */
void InvoiceDialog::discountChange()
{
    calculateSum();
    ui_->pushButtonSave->setEnabled(true);
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
        const int rowNum = ui_->tableWidgetCommodities->rowCount() == 0 ? 0 : ui_->tableWidgetCommodities->rowCount() - 1;
        ui_->tableWidgetCommodities->insertRow(rowNum);

        ui_->tableWidgetCommodities->setColumnCount(CommodityVisualFields::DISCOUNT - CommodityVisualFields::ID + 1);

        QStringList headers;
        for(int i = CommodityVisualFields::ID; i <= CommodityVisualFields::DISCOUNT; ++i)
        {
            ui_->tableWidgetCommodities->setItem(rowNum, i, new QTableWidgetItem(dialog.ret.field(i)));
            headers.push_back(CommodityVisualData::header(i));
        }

        ui_->tableWidgetCommodities->hideColumn(CommodityVisualFields::ID);
        ui_->tableWidgetCommodities->setHorizontalHeaderLabels(headers);
        ui_->tableWidgetCommodities->resizeColumnsToContents();

        ui_->pushButtonSave->setEnabled(true);
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
    ui_->tableWidgetCommodities->removeRow(ui_->tableWidgetCommodities->currentRow());
    calculateSum();
    ui_->pushButtonSave->setEnabled(true);
    unsaved = true;
}




/**
 * @brief Allows to edit selected product. Opens ChangeAmountDialog window.
 *
 */
void InvoiceDialog::editCommodity_()
{
    // we can only modify quantity
//    ChangeAmountDialog changeAmount(this);
//    changeAmount.lineEditCommodityName->setText(ui_->tableWidgetCommodities-> item(ui_->tableWidgetCommodities->currentRow(), 1)->text());
//    changeAmount.lineEditCommodityCode->setText(ui_->tableWidgetCommodities-> item(ui_->tableWidgetCommodities->currentRow(), 2)->text());
//    changeAmount.spinBoxCommodityAmount->setValue(ui_->tableWidgetCommodities-> item(ui_->tableWidgetCommodities->currentRow(), 4)->text().toInt());
//    if (changeAmount.exec() == QDialog::Accepted)
//    {
//        const int currentRow = ui_->tableWidgetCommodities->currentRow();
//        ui_->tableWidgetCommodities->item(currentRow,
//                     CommodityVisualFields::QUANTITY)->
//                        setText(changeAmount.spinBoxCommodityAmount->cleanText());
//        ui_->pushButtonSave->setEnabled(true);
//        unsaved = true;
//        calculateSum();
//    }
}


/**
 * @brief Asks the user if save the invoice
 *
 */
void InvoiceDialog::canQuit()
{
    // canClose == false -> data changed
    if (unsaved || QMessageBox::warning(this, qApp->applicationName(), trUtf8("Dane zostały zmienione, czy chcesz zapisać?"),
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


void InvoiceDialog::genInvoiceNumber_(const QString& invNumFormat, const QDate& issuanceDate, const int invoiceType, const QString& counterpartyName)
{
    const QString invNum(db_->modelInvoice()->generateInvoiceNumber(invNumFormat, issuanceDate,
                                                                    InvoiceTypeData::name(invoiceType),
                                                                    counterpartyName));
    ui_->lineEditInvNumber->setText(invNum);
}


/**
 * @brief
 *
 * @param item
 */
void InvoiceDialog::tableActivated_(QTableWidgetItem * item)
{
    if (item != nullptr && !item->text().isEmpty())
    {
        ui_->pushButtonRemoveCommodity->setEnabled(true);
        ui_->pushButtonEditCommodity->setEnabled(true);
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
    if(ui_->comboBoxPayment->currentIndex() > 0)
    {
        ui_->dateEditDayOfPayment->setEnabled(true);
    }
    else
    {
        ui_->dateEditDayOfPayment->setEnabled(false);
    }

    if(ui_->comboBoxPayment->currentIndex() == ui_->comboBoxPayment->count() - 1)
    {
        SettingsGlobal settings;
        if (settings.stringToDouble(ui_->labelSumGrossVal->text()) == 0)
        {
            QMessageBox::warning(this, qApp->applicationName(), trUtf8("Taki sposób płatności nie może zostać wybrany, ponieważ kwota do zapłaty wynosi 0."));
            ui_->comboBoxPayment->setCurrentIndex(0);
            return;
        }
        CustomPaymentDialog cp;
        cp.setInvoiceAmount(settings.stringToDouble(ui_->labelSumGrossVal->text()));
        if (cp.exec() ==  QDialog::Accepted)
        {
            custPaymData = cp.custPaymData;
            ui_->dateEditDayOfPayment->setEnabled(false);
        }
        else
        {
            ui_->comboBoxPayment->setCurrentIndex(0);
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
    ui_->pushButtonSave->setEnabled(true);
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
    ui_->pushButtonSave->setEnabled(true);
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
            saveFailed = true;
            unsaved = true;
        }
        ui_->pushButtonSave->setEnabled(false);
        ui_->pushButtonRemoveCommodity->setEnabled(false);
        ui_->pushButtonEditCommodity->setEnabled(false);
    }
    else
    {
        qDebug() << "InvoiceDialog::saveInvoice() - mapper_submit - lastError: "
                 << db_->modelInvoice()->lastError().text();
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
void InvoiceDialog::printInvoice()
{
    if(!validateForm_())
        return;

    SettingsGlobal s;
    QTextStream stream;
    QString invoiceHTMLTemplate, styleCSS;

    QFile file(":/res/templates/template.html");
    if(file.open(QIODevice::ReadOnly))
    {
        stream.setDevice(&file);
        invoiceHTMLTemplate = stream.readAll();
        file.close();

        file.setFileName("style.css");
        if(file.open(QIODevice::ReadOnly))
        {
            stream.setDevice(&file);
            styleCSS = stream.readAll();
            file.close();
        }
        else
        {
            qDebug() << "File " << file.fileName() << " couldn't be opened.";
        }

        const QString logo(s.value(s.LOGO).toString());
        const QString stampStr(logo.isEmpty() ? trUtf8("Pieczęć wystawcy") : QString("<img src=\"%1\">").arg(logo));

        QString sellerAttrList;

        s.beginGroup("printpos");
        if(s.value(s.SELLER_NAME).toBool())
            sellerAttrList += QString("<li>%1</li>").arg(s.value(s.SELLER_NAME).toString());
        if(s.value(s.SELLER_ADDRESS).toBool())
            sellerAttrList += QString("<li>%1</li>").arg(s.value(s.SELLER_ADDRESS).toString());
        if(s.value(s.SELLER_LOCATION).toBool())
            sellerAttrList += QString("<li>%1</li>").arg(s.value(s.SELLER_LOCATION).toString());
        if(s.value(s.SELLER_TAXID).toBool())
            sellerAttrList += trUtf8("<li>NIP: %1</li>").arg(s.value(s.SELLER_TAXID).toString());
        if(s.value(s.SELLER_ACCOUNT).toBool())
            sellerAttrList += trUtf8("<li>Nr konta: %1</li>").arg(s.value(s.SELLER_ACCOUNT).toString().replace("-", " "));
        s.endGroup();
        const QString sellerHTML(QString("<h1>Sprzedawca:</h1><ul>%1</ul>").arg(sellerAttrList));

        const QList<CommodityVisualData> products(getCommoditiesVisualData_());
        QString productsHTML;
        foreach(CommodityVisualData cvd, products)
        {
            productsHTML += "<tr>";
            if(s.contains(s.keyName(s.ORDER_NUMBER)))
            {
                productsHTML += QString("<td>%1</td>").arg(cvd.field(CommodityVisualFields::ID));
            }

            if(s.contains(s.keyName(s.NAME)))
            {
                productsHTML += QString("<td>%1</td>").arg(cvd.field(CommodityVisualFields::NAME));
            }

            if(s.contains(s.keyName(s.PKWIU)))
            {
                productsHTML += QString("<td>%1</td>").arg(cvd.field(CommodityVisualFields::PKWIU));
            }

            if(s.contains(s.keyName(s.QUANTITY)))
            {
                productsHTML += QString("<td>%1</td>").arg(cvd.field(CommodityVisualFields::QUANTITY));
            }

            if(s.contains(s.keyName(s.INTERNAT_UNIT)))
            {
                productsHTML += QString("<td>%1</td>").arg(cvd.field(CommodityVisualFields::UNIT));
            }

            if(s.contains(s.keyName(s.NET_VAL)))
            {
                productsHTML += QString("<td>%1</td>").arg(cvd.field(CommodityVisualFields::NET));
            }

            if(s.contains(s.keyName(s.DISCOUNT)))
            {
                productsHTML += QString("<td>%1</td>").arg(cvd.field(CommodityVisualFields::DISCOUNT));
            }

            if(s.contains(s.keyName(s.VAT_VAL)))
            {
                productsHTML += QString("<td>%1</td>").arg(cvd.field(CommodityVisualFields::VAT));
            }

            productsHTML += "</tr>";
        }

        QString summaryHTML(QString("<span>%1 %2 %3</span>").arg(trUtf8("Do zapłaty:")).arg(ui_->labelSumGrossVal->text()).arg(ui_->comboBoxCurrency->currentText()));
        summaryHTML += QString("%1 %2").arg(trUtf8("słownie:")).arg(ConvertAmount::convertPL(ui_->labelSumGrossVal->text(), ui_->comboBoxCurrency->currentText()));

        if (ui_->comboBoxPayment->currentIndex() == 0)
        {
            summaryHTML += trUtf8("forma płatności: ") + ui_->comboBoxPayment->currentText() + "<b>";
            summaryHTML += trUtf8("Zapłacono gotówką");
        }
        else if((ui_->comboBoxPayment->currentIndex() == ui_->comboBoxPayment->count() -1) && (custPaymData != nullptr))
        {
            summaryHTML += "<span style=\"toPay\">";
            summaryHTML += QString(trUtf8("Zapłacono: ") + custPaymData->payment1 + ": "
                                  +  s.numberToString(custPaymData->amount1) + " " + ui_->comboBoxCurrency->currentText() + " "
                                  + custPaymData->date1.toString(s.dateFormatExternal()) + "<br>");
            summaryHTML += QString(trUtf8("Zaległości: ") + custPaymData->payment2 + ": "
                                  +  s.numberToString(custPaymData->amount2) + " " + ui_->comboBoxCurrency->currentText() + " "
                                  + custPaymData->date2.toString(s.dateFormatExternal()));
            summaryHTML += "</span>";
        }
        else
        {
            summaryHTML += trUtf8("forma płatności: ") + ui_->comboBoxPayment->currentText() + "<b>";
            summaryHTML += "<span style=\"payDate\">";
            summaryHTML += trUtf8("termin płatności: ") + ui_->dateEditDayOfPayment->date().toString(s.dateFormatExternal());
            summaryHTML += "</span></b>";
        }

        summaryHTML += "<span class=\"additionalText\">"	+ ui_->lineEditAdditionalText->text() + "</span>";
        summaryHTML += "</td>";
        summaryHTML += "<td width=\"3%\">&nbsp;</td>";
        summaryHTML += "<td width=\"48%\" valign=\"top\">";

        summaryHTML += "<table width=\"90%\" border=\"0\">";
        summaryHTML += "<tr class=\"stawkiHeader\"><td colspan=\"4\">";
        summaryHTML += trUtf8("Ogółem stawkami:");
        summaryHTML += "</td></tr>";
        summaryHTML += getGroupedSums();
        summaryHTML += "<tr>";
        summaryHTML += "<td>&nbsp;</td>"; // netto
        summaryHTML += "<td>&nbsp;</td>"; // stawka
        summaryHTML += "<td>&nbsp;</td>"; // podatek
        summaryHTML += "<td>&nbsp;</td>"; // brutto
        summaryHTML += "</tr>";
        summaryHTML += "</table>";
        summaryHTML += "</td>";
        summaryHTML += "</tr>";
        summaryHTML += "</table>";
        summaryHTML += "</td></tr>";


        docHTML = invoiceHTMLTemplate.arg(s.value(s.LANG).toString())
                                  .arg(ui_->comboBoxInvoiceType->currentText())
                                  .arg(styleCSS)
                                  .arg(stampStr)
                                  .arg(ui_->comboBoxInvoiceType->currentText())
                                  .arg(trUtf8("Nr: %1").arg(ui_->lineEditInvNumber->text()))
                                  .arg(trUtf8("Data wystawienia: %1").arg(ui_->dateEditDateOfIssuance->date().toString(s.dateFormatExternal())))
                                  .arg(trUtf8("Data sprzedaży: %1").arg(ui_->dateEditDateOfSell->date().toString(s.dateFormatExternal())))
                                  .arg(trUtf8("ORYGINAŁ"))
                                  .arg(sellerHTML)
                                  .arg(trUtf8("<h1>Nabywca:</h1><ul><li>%1</li></ul>").arg(ui_->comboBoxCounterparties->currentText()) )
                                  .arg(productsHTML)
                                  .arg(trUtf8("Wartość Netto"))
                                  .arg(trUtf8("Kwota VAT"))
                                  .arg(trUtf8("Wartość Brutto"))
                                  .arg(trUtf8("Razem:"))
                                  .arg(ui_->labelSumNetVal->text())
                                  .arg(s.numberToString(s.stringToDouble(ui_->labelSumGrossVal->text()) - s.stringToDouble(ui_->labelSumNetVal->text()), 'f', 2))
                                  .arg(ui_->labelSumGrossVal->text())
                                  .arg(summaryHTML);

        file.setFileName("debugInvoice.html");
        file.open(QIODevice::WriteOnly);
        stream.setDevice(&file);
        stream << docHTML;
        file.close();

        QPrinter printer(QPrinter::HighResolution);
        QPrintPreviewDialog preview(&printer, this);
        preview.setWindowFlags(Qt::Window);
        preview.setWindowTitle(trUtf8("%1 - Podgląd wydruku").arg(ui_->comboBoxInvoiceType->currentText()));
        connect(&preview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(printPaintRequested_(QPrinter*)));
        preview.exec();
    }
    else
    {
        qDebug() << "File " << file.fileName() << " couldn't be opened.";
    }

    //invStrList.clear();
    //makeInvoiceHeaderHTML(comboBoxInvoiceType->currentIndex() + 1);

    //makeInvoiceHeader(comboBoxInvoiceType->currentIndex() + 1, true, false, true);
    //makeInvoiceBody();
    //makeInvoiceProducts();
    //makeInvoiceSumm();
    //makeInvoiceSummAll();
    //makeInvoiceFooter();

    const int numberOfCopies = s.value(s.NUMBER_OF_COPIES, 2).toInt();
    for (int i = 0; i < numberOfCopies; ++i)
    {
        // print copy
        //makeInvoiceHeader(comboBoxInvoiceType->currentIndex() + 1, true, true, false);
        //makeInvoiceBody();
        //makeInvoiceProducts();
        //makeInvoiceSumm();
        //makeInvoiceSummAll();
        //makeInvoiceFooter();
    }

    //makeInvoiceFooterHtml();
    //print();
    //pushButtonSave->setFocus();
}


void InvoiceDialog::printPaintRequested_(QPrinter *printer)
{
    QTextDocument doc(this);
    doc.setHtml(docHTML);
    doc.print(printer);
}


void InvoiceDialog::retranslateUi_()
{
    QTranslator appTranslator;
    SettingsGlobal s;
    appTranslator.load(QString("translations/qfaktury_") + s.value(s.LANG).toString());
    qApp->installTranslator(&appTranslator);
    ui_->retranslateUi(this);
}


/** Check if data on the form is correct
 */
/**
 * @brief
 *
 * @return bool
 */
bool InvoiceDialog::validateForm_()
{
    if(ui_->lineEditInvNumber->text().isEmpty())
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Brak numeru kolejnego faktury"),
                              QMessageBox::Ok);
        saveFailed = true;
        return false;
    }

    if(ui_->comboBoxInvoiceType->currentIndex() == -1)
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Wybierz typ faktury."),
                              QMessageBox::Ok);
        saveFailed = true;
        return false;
    }

    if(ui_->comboBoxCounterparties->currentIndex() == -1)
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Wybierz kontrahenta."),
                              QMessageBox::Ok);
        saveFailed = true;
        return false;
    }

    if(ui_->tableWidgetCommodities->rowCount() == 0)
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Wybierz towary lub usługi do sprzedania."),
                              QMessageBox::Ok);
        saveFailed = true;
        return false;
    }


    if(ui_->comboBoxPayment->currentIndex() == -1)
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Wybierz rodzaj płatności."),
                              QMessageBox::Ok);
        saveFailed = true;
        return false;
    }


    if(ui_->comboBoxCurrency->currentIndex() == -1)
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Wybierz walutę rozliczeniową."),
                              QMessageBox::Ok);
        saveFailed = true;
        return false;
    }

    return true;
}



/**
 * @brief
 *
 */
//void InvoiceDialog::makeInvoiceHeaderHTML(const int invoiceType)
//{
//    invStrList += "<html><head>";
//    invStrList += "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />";
//    invStrList += "<meta name=\"creator\" value=\"http://www.e-linux.pl\" />";
//    invStrList += "</head>";

//    invStrList += "<title>"+ InvoiceTypeData::InvoiceTypeToString(invoiceType)  +"</title>";

//    invStrList += "<style type=\"text/css\"> ";
//    SettingsGlobal s;
//    QFile file(s.getTemplate());
//    if (file.open(QIODevice::ReadOnly))
//    {
//        QTextStream stream(&file);
//        QString line;
//        while (!stream.atEnd())
//        {
//            line = stream.readLine();
//            invStrList += line;
//        }
//        file.close();
//    }
//    else
//    {
//        qWarning() << "Could not open CSS file: " << file.fileName();
//    }
//    invStrList += "</style>";
//    invStrList += "<body>";
//}


/**
 * @brief
 *
 * @param sellDate
 * @param breakPage
 * @param original
 */
//void InvoiceDialog::makeInvoiceHeader(const int invoiceType, const bool sellDate, const bool breakPage, const bool original)
//{
//    QString breakPageStr = "class=\"page_break\"";
//    if (breakPage == false) breakPageStr = "";

//    invStrList += "<table comment=\"header table\" width=\"100%\" border=\"0\"" + breakPageStr + ">";
//    invStrList += "<tr>";
//    invStrList += "<td width=\"60%\" align=\"center\" valign=\"bottom\">";
//    invStrList += "<span class=\"stamp\">";

//    SettingsGlobal s;
//    const QString logo(s.value(s.LOGO)).toString());
//    if (!logo.isEmpty())
//    {
//        invStrList += "<img src=\"" + logo + "\" width=\"100\" " + " height=\"100\"+ >";
//    }
//    else
//    {
//        invStrList += trUtf8("Pieczęć wystawcy");
//    }
//    invStrList += "</span>";
//    invStrList += "</td>";

//    invStrList += "<td align=\"right\">";
//    invStrList += "<span style=\"font-size:12pt; font-weight:600\">";
//    invStrList += InvoiceTypeData::InvoiceTypeToString(invoiceType) + "<br/>";
//    invStrList += trUtf8("Nr: ") + lineEditInvNumber->text() + "<br></span>";
//    invStrList += "<span class=\"dates\">" + trUtf8("Data wystawienia: ")
//            + dateEditDateOfIssuance->date().toString(s.getDateFormat()) + "<br>";

//    if (sellDate)
//        invStrList += trUtf8("Data sprzedaży: ")
//                + dateEditDateOfSell->date().toString(s.getDateFormat())
//                + "<br>";
//    invStrList += "</span></td><td width=\"3%\">&nbsp;</td>";
//    invStrList += "</tr>";
//    invStrList += "<tr>";
//    invStrList += "<td colspan=\"2\" align=\"right\" valign=\"top\"><br>";
//    if (original) {
//        invStrList += trUtf8("ORYGINAŁ");
//    } else {
//        invStrList += trUtf8("KOPIA");
//    }
//    invStrList += "<br></td><td width=\"3%\">&nbsp;</td>";
//    invStrList += "</tr>";
//    invStrList += "</table>";
//    invStrList += "<hr>";
//    invStrList += "</td></tr>";
//}


/**
 * @brief
 *
 */
//void InvoiceDialog::makeInvoiceBody()
//{
//    invStrList += "<tr><td>";

//    invStrList += "<table width=\"100%\" border=\"0\">";
//    invStrList += "<tr class=\"persons\">";
//    invStrList += "<td width=\"20\">&nbsp;</td>";
//    invStrList += "<td width=\"48%\"> ";
//    invStrList += trUtf8("Sprzedawca:")+"<br>";

//    //QSettings userSettings("elinux", "user");
//    SettingsGlobal s;

//    s.beginGroup("printpos");
//    if (s.value(s.USER_NAME)).toBool())
//        invStrList += s.value(s.USER_NAME)).toString() + "<br>";
//    if (s.value(s.USER_ADDRESS)).toBool())
//        invStrList += s.value(s.USER_ADDRESS)).toString() + "<br>"; // trUtf8("Ul. ") +
////    if (settings.value("usermiejscowosc").toBool())
////        invStrList += userSettings.value("zip").toString() + " ";
//    if (s.value(s.USER_LOCATION)).toBool())
//        invStrList += s.value(s.USER_LOCATION)).toString() + "<br>";
//    if (s.value(s.USER_TAXID)).toBool())
//        invStrList += trUtf8("NIP: ") + s.value(s.USER_TAXID)).toString() + "<br>";
//    if (s.value(s.USER_ACCOUNT)).toBool())
//        invStrList += trUtf8("Nr konta: ")
//                + s.value(s.USER_ACCOUNT)).toString().replace("-", " ") + "<br>";
//    s.endGroup();

//    invStrList += "</td>";
//    invStrList += "<td width=\"20\">&nbsp;</td>";
//    invStrList += "<td width=\"48%\">";
//    invStrList += trUtf8("Nabywca:") + "<br>" + comboBoxCounterparties->currentText().replace(",", "<br>") + "<br>";
//    invStrList += "</td>";
//    invStrList += "</tr>";
//    invStrList += "</table>";

//    invStrList += "<hr>";
//    invStrList += "</td></tr>";
//}

/**
 * @brief
 *
 */
//void InvoiceDialog::makeInvoiceProductsHeader()
//{
//    SettingsGlobal s;
//    int currentPercent = 0;
//    invStrList += "<tr align=\"center\" valign=\"middle\" class=\"productsHeader\" >";

//    if (s.value(s.ORDER_NUMBER)).toBool())
//    {
//        currentPercent = 3;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Lp.") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 3;
//    }
//    if (s.value(s.NAME)).toBool())
//    {
//        currentPercent += 25;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Nazwa") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 25;
//    }
//    if (s.value(s.CODE)).toBool())
//    {
//        currentPercent += 7;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Kod") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 7;
//    }
//    if (s.value(s.PKWIU)).toBool())
//    {
//        currentPercent += 7;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("PKWiU") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 7;
//    }
//    if (s.value(s.QUANTITY)).toBool())
//    {
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(7) + "%\">" + trUtf8("Ilość") + "</td>";
//    } else {
//        currentPercent += 7;
//    }
//    if (s.value(s.INTERNAT_UNIT)).toBool())
//    {
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(3) + "%\">" + trUtf8("jm.") + "</td>";
//    } else {
//        currentPercent += 3;
//    }
//    if (s.value(s.UNIT_PRICE)).toBool())
//    {
//        currentPercent += 7;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Cena jdn.") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 7;
//    }
//    if (s.value(s.NET_VAL)).toBool())
//    {
//        currentPercent += 7;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Wartość Netto") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 7;
//    }
//    if (s.value(s.DISCOUNT)).toBool())
//    {
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(3) + "%\">" + trUtf8("Rabat %") + "</td>";
//    } else {
//        currentPercent += 3;
//    }
//    if (s.value(s.DISCOUNT_VAL)).toBool())
//    {
//        currentPercent += 3;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Rabat Wartość") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 3;
//    }
//    if (s.value(s.NET_AFTER)).toBool())
//    {
//        currentPercent += 7;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Netto po rabacie") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 7;
//    }
//    if (s.value(s.VAT_VAL)).toBool())
//    {
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(7) + "%\">" + trUtf8("Stawka VAT") + "</td>";
//    } else {
//        currentPercent += 7;
//    }
//    if (s.value(s.VAT_PRICE)).toBool())
//    {
//        currentPercent += 7;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Kwota Vat") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 7;
//    }
//    if (s.value(s.GROSS_VAL)).toBool())
//    {
//        currentPercent += 7;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Wartość Brutto") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 7;
//    }
//    invStrList += "</tr>";
//}

/**
 * @brief
 *
 */
//void InvoiceDialog::makeInvoiceProducts()
//{
//    SettingsGlobal s;

//    invStrList += "<tr><td>";
//    invStrList += "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\" width=\"100%\">";

//    makeInvoiceProductsHeader();

//    for (int i = 0; i < tableWidgetCommodities->rowCount(); ++i)
//    {
//        invStrList += "<tr valign=\"middle\" align=\"center\" class=\"products\">";
//        // lp, nazwa, kod, pkwiu, ilosc, jm, rabat, cena jm., netto, vat, brutto
//        if (s.value(s.ORDER_NUMBER)).toBool())
//            invStrList += "<td align=\"center\" >" + s.numberToString(i + 1) + "</td>";
//        if (s.value(s.NAME)).toBool())
//            invStrList += "<td align=\"left\">" + tableWidgetCommodities->item(i, CommodityVisualFields::NAME)->text() + "</td>";
//        //if (s.value(s.CODE)).toBool())
//          //  invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, CommodityVisualFields::)->text() + "</td>";
//        if (s.value(s.PKWIU)).toBool())
//            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, CommodityVisualFields::PKWIU)->text() + "</td>";
//        if (s.value(s.QUANTITY)).toBool())
//            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, CommodityVisualFields::QUANTITY)->text() + "</td>";
//        if (s.value(s.INTERNAT_UNIT)).toBool())
//            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, CommodityVisualFields::UNIT)->text() + "</td>";
//        //if (s.value(s.UNIT_PRICE)).toBool())
//          //  invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, CommodityVisualFields::)->text() + "</td>";
//        const double discountVal = s.stringToDouble(tableWidgetCommodities->item(i, CommodityVisualFields::NET)->text()) *
//                (tableWidgetCommodities->item(i, 6)->text().toDouble() * 0.01);
//        const double nettMinusDisc = s.stringToDouble(tableWidgetCommodities->item(i, CommodityVisualFields::NET)->text()) - discountVal;
//        if (s.value(s.NET_VAL)).toBool())
//            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, CommodityVisualFields::NET)->text()	+ "</td>"; // netto
//        if (s.value(s.DISCOUNT)).toBool())
//            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, CommodityVisualFields::DISCOUNT)->text()	+ "%</td>"; // rabat
//        if (s.value(s.DISCOUNT_VAL)).toBool())
//            invStrList += "<td align=\"center\" >" + s.numberToString(discountVal, 'f',  2)	+ "</td>";
//        if (s.value(s.NET_AFTER)).toBool())
//            invStrList += "<td align=\"center\" >" + s.numberToString(nettMinusDisc, 'f', 2) + "</td>";
//        if (s.value(s.VAT_VAL)).toBool())
//            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, CommodityVisualFields::VAT)->text()	+ "%</td>";
//        const double vatPrice = s.stringToDouble(tableWidgetCommodities->item(i, 10)->text())
//                - s.stringToDouble(tableWidgetCommodities->item(i, CommodityVisualFields::VAT)->text()); // brutt-nett :)
//        if (s.value(s.VAT_PRICE)).toBool())
//            invStrList += "<td align=\"center\" >" + s.numberToString(vatPrice, 'f', 2) + "</td>";
//        if (s.value(s.GROSS_VAL)).toBool())
//            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, 10)->text() + "</td>";
//        invStrList += "</tr>";
//    }

//    invStrList += "</table>";
//}

/**
 * @brief
 *
 */
//void InvoiceDialog::makeInvoiceSumm()
//{
//    SettingsGlobal s;

//    const double vatPrice = s.stringToDouble(labelSumGrossVal->text()) - s.stringToDouble(labelSumNetVal->text());
//    invStrList += "<br><table width=\"100%\" border=\"0\" cellpadding=\"5\">";
//    invStrList += "<tr class=\"productsSumHeader\" valign=\"middle\">";
//    invStrList += "<td width=\"67%\" align=\"center\">&nbsp;</td>";
//    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Wartość Netto") + "</td>"; // netto
//    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Kwota VAT") + "</td>";// vat
//    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Wartość Brutto") + "</td>"; // brutto
//    invStrList += "</tr><tr class=\"productsSum\">";
//    invStrList += "<td align=\"right\">" + trUtf8("Razem:") + "</td>";
//    invStrList += "<td align=\"center\">" + labelSumNetVal->text() + "</td>"; // netto
//    invStrList += "<td align=\"center\">" + s.numberToString(vatPrice, 'f', 2) + "</td>";// vat
//    invStrList += "<td align=\"center\">" + labelSumGrossVal->text() + "</td>"; // brutto
//    invStrList += "</tr>";
//    invStrList += "</table><br><br>";
//}

/**
 * @brief
 *
 */
//void InvoiceDialog::makeInvoiceSummAll()
//{
//    SettingsGlobal s;

//    invStrList += "</td></tr>"; // closing products row
//    invStrList += "<tr comment=\"podsumowanie\"><td>";

//    invStrList += "<table width=\"100%\" border=\"0\">";
//    invStrList += "<tr class=\"summary\">";
//    invStrList += "<td width=\"3%\">&nbsp;</td>";
//    invStrList += "<td width=\"48%\"><span style=\"toPay\">";
//    invStrList += trUtf8("Do zapłaty: ") + labelSumGrossVal->text() + " "
//            + comboBoxCurrency->currentText() + "</span><br>";

//    invStrList += trUtf8("słownie:")
//            + ConvertAmount::convertPL(labelSumGrossVal->text(), comboBoxCurrency->currentText()) + "<br>";
//    //QString paym1 = sett().value("paym1").toString();
//    if (comboBoxPayment->currentIndex() == 0) {
//        invStrList += trUtf8("forma płatności: ") + comboBoxPayment->currentText() + "<br><b>";
//        invStrList += trUtf8("Zapłacono gotówką") + "<br>";
//    } else if ((comboBoxPayment->currentIndex() == comboBoxPayment->count() -1) && (custPaymData != NULL)) {
//        invStrList += "<span style=\"toPay\">";
//        invStrList += QString(trUtf8("Zapłacono: ") + custPaymData->payment1 + ": "
//                              +  s.numberToString(custPaymData->amount1) + " " + comboBoxCurrency->currentText() + " "
//                              + custPaymData->date1.toString(s.getDateFormat()) + "<br>");
//        invStrList += QString(trUtf8("Zaległości: ") + custPaymData->payment2 + ": "
//                              +  s.numberToString(custPaymData->amount2) + " " + comboBoxCurrency->currentText() + " "
//                              + custPaymData->date2.toString(s.getDateFormat()));
//        invStrList += "</span>";
//    }  else {
//        invStrList += trUtf8("forma płatności: ") + comboBoxPayment->currentText() + "<br><b>";
//        invStrList += "<span style=\"payDate\">";
//        invStrList += trUtf8("termin płatności: ")
//                + dateEditDayOfPayment->date().toString(s.getDateFormat())	+ "<br>";
//        invStrList += "</span>";
//    }

//    invStrList += "</b><br><br>";
//    invStrList += "<span class=\"additionalText\">"	+ lineEditAdditionalText->text() + "</span>";
//    invStrList += "</td>";
//    invStrList += "<td width=\"3%\">&nbsp;</td>";
//    invStrList += "<td width=\"48%\" valign=\"top\">";

//    invStrList += "<table width=\"90%\" border=\"0\">";
//    invStrList += "<tr class=\"stawkiHeader\"><td colspan=\"4\">";
//    invStrList += trUtf8("Ogółem stawkami:");
//    invStrList += "</td></tr>";
//    invStrList += getGroupedSums();
//    invStrList += "<tr>";
//    invStrList += "<td>&nbsp;</td>"; // netto
//    invStrList += "<td>&nbsp;</td>"; // stawka
//    invStrList += "<td>&nbsp;</td>"; // podatek
//    invStrList += "<td>&nbsp;</td>"; // brutto
//    invStrList += "</tr>";
//    invStrList += "</table>";

//    invStrList += "</td>";
//    invStrList += "</tr>";
//    invStrList += "</table>";

//    invStrList += "</td></tr>";
//}

/**
 * @brief
 *
 */
//void InvoiceDialog::makeInvoiceFooter() {
//    invStrList += "<tr comment=\"podpis\" align=\"center\"><td>";
//    invStrList += "<br><br><br><br>";

//    invStrList += "<table width=\"80%\" border=\"0\">";
//    invStrList += "<tr>";
//    invStrList += "<td width=\"3%\">&nbsp;</td>";
//    invStrList += "<td width=\"43%\" align=\"center\"> ";
//    invStrList += "<hr width=\"100%\" noshade=\"noshade\" color=\"black\" />";
//    invStrList += "</td>";
//    invStrList += "<td width=\"7%\">&nbsp;</td>";
//    invStrList += "<td width=\"3%\">&nbsp;</td>";
//    invStrList += "<td width=\"43%\" align=\"center\"> ";
//    invStrList += "<hr width=\"100%\" noshade=\"noshade\" color=\"black\" />";
//    invStrList += "</td>";
//    invStrList += "</tr>";
//    invStrList += "<tr class=\"signature\">";
//    invStrList += "<td width=\"3%\">&nbsp;</td>";
//    invStrList += "<td width=\"43%\" align=\"center\"> ";
//    invStrList += trUtf8("Imię i nazwisko osoby upoważnionej")
//            + "<br>" + trUtf8(" do wystawienia faktury VAT");
//    invStrList += "</td>";
//    invStrList += "<td width=\"7%\">&nbsp;</td>";
//    invStrList += "<td width=\"3%\">&nbsp;</td>";
//    invStrList += "<td width=\"43%\" align=\"center\">";
//    invStrList += trUtf8("Imię i nazwisko osoby upoważnionej")
//            + "<br>" + trUtf8(" do odbioru faktury VAT");
//    invStrList += "</td>";
//    invStrList += "</tr>";
//    invStrList += "</table>";

//    invStrList += "</td></tr>";
//    invStrList += "</table>";

//}

/**
 * @brief
 *
 */
//void InvoiceDialog::makeInvoiceFooterHtml() {
//    invStrList += "</body>";
//    invStrList += "</html>";
//}

/** getGroupedSums
 *  Groups prices by VAT
 */
/**
 * @brief
 *
 * @return QString
 */
QString InvoiceDialog::getGroupedSums()
{
    QStringList out;
    SettingsGlobal s;
    const QStringList rates = s.value(s.VAT_RATES).toString().split("|");

    QMap<int, double> rateNet;
    QMap<int, double> rateVat;
    QMap<int, double> ratesGross;
    // every currency in int value is equal vat currency

    const int ssize = rates.size();

    // lp, nazwa, kod, pkwiu, ilosc, jm, rabat, cena jm., netto, vat, brutto
    for (int i = 0; i < ui_->tableWidgetCommodities->rowCount(); ++i)
    {
        for (int y = 0; y < ssize; ++y)
        {
            if (rates[y] == ui_->tableWidgetCommodities->item(i, CommodityVisualFields::VAT)->text())
            {
                rateNet[y] += s.stringToDouble(ui_->tableWidgetCommodities->item(i, CommodityVisualFields::NET)->text());
                ratesGross[y] += 0;//s.stringToDouble(tableWidgetCommodities->item(i, CommodityVisualFields::)->text());
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
        out += "<td width=\"30%\">" + s.numberToString(rateNet[y], 'f', 2) + "</td>"; // netto
        out += "<td width=\"10%\">" + rates[y] + "</td>"; // stawka
        out += "<td width=\"30%\">" + s.numberToString(rateVat[y], 'f', 2) + "</td>"; // podatek
        out += "<td width=\"30%\">" + s.numberToString(ratesGross[y], 'f', 2) + "</td>"; // brutto
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
    ui_->lineEditInvNumber->setEnabled(isAllowed);
    ui_->dateEditDateOfSell->setEnabled(isAllowed);
    ui_->dateEditDateOfIssuance->setEnabled(isAllowed);
    ui_->tableWidgetCommodities->setEnabled(isAllowed);
    ui_->spinBoxDiscount->setEnabled(isAllowed);
    ui_->labelDiscount1->setEnabled(isAllowed);
    ui_->comboBoxPayment->setEnabled(isAllowed);
    ui_->dateEditDayOfPayment->setEnabled(isAllowed);
    ui_->lineEditAdditionalText->setEnabled(isAllowed);
    ui_->pushButtonAddCommodity->setEnabled(isAllowed);
    ui_->pushButtonRemoveCommodity->setEnabled(isAllowed);
    ui_->pushButtonEditCommodity->setEnabled(isAllowed);
    ui_->checkBoxDiscount->setEnabled(isAllowed);
    ui_->comboBoxCounterparties->setEnabled(isAllowed);
    ui_->comboBoxCurrency->setEnabled(isAllowed);
    ui_->pushButtonSave->setEnabled(isAllowed);
    ui_->pushButtonAddCounterparty->setEnabled(isAllowed);
    ui_->dateEditDayOfPayment->setEnabled(isAllowed);
    if (!isAllowed || ui_->spinBoxDiscount->value() == 0) {
        ui_->checkBoxDiscount->setChecked(false);
        ui_->spinBoxDiscount->setEnabled(false);
    } else {
        ui_->checkBoxDiscount->setChecked(true);
        ui_->spinBoxDiscount->setEnabled(true);
    }
    if (isAllowed && (ui_->comboBoxPayment->currentIndex() > 0)) {
        ui_->dateEditDayOfPayment->setEnabled(true);
    } else {
        ui_->dateEditDayOfPayment->setEnabled(false);
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
    SettingsGlobal s;
    netTotal = discountTotal = grossTotal = 0.0;

    for (int i = 0; i < ui_->tableWidgetCommodities->rowCount(); ++i)
    {
        const double quantity = s.stringToDouble(ui_->tableWidgetCommodities->item(i, CommodityVisualFields::QUANTITY)->text());
        const double netVal = s.stringToDouble(ui_->tableWidgetCommodities->item(i, CommodityVisualFields::NET)->text());
        const double vatRate = 0.01 * s.stringToDouble(ui_->tableWidgetCommodities->item(i, CommodityVisualFields::VAT)->text());
        const double discountRate = 0.01 * s.stringToDouble(ui_->tableWidgetCommodities->item(i, CommodityVisualFields::DISCOUNT)->text());

        const double grossVal = netVal + netVal * vatRate;
        const double discountValue = discountRate * grossVal;

        netTotal += netVal * quantity;
        discountTotal += discountValue * quantity;
        grossTotal += (grossVal - discountValue) * quantity;
    }

    ui_->labelSumNetVal->setText(s.numberToString(netTotal, 'f', 2));
    ui_->labelDiscountVal->setText(s.numberToString(discountTotal, 'f', 2));
    ui_->labelSumGrossVal->setText(s.numberToString(grossTotal, 'f', 2));
}


/**
 * @brief
 *
 * @param commodities
 */
void InvoiceDialog::fillTableCommodity_(const QList<CommodityVisualData> &commodities)
{
    ui_->tableWidgetCommodities->setColumnCount(CommodityVisualFields::DISCOUNT - CommodityVisualFields::ID + 1);
    ui_->tableWidgetCommodities->setRowCount(commodities.size());
    for(int r = 0; r < commodities.size(); ++r)
    {
        for(int c = CommodityVisualFields::ID; c <= CommodityVisualFields::DISCOUNT; ++c)
        {
            ui_->tableWidgetCommodities->setItem(r, c, new QTableWidgetItem(commodities.at(r).field(c)));
        }
    }

    QStringList headers;
    for(int c = CommodityVisualFields::ID; c <= CommodityVisualFields::DISCOUNT; ++c)
    {
        headers.append(CommodityVisualData::header(c));
    }
    ui_->tableWidgetCommodities->setHorizontalHeaderLabels(headers);
    ui_->tableWidgetCommodities->hideColumn(CommodityVisualFields::ID);
    ui_->tableWidgetCommodities->resizeColumnsToContents();
}


/**
 * @brief
 *
 * @return InvoiceData
 */
InvoiceData InvoiceDialog::getInvoiceData_() const
{
    InvoiceData ret;
    ret.setField(InvoiceFields::ADDIT_TEXT, ui_->lineEditAdditionalText->text());
    ret.setField(InvoiceFields::COUNTERPARTY_ID, db_->modelCounterparty()->
                 data(db_->modelCounterparty()->index(ui_->comboBoxCounterparties->currentIndex(),
                            CounterpartyFields::ID)).toLongLong());
    ret.setField(InvoiceFields::CURRENCY_ID, ui_->comboBoxCurrency->currentIndex() + 1);
    ret.setField(InvoiceFields::DISCOUNT, ui_->spinBoxDiscount->value());
    ret.setField(InvoiceFields::INV_NUMBER, ui_->lineEditInvNumber->text());
    ret.setField(InvoiceFields::ISSUANCE_DATE, ui_->dateEditDateOfIssuance->date());
    ret.setField(InvoiceFields::PAYMENT_DATE, ui_->dateEditDayOfPayment->date());
    ret.setField(InvoiceFields::PAYMENT_ID, ui_->comboBoxPayment->currentIndex() + 1);
    ret.setField(InvoiceFields::SELLING_DATE, ui_->dateEditDateOfSell->date());
    ret.setField(InvoiceFields::TYPE_ID, ui_->comboBoxInvoiceType->currentIndex() + 1);

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

    for(int r = 0; r < ui_->tableWidgetCommodities->rowCount(); ++r)
    {
        CommodityVisualData d;
        for(int c = CommodityVisualFields::ID; c <= CommodityVisualFields::DISCOUNT; ++c)
        {
            d.setField(c, ui_->tableWidgetCommodities->item(r, c)->data(Qt::DisplayRole));
        }
        ret.append(d);
    }

    return ret;
}


/**
 * @brief
 *
 */
void InvoiceDialog::setInitialComboBoxIndexes_(const QString &invoiceType,
                                               const QString &paymentType,
                                               const QString &defaultCurrency)
{
    ui_->comboBoxInvoiceType->setCurrentIndex(ui_->comboBoxInvoiceType->findText(invoiceType));
    ui_->comboBoxPayment->setCurrentIndex(ui_->comboBoxPayment->findText(paymentType));
    ui_->comboBoxCurrency->setCurrentIndex(ui_->comboBoxCurrency->findText(defaultCurrency));
}
