#include "CounterpartyDialog.h"

/** Constructor
 */
CounterpartyDialog::CounterpartyDialog(QWidget *parent, Database *db, const QModelIndex &id) :
    QDialog(parent), db_(db), id_(id)
{
    setupUi(this);
    init();

    if(id.isValid())
    {
        setWindowTitle(trUtf8("Edytuj kontrahenta"));
        comboBoxAdditionalPhones->setModel(db_->modelPhone());
        comboBoxAdditionalPhones->setModelColumn(PhoneFields::NUMBER);
        comboBoxAdditionalEmails->setModel(db_->modelEmail());
        comboBoxAdditionalEmails->setModelColumn(EmailFields::EMAIL);
        mapper_.setCurrentIndex(id.row());
    }
    else
    {
        groupBoxAdditionalContacts->setVisible(false); //because at this moment there is no id_counterparty

        db_->modelCounterparty()->insertRow(db_->modelCounterparty()->rowCount());
        mapper_.toLast();
        comboBoxType->setCurrentIndex(CounterpartyTypeData::COMPANY - 1);
        comboBoxCountry->setCurrentIndex(0); //TODO: i18n
    }
    connect(pushButtonOK, SIGNAL(clicked()), this, SLOT(okClick_()));
    connect(pushButtonEditTypeList, SIGNAL(clicked()), this, SLOT(editCounterpartyTypeList_()));
    connect(pushButtonEditCountryList, SIGNAL(clicked()), this, SLOT(editCountryList_()));
    connect(pushButtonEditEmailList, SIGNAL(clicked()), this, SLOT(editEmailList_()));
    connect(pushButtonEditFormat, SIGNAL(clicked()), this, SLOT(editFormat_()));
    connect(pushButtonEditPhoneList, SIGNAL(clicked()), this, SLOT(editPhoneList_()));
    connect(pushButtonShowExamples, SIGNAL(clicked()), this, SLOT(showExamples_()));
}


CounterpartyDialog::~CounterpartyDialog()
{
    db_->modelCounterparty()->revertAll();
}



/** init()
 */
void CounterpartyDialog::init()
{
    SettingsGlobal s;
    comboBoxType->setModel(db_->modelCounterpartyType());
    comboBoxType->setModelColumn(CounterpartyTypeFields::TYPE);
    comboBoxType->setEditable(false);

    comboBoxCountry->setModel(db_->modelCountry());
    comboBoxCountry->setModelColumn(CountryFields::NAME);

    mapper_.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper_.setItemDelegate(new QSqlRelationalDelegate(this));
    mapper_.setModel(db_->modelCounterparty());
    mapper_.addMapping(lineEditName, CounterpartyFields::NAME);
    mapper_.addMapping(comboBoxType, CounterpartyFields::TYPE_ID);
    mapper_.addMapping(comboBoxCountry, CounterpartyFields::COUNTRY_ID);
    mapper_.addMapping(lineEditLocation, CounterpartyFields::LOCATION);
    mapper_.addMapping(lineEditPostalCode, CounterpartyFields::POSTAL_CODE);
    mapper_.addMapping(lineEditAddress, CounterpartyFields::STREET);
    mapper_.addMapping(lineEditTaxID, CounterpartyFields::TAX_IDENT);
    mapper_.addMapping(lineEditAccountName, CounterpartyFields::ACCOUNT_NAME);
    mapper_.addMapping(lineEditWWW, CounterpartyFields::WWW);
    mapper_.addMapping(lineEditPrimaryEmail, CounterpartyFields::PRIMARY_EMAIL);
    mapper_.addMapping(lineEditPrimaryPhone, CounterpartyFields::PRIMARY_PHONE);
    mapper_.addMapping(lineEditInvNumberFormat, CounterpartyFields::INV_NUM_FORMAT);

    lineEditTaxID->setInputMask(s.value(s.keyName(s.TAXID_MASK)).toString());
    lineEditAccountName->setInputMask(s.value(s.keyName(s.ACCOUNT_MASK)).toString());
}

// --------- SLOTS START --
/** Slot - ok & save
 */
void CounterpartyDialog::okClick_()
{
    if (validate_())
    {
        mapper_.submit();
        if(db_->modelCounterparty()->lastError().type() == QSqlError::NoError)
        {
            accept();
        }
        else
        {
            qDebug() << "lastError(): " << db_->modelCounterparty()->lastError() <<
                        ", lastQuery(): " << db_->modelCounterparty()->query().lastQuery();
        }
    }
}
// --------- SLOTS END --



//********************** VALIDATION START ************************
/** Validate form
 *  Don't save when no
 *  "name", "city", "street", "tic"
 */
bool CounterpartyDialog::validateForm_(QString &missing) {
    if (lineEditName->text().isEmpty()) {
        missing = labelName->text();
        lineEditName->setFocus();
        return false;
    }

    if (lineEditLocation->text().isEmpty()) {
        missing = labelLocation->text();
        lineEditLocation->setFocus();
        return false;
    }

    if (lineEditPostalCode->text().isEmpty()) {
        missing = labelPostalCode->text();
        lineEditPostalCode->setFocus();
        return false;
    }


    if (lineEditAddress->text().isEmpty()) {
        missing = labelStreet->text();
        lineEditAddress->setFocus();
        return false;
    }

    if (lineEditTaxID->text().isEmpty()) {
        missing = labelTaxID->text();
        lineEditTaxID->setFocus();
        return false;
    }

    return true;
}

/** validate()
 */
bool CounterpartyDialog::validate_()
{
    QString missing;
    if (!validateForm_(missing))
    {
        QMessageBox::warning(this, qApp->applicationName(),
                    trUtf8("Kontrahent nie moze zostać zapisany, ponieważ brakuje wymaganych danych w polu: ") + missing);
        return false;
    }

    return true;
}
//********************** VALIDATION  END ************************



// helper method which sets "-" in input forms
QString CounterpartyDialog::isEmpty_(const QString &in)
{
    if (in.isEmpty()) return "-";
    return in;
}


void CounterpartyDialog::editCountryList_()
{
    CountryDialog dialog(this, db_);
    if(dialog.exec() == QDialog::Accepted)
    {
        if(!db_->modelCountry()->submitAll())
        {
            db_->modelCountry()->revertAll();
            const QString msg(QString("%1\n%2")
                              .arg(trUtf8("Błąd edycji listy krajów"))
                              .arg(db_->modelCounterpartyType()->lastError().text()));
            QMessageBox::warning(this, trUtf8("Lista krajów"), msg);
        }
    }
}


void CounterpartyDialog::editCounterpartyTypeList_()
{
    CounterpartyTypeDialog dialog(this, db_);
    if(dialog.exec() == QDialog::Accepted)
    {
        if(!db_->modelCounterpartyType()->submitAll())
        {
            db_->modelCounterpartyType()->revertAll();
            const QString msg(QString("%1\n%2")
                              .arg(trUtf8("Błąd edycji listy typów kontrahentów"))
                              .arg(db_->modelCounterpartyType()->lastError().text()));
            QMessageBox::warning(this, trUtf8("Lista typów kontrahentów"), msg);
        }
    }
}


void CounterpartyDialog::editEmailList_()
{
    EmailDialog dialog(this, db_, id_);
    if(dialog.exec() == QDialog::Accepted)
    {
        if(!db_->modelEmail()->submitAll())
        {
            const QString msg(QString("%1:\n\n%2")
                              .arg(trUtf8("Błąd edycji listy emaili kontrahenta"))
                              .arg(db_->modelEmail()->lastError().text()));
            QMessageBox::warning(this, trUtf8("Lista emaili"), msg);
        }
    }
    db_->modelEmail()->revertAll();
}


void CounterpartyDialog::editPhoneList_()
{
    PhoneDialog dialog(this, db_, id_);
    if(dialog.exec() == QDialog::Accepted)
    {
        if(!db_->modelPhone()->submitAll())
        {
            const QString msg(QString("%1:\n\n%2")
                              .arg(trUtf8("Błąd edycji listy telefonów kontrahenta"))
                              .arg(db_->modelPhone()->lastError().text()));
            QMessageBox::warning(this, trUtf8("Lista telefonów"), msg);
        }
    }
    db_->modelPhone()->revertAll();
}


void CounterpartyDialog::editFormat_()
{
    InvoiceNumberFormatEditDialog dialog(this, db_, lineEditInvNumberFormat->text());
    if(QDialog::Accepted == dialog.exec())
    {
        this->lineEditInvNumberFormat->setText(dialog.format());
    }
}


void CounterpartyDialog::showExamples_()
{
    InvoiceNumberFormatExamplesDialog dialog(this, db_, lineEditInvNumberFormat->text(), lineEditName->text());
    dialog.exec();
}
