/*
 * CustomPayment.cpp
 *
 *  Created on: Apr 4, 2009
 *      Author: moux
 */

#include "CustomPaymentDialog.h"


/*
	Kwota do zapłaty  : 554.90 zł
	Zapłacono gotówką : 254,90 zł dnia {Data bieżąca}
	Kwota zaległośći  : 300 zł 7 dni Przelew {Data Zapłaty}
*/


CustomPaymentDialog::CustomPaymentDialog(QWidget *parent): QDialog(parent) {
	setupUi(this);
	init();
}

CustomPaymentDialog::~CustomPaymentDialog() {
}

void CustomPaymentDialog::init()
{
    SettingsGlobal s;

    connect(pushButtonOK, SIGNAL(clicked()), this, SLOT(okClicked()));
    connect(pushButtonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect( doubleSpinBoxAmount1, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxAmount1Changed(double)));
    connect( doubleSpinBoxAmount2, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxAmount2Changed(double)));

    const QStringList payments(s.value(s.keyName(s.PAYMENT_TYPE)).toString().split("|"));
    comboBoxPayment1->insertItems(0, payments);
    const int removeLast = comboBoxPayment1->count() - 1;
    comboBoxPayment1->removeItem(removeLast);

    comboBoxPayment2->insertItems(0, payments);
    comboBoxPayment2->removeItem(removeLast);

    dateEditDayOfPayment1->setDate(QDate::currentDate());
    dateEditDayOfPayment2->setDate(QDate::currentDate());
}

void CustomPaymentDialog::setInvoiceAmount(const double &a)
{
	invoiceAmount = a;
    doubleSpinBoxAmount1->setValue(invoiceAmount);
    doubleSpinBoxAmount1->setMaximum(invoiceAmount);
    doubleSpinBoxAmount2->setMaximum(invoiceAmount);
}

bool CustomPaymentDialog::validateForm() {
    if ((doubleSpinBoxAmount1->value() == 0) ||  (doubleSpinBoxAmount2->value() == 0)) {
        QMessageBox::critical(0, qApp->applicationName(),
				trUtf8("Jedna z kwot do zapłaty jest równa 0. Wybierz inny sposób płatności lub zmień kwoty."));
		return false;
	}

	return true;
}

// ---------- SLOT START ------------

void CustomPaymentDialog::doubleSpinBoxAmount1Changed()
{
    disconnect( doubleSpinBoxAmount2, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxAmount2Changed(double)));
    doubleSpinBoxAmount2->setValue( invoiceAmount -  doubleSpinBoxAmount1->value());
    connect( doubleSpinBoxAmount2, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxAmount2Changed(double)));
}

void CustomPaymentDialog::doubleSpinBoxAmount2Changed( )
{
    disconnect( doubleSpinBoxAmount1, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxAmount1Changed(double)));
    doubleSpinBoxAmount1->setValue( invoiceAmount -  doubleSpinBoxAmount2->value());
    connect( doubleSpinBoxAmount1, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxAmount1Changed(double)));
}

void CustomPaymentDialog::okClicked()
{
    if (validateForm())
    {
		custPaymData = new CustomPaymData();
        custPaymData->payment1 = comboBoxPayment1->currentText();
        custPaymData->amount1 = doubleSpinBoxAmount1->value();
        custPaymData->date1 = dateEditDayOfPayment1->date();
        custPaymData->payment2 = comboBoxPayment2->currentText();
        custPaymData->amount2 = doubleSpinBoxAmount2->value();
        custPaymData->date2 = dateEditDayOfPayment2->date();
		accept();
	}
}

// ---------- SLOT END ------------
