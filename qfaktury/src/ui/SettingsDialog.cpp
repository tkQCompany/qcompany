#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, Database *db)
    :QDialog(parent), ui_(new Ui::SettingsDialog), db_(db)
{
    ui_->setupUi(this);
    init_();
}


SettingsDialog::~SettingsDialog()
{
    delete ui_;
}

void SettingsDialog::init_()
{
    // connect all slots
    connect(ui_->pushButtonApply, SIGNAL(clicked()), this, SLOT(apply_()));
    connect(ui_->pushButtonCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui_->pushButtonOK, SIGNAL(clicked()), this, SLOT(okButtonClick_()));
    connect(ui_->pushButtonUnitAdd, SIGNAL(clicked()), this, SLOT(unitAddBtnClick_()));
    connect(ui_->pushButtonUnitRemove, SIGNAL(clicked()), this, SLOT(unitDelBtnClick_()));
    connect(ui_->pushButtonPaymentAdd, SIGNAL(clicked()), this, SLOT(paymAddBtnClick_()));
    connect(ui_->pushButtonPaymentRemove, SIGNAL(clicked()), this, SLOT(paymDelBtnClick_()));
    connect(ui_->pushButtonPaymentDown, SIGNAL(clicked()), this, SLOT(paymDownBtnClick_()));
    connect(ui_->pushButtonPaymentUp, SIGNAL(clicked()), this, SLOT(paymUpBtnClick_()));
    connect(ui_->pushButtonShowExamples, SIGNAL(clicked()), this, SLOT(showExamples_()));
    connect(ui_->pushButtonVATAdd, SIGNAL(clicked()), this, SLOT(vatAddBtnClick_()));
    connect(ui_->pushButtonVATRemove, SIGNAL(clicked()), this, SLOT(vatDelBtnClick_()));
    connect(ui_->pushButtonVATUp, SIGNAL(clicked()), this, SLOT(vatUpBtnClick_()));
    connect(ui_->pushButtonVATDown, SIGNAL(clicked()), this, SLOT(vatDownBtnClick_()));
    connect(ui_->pushButtonCorrectionReasonAdd, SIGNAL(clicked()), this, SLOT(corrAddBtnClick_()));
    connect(ui_->pushButtonCorrectionReasonRemove, SIGNAL(clicked()), this, SLOT(corrDelBtnClick_()));
    connect(ui_->pushButtonChooseLogo, SIGNAL(clicked()), this, SLOT(addLogoBtnClick_()));
    connect(ui_->pushButtonDefault, SIGNAL(clicked()), this, SLOT(setDefaultClick_()));
    connect(ui_->pushButtonMaskHelp, SIGNAL(clicked()), this, SLOT(maskHelpClick_()));
    connect(ui_->pushButtonFormatEdit, SIGNAL(clicked()), this, SLOT(editFormat_()));
    connect(ui_->pushButtonUpdateRates, SIGNAL(clicked()), this, SLOT(updateCurrenciesRates_()));

    connect(ui_->comboBoxCSS, SIGNAL( currentIndexChanged (int)), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->comboBoxLanguage, SIGNAL( currentIndexChanged (int)), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->lineEditLogo, SIGNAL(  textChanged (const QString &)), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->lineEditTaxIDMask, SIGNAL(  textChanged (const QString &)), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->lineEditAccountMask, SIGNAL(  textChanged (const QString &)), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->spinBoxNumCopies, SIGNAL(  valueChanged (const QString &) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxInvEdit, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxInvSymbolEdit, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxProductNameEdit, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->textEditAdditionalText,  SIGNAL( textChanged() ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxName, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxLocation, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxAddress, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxAccountName, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxTaxID, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxPhone, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxEmail, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxWWW, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );

    connect(ui_->checkBoxFieldID, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxFieldName, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxFieldPostalCode, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxFieldPKWIU, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxFieldAmount, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxFieldUnit, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxFieldUnitPrice, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxFieldNetVal, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxFieldDiscount, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxFieldDiscountVal, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxFieldNetAfterDiscount, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxFieldVAT, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxFieldVATVal, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect(ui_->checkBoxFieldGrossVal, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );

    connect(db_->modelCurrency(), SIGNAL(updatingCurrenciesRatesFinished()), this, SLOT(updateCurrenciesTableView_()));

    ui_->comboBoxLanguage->clear();
    ui_->comboBoxLanguage->insertItems(0, getTranslations_());

    ui_->comboBoxCSS->clear();
    ui_->comboBoxCSS->insertItems(0, getTemplates_());

    readSettings_();

    ui_->pushButtonApply->setEnabled(false);

    ui_->tableViewCurrency->setModel(db_->modelCurrency());
    ui_->tableViewCurrency->hideColumn(CurrencyFields::ID_CURRENCY);

    ui_->listViewUnit->setModel(db_->modelUnit());
    ui_->listViewUnit->setModelColumn(UnitFields::NAME);
}

/** Slot - maskHelpClick
 */
void SettingsDialog::maskHelpClick_()
{
    QDesktopServices::openUrl(QUrl("http://doc.trolltech.com/4.5/qlineedit.html#inputMask-prop"));
}

/** Slot - Apply
 */
void SettingsDialog::apply_()
{
    saveSettings_();
    ui_->pushButtonApply->setEnabled(false);
}

/** Slot - OK
 */
void SettingsDialog::okButtonClick_()
{
    saveSettings_();
    accept();
}

/** Slot applyBtn
 */
void SettingsDialog::applyBtnEnable_()
{
    ui_->pushButtonApply->setEnabled(true);
}


/** Slot - set all to default
 */
void SettingsDialog::setDefaultClick_()
{
    if (QMessageBox::question(this, qApp->applicationName(), trUtf8("Czy na pewno chcesz przywrócic ustawienia domyślne?"),
                              QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        SettingsGlobal s;
        s.resetSettings();
        // is this required? //TODO
        readSettings_();
    }
}


void SettingsDialog::showExamples_()
{
    InvoiceNumberFormatExamplesDialog dialog(this, db_, ui_->lineEditFormat->text());
    dialog.exec();
}


void SettingsDialog::updateCurrenciesRates_()
{
    db_->modelCurrency()->updateCurrenciesRates();
}


void SettingsDialog::updateCurrenciesTableView_()
{
    db_->modelCurrency()->select();
    SettingsGlobal s;
    s.setValue(s.keyName(s.LAST_UPDATE_EXCHANGE_RATES),
               QDate::currentDate().toString(s.dateFormatInternal()));
    s.setValue(s.keyName(s.LAST_UPDATE_EXCHANGE_RATES_CENTRAL_BANK),
               db_->modelCurrency()->lastUpdateByCentralBank());
    ui_->dateEditLastUpdate->setDate(QDate::currentDate());
    ui_->dateEditLastUpdateByCentralBank->setDate(db_->modelCurrency()->lastUpdateByCentralBank());
    QMessageBox::information(this, trUtf8("Status aktualizacji"), trUtf8("Aktualizacja kursów średnich walut NBP zakończona sukcesem."));
}


/** Slot add logo
 */
void SettingsDialog::addLogoBtnClick_()
{
    const QString ofn(QFileDialog::getOpenFileName(this, trUtf8("Wybierz plik do wstawienia jako logo"), "",
                                               trUtf8("Obrazki (*.jpg *.png)")));

    ui_->lineEditLogo->setText(ofn);
    ui_->pushButtonApply->setEnabled(true);
}


void SettingsDialog::unitAddBtnClick_()
{
    const QString title(trUtf8("Dodawanie jednostki"));
    if(ui_->lineEditUnit->text().isEmpty())
    {
        QMessageBox::warning(this, title, trUtf8("Wpisz jednostkę, by ją dodać"));
    }
    else
    {
        if(db_->modelUnit()->addUnit(ui_->lineEditUnit->text()))
        {
            QMessageBox::information(this, title,
                                     trUtf8("Dodawanie jednostki '%1' zakończyło się sukcesem")
                                     .arg(ui_->lineEditUnit->text()));
        }
    }
}


void SettingsDialog::unitDelBtnClick_()
{
    const QString title(trUtf8("Usuwanie jednostki"));
    const QModelIndexList choice(ui_->listViewUnit->selectionModel()->selectedIndexes());
    if(choice.isEmpty())
    {
        QMessageBox::information(this, title,
                                 trUtf8("Wybierz jednostkę do skasowania"));
    }
    else
    {
        const QString unit(choice.at(0).data().toString());
        if(db_->modelUnit()->deleteUnit(unit))
        {
            QMessageBox::information(this, title,
                                     trUtf8("Usuwanie jednostki '%1' zakończyło się sukcesem")
                                     .arg(unit));
        }
    }
}

/** Slot korekty reason add
 */
void SettingsDialog::corrAddBtnClick_()
{
    addToListWidget_(ui_->listWidgetCorrectionReason, ui_->lineEditCorrectionReason);
}

/** Slot korekty reason delete
 */
void SettingsDialog::corrDelBtnClick_()
{
    delFromListWidget_(ui_->listWidgetCorrectionReason);
}

/** Slot predefined VAT value add
 */
void SettingsDialog::vatAddBtnClick_()
{
    addToListWidget_(ui_->listWidgetVAT, ui_->lineEditVAT);
}

/** Slot predefined VAT value delete
 */
void SettingsDialog::vatDelBtnClick_()
{
    delFromListWidget_(ui_->listWidgetVAT);
}

/** Slot delete payment type click
 */
void SettingsDialog::paymDelBtnClick_()
{
    delFromListWidget_(ui_->listWidgetPayment);
}

/** Slot add payment type click
 */
void SettingsDialog::paymAddBtnClick_()
{
    addToListWidget_(ui_->listWidgetPayment, ui_->lineEditPayment);
}
//----------------------- List box Slots END ---

//---------------------- UP DOWN SLOTS START----
/** Slot move VAT value up
 */
void SettingsDialog::vatUpBtnClick_()
{
    moveUpElement_(ui_->listWidgetVAT);
}

/** Slot move VAT value down
 */
void SettingsDialog::vatDownBtnClick_()
{
    moveDownElement_(ui_->listWidgetVAT);
}

/** Slot move payment value up
 */
void SettingsDialog::paymUpBtnClick_()
{
    moveUpElement_(ui_->listWidgetPayment);
}

/** Slot move payment value down
 */
void SettingsDialog::paymDownBtnClick_()
{
    moveDownElement_(ui_->listWidgetPayment);
}


/** Used for parsing
 */
QString SettingsDialog::getItemsToString_(const QListWidget *lw)
{
    QString tmp;
    const int posCount = lw->count ();
    for (int selNr = 0; selNr < posCount; ++selNr)
    {
        tmp += lw->item(selNr)->text();
        if (selNr != posCount -1)
            tmp += "|";
    }
    return  tmp;
}


void SettingsDialog::saveSettings_()
{
    SettingsGlobal s;

    s.setValue(s.keyName(s.LANG), ui_->comboBoxLanguage->currentText());
    s.setValue(s.keyName(s.CSS), ui_->comboBoxCSS->currentText());

    s.beginGroup("printpos");
    s.setValue(s.keyName(s.SELLER_NAME), ui_->checkBoxName->isChecked());
    s.setValue(s.keyName(s.SELLER_LOCATION), ui_->checkBoxLocation->isChecked());
    s.setValue(s.keyName(s.SELLER_ADDRESS), ui_->checkBoxAddress->isChecked());
    s.setValue(s.keyName(s.SELLER_ACCOUNT), ui_->checkBoxAccountName->isChecked());
    s.setValue(s.keyName(s.SELLER_TAXID), ui_->checkBoxTaxID->isChecked());
    s.setValue(s.keyName(s.SELLER_PHONE), ui_->checkBoxPhone->isChecked());
    s.setValue(s.keyName(s.SELLER_MAIL), ui_->checkBoxEmail->isChecked());
    s.setValue(s.keyName(s.SELLER_WWW), ui_->checkBoxWWW->isChecked());
    s.endGroup();

    s.setValue(s.keyName(s.FIRST_RUN), false);
    s.setValue(s.keyName(s.LOGO), ui_->lineEditLogo->text());
    //s.setValue(s.keyName(s.UNITS), getItemsToString_(listWidgetUnit));
    s.setValue(s.keyName(s.VAT_RATES), getItemsToString_(ui_->listWidgetVAT).remove("%"));
    //s.setValue(s.keyName(s.CURRENCIES), getItemsToString_(listWidgetCurrency));
    s.setValue(s.keyName(s.CORRECTION_REASONS), getItemsToString_(ui_->listWidgetCorrectionReason));
    s.setValue(s.keyName(s.PAYMENT_TYPE), getItemsToString_(ui_->listWidgetPayment));
    s.setValue(s.keyName(s.ADDIT_TEXT), ui_->textEditAdditionalText->toPlainText());

    s.setValue(s.keyName(s.EDIT), ui_->checkBoxInvEdit->isChecked());
    s.setValue(s.keyName(s.EDIT_SYMBOL), ui_->checkBoxInvSymbolEdit->isChecked());
    s.setValue(s.keyName(s.EDIT_NAME), ui_->checkBoxProductNameEdit->isChecked());
    s.setValue(s.keyName(s.TAXID_MASK), ui_->lineEditTaxIDMask->text());
    s.setValue(s.keyName(s.ACCOUNT_MASK), ui_->lineEditAccountMask->text());
    s.setValue(s.keyName(s.NUMBER_OF_COPIES), ui_->spinBoxNumCopies->value());

    s.beginGroup("faktury_pozycje");
    s.setValue(s.keyName(s.ORDER_NUMBER), ui_->checkBoxFieldID->isChecked());
    s.setValue(s.keyName(s.NAME), ui_->checkBoxFieldName->isChecked());
    s.setValue(s.keyName(s.CODE), ui_->checkBoxFieldPostalCode->isChecked());
    s.setValue(s.keyName(s.PKWIU), ui_->checkBoxFieldPKWIU->isChecked());
    s.setValue(s.keyName(s.QUANTITY), ui_->checkBoxFieldAmount->isChecked());
    s.setValue(s.keyName(s.INTERNAT_UNIT), ui_->checkBoxFieldUnit->isChecked());
    s.setValue(s.keyName(s.UNIT_PRICE), ui_->checkBoxFieldUnitPrice->isChecked());
    s.setValue(s.keyName(s.NET_VAL), ui_->checkBoxFieldNetVal->isChecked());
    s.setValue(s.keyName(s.DISCOUNT), ui_->checkBoxFieldDiscount->isChecked());
    s.setValue(s.keyName(s.DISCOUNT_VAL), ui_->checkBoxFieldDiscountVal->isChecked());
    s.setValue(s.keyName(s.NET_AFTER), ui_->checkBoxFieldNetAfterDiscount->isChecked());
    s.setValue(s.keyName(s.VAT_PRICE), ui_->checkBoxFieldVAT->isChecked());
    s.setValue(s.keyName(s.VAT_VAL), ui_->checkBoxFieldVATVal->isChecked());
    s.setValue(s.keyName(s.GROSS_VAL), ui_->checkBoxFieldGrossVal->isChecked());
    s.endGroup();

}


void SettingsDialog::readSettings_()
{
    int curr = 0;
    SettingsGlobal s;

    ui_->lineEditLogo->setText(s.value(s.LOGO).toString());
    ui_->lineEditTaxIDMask->setText(s.value(s.TAXID_MASK).toString());
    ui_->lineEditAccountMask->setText(s.value(s.ACCOUNT_MASK).toString());
    ui_->lineEditFormat->setText(s.value(s.DEFAULT_INV_NUM_FORMAT).toString());

    //listWidgetUnit->clear();
    //listWidgetUnit->addItems(s.value(s.UNITS)).toString().split("|"));
    ui_->listWidgetVAT->clear();
    ui_->listWidgetVAT->addItems(s.value(s.VAT_RATES).toString().split("|"));
    //listWidgetCurrency->addItems(s.value(s.CURRENCIES)).toString().split("|"));
    ui_->listWidgetPayment->clear();
    ui_->listWidgetPayment->addItems(s.value(s.PAYMENT_TYPE).toString().split("|"));

    ui_->listWidgetCorrectionReason->clear();
    ui_->listWidgetCorrectionReason->addItems(s.value(s.CORRECTION_REASONS).toString().split("|"));

    curr = getTranslations_().indexOf(s.value(s.LANG).toString());
    ui_->comboBoxLanguage->setCurrentIndex(curr);

    curr = getTemplates_().indexOf(s.value(s.CSS).toString());
    ui_->comboBoxCSS->setCurrentIndex(curr);

    ui_->checkBoxFieldID->setChecked(s.value(s.ORDER_NUMBER).toBool());
    ui_->checkBoxFieldName->setChecked(s.value(s.NAME).toBool());
    ui_->checkBoxFieldPostalCode->setChecked(s.value(s.CODE).toBool());
    ui_->checkBoxFieldPKWIU->setChecked(s.value(s.PKWIU).toBool());
    ui_->checkBoxFieldAmount->setChecked(s.value(s.QUANTITY).toBool());
    ui_->checkBoxFieldUnit->setChecked(s.value(s.INTERNAT_UNIT).toBool());
    ui_->checkBoxFieldUnitPrice->setChecked(s.value(s.UNIT_PRICE).toBool());
    ui_->checkBoxFieldNetVal->setChecked(s.value(s.NET_VAL).toBool());
    ui_->checkBoxFieldDiscount->setChecked(s.value(s.DISCOUNT).toBool());
    ui_->checkBoxFieldDiscountVal->setChecked(s.value(s.DISCOUNT_VAL).toBool());
    ui_->checkBoxFieldNetAfterDiscount->setChecked(s.value(s.NET_AFTER).toBool());
    ui_->checkBoxFieldVAT->setChecked(s.value(s.VAT_VAL).toBool());
    ui_->checkBoxFieldVATVal->setChecked(s.value(s.VAT_PRICE).toBool());
    ui_->checkBoxFieldGrossVal->setChecked(s.value(s.GROSS_VAL).toBool());

    ui_->textEditAdditionalText->setText(s.value(s.ADDIT_TEXT).toString());
    ui_->checkBoxInvEdit->setChecked(s.value(s.EDIT).toBool());
    ui_->checkBoxInvSymbolEdit->setChecked(s.value(s.EDIT_SYMBOL).toBool());
    ui_->checkBoxProductNameEdit->setChecked(s.value(s.EDIT_NAME).toBool());

    ui_->spinBoxNumCopies->setValue(s.value(s.NUMBER_OF_COPIES).toInt());

    ui_->checkBoxName->setChecked(s.value(s.SELLER_NAME).toBool());
    ui_->checkBoxLocation->setChecked(s.value(s.SELLER_LOCATION).toBool());
    ui_->checkBoxAddress->setChecked(s.value(s.SELLER_ADDRESS).toBool());
    ui_->checkBoxAccountName->setChecked(s.value(s.SELLER_ACCOUNT).toBool());
    ui_->checkBoxTaxID->setChecked(s.value(s.SELLER_TAXID).toBool());
    ui_->checkBoxPhone->setChecked(s.value(s.SELLER_PHONE).toBool());
    ui_->checkBoxEmail->setChecked(s.value(s.SELLER_MAIL).toBool());
    ui_->checkBoxWWW->setChecked(s.value(s.SELLER_WWW).toBool());

    ui_->dateEditLastUpdate->setDate(s.value(s.LAST_UPDATE_EXCHANGE_RATES).toDate());
    ui_->dateEditLastUpdateByCentralBank->setDate(
                s.value(s.LAST_UPDATE_EXCHANGE_RATES_CENTRAL_BANK).toDate());
}


QStringList SettingsDialog::getTemplates_()
{
    QStringList templates;
    QString ret;
    QString path = "/templates/";

    if (templates.isEmpty())
    {
        QDir allFiles;
        allFiles.setPath(path);
        allFiles.setFilter(QDir::Files);
        QStringList filters;
        filters << "*css";
        allFiles.setNameFilters(filters);
        QStringList tmp = allFiles.entryList();
        templates = tmp;
    }

    if (templates.count() > 1) {
        ret = QDir::currentPath() + "/templates/style.css";
        path = QDir::currentPath() + "/templates/";
    } else {
        return templates;
    }

    QFile f;
    f.setFileName(ret);
    if (!f.exists()) {
        ret = "/usr/share/local/qfaktury/templates/style.css";
        templates.append("style.css");
        return templates;
    }

    return templates;
}


// returns list of translations
QStringList SettingsDialog::getTranslations_() const
{
    const QDir languages(":/res/translations");
    QStringList translations = languages.entryList();
    translations.replaceInStrings("qfaktury_", "");
    translations.replaceInStrings(".qm", "");
    if(!translations.contains("pl"))
    {
        translations << "pl";
    }
    translations.sort();
    return translations;
}

void SettingsDialog::editFormat_()
{
    InvoiceNumberFormatEditDialog dialog(this, db_, ui_->lineEditFormat->text());
    if(QDialog::Accepted == dialog.exec())
        ui_->lineEditFormat->setText(dialog.format());
}


bool SettingsDialog::addToListWidget_(QListWidget *listTo, QLineEdit *lineEditFrom)
{
    if (!lineEditFrom->text().isEmpty())
    {
        listTo->addItem(lineEditFrom->text());
        lineEditFrom->clear();
    }
    else
    {
        QMessageBox::information(this, trUtf8("Uwaga!!"),trUtf8("Nie można dodać. Pole jest puste."), QMessageBox::Ok);
        return false;
    }
    ui_->pushButtonApply->setEnabled(true);
    return true;
}


void SettingsDialog::delFromListWidget_(QListWidget *list)
{
    const int pos = list->currentRow();
    if(pos >= 0)
    {
        list->takeItem(pos);
    }
    else
    {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Musisz coś zaznaczyć, żeby usuwać."),
                                 QMessageBox::Ok);
        return;
    }
    ui_->pushButtonApply->setEnabled(true);
}


void SettingsDialog::moveUpElement_(QListWidget *list)
{
    const int selNr = list->currentRow();
    if (selNr == 0)
    {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Nie można przenieść w górę, już jest najwyżej."),
                                 QMessageBox::Ok);
        return;
    }

    if (selNr > 0)
    {
        QListWidgetItem *item = list->item(list->currentRow());
        list->takeItem(selNr);
        list->insertItem(selNr - 1, item);
        list->setCurrentRow(selNr - 1);
    }
    else
    {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Musisz coś zaznaczyć, żeby przesuwać."), QMessageBox::Ok);
        return;
    }
    ui_->pushButtonApply->setEnabled(true);
}


void SettingsDialog::moveDownElement_(QListWidget *list)
{
    const int selNr = list->currentRow();
    if (selNr == list->count() - 1)
    {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Nie można przenieść w dół, już jest najniżej."),
                                 QMessageBox::Ok);
        return;
    }

    if (selNr >= 0)
    {
        QListWidgetItem *item = list->item(list->currentRow());
        list->takeItem(selNr);
        list->insertItem(selNr + 1, item);
        list->setCurrentRow(selNr + 1);
    }
    else
    {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Musisz coś zaznaczyć, żeby przesuwać."), QMessageBox::Ok);
        return;
    }
    ui_->pushButtonApply->setEnabled(true);
}
