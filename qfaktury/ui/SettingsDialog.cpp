#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent, Database *db) :QDialog(parent), db_(db)
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
    InvoiceNumberFormatExamplesDialog dialog(this, db_, lineEditFormat->text());
    dialog.exec();
}

/** Slot used to change location of invoiced
 */
void SettingsDialog::workingDirBtnClick_()
{
    const QString dir(QFileDialog::getExistingDirectory(this, trUtf8("Open Directory"),
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


void SettingsDialog::saveSettings_()
{
    SettingsGlobal s;

    s.setValue(s.keyName(s.LANG), comboBoxLanguage->currentText());
    s.setValue(s.keyName(s.CSS), comboBoxCSS->currentText());
    s.setValue(s.keyName(s.WORKING_DIR), lineEditWorkDir->text());

    s.beginGroup("printpos");
    s.setValue(s.keyName(s.USER_NAME), checkBoxName->isChecked());
    s.setValue(s.keyName(s.USER_LOCATION), checkBoxLocation->isChecked());
    s.setValue(s.keyName(s.USER_ADDRESS), checkBoxAddress->isChecked());
    s.setValue(s.keyName(s.USER_ACCOUNT), checkBoxAccountName->isChecked());
    s.setValue(s.keyName(s.USER_TAXID), checkBoxTaxID->isChecked());
    s.setValue(s.keyName(s.USER_PHONE), checkBoxPhone->isChecked());
    s.setValue(s.keyName(s.USER_MAIL), checkBoxEmail->isChecked());
    s.setValue(s.keyName(s.USER_WWW), checkBoxWWW->isChecked());
    s.endGroup();

    s.setValue(s.keyName(s.FIRST_RUN), false);
    s.setValue(s.keyName(s.LOGO), lineEditLogo->text());
    s.setValue(s.keyName(s.UNITS), getItemsToString_(listWidgetUnit));
    s.setValue(s.keyName(s.VAT_RATES), getItemsToString_(listWidgetVAT).remove("%"));
    s.setValue(s.keyName(s.CURRENCIES), getItemsToString_(listWidgetCurrency));
    s.setValue(s.keyName(s.CORRECTION_REASON), getItemsToString_(listWidgetCorrectionReason));
    s.setValue(s.keyName(s.PAYMENT_TYPE), getItemsToString_(listWidgetPayment)); //TODO: uwaga!! get first
    s.setValue(s.keyName(s.ADDIT_TEXT), textEditAdditionalText->toPlainText());

    s.setValue(s.keyName(s.EDIT), checkBoxInvEdit->isChecked());
    s.setValue(s.keyName(s.EDIT_SYMBOL), checkBoxInvSymbolEdit->isChecked());
    s.setValue(s.keyName(s.EDIT_NAME), checkBoxProductNameEdit->isChecked());
    s.setValue(s.keyName(s.TAXID_MASK), lineEditTaxIDMask->text());
    s.setValue(s.keyName(s.ACCOUNT_MASK), lineEditAccountMask->text());
    s.setValue(s.keyName(s.NUMBER_OF_COPIES), spinBoxNumCopies->value());

    s.beginGroup("faktury_pozycje");
    s.setValue(s.keyName(s.ORDER_NUMBER), checkBoxFieldID->isChecked());
    s.setValue(s.keyName(s.NAME), checkBoxFieldName->isChecked());
    s.setValue(s.keyName(s.CODE), checkBoxFieldPostalCode->isChecked());
    s.setValue(s.keyName(s.PKWIU), checkBoxFieldPKWIU->isChecked());
    s.setValue(s.keyName(s.QUANTITY), checkBoxFieldAmount->isChecked());
    s.setValue(s.keyName(s.INTERNAT_UNIT), checkBoxFieldUnit->isChecked());
    s.setValue(s.keyName(s.UNIT_PRICE), checkBoxFieldUnitPrice->isChecked());
    s.setValue(s.keyName(s.NET_VAL), checkBoxFieldNetVal->isChecked());
    s.setValue(s.keyName(s.DISCOUNT), checkBoxFieldDiscount->isChecked());
    s.setValue(s.keyName(s.DISCOUNT_VAL), checkBoxFieldDiscountVal->isChecked());
    s.setValue(s.keyName(s.NET_AFTER), checkBoxFieldNetAfterDiscount->isChecked());
    s.setValue(s.keyName(s.VAT_PRICE), checkBoxFieldVAT->isChecked());
    s.setValue(s.keyName(s.VAT_VAL), checkBoxFieldVATVal->isChecked());
    s.setValue(s.keyName(s.GROSS_VAL), checkBoxFieldGrossVal->isChecked());
    s.endGroup();

}


void SettingsDialog::readSettings_()
{
    int curr = 0;
    SettingsGlobal s;

    lineEditLogo->setText(s.value(s.keyName(s.LOGO)).toString());
    lineEditWorkDir->setText(s.value(s.keyName(s.WORKING_DIR)).toString());
    lineEditTaxIDMask->setText(s.value(s.keyName(s.TAXID_MASK)).toString());
    lineEditAccountMask->setText(s.value(s.keyName(s.ACCOUNT_MASK)).toString());
    lineEditFormat->setText(s.value(s.keyName(s.DEFAULT_INV_NUM_FORMAT)).toString());

    listWidgetUnit->clear();
    listWidgetUnit->addItems(s.value(s.keyName(s.UNITS)).toString().split("|"));
    listWidgetVAT->clear();
    listWidgetVAT->addItems(s.value(s.keyName(s.VAT_RATES)).toString().split("|"));
    listWidgetCurrency->clear();
    listWidgetCurrency->addItems(s.value(s.keyName(s.CURRENCIES)).toString().split("|"));
    listWidgetPayment->clear();
    listWidgetPayment->addItems(s.value(s.keyName(s.PAYMENT_TYPE)).toString().split("|"));

    listWidgetCorrectionReason->clear();
    listWidgetCorrectionReason->addItems(s.value(s.keyName(s.CORRECTION_REASON)).toString().split("|"));

    curr = getTranslations_().indexOf(s.value(s.keyName(s.LANG)).toString());
    comboBoxLanguage->setCurrentIndex(curr);

    curr = getTemplates_().indexOf(s.value(s.keyName(s.CSS)).toString());
    comboBoxCSS->setCurrentIndex(curr);

    checkBoxFieldID->setChecked(s.value(s.keyName(s.ORDER_NUMBER)).toBool());
    checkBoxFieldName->setChecked(s.value(s.keyName(s.NAME)).toBool());
    checkBoxFieldPostalCode->setChecked(s.value(s.keyName(s.CODE)).toBool());
    checkBoxFieldPKWIU->setChecked(s.value(s.keyName(s.PKWIU)).toBool());
    checkBoxFieldAmount->setChecked(s.value(s.keyName(s.QUANTITY)).toBool());
    checkBoxFieldUnit->setChecked(s.value(s.keyName(s.INTERNAT_UNIT)).toBool());
    checkBoxFieldUnitPrice->setChecked(s.value(s.keyName(s.UNIT_PRICE)).toBool());
    checkBoxFieldNetVal->setChecked(s.value(s.keyName(s.NET_VAL)).toBool());
    checkBoxFieldDiscount->setChecked(s.value(s.keyName(s.DISCOUNT)).toBool());
    checkBoxFieldDiscountVal->setChecked(s.value(s.keyName(s.DISCOUNT_VAL)).toBool());
    checkBoxFieldNetAfterDiscount->setChecked(s.value(s.keyName(s.NET_AFTER)).toBool());
    checkBoxFieldVAT->setChecked(s.value(s.keyName(s.VAT_VAL)).toBool());
    checkBoxFieldVATVal->setChecked(s.value(s.keyName(s.VAT_PRICE)).toBool());
    checkBoxFieldGrossVal->setChecked(s.value(s.keyName(s.GROSS_VAL)).toBool());

    textEditAdditionalText->setText(s.value(s.keyName(s.ADDIT_TEXT)).toString());
    checkBoxInvEdit->setChecked(s.value(s.keyName(s.EDIT)).toBool());
    checkBoxInvSymbolEdit->setChecked(s.value(s.keyName(s.EDIT_SYMBOL)).toBool());
    checkBoxProductNameEdit->setChecked(s.value(s.keyName(s.EDIT_NAME)).toBool());

    spinBoxNumCopies->setValue(s.value(s.keyName(s.NUMBER_OF_COPIES)).toInt());

    checkBoxName->setChecked(s.value(s.keyName(s.USER_NAME)).toBool());
    checkBoxLocation->setChecked(s.value(s.keyName(s.USER_LOCATION)).toBool());
    checkBoxAddress->setChecked(s.value(s.keyName(s.USER_ADDRESS)).toBool());
    checkBoxAccountName->setChecked(s.value(s.keyName(s.USER_ACCOUNT)).toBool());
    checkBoxTaxID->setChecked(s.value(s.keyName(s.USER_TAXID)).toBool());
    checkBoxPhone->setChecked(s.value(s.keyName(s.USER_PHONE)).toBool());
    checkBoxEmail->setChecked(s.value(s.keyName(s.USER_MAIL)).toBool());
    checkBoxWWW->setChecked(s.value(s.keyName(s.USER_WWW)).toBool());
}


QStringList SettingsDialog::getTemplates_()
{
    QStringList templates;
    QString ret;
    SettingsGlobal s;
    QString path = s.getWorkingDir() + "/templates/";

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
    InvoiceNumberFormatEditDialog dialog(this, db_, lineEditFormat->text());
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
