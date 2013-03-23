/*
 * CustomPayment.cpp
 *
 *  Created on: Apr 4, 2009
 *      Author: moux
 */

#include "CustomPaymentDialog.h"
#include "ui_CustomPaymentDialog.h"


/*
	Kwota do zapłaty  : 554.90 zł
	Zapłacono gotówką : 254,90 zł dnia {Data bieżąca}
	Kwota zaległośći  : 300 zł 7 dni Przelew {Data Zapłaty}
*/


CustomPaymentDialog::CustomPaymentDialog(QWidget *parent):
    QDialog(parent), ui_(new Ui::CustomPaymentDialog)
{
    ui_->setupUi(this);
	init();
}

CustomPaymentDialog::~CustomPaymentDialog()
{
    delete ui_;
}

void CustomPaymentDialog::init()
{
    SettingsGlobal s;

    connect(ui_->pushButtonOK, SIGNAL(clicked()), this, SLOT(okClicked()));
    connect(ui_->pushButtonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui_->doubleSpinBoxAmount1, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxAmount1Changed(double)));
    connect(ui_->doubleSpinBoxAmount2, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxAmount2Changed(double)));

    const QStringList payments(s.value(s.PAYMENT_TYPE).toString().split("|"));
    ui_->comboBoxPayment1->insertItems(0, payments);
    const int removeLast = ui_->comboBoxPayment1->count() - 1;
    ui_->comboBoxPayment1->removeItem(removeLast);

    ui_->comboBoxPayment2->insertItems(0, payments);
    ui_->comboBoxPayment2->removeItem(removeLast);

    ui_->dateEditDayOfPayment1->setDate(QDate::currentDate());
    ui_->dateEditDayOfPayment2->setDate(QDate::currentDate());
}

void CustomPaymentDialog::setInvoiceAmount(const double &a)
{
	invoiceAmount = a;
    ui_->doubleSpinBoxAmount1->setValue(invoiceAmount);
    ui_->doubleSpinBoxAmount1->setMaximum(invoiceAmount);
    ui_->doubleSpinBoxAmount2->setMaximum(invoiceAmount);
}

bool CustomPaymentDialog::validateForm() {
    if ((ui_->doubleSpinBoxAmount1->value() == 0) ||  (ui_->doubleSpinBoxAmount2->value() == 0)) {
        QMessageBox::warning(this, qApp->applicationName(),
				trUtf8("Jedna z kwot do zapłaty jest równa 0. Wybierz inny sposób płatności lub zmień kwoty."));
		return false;
	}

	return true;
}

// ---------- SLOT START ------------

void CustomPaymentDialog::doubleSpinBoxAmount1Changed()
{
    disconnect(ui_->doubleSpinBoxAmount2, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxAmount2Changed(double)));
    ui_->doubleSpinBoxAmount2->setValue( invoiceAmount -  ui_->doubleSpinBoxAmount1->value());
    connect(ui_->doubleSpinBoxAmount2, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxAmount2Changed(double)));
}

void CustomPaymentDialog::doubleSpinBoxAmount2Changed( )
{
    disconnect(ui_->doubleSpinBoxAmount1, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxAmount1Changed(double)));
    ui_->doubleSpinBoxAmount1->setValue( invoiceAmount -  ui_->doubleSpinBoxAmount2->value());
    connect(ui_->doubleSpinBoxAmount1, SIGNAL(valueChanged(double)), this, SLOT(doubleSpinBoxAmount1Changed(double)));
}

void CustomPaymentDialog::okClicked()
{
    if (validateForm())
    {
		custPaymData = new CustomPaymData();
        custPaymData->payment1 = ui_->comboBoxPayment1->currentText();
        custPaymData->amount1 = ui_->doubleSpinBoxAmount1->value();
        custPaymData->date1 = ui_->dateEditDayOfPayment1->date();
        custPaymData->payment2 = ui_->comboBoxPayment2->currentText();
        custPaymData->amount2 = ui_->doubleSpinBoxAmount2->value();
        custPaymData->date2 = ui_->dateEditDayOfPayment2->date();
		accept();
	}
}

// ---------- SLOT END ------------
