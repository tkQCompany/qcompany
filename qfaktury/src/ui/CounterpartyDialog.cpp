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
    QDialog(parent), ui(new Ui::CounterpartyDialog), db(db), id(id)
{
    ui->setupUi(this);
    init_();

    if(id.isValid())
    {
        if(myCompany)
        {
            proxyModelCounterpartyType.setFilterRegExp(
                        QRegExp(QString("%1").arg(CounterpartyTypeData::MY_COMPANY)));
        }
        setWindowTitle(trUtf8("Edytuj kontrahenta"));
        ui->comboBoxAdditionalPhones->setModel(db->modelPhone());
        ui->comboBoxAdditionalPhones->setModelColumn(PhoneFields::NUMBER);
        ui->comboBoxAdditionalEmails->setModel(db->modelEmail());
        ui->comboBoxAdditionalEmails->setModelColumn(EmailFields::EMAIL);
        mapper.setCurrentIndex(id.row());
    }
    else
    {
        ui->groupBoxAdditionalContacts->setVisible(false); //because at this moment there is no id_counterparty

        db->modelCounterparty()->insertRow(db->modelCounterparty()->rowCount());
        mapper.toLast();
        ui->comboBoxType->setCurrentIndex(ui->comboBoxType->findText(CounterpartyTypeData::name(CounterpartyTypeData::COMPANY)));

        SettingsGlobal s;
        ui->comboBoxCountry->setCurrentIndex(ui->comboBoxCountry->findText(s.value(s.COUNTRY).toString()));
    }

    if(db->modelCounterparty()->isInvNumFormatEmpty(id))
    {
        SettingsGlobal s;
        ui->lineEditInvNumberFormat->setText(s.value(s.DEFAULT_INV_NUM_FORMAT).toString());
    }

    connect(ui->pushButtonOK, SIGNAL(clicked()), this, SLOT(okClick()));
    connect(ui->pushButtonEditTypeList, SIGNAL(clicked()), this, SLOT(editCounterpartyTypeList()));
    connect(ui->pushButtonEditEmailList, SIGNAL(clicked()), this, SLOT(editEmailList()));
    connect(ui->pushButtonEditFormat, SIGNAL(clicked()), this, SLOT(editFormat()));
    connect(ui->pushButtonEditPhoneList, SIGNAL(clicked()), this, SLOT(editPhoneList()));
    connect(ui->pushButtonShowExamples, SIGNAL(clicked()), this, SLOT(showExamples()));
}


CounterpartyDialog::~CounterpartyDialog()
{
    db->modelCounterparty()->revertAll();
    delete ui;
}



/** init()
 */
void CounterpartyDialog::init_()
{
    proxyModelCounterpartyType.setSourceModel(db->modelCounterpartyType());
    proxyModelCounterpartyType.setFilterKeyColumn(CounterpartyTypeFields::ID);
    proxyModelCounterpartyType.setFilterRegExp(QRegExp(QString("[02-9]+")));

    ui->comboBoxType->setModel(&proxyModelCounterpartyType);
    ui->comboBoxType->setEditable(false);
    ui->comboBoxType->setModelColumn(CounterpartyTypeFields::TYPE);

    ui->comboBoxCountry->setModel(db->modelCountry());

    mapper.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper.setItemDelegate(new CounterpartyTypeDelegate(this));
    mapper.setModel(db->modelCounterparty());
    mapper.addMapping(ui->lineEditName, CounterpartyFields::NAME);
    mapper.addMapping(ui->comboBoxType, CounterpartyFields::TYPE_ID);
    mapper.addMapping(ui->comboBoxCountry, CounterpartyFields::COUNTRY);
    mapper.addMapping(ui->lineEditLocation, CounterpartyFields::LOCATION);
    mapper.addMapping(ui->lineEditPostalCode, CounterpartyFields::POSTAL_CODE);
    mapper.addMapping(ui->lineEditAddress, CounterpartyFields::STREET);
    mapper.addMapping(ui->lineEditTaxID, CounterpartyFields::TAX_IDENT);
    mapper.addMapping(ui->lineEditAccountName, CounterpartyFields::ACCOUNT_NAME);
    mapper.addMapping(ui->lineEditWWW, CounterpartyFields::WWW);
    mapper.addMapping(ui->lineEditPrimaryEmail, CounterpartyFields::PRIMARY_EMAIL);
    mapper.addMapping(ui->lineEditPrimaryPhone, CounterpartyFields::PRIMARY_PHONE);
    mapper.addMapping(ui->lineEditInvNumberFormat, CounterpartyFields::INV_NUM_FORMAT);

    SettingsGlobal s;
    ui->lineEditTaxID->setInputMask(s.value(s.TAXID_MASK).toString());
    ui->lineEditAccountName->setInputMask(s.value(s.ACCOUNT_MASK).toString());
}


void CounterpartyDialog::okClick()
{
    if(validate_())
    {
        if(mapper.submit() && (db->modelCounterparty()->lastError().type() == QSqlError::NoError))
        {
            accept();
        }
        else
        {
            qDebug() << "CounterpartyDialog::okClick_(), lastError(): " << db->modelCounterparty()->lastError() <<
                        ", lastQuery(): " << db->modelCounterparty()->query().lastQuery();
        }
    }
}


/** Validate form
 */
QString CounterpartyDialog::validateForm_()
{
    if (ui->lineEditName->text().isEmpty())
    {
        ui->lineEditName->setFocus();
        return ui->labelName->text();
    }

    if (ui->lineEditLocation->text().isEmpty())
    {
        ui->lineEditLocation->setFocus();
        return ui->labelLocation->text();
    }

    if (ui->lineEditPostalCode->text().isEmpty())
    {
        ui->lineEditPostalCode->setFocus();
        return ui->labelPostalCode->text();
    }


    if (ui->lineEditAddress->text().isEmpty())
    {
        ui->lineEditAddress->setFocus();
        return ui->labelStreet->text();
    }

    if (ui->lineEditTaxID->text().isEmpty())
    {
        ui->lineEditTaxID->setFocus();
        return ui->labelTaxID->text();
    }

    if(ui->comboBoxCountry->currentIndex() == -1)
    {
        ui->comboBoxCountry->setFocus();
        return ui->labelCountry->text();
    }

    if(ui->comboBoxType->currentIndex() == -1)
    {
        ui->comboBoxType->setFocus();
        return ui->labelType->text();
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


void CounterpartyDialog::editCounterpartyTypeList()
{
    CounterpartyTypeDialog dialog(this, db);
    db->modelCounterpartyType()->setMyCompanyVisible(false);
    if(dialog.exec() == QDialog::Accepted)
    {
        if(!db->modelCounterpartyType()->submitAll())
        {
            db->modelCounterpartyType()->revertAll();
            const QString msg(QString("%1\n%2")
                              .arg(trUtf8("Błąd edycji listy typów kontrahentów"))
                              .arg(db->modelCounterpartyType()->lastError().text()));
            QMessageBox::warning(this, trUtf8("Lista typów kontrahentów"), msg);
        }
    }
    db->modelCounterpartyType()->setMyCompanyVisible(true);
}


void CounterpartyDialog::editEmailList()
{
    EmailDialog dialog(this, db, id);
    if(dialog.exec() == QDialog::Accepted)
    {
        if(!db->modelEmail()->submitAll())
        {
            const QString msg(QString("%1:\n\n%2")
                              .arg(trUtf8("Błąd edycji listy emaili kontrahenta"))
                              .arg(db->modelEmail()->lastError().text()));
            QMessageBox::warning(this, trUtf8("Lista emaili"), msg);
        }
    }
    db->modelEmail()->revertAll();
}


void CounterpartyDialog::editPhoneList()
{
    PhoneDialog dialog(this, db, id);
    if(dialog.exec() == QDialog::Accepted)
    {
        if(!db->modelPhone()->submitAll())
        {
            const QString msg(QString("%1:\n\n%2")
                              .arg(trUtf8("Błąd edycji listy telefonów kontrahenta"))
                              .arg(db->modelPhone()->lastError().text()));
            QMessageBox::warning(this, trUtf8("Lista telefonów"), msg);
        }
    }
    db->modelPhone()->revertAll();
}


void CounterpartyDialog::editFormat()
{
    InvoiceNumberFormatEditDialog dialog(this, db, ui->lineEditInvNumberFormat->text());
    if(QDialog::Accepted == dialog.exec())
    {
        ui->lineEditInvNumberFormat->setText(dialog.format());
    }
}


void CounterpartyDialog::showExamples()
{
    InvoiceNumberFormatExamplesDialog dialog(this, db, ui->lineEditInvNumberFormat->text(), ui->lineEditName->text());
    dialog.exec();
}
