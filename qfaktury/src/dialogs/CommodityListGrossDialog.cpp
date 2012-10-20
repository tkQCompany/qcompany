/*
 * TowaryBruttoLista.cpp
 *
 *  Created on: Apr 3, 2009
 *      Author: TPIELECH
 */

#include "CommodityListGrossDialog.h"


CommodityListGrossDialog::CommodityListGrossDialog(QWidget *parent, Database *db):
    CommodityListDialog(parent, db)
{
}



/** Slot
 *  Accept and close
 */
void CommodityListGrossDialog::doAccept()
{
    if (doubleSpinBoxAmount->text().isEmpty() || doubleSpinBoxAmount->value() < 0.001)
    {
        QMessageBox::information(this, qApp->applicationName(), trUtf8("Podaj ilość"), QMessageBox::Ok);
        return;
    }

    if (!lineEditName->text().isEmpty())
    {
//        CommodityDataList *list;
//        if (comboBoxCommodities->currentIndex() == CommodityData::GOODS)
//        {
//            list = &listGoods;
//        }
//        if (comboBoxCommodities->currentIndex() == CommodityData::SERVICES)
//        {
//            list = &listServices;
//        }
//        ret = QString("%1|%2|%3|%4|%5|%6|%7|%8|%9|%10")
//                .arg(selectedItem)
//                .arg(list->operator [](id)->postalCode)
//                .arg(list->operator [](id)->pkwiu)
//                .arg(trimZeros(doubleSpinBoxAmount->cleanText()))
//                .arg(list->operator [](id)->unit)
//                .arg(spinBoxDiscount->cleanText())
//                .arg(getPriceOfCurrent())
//                .arg(labelNetVal->text())
//                .arg(sett().numberToString(vats[selectedItem]))
//                .arg(labelGrossVal->text());
        accept();
    }
    else
    {
        QMessageBox::information(this, qApp->applicationName(), trUtf8("Wskaż towar"), QMessageBox::Ok);
    }
}

void CommodityListGrossDialog::updateNetVal()
{
    /*const QList<QListWidgetItem *> items = listWidgetCommodities->selectedItems();
    if (items.size() == 1)
    {
        QListWidgetItem *item = items[0];
        const double price = doubleSpinBoxAmount->value() * doubleSpinBoxPrice->value(); // price * quantity
        const double discount = (price * spinBoxDiscount->value()) * 0.01;
        const double wb = price - discount;
        const int sp = vats[item->text()];
        const double vat = (wb * sp) / (100 + sp);
        //QString vatStr = sett().numberToString(vat, 'f', 8);

        const double netto2 = wb - vat;
        labelGrossVal->setText(sett().numberToString(wb, 'f', 2));
        labelNetVal->setText(sett().numberToString(netto2, 'f', 2));
    }
    */
}
