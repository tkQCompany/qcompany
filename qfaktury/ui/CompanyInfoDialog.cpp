#include "CompanyInfoDialog.h"


CompanyInfoDialog::CompanyInfoDialog(QWidget *parent, Database *db): QDialog(parent), db_(db)
{
    setupUi(this);
    init();
}


/*!
  * init function read settings
  !*/
void CompanyInfoDialog::init()
{
  QSettings settings;
  lineEditName->setText (settings.value ("name").toString());
  lineEditLocation->setText (settings.value ("city").toString());
  lineEditPostalCode->setText (settings.value ("zip").toString());
  lineEditAddress->setText (settings.value ("address").toString());
  lineEditAccountName->setText (settings.value ("account").toString());
  if (!settings.value ("secIdType").isNull() )
  {
     comboBoxFirstID->setCurrentIndex(comboBoxFirstID->findText(settings.value ("secIdType").toString()));
  }

  lineEditTaxID->setText (settings.value ("tic").toString());
  lineEditSecondID->setText (settings.value ("regon").toString());

  lineEditTaxID->setInputMask(sett().value("ticMask", "999-99-999-99; ").toString());
  lineEditAccountName->setInputMask(sett().value("accountMask", "99-9999-9999-9999-9999-9999-9999; ").toString());

  connect(pushButtonOK, SIGNAL(clicked()), this, SLOT(okClick()));
}

/*!
  *   save settings
  !*/
void CompanyInfoDialog::okClick ()
{
  QSettings settings;
  settings.setValue ("name", lineEditName->text ());	// zapis String
  settings.setValue ("city", lineEditLocation->text ());
  settings.setValue ("zip", lineEditPostalCode->text ());
  settings.setValue ("address", lineEditAddress->text ());
  settings.setValue ("account", lineEditAccountName->text ());
  settings.setValue ("tic", lineEditTaxID->text ());
  settings.setValue ("secIdType", comboBoxFirstID->currentText ());
  settings.setValue ("regon", lineEditSecondID->text ());
  close ();
}
