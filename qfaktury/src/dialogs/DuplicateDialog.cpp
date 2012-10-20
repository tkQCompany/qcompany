/*
 * Duplikat.cpp
 *
 *  Created on: Mar 31, 2009
 *      Author: moux
 */

#include "DuplicateDialog.h"

// constructor
DuplicateDialog::DuplicateDialog(QWidget *parent, Database *db):
    InvoiceDialog(parent, db)
{
}

void DuplicateDialog::init() {
    QLabel *labelDupDate = new QLabel(this);
    labelDupDate->setText(trUtf8("Data duplikatu:"));
    labelDupDate->setAlignment(Qt::AlignRight);
    addDataLabels->addWidget(labelDupDate);

    duplicateDate = new QDateEdit(this);
    duplicateDate->setObjectName(QString::fromUtf8("duplicateDate"));
    duplicateDate->setCalendarPopup(true);
    duplicateDate->setDisplayFormat(sett().getDateFormat());
    duplicateDate->setDate(QDate::currentDate());
    addData->addWidget(duplicateDate);

    setIsEditAllowed(false); // since it's a duplikat
    pushButtonSave->setEnabled(false);
}

/** Adds duplicate requirements
 */
void DuplicateDialog::makeInvoiceHeader(const bool sellDate, const bool breakPage, const bool original) {

    QString breakPageStr = "class=\"page_break\"";
    if (breakPage == false) breakPageStr = "";

    invStrList += "<table comment=\"headar table\" width=\"100%\" border=\"0\"" + breakPageStr + ">";
    invStrList += "<tr>";
    invStrList += "<td width=\"60%\" align=\"center\" valign=\"bottom\">";
    invStrList += "<span class=\"stamp\">";
    QString logo = sett().value("logo").toString();
    if (logo != "") {
        invStrList += "<img src=\"" + logo + "\" width=\"100\" " + " height=\"100\"+ >";
    } else {
        invStrList += trUtf8("Pieczęć wystawcy");
    }
    invStrList += "</span>";
    invStrList += "</td>";

    invStrList += "<td align=\"right\">";
    invStrList += "<span style=\"font-size:12pt; font-weight:600\">";
    invStrList += InvoiceTypeData::InvoiceTypeToString(invType) + "<br/>";
    invStrList += trUtf8("Nr: ") + lineEditInvNumber->text() + "<br></span>";
    invStrList += "<span style=\"font-size:11pt; font-weight:600\">";
    invStrList += trUtf8("Duplikat z dnia: ") + "<b>" +
            duplicateDate->date().toString(sett().getDateFormat()) +  "</b></span><br>";
    invStrList += "<span class=\"dates\">" + trUtf8("Data wystawienia: ")
            + dateEditDateOfIssuance->date().toString(sett().getDateFormat()) + "<br>";

    if (sellDate)
        invStrList += trUtf8("Data sprzedaży: ")
                + dateEditDateOfSell->date().toString(sett().getDateFormat())
                + "<br>";
    invStrList += "</span>";
    invStrList += "</td><td width=\"3%\">&nbsp;</td>";

    invStrList += "</tr>";
    invStrList += "<tr>";

    invStrList += "<td colspan=\"2\" align=\"right\" valign=\"top\"><br>";
    if (original) {
        invStrList += trUtf8("ORYGINAŁ");
    } else {
        invStrList += trUtf8("KOPIA");
    }
    invStrList += "<br></td><td width=\"3%\">&nbsp;</td>";
    invStrList += "</tr>";
    invStrList += "</table>";
    invStrList += "<hr>";
    invStrList += "</td></tr>";
}


/** Slot
 *  canQuit slot
 */
void DuplicateDialog::canQuit() {
    reject();
}

