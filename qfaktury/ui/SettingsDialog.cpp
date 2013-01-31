#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :QDialog(parent)
{
    setupUi(this);
    init_();
}

void SettingsDialog::init_()
{
    // connect all slots
    connect(pushButtonApply, SIGNAL(clicked()), this, SLOT(apply_()));
    connect(pushButtonCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(pushButtonOK, SIGNAL(clicked()), this, SLOT(okButtonClick_()));
    connect(pushButtonUnitAdd, SIGNAL(clicked()), this, SLOT(currAddBtnClick_()));
    connect(pushButtonUnitRemove, SIGNAL(clicked()), this, SLOT(currDelBtnClick_()));
    connect(pushButtonCurrencyAdd, SIGNAL(clicked()), this, SLOT(currencyAddBtnClick_()));
    connect(pushButtonCurrencyRemove, SIGNAL(clicked()), this, SLOT(currencyDelBtnClick_()));
    connect(pushButtonPaymentAdd, SIGNAL(clicked()), this, SLOT(paymAddBtnClick_()));
    connect(pushButtonPaymentRemove, SIGNAL(clicked()), this, SLOT(paymDelBtnClick_()));
    connect(pushButtonPaymentDown, SIGNAL(clicked()), this, SLOT(paymDownBtnClick_()));
    connect(pushButtonPaymentUp, SIGNAL(clicked()), this, SLOT(paymUpBtnClick_()));
    connect(pushButtonShowExamples, SIGNAL(clicked()), this, SLOT(showExamples_()));
    connect(pushButtonVATAdd, SIGNAL(clicked()), this, SLOT(vatAddBtnClick_()));
    connect(pushButtonVATRemove, SIGNAL(clicked()), this, SLOT(vatDelBtnClick_()));
    connect(pushButtonVATUp, SIGNAL(clicked()), this, SLOT(vatUpBtnClick_()));
    connect(pushButtonVATDown, SIGNAL(clicked()), this, SLOT(vatDownBtnClick_()));
    connect(pushButtonCorrectionReasonAdd, SIGNAL(clicked()), this, SLOT(corrAddBtnClick_()));
    connect(pushButtonCorrectionReasonRemove, SIGNAL(clicked()), this, SLOT(corrDelBtnClick_()));
    connect(pushButtonChooseLogo, SIGNAL(clicked()), this, SLOT(addLogoBtnClick_()));
    connect(pushButtonWorkDir, SIGNAL(clicked()), this, SLOT(workingDirBtnClick_()));
    connect(pushButtonDefault, SIGNAL(clicked()), this, SLOT(setDefaultClick_()));
    connect(pushButtonMaskHelp, SIGNAL(clicked()), this, SLOT(maskHelpClick_()));
    connect(pushButtonFormatEdit, SIGNAL(clicked()), this, SLOT(editFormat_()));

    connect( comboBoxCSS, SIGNAL( currentIndexChanged (int)), this, SLOT( applyBtnEnable_() ) );
    connect( comboBoxLanguage, SIGNAL( currentIndexChanged (int)), this, SLOT( applyBtnEnable_() ) );
    connect( lineEditLogo, SIGNAL(  textChanged (const QString &)), this, SLOT( applyBtnEnable_() ) );
    connect( lineEditWorkDir, SIGNAL(  textChanged (const QString &)), this, SLOT( applyBtnEnable_() ) );
    connect( lineEditTaxIDMask, SIGNAL(  textChanged (const QString &)), this, SLOT( applyBtnEnable_() ) );
    connect( lineEditAccountMask, SIGNAL(  textChanged (const QString &)), this, SLOT( applyBtnEnable_() ) );
    connect( spinBoxNumCopies, SIGNAL(  valueChanged (const QString &) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxInvEdit, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxInvSymbolEdit, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxProductNameEdit, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( textEditAdditionalText,  SIGNAL( textChanged() ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxName, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxLocation, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxAddress, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxAccountName, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxTaxID, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxPhone, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxEmail, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxWWW, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );

    connect( checkBoxFieldID, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxFieldName, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxFieldPostalCode, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxFieldPKWIU, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxFieldAmount, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxFieldUnit, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxFieldUnitPrice, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxFieldNetVal, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxFieldDiscount, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxFieldDiscountVal, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxFieldNetAfterDiscount, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxFieldVAT, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxFieldVATVal, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );
    connect( checkBoxFieldGrossVal, SIGNAL( stateChanged(int) ), this, SLOT( applyBtnEnable_() ) );

    comboBoxLanguage->clear();
    comboBoxLanguage->insertItems(0, getTranslations_());

    comboBoxCSS->clear();
    comboBoxCSS->insertItems(0, getTemplates_());

    readSettings_();

    pushButtonApply->setEnabled(false);

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
    pushButtonApply->setEnabled(false);
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
    pushButtonApply->setEnabled(true);
}


/** Slot - set all to default
 */
void SettingsDialog::setDefaultClick_()
{
    if (QMessageBox::question(this, qApp->applicationName(), trUtf8("Czy na pewno chcesz przywrócic ustawienia domyślne?"),
                              QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
        return;

    sett().resetSettings();
    // is this required? //TODO
    readSettings_();
}


void SettingsDialog::showExamples_()
{
    InvoiceNumberFormatExamplesDialog dialog(this, lineEditFormat->text());
    dialog.exec();
}

/** Slot used to change location of invoiced
 */
void SettingsDialog::workingDirBtnClick_()
{
    const QString dir(QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home", QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks));
    lineEditWorkDir->setText(dir);
    pushButtonApply->setEnabled(true);
}


/** Slot add logo
 */
void SettingsDialog::addLogoBtnClick_()
{
    const QString ofn(QFileDialog::getOpenFileName(this, trUtf8("Wybierz plik do wstawienia jako logo"), "",
                                               trUtf8("Obrazki (*.jpg *.png)")));

    lineEditLogo->setText(ofn);
    pushButtonApply->setEnabled(true);
}

//----------------------- List box Slots START ---
//@TODO merge into 1 function

/** Slot add currency
 */
void SettingsDialog::currAddBtnClick_()
{
    addToListWidget_(listWidgetUnit, lineEditUnit);
}


/** Slot del currency
 */
void SettingsDialog::currDelBtnClick_()
{
    delFromListWidget_(listWidgetUnit);
}

/** Slot korekty reason add
 */
void SettingsDialog::corrAddBtnClick_()
{
    addToListWidget_(listWidgetCorrectionReason, lineEditCorrectionReason);
}

/** Slot korekty reason delete
 */
void SettingsDialog::corrDelBtnClick_()
{
    delFromListWidget_(listWidgetCorrectionReason);
}

/** Slot predefined VAT value add
 */
void SettingsDialog::vatAddBtnClick_()
{
    addToListWidget_(listWidgetVAT, lineEditVAT);
}

/** Slot predefined VAT value delete
 */
void SettingsDialog::vatDelBtnClick_()
{
    delFromListWidget_(listWidgetVAT);
}

/** Slot add currency
 */
void SettingsDialog::currencyAddBtnClick_()
{
    addToListWidget_(listWidgetCurrency, lineEditCurrency);
}

/** Slot del currency
 */
void SettingsDialog::currencyDelBtnClick_()
{
    delFromListWidget_(listWidgetCurrency);
}

/** Slot delete payment type click
 */
void SettingsDialog::paymDelBtnClick_()
{
    delFromListWidget_(listWidgetPayment);
}

/** Slot add payment type click
 */
void SettingsDialog::paymAddBtnClick_()
{
    addToListWidget_(listWidgetPayment, lineEditPayment);
}
//----------------------- List box Slots END ---

//---------------------- UP DOWN SLOTS START----
/** Slot move VAT value up
 */
void SettingsDialog::vatUpBtnClick_()
{
    moveUpElement_(listWidgetVAT);
}

/** Slot move VAT value down
 */
void SettingsDialog::vatDownBtnClick_()
{
    moveDownElement_(listWidgetVAT);
}

/** Slot move payment value up
 */
void SettingsDialog::paymUpBtnClick_()
{
    moveUpElement_(listWidgetPayment);
}

/** Slot move payment value down
 */
void SettingsDialog::paymDownBtnClick_()
{
    moveDownElement_(listWidgetPayment);
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

/** Save all sett()
 */
void SettingsDialog::saveSettings_()
{
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::LANG), comboBoxLanguage->currentText());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::CSS), comboBoxCSS->currentText());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::WORKING_DIR), lineEditWorkDir->text());

    sett().beginGroup("printpos");
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::USER_NAME), checkBoxName->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::USER_LOCATION), checkBoxLocation->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::USER_ADDRESS), checkBoxAddress->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::USER_ACCOUNT), checkBoxAccountName->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::USER_TAXID), checkBoxTaxID->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::USER_PHONE), checkBoxPhone->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::USER_MAIL), checkBoxEmail->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::USER_WWW), checkBoxWWW->isChecked());
    sett().endGroup();

    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::FIRST_RUN), false);
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::LOGO), lineEditLogo->text());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::UNITS), getItemsToString_(listWidgetUnit));
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::VAT_RATES), getItemsToString_(listWidgetVAT).remove("%"));
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::CURRENCIES), getItemsToString_(listWidgetCurrency));
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::CORRECTION_REASON), getItemsToString_(listWidgetCorrectionReason));
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::PAYMENT_TYPE), getItemsToString_(listWidgetPayment)); //TODO: uwaga!! get first
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::ADDIT_TEXT), textEditAdditionalText->toPlainText());

    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::EDIT), checkBoxInvEdit->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::EDIT_SYMBOL), checkBoxInvSymbolEdit->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::EDIT_NAME), checkBoxProductNameEdit->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::TAXID_MASK), lineEditTaxIDMask->text());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::ACCOUNT_MASK), lineEditAccountMask->text());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::NUMBER_OF_COPIES), spinBoxNumCopies->value());

    sett().beginGroup("faktury_pozycje");
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::ORDER_NUMBER), checkBoxFieldID->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::NAME), checkBoxFieldName->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::CODE), checkBoxFieldPostalCode->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::PKWIU), checkBoxFieldPKWIU->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::QUANTITY), checkBoxFieldAmount->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::INTERNAT_UNIT), checkBoxFieldUnit->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::UNIT_PRICE), checkBoxFieldUnitPrice->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::NET_VAL), checkBoxFieldNetVal->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::DISCOUNT), checkBoxFieldDiscount->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::DISCOUNT_VAL), checkBoxFieldDiscountVal->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::NET_AFTER), checkBoxFieldNetAfterDiscount->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::VAT_PRICE), checkBoxFieldVAT->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::VAT_VAL), checkBoxFieldVATVal->isChecked());
    sett().setValue(SettingsGlobal::keyName(SettingsGlobal::GROSS_VAL), checkBoxFieldGrossVal->isChecked());
    sett().endGroup();

}

/** Read all sett()
 */
void SettingsDialog::readSettings_()
{
    int curr = 0;

    lineEditLogo->setText(sett().value(SettingsGlobal::keyName(SettingsGlobal::LOGO)).toString());
    lineEditWorkDir->setText(sett().value(SettingsGlobal::keyName(SettingsGlobal::WORKING_DIR)).toString());

    listWidgetUnit->clear();
    listWidgetUnit->addItems(sett().value(SettingsGlobal::keyName(SettingsGlobal::UNITS)).toString().split("|"));
    listWidgetVAT->clear();
    listWidgetVAT->addItems(sett().value(SettingsGlobal::keyName(SettingsGlobal::VAT_RATES)).toString().split("|"));
    listWidgetCurrency->clear();
    listWidgetCurrency->addItems(sett().value(SettingsGlobal::keyName(SettingsGlobal::CURRENCIES)).toString().split("|"));
    listWidgetPayment->clear();
    listWidgetPayment->addItems(sett().value(SettingsGlobal::keyName(SettingsGlobal::PAYMENT_TYPE)).toString().split("|"));

    listWidgetCorrectionReason->clear();
    listWidgetCorrectionReason->addItems(sett().value(SettingsGlobal::keyName(SettingsGlobal::CORRECTION_REASON)).toString().split("|"));

    curr = getTranslations_().indexOf(sett().value(SettingsGlobal::keyName(SettingsGlobal::LANG)).toString());
    comboBoxLanguage->setCurrentIndex(curr);

    curr = getTemplates_().indexOf(sett().value(SettingsGlobal::keyName(SettingsGlobal::CSS)).toString());
    comboBoxCSS->setCurrentIndex(curr);

    lineEditTaxIDMask->setText(sett().value(SettingsGlobal::keyName(SettingsGlobal::TAXID_MASK)).toString());
    lineEditAccountMask->setText(sett().value(SettingsGlobal::keyName(SettingsGlobal::ACCOUNT_MASK)).toString());

    sett().beginGroup("faktury_pozycje");
    checkBoxFieldID->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::ORDER_NUMBER)).toBool());
    checkBoxFieldName->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::NAME)).toBool());
    checkBoxFieldPostalCode->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::CODE)).toBool());
    checkBoxFieldPKWIU->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::PKWIU)).toBool());
    checkBoxFieldAmount->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::QUANTITY)).toBool());
    checkBoxFieldUnit->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::INTERNAT_UNIT)).toBool());
    checkBoxFieldUnitPrice->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::UNIT_PRICE)).toBool());
    checkBoxFieldNetVal->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::NET_VAL)).toBool());
    checkBoxFieldDiscount->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::DISCOUNT)).toBool());
    checkBoxFieldDiscountVal->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::DISCOUNT_VAL)).toBool());
    checkBoxFieldNetAfterDiscount->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::NET_AFTER)).toBool());
    checkBoxFieldVAT->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::VAT_VAL)).toBool());
    checkBoxFieldVATVal->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::VAT_PRICE)).toBool());
    checkBoxFieldGrossVal->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::GROSS_VAL)).toBool());
    sett().endGroup();

    textEditAdditionalText->setText(sett().value(SettingsGlobal::keyName(SettingsGlobal::ADDIT_TEXT)).toString());
    checkBoxInvEdit->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::EDIT)).toBool());
    checkBoxInvSymbolEdit->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::EDIT_SYMBOL)).toBool());
    checkBoxProductNameEdit->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::EDIT_NAME)).toBool());

    spinBoxNumCopies->setValue(sett().value(SettingsGlobal::keyName(SettingsGlobal::NUMBER_OF_COPIES)).toInt());

    sett().beginGroup("printpos");
    checkBoxName->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::USER_NAME)).toBool());
    checkBoxLocation->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::USER_LOCATION)).toBool());
    checkBoxAddress->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::USER_ADDRESS)).toBool());
    checkBoxAccountName->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::USER_ACCOUNT)).toBool());
    checkBoxTaxID->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::USER_TAXID)).toBool());
    checkBoxPhone->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::USER_PHONE)).toBool());
    checkBoxEmail->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::USER_MAIL)).toBool());
    checkBoxWWW->setChecked(sett().value(SettingsGlobal::keyName(SettingsGlobal::USER_WWW)).toBool());
    sett().endGroup();

    read_=true;
    // readTemplate();
}


QStringList SettingsDialog::getTemplates_()
{
    QStringList templates;
    QString ret;
    QString path = sett().getWorkingDir() + "/templates/";

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
    InvoiceNumberFormatEditDialog dialog(this, lineEditFormat->text());
    if(QDialog::Accepted == dialog.exec())
        lineEditFormat->setText(dialog.format());
}


void SettingsDialog::addToListWidget_(QListWidget *listTo, QLineEdit *lineEditFrom)
{
    if (!lineEditFrom->text().isEmpty())
    {
        listTo->addItem(lineEditFrom->text());
        lineEditFrom->clear();
    }
    else
    {
        QMessageBox::information(this, trUtf8("Uwaga!!"),trUtf8("Nie można dodać. Pole jest puste."), QMessageBox::Ok);
        return;
    }
    pushButtonApply->setEnabled(true);
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
    pushButtonApply->setEnabled(true);
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
    pushButtonApply->setEnabled(true);
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
    pushButtonApply->setEnabled(true);
}
