/*
 * Duplikat.cpp
 *
 *  Created on: Mar 31, 2009
 *      Author: moux
 */

#include "DuplicateDialog.h"
#include "ui_InvoiceDialog.h"
#include "InvoiceDialogImpl.h"

class DuplicateDialog::DuplicateDialogImpl: public InvoiceDialogImpl
{
public:
    DuplicateDialogImpl(QWidget *parent, Database *database) :
        InvoiceDialogImpl(parent, database) {}
};

DuplicateDialog::DuplicateDialog(QWidget *parent, Database *db, InvoiceTypeData::Type invoiceType, const QModelIndex &idInvoice):
    InvoiceDialog(parent, db, invoiceType, idInvoice, false), pImpl_(new DuplicateDialogImpl(parent, db))
{
    setPImpl(pImpl_);
    init_();
}


DuplicateDialog::~DuplicateDialog()
{
    delete pImpl_;
}

void DuplicateDialog::init_()
{
    SettingsGlobal s;

    QLabel *labelDupDate = new QLabel(this);
    labelDupDate->setText(trUtf8("Data duplikatu:"));
    labelDupDate->setAlignment(Qt::AlignRight);
    //addDataLabels->addWidget(labelDupDate);

    duplicateDate_ = new QDateEdit(this);
    duplicateDate_->setObjectName(QString::fromUtf8("duplicateDate"));
    duplicateDate_->setCalendarPopup(true);
    duplicateDate_->setDisplayFormat(s.dateFormatExternal());
    duplicateDate_->setDate(QDate::currentDate());
    //addData->addWidget(duplicateDate);

    //setIsEditAllowed(false); // since it's a duplikat
    pImpl_->ui->pushButtonSave->setEnabled(false);
}

/** Adds duplicate requirements
 */
//void DuplicateDialog::makeInvoiceHeader(const bool sellDate, const bool breakPage, const bool original) {

//    SettingsGlobal s;
//    QString breakPageStr = "class=\"page_break\"";
//    if (breakPage == false) breakPageStr = "";

//    invStrList += "<table comment=\"headar table\" width=\"100%\" border=\"0\"" + breakPageStr + ">";
//    invStrList += "<tr>";
//    invStrList += "<td width=\"60%\" align=\"center\" valign=\"bottom\">";
//    invStrList += "<span class=\"stamp\">";
//    const QString logo(s.value(s.LOGO)).toString());
//    if (!logo.isEmpty())
//    {
//        invStrList += "<img src=\"" + logo + "\" width=\"100\" " + " height=\"100\"+ >";
//    } else {
//        invStrList += trUtf8("Pieczęć wystawcy");
//    }
//    invStrList += "</span>";
//    invStrList += "</td>";

//    invStrList += "<td align=\"right\">";
//    invStrList += "<span style=\"font-size:12pt; font-weight:600\">";
//    invStrList += InvoiceTypeData::InvoiceTypeToString(InvoiceTypeData::VAT) + "<br/>";
//    invStrList += trUtf8("Nr: ") + lineEditInvNumber->text() + "<br></span>";
//    invStrList += "<span style=\"font-size:11pt; font-weight:600\">";
//    invStrList += trUtf8("Duplikat z dnia: ") + "<b>" +
//            duplicateDate->date().toString(s.getDateFormat()) +  "</b></span><br>";
//    invStrList += "<span class=\"dates\">" + trUtf8("Data wystawienia: ")
//            + dateEditDateOfIssuance->date().toString(s.getDateFormat()) + "<br>";

//    if (sellDate)
//        invStrList += trUtf8("Data sprzedaży: ")
//                + dateEditDateOfSell->date().toString(s.getDateFormat())
//                + "<br>";
//    invStrList += "</span>";
//    invStrList += "</td><td width=\"3%\">&nbsp;</td>";

//    invStrList += "</tr>";
//    invStrList += "<tr>";

//    invStrList += "<td colspan=\"2\" align=\"right\" valign=\"top\"><br>";
//    if (original) {
//        invStrList += trUtf8("ORYGINAŁ");
//    } else {
//        invStrList += trUtf8("KOPIA");
//    }
//    invStrList += "<br></td><td width=\"3%\">&nbsp;</td>";
//    invStrList += "</tr>";
//    invStrList += "</table>";
//    invStrList += "<hr>";
//    invStrList += "</td></tr>";
//}


/** Slot
 *  canQuit slot
 */
void DuplicateDialog::canQuit() {
    reject();
}

