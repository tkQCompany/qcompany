#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

#include "CounterpartyDialog.h"
#include "ui_CounterpartyDialog.h"
#include "CounterpartyTypeData.h"
#include "Database.h"
#include "SettingsGlobal.h"
#include "CounterpartyTypeDelegate.h"
#include "CounterpartyTypeDialog.h"
#include "EmailDialog.h"
#include "PhoneDialog.h"
#include "InvoiceNumberFormatEditDialog.h"
#include "InvoiceNumberFormatExamplesDialog.h"
#include "ModelPhone.h"
#include "PhoneData.h"
#include "ModelEmail.h"
#include "EmailData.h"
#include "ModelCounterparty.h"
#include "ModelCounterpartyType.h"
#include "ModelCountry.h"
#include "CounterpartyData.h"


/** Constructor
 */
CounterpartyDialog::CounterpartyDialog(QWidget *parent, Database *db, const QModelIndex &id, const bool myCompany) :
    QDialog(parent), ui_(new Ui::CounterpartyDialog), db_(db), id_(id)
{
    ui_->setupUi(this);
    init();

    if(id.isValid())
    {
        if(myCompany)
        {
            proxyModelCounterpartyType_.setFilterRegExp(
                        QRegExp(QString("%1").arg(CounterpartyTypeData::MY_COMPANY + 1)));
        }
        setWindowTitle(trUtf8("Edytuj kontrahenta"));
        ui_->comboBoxAdditionalPhones->setModel(db_->modelPhone());
        ui_->comboBoxAdditionalPhones->setModelColumn(PhoneFields::NUMBER);
        ui_->comboBoxAdditionalEmails->setModel(db_->modelEmail());
        ui_->comboBoxAdditionalEmails->setModelColumn(EmailFields::EMAIL);
        mapper_.setCurrentIndex(id.row());
    }
    else
    {
        ui_->groupBoxAdditionalContacts->setVisible(false); //because at this moment there is no id_counterparty

        db_->modelCounterparty()->insertRow(db_->modelCounterparty()->rowCount());
        mapper_.toLast();
        ui_->comboBoxType->setCurrentIndex(ui_->comboBoxType->findText(CounterpartyTypeData::name(CounterpartyTypeData::COMPANY)));

        SettingsGlobal s;
        ui_->comboBoxCountry->setCurrentIndex(ui_->comboBoxCountry->findText(s.value(s.COUNTRY).toString()));
    }

    if(db_->modelCounterparty()->isInvNumFormatEmpty(id))
    {
        SettingsGlobal s;
        ui_->lineEditInvNumberFormat->setText(s.value(s.DEFAULT_INV_NUM_FORMAT).toString());
    }

    connect(ui_->pushButtonOK, SIGNAL(clicked()), this, SLOT(okClick_()));
    connect(ui_->pushButtonEditTypeList, SIGNAL(clicked()), this, SLOT(editCounterpartyTypeList_()));
    connect(ui_->pushButtonEditEmailList, SIGNAL(clicked()), this, SLOT(editEmailList_()));
    connect(ui_->pushButtonEditFormat, SIGNAL(clicked()), this, SLOT(editFormat_()));
    connect(ui_->pushButtonEditPhoneList, SIGNAL(clicked()), this, SLOT(editPhoneList_()));
    connect(ui_->pushButtonShowExamples, SIGNAL(clicked()), this, SLOT(showExamples_()));
}


CounterpartyDialog::~CounterpartyDialog()
{
    db_->modelCounterparty()->revertAll();
    delete ui_;
}



/** init()
 */
void CounterpartyDialog::init()
{
    proxyModelCounterpartyType_.setSourceModel(db_->modelCounterpartyType());
    proxyModelCounterpartyType_.setFilterKeyColumn(CounterpartyTypeFields::ID);
    proxyModelCounterpartyType_.setFilterRegExp(QRegExp(QString("[02-9]+")));

    ui_->comboBoxType->setModel(&proxyModelCounterpartyType_);
    ui_->comboBoxType->setEditable(false);
    ui_->comboBoxType->setModelColumn(CounterpartyTypeFields::TYPE);

    ui_->comboBoxCountry->setModel(db_->modelCountry());

    mapper_.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper_.setItemDelegate(new CounterpartyTypeDelegate(this));
    mapper_.setModel(db_->modelCounterparty());
    mapper_.addMapping(ui_->lineEditName, CounterpartyFields::NAME);
    mapper_.addMapping(ui_->comboBoxType, CounterpartyFields::TYPE_ID);
    mapper_.addMapping(ui_->comboBoxCountry, CounterpartyFields::COUNTRY);
    mapper_.addMapping(ui_->lineEditLocation, CounterpartyFields::LOCATION);
    mapper_.addMapping(ui_->lineEditPostalCode, CounterpartyFields::POSTAL_CODE);
    mapper_.addMapping(ui_->lineEditAddress, CounterpartyFields::STREET);
    mapper_.addMapping(ui_->lineEditTaxID, CounterpartyFields::TAX_IDENT);
    mapper_.addMapping(ui_->lineEditAccountName, CounterpartyFields::ACCOUNT_NAME);
    mapper_.addMapping(ui_->lineEditWWW, CounterpartyFields::WWW);
    mapper_.addMapping(ui_->lineEditPrimaryEmail, CounterpartyFields::PRIMARY_EMAIL);
    mapper_.addMapping(ui_->lineEditPrimaryPhone, CounterpartyFields::PRIMARY_PHONE);
    mapper_.addMapping(ui_->lineEditInvNumberFormat, CounterpartyFields::INV_NUM_FORMAT);

    SettingsGlobal s;
    ui_->lineEditTaxID->setInputMask(s.value(s.TAXID_MASK).toString());
    ui_->lineEditAccountName->setInputMask(s.value(s.ACCOUNT_MASK).toString());
}


void CounterpartyDialog::okClick_()
{
    if(validate_())
    {
        if(mapper_.submit() && (db_->modelCounterparty()->lastError().type() == QSqlError::NoError))
        {
            accept();
        }
        else
        {
            qDebug() << "CounterpartyDialog::okClick_(), lastError(): " << db_->modelCounterparty()->lastError() <<
                        ", lastQuery(): " << db_->modelCounterparty()->query().lastQuery();
        }
    }
}


/** Validate form
 */
QString CounterpartyDialog::validateForm_()
{
    if (ui_->lineEditName->text().isEmpty())
    {
        ui_->lineEditName->setFocus();
        return ui_->labelName->text();
    }

    if (ui_->lineEditLocation->text().isEmpty())
    {
        ui_->lineEditLocation->setFocus();
        return ui_->labelLocation->text();
    }

    if (ui_->lineEditPostalCode->text().isEmpty())
    {
        ui_->lineEditPostalCode->setFocus();
        return ui_->labelPostalCode->text();
    }


    if (ui_->lineEditAddress->text().isEmpty())
    {
        ui_->lineEditAddress->setFocus();
        return ui_->labelStreet->text();
    }

    if (ui_->lineEditTaxID->text().isEmpty())
    {
        ui_->lineEditTaxID->setFocus();
        return ui_->labelTaxID->text();
    }

    if(ui_->comboBoxCountry->currentIndex() == -1)
    {
        ui_->comboBoxCountry->setFocus();
        return ui_->labelCountry->text();
    }

    if(ui_->comboBoxType->currentIndex() == -1)
    {
        ui_->comboBoxType->setFocus();
        return ui_->labelType->text();
    }

    return QString();
}

/** validate()
 */
bool CounterpartyDialog::validate_()
{
    QString missing;
    if(!(missing = validateForm_()).isEmpty())
    {
        QMessageBox::warning(this, qApp->applicationName(),
                    trUtf8("Kontrahent nie moze zostać zapisany, ponieważ brakuje wymaganych danych w polu: ") + missing);
        return false;
    }

    return true;
}


// helper method which sets "-" in input forms
QString CounterpartyDialog::isEmpty_(const QString &in)
{
    if (in.isEmpty()) return "-";
    return in;
}


void CounterpartyDialog::editCounterpartyTypeList_()
{
    CounterpartyTypeDialog dialog(this, db_);
    db_->modelCounterpartyType()->setMyCompanyVisible(false);
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
    db_->modelCounterpartyType()->setMyCompanyVisible(true);
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
    InvoiceNumberFormatEditDialog dialog(this, db_, ui_->lineEditInvNumberFormat->text());
    if(QDialog::Accepted == dialog.exec())
    {
        ui_->lineEditInvNumberFormat->setText(dialog.format());
    }
}


void CounterpartyDialog::showExamples_()
{
    InvoiceNumberFormatExamplesDialog dialog(this, db_, ui_->lineEditInvNumberFormat->text(), ui_->lineEditName->text());
    dialog.exec();
}
