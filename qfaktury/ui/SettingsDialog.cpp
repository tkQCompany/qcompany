#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :QDialog(parent)
{
    setupUi(this);
    init();
}

void SettingsDialog::init()
{
    // connect all slots
    connect(pushButtonApply, SIGNAL(clicked()), this, SLOT(apply()));
    connect(pushButtonCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(pushButtonOK, SIGNAL(clicked()), this, SLOT(okButtonClick()));
    connect(pushButtonUnitAdd, SIGNAL(clicked()), this, SLOT(currAddBtnClick()));
    connect(pushButtonUnitRemove, SIGNAL(clicked()), this, SLOT(currDelBtnClick()));
    connect(pushButtonCurrencyAdd, SIGNAL(clicked()), this, SLOT(currencyAddBtnClick()));
    connect(pushButtonCurrencyRemove, SIGNAL(clicked()), this, SLOT(currencyDelBtnClick()));
    connect(pushButtonPaymentAdd, SIGNAL(clicked()), this, SLOT(paymAddBtnClick()));
    connect(pushButtonPaymentRemove, SIGNAL(clicked()), this, SLOT(paymDelBtnClick()));
    connect(pushButtonPaymentDown, SIGNAL(clicked()), this, SLOT(paymDownBtnClick()));
    connect(pushButtonPaymentUp, SIGNAL(clicked()), this, SLOT(paymUpBtnClick()));
    connect(pushButtonShowExamples, SIGNAL(clicked()), this, SLOT(showExamples_()));
    connect(pushButtonVATAdd, SIGNAL(clicked()), this, SLOT(vatAddBtnClick()));
    connect(pushButtonVATRemove, SIGNAL(clicked()), this, SLOT(vatDelBtnClick()));
    connect(pushButtonVATUp, SIGNAL(clicked()), this, SLOT(vatUpBtnClick()));
    connect(pushButtonVATDown, SIGNAL(clicked()), this, SLOT(vatDownBtnClick()));
    connect(pushButtonCorrectionReasonAdd, SIGNAL(clicked()), this, SLOT(corrAddBtnClick()));
    connect(pushButtonCorrectionReasonRemove, SIGNAL(clicked()), this, SLOT(corrDelBtnClick()));
    connect(pushButtonChooseLogo, SIGNAL(clicked()), this, SLOT(addLogoBtnClick()));
    connect(pushButtonWorkDir, SIGNAL(clicked()), this, SLOT(workingDirBtnClick()));
    connect(pushButtonDefault, SIGNAL(clicked()), this, SLOT(setDefaultClick()));
    connect(pushButtonDefaultAdditionalText, SIGNAL(clicked()), this, SLOT(defTextBtnClick()));
    connect(pushButtonMaskHelp, SIGNAL(clicked()), this, SLOT(maskHelpClick()));
    connect(pushButtonFormatEdit, SIGNAL(clicked()), this, SLOT(editFormat_()));

    connect( comboBoxCSS, SIGNAL( currentIndexChanged (int)), this, SLOT( zastBtnEnable() ) );
    connect( comboBoxLanguage, SIGNAL( currentIndexChanged (int)), this, SLOT( zastBtnEnable() ) );
    connect( lineEditLogo, SIGNAL(  textChanged (const QString &)), this, SLOT( zastBtnEnable() ) );
    connect( lineEditWorkDir, SIGNAL(  textChanged (const QString &)), this, SLOT( zastBtnEnable() ) );
    connect( lineEditTaxIDMask, SIGNAL(  textChanged (const QString &)), this, SLOT( zastBtnEnable() ) );
    connect( lineEditAccountMask, SIGNAL(  textChanged (const QString &)), this, SLOT( zastBtnEnable() ) );
    connect( spinBoxNumCopies, SIGNAL(  valueChanged (const QString &) ), this, SLOT( zastBtnEnable() ) );    
    connect( checkBoxInvEdit, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxInvSymbolEdit, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxProductNameEdit, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( textEditAdditionalText,  SIGNAL( textChanged() ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxName, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxLocation, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxAddress, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxAccountName, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxTaxID, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxPhone, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxEmail, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxWWW, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );

    connect( checkBoxFieldID, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxFieldName, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxFieldPostalCode, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxFieldPKWIU, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxFieldAmount, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxFieldUnit, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxFieldUnitPrice, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxFieldNetVal, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxFieldDiscount, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxFieldDiscountVal, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxFieldNetAfterDiscount, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxFieldVAT, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxFieldVATVal, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );
    connect( checkBoxFieldGrossVal, SIGNAL( stateChanged(int) ), this, SLOT( zastBtnEnable() ) );

    comboBoxLanguage->clear();
    comboBoxLanguage->insertItems(0, getTranslations());

    comboBoxCSS->clear();
    comboBoxCSS->insertItems(0, getTemplates());

    readSettings();

    // disable apply button :)
    pushButtonApply->setEnabled(false);

}

/** Slot - maskHelpClick
 */
void SettingsDialog::maskHelpClick() {
    QDesktopServices::openUrl(QUrl("http://doc.trolltech.com/4.5/qlineedit.html#inputMask-prop"));
}

/** Slot - Apply
 */
void SettingsDialog::apply () {
    saveSettings ();
    pushButtonApply->setEnabled(false);
}

/** Slot - OK
 */
void SettingsDialog::okButtonClick () {
    saveSettings ();
    accept ();
}

/** Slot applyBtn
 */
void SettingsDialog::zastBtnEnable()
{
    pushButtonApply->setEnabled(true);
}


/** Slot - set default text
 */
void SettingsDialog::defTextBtnClick() {
    textEditAdditionalText->setText ( trUtf8("towar odebrałem zgodnie z fakturą") );
}

/** Slot - set all to default
 */
void SettingsDialog::setDefaultClick() {
    if (QMessageBox::question(this, qApp->applicationName(), trUtf8("Czy napewno chcesz przywrócic ustawienia domyślne?"),
                              QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
        return;

    sett().resetSettings();
    // is this required? //TODO
    readSettings();
}


void SettingsDialog::showExamples_()
{
    InvoiceNumberFormatExamplesDialog dialog(this, lineEditFormat->text());
    dialog.exec();
}

/** Slot used to change location of invoiced
 */
void SettingsDialog::workingDirBtnClick() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home", QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    lineEditWorkDir->setText(dir);
    pushButtonApply->setEnabled(true);
}


/** Slot add logo
 */
void SettingsDialog::addLogoBtnClick() {
    QString ofn = QFileDialog::getOpenFileName(this, trUtf8("Wybierz plik do wstawienia jako logo"), "",
                                               trUtf8("Obrazki (*.jpg *.png)"));

    lineEditLogo->setText(ofn);
    pushButtonApply->setEnabled(true);
}

//----------------------- List box Slots START ---
//@TODO merge into 1 function

/** Slot add currency
 */
void SettingsDialog::currAddBtnClick() {
    if (lineEditUnit->text() != "") {
        listWidgetUnit->addItem(lineEditUnit->text());
        lineEditUnit->clear();
    } else {
        QMessageBox::information(this, trUtf8("Uwaga!!"),trUtf8("Nie można dodać. Pole jest puste."), QMessageBox::Ok);
        return;
    }
    pushButtonApply->setEnabled(true);
}


/** Slot del currency
 */
void SettingsDialog::currDelBtnClick() {
    int selNr = listWidgetUnit->currentRow();
    if (selNr >= 0) {
        listWidgetUnit->takeItem(selNr);
    } else {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Musisz coś zaznaczyś, żeby usuwać."),
                                 QMessageBox::Ok);
        return;
    }
    pushButtonApply->setEnabled(true);
}

/** Slot korekty reason add
 */
void SettingsDialog::corrAddBtnClick()
{
    if (!lineEditCorrectionReason->text().isEmpty())
    {
        listWidgetCorrectionReason->addItem(lineEditCorrectionReason->text());
        lineEditCorrectionReason->clear();
    } else {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Nie można dodać. Pole jest puste."),
                                 QMessageBox::Ok);
        return;
    }
    pushButtonApply->setEnabled(true);
}

/** Slot korekty reason delete
 */
void SettingsDialog::corrDelBtnClick()
{
    const int selNr = listWidgetCorrectionReason->currentRow();
    if (selNr >= 0) {
        listWidgetCorrectionReason->takeItem(selNr);
    } else {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Musisz coś zaznaczyś, żeby usuwać."),
                                 QMessageBox::Ok);
        return;
    }
    pushButtonApply->setEnabled(true);
}

/** Slot predefined VAT value add
 */
void SettingsDialog::vatAddBtnClick() {
    if (lineEditVAT->text() != "") {
        listWidgetVAT->addItem(lineEditVAT->text());
        lineEditVAT->clear();
    } else {
        QMessageBox::information(this, trUtf8("Uwaga!!"),
                                 trUtf8("Nie można dodać. Pole jest puste."), QMessageBox::Ok);
        return;
    }
    pushButtonApply->setEnabled(true);
}

/** Slot predefined VAT value delete
 */
void SettingsDialog::vatDelBtnClick() {
    int selNr = listWidgetVAT->currentRow();
    if (selNr >= 0) {
        listWidgetVAT->takeItem(selNr);
    } else {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Musisz coś zaznaczyć, żeby usuwać."),
                                 QMessageBox::Ok);
        return;
    }
    pushButtonApply->setEnabled(true);
}

/** Slot add currency
 */
void SettingsDialog::currencyAddBtnClick()
{
    if ( lineEditCurrency->text() != "" ) {
        listWidgetCurrency->addItem(lineEditCurrency->text());
        lineEditCurrency->clear();
    } else {
        QMessageBox::information (this, trUtf8("Uwaga!!"), trUtf8("Nie można dodać. Pole jest puste."),
                                  QMessageBox::Ok);
        return;
    }
    pushButtonApply->setEnabled(true);
}

/** Slot del currency
 */
void SettingsDialog::currencyDelBtnClick() {
    int selNr = listWidgetCurrency->currentRow();
    if (selNr >= 0) {
        listWidgetCurrency->takeItem(selNr);
    } else {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Musisz coś zaznaczyś, żeby usuwać."),
                                 QMessageBox::Ok);
        return;
    }
    pushButtonApply->setEnabled(true);
}

/** Slot delete payment type click
 */
void SettingsDialog::paymDelBtnClick() {
    int selNr = listWidgetPayment->currentRow();
    if (selNr >= 0) {
        listWidgetPayment->takeItem(selNr);
    } else {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Musisz coś zaznaczyś, żeby usuwać."),
                                 QMessageBox::Ok);
        return;
    }
    pushButtonApply->setEnabled(true);
}

/** Slot add payment type click
 */
void SettingsDialog::paymAddBtnClick() {
    if (!lineEditPayment->text().isEmpty())
    {
        listWidgetPayment->addItem(lineEditPayment->text());
        lineEditPayment->clear();
    }
    else
    {
        QMessageBox::information(this, trUtf8("Uwaga!!"),
                                 trUtf8("Nie można dodać. Pole jest puste."), QMessageBox::Ok);
        return;
    }
    pushButtonApply->setEnabled(true);
}
//----------------------- List box Slots END ---

//---------------------- UP DOWN SLOTS START----
/** Slot move VAT value up
 */
void SettingsDialog::vatUpBtnClick() {
    const int selNr = listWidgetVAT->currentRow();

    if (selNr == 0) {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Nie można przenieść w górę, już jest najwyżej."),
                                 QMessageBox::Ok);
        return;
    }

    if (selNr > 0) {
        QListWidgetItem *item = listWidgetVAT->item(listWidgetVAT->currentRow());
        listWidgetVAT->takeItem(selNr);
        listWidgetVAT->insertItem(selNr - 1, item);
        listWidgetVAT->setCurrentRow(selNr - 1);
    } else {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Musisz coś zaznaczyć, żeby przesuwać."), QMessageBox::Ok);
        return;
    }
    pushButtonApply->setEnabled(true);
}

/** Slot move VAT value down
 */
void SettingsDialog::vatDownBtnClick() {
    const int selNr = listWidgetVAT->currentRow();
    int recCount = listWidgetVAT->count();

    if (selNr == recCount - 1) {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Nie można przenieść w dół, już jest najniżej."),
                                 QMessageBox::Ok);
        return;
    }

    if (selNr >= 0) {
        QListWidgetItem *item = listWidgetVAT->item(listWidgetVAT->currentRow());
        listWidgetVAT->takeItem(selNr);
        listWidgetVAT->insertItem(selNr + 1, item);
        listWidgetVAT->setCurrentRow(selNr + 1);
    } else {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Musisz coś zaznaczyć, żeby przesuwać."), QMessageBox::Ok);
        return;
    }
    pushButtonApply->setEnabled(true);
}

/** Slot move payment value up
 */
void SettingsDialog::paymUpBtnClick() {
    int selNr = listWidgetPayment->currentRow();

    if (selNr == 0) {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Nie można przenieść w górę, już jest najwyżej."),
                                 QMessageBox::Ok);
        return;
    }

    if (selNr > 0) {
        QListWidgetItem *item = listWidgetPayment->item(listWidgetPayment->currentRow());
        listWidgetPayment->takeItem(selNr);
        listWidgetPayment->insertItem(selNr - 1, item);
        listWidgetPayment->setCurrentRow(selNr - 1);
    } else {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Musisz coś zaznaczyć, żeby przesuwać."), QMessageBox::Ok);
        return;
    }
    pushButtonApply->setEnabled(true);
}

/** Slot move payment value down
 */
void SettingsDialog::paymDownBtnClick() {
    int selNr = listWidgetPayment->currentRow();
    int recCount = listWidgetPayment->count();

    if (selNr == recCount - 1) {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Nie można przenieść w dół, już jest najniżej."),
                                 QMessageBox::Ok);
        return;
    }

    if (selNr >= 0) {
        QListWidgetItem *item = listWidgetPayment->item(listWidgetPayment->currentRow());
        listWidgetPayment->takeItem(selNr);
        listWidgetPayment->insertItem(selNr + 1, item);
        listWidgetPayment->setCurrentRow(selNr + 1);
    } else {
        QMessageBox::information(this, trUtf8("Uwaga!!"), trUtf8("Musisz coś zaznaczyć, żeby przesuwać."), QMessageBox::Ok);
        return;
    }
    pushButtonApply->setEnabled(true);
}
//---------------------- UP DOWN SLOTS END----



/** Used for parsing
 */
QString SettingsDialog::getItemsToString(const QListWidget *lw)
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
void SettingsDialog::saveSettings() {
    sett().setValue("lang", comboBoxLanguage->currentText());
    sett().setValue("css", comboBoxCSS->currentText());
    sett().setValue("working_dir", lineEditWorkDir->text());

    sett().beginGroup("printpos");
    sett().setValue("usernazwa", checkBoxName->isChecked());
    sett().setValue("usermiejscowosc", checkBoxLocation->isChecked());
    sett().setValue("useradres", checkBoxAddress->isChecked());
    sett().setValue("userkonto", checkBoxAccountName->isChecked());
    sett().setValue("usernip", checkBoxTaxID->isChecked());
    sett().setValue("userphone", checkBoxPhone->isChecked());
    sett().setValue("usermail", checkBoxEmail->isChecked());
    sett().setValue("userwww", checkBoxWWW->isChecked());
    sett().endGroup();

    sett().setValue("firstrun", false);
    sett().setValue("logo", lineEditLogo->text());
    sett().setValue("jednostki", getItemsToString(listWidgetUnit));
    sett().setValue("stawki", getItemsToString(listWidgetVAT).remove("%"));
    sett().setValue("waluty", getItemsToString(listWidgetCurrency));
    sett().setValue("pkorekty", getItemsToString(listWidgetCorrectionReason));
    sett().setValue("payments", getItemsToString(listWidgetPayment)); // uwaga!! get first
    sett().setValue("paym1", listWidgetPayment->item(0)->text());
    sett().setValue("addText", textEditAdditionalText->toPlainText());

    sett().setValue("edit", checkBoxInvEdit->isChecked());
    sett().setValue("editSymbol", checkBoxInvSymbolEdit->isChecked());
    sett().setValue("editName", checkBoxProductNameEdit->isChecked());
    sett().setValue("ticMask", lineEditTaxIDMask->text());
    sett().setValue("accountMask", lineEditAccountMask->text());
    sett().setValue("numberOfCopies", spinBoxNumCopies->value());

    sett().beginGroup("faktury_pozycje");
    sett().setValue("Lp", checkBoxFieldID->isChecked());
    sett().setValue("Nazwa", checkBoxFieldName->isChecked());
    sett().setValue("Kod", checkBoxFieldPostalCode->isChecked());
    sett().setValue("pkwiu", checkBoxFieldPKWIU->isChecked());
    sett().setValue("ilosc", checkBoxFieldAmount->isChecked());
    sett().setValue("jm", checkBoxFieldUnit->isChecked());
    sett().setValue("cenajedn", checkBoxFieldUnitPrice->isChecked());
    sett().setValue("wartnetto", checkBoxFieldNetVal->isChecked());
    sett().setValue("rabatperc", checkBoxFieldDiscount->isChecked());
    sett().setValue("rabatval", checkBoxFieldDiscountVal->isChecked());
    sett().setValue("nettoafter", checkBoxFieldNetAfterDiscount->isChecked());
    sett().setValue("vatval", checkBoxFieldVAT->isChecked());
    sett().setValue("vatprice", checkBoxFieldVATVal->isChecked());
    sett().setValue("bruttoval", checkBoxFieldGrossVal->isChecked());
    sett().endGroup();

}

/** Read all sett()
 */
void SettingsDialog::readSettings()
{
    int curr = 0;

    lineEditLogo->setText(sett().value("logo").toString());
    lineEditWorkDir->setText(sett().value("working_dir").toString());

    listWidgetUnit->clear();
    listWidgetUnit->addItems(sett().value("jednostki").toString().split("|"));
    listWidgetVAT->clear();
    listWidgetVAT->addItems(sett().value("stawki").toString().split("|"));
    listWidgetCurrency->clear();
    listWidgetCurrency->addItems(sett().value("waluty").toString().split("|"));
    listWidgetPayment->clear();
    listWidgetPayment->addItems(sett().value("payments").toString().split("|"));

    listWidgetCorrectionReason->clear();
    listWidgetCorrectionReason->addItems(sett().value("pkorekty").toString().split("|"));

    curr = getTranslations().indexOf(sett().value("lang").toString());
    comboBoxLanguage->setCurrentIndex(curr);

    curr = getTemplates().indexOf(sett().value("css").toString());
    comboBoxCSS->setCurrentIndex(curr);

    lineEditTaxIDMask->setText(sett().value("ticMask", "999-99-999-99; ").toString());
    lineEditAccountMask->setText(sett().value("accountMask", "99-9999-9999-9999-9999-9999-9999; ").toString());

    sett().beginGroup("faktury_pozycje");
    checkBoxFieldID->setChecked(sett().value("Lp").toBool());
    checkBoxFieldName->setChecked(sett().value("Nazwa").toBool());
    checkBoxFieldPostalCode->setChecked(sett().value("Kod").toBool());
    checkBoxFieldPKWIU->setChecked(sett().value("pkwiu").toBool());
    checkBoxFieldAmount->setChecked(sett().value("ilosc").toBool());
    checkBoxFieldUnit->setChecked(sett().value("jm").toBool());
    checkBoxFieldUnitPrice->setChecked(sett().value("cenajedn").toBool());
    checkBoxFieldNetVal->setChecked(sett().value("wartnetto").toBool());
    checkBoxFieldDiscount->setChecked(sett().value("rabatperc").toBool());
    checkBoxFieldDiscountVal->setChecked(sett().value("rabatval").toBool());
    checkBoxFieldNetAfterDiscount->setChecked(sett().value("nettoafter").toBool());
    checkBoxFieldVAT->setChecked(sett().value("vatval").toBool());
    checkBoxFieldVATVal->setChecked(sett().value("vatprice").toBool());
    checkBoxFieldGrossVal->setChecked(sett().value("bruttoval").toBool());
    sett().endGroup();

    textEditAdditionalText->setText(sett().value("addText").toString());

    checkBoxInvEdit->setChecked(sett().value("edit").toBool());

    checkBoxInvSymbolEdit->setChecked(sett().value("editSymbol").toBool());
    checkBoxProductNameEdit->setChecked(sett().value("editName").toBool());

    spinBoxNumCopies->setValue(sett(). value("numberOfCopies").toInt());

    sett().beginGroup("printpos");
    checkBoxName->setChecked(sett().value("usernazwa").toBool());
    checkBoxLocation->setChecked(sett().value("usermiejscowosc").toBool());
    checkBoxAddress->setChecked(sett().value("useradres").toBool());
    checkBoxAccountName->setChecked(sett().value("userkonto").toBool());
    checkBoxTaxID->setChecked(sett().value("usernip").toBool());
    checkBoxPhone->setChecked(sett().value("userphone").toBool());
    checkBoxEmail->setChecked(sett().value("usermail").toBool());
    checkBoxWWW->setChecked(sett().value("userwww").toBool());
    sett().endGroup();

    checkBoxInvEdit->setChecked(sett().value("edit") .toBool());
    checkBoxInvSymbolEdit->setChecked(sett().value("editSymbol") .toBool());

    read=true;
    // readTemplate();
}


// returns list of translations
QStringList SettingsDialog::getTemplates()
{
    QStringList templates;
    QString ret;
    QString path = sett().getWorkingDir() + "/templates/";

    if (templates.isEmpty()) {
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
QStringList SettingsDialog::getTranslations() const
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
