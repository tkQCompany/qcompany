#include "CommodityListDialog.h"

/** Constructor
 */
CommodityListDialog::CommodityListDialog(QWidget *parent, Database *db):
    QDialog(parent), db_(db), netVal_(0.0)
{
    setupUi(this);
    init();
}

/** Init
 */
void CommodityListDialog::init()
{
    listViewCommodities->setModel(db_->modelCommodity());
    listViewCommodities->setModelColumn(CommodityFields::NAME);
    comboBoxCommodities->setModel(db_->modelCommodityType());
    comboBoxCommodities->setModelColumn(CommodityTypeFields::TYPE);

    widgetMapper_.setModel(db_->modelCommodity());
    db_->modelCommodity()->setFilter(QString("`type_id` = %1").arg(CommodityTypeData::GOODS));
    widgetMapper_.addMapping(comboBoxCommodities, CommodityFields::TYPE_ID);
    widgetMapper_.addMapping(lineEditName, CommodityFields::NAME);
    widgetMapper_.addMapping(doubleSpinBoxPriceNet1, CommodityFields::NET1);
    widgetMapper_.addMapping(doubleSpinBoxPriceNet2, CommodityFields::NET2);
    widgetMapper_.addMapping(doubleSpinBoxPriceNet3, CommodityFields::NET3);
    widgetMapper_.addMapping(doubleSpinBoxPriceNet4, CommodityFields::NET4);
    widgetMapper_.addMapping(doubleSpinBoxAmount, CommodityFields::QUANTITY);
    //widgetMapper_.addMapping(spinBoxDiscount, CommodityFields::DISCOUNT);
    widgetMapper_.toFirst();

    listViewCommodities->selectionModel()->setCurrentIndex(db_->modelCommodity()->index(0, 0), QItemSelectionModel::Rows);
    comboBoxChosenNetPriceChanged(comboBoxChosenNetPrice->currentIndex());

	if (sett().value("editName", false).toBool()) {
        lineEditName->setEnabled(true);
	} else {
        lineEditName->setEnabled(false);
	}

	// connects
    connect(pushButtonOK, SIGNAL( clicked() ), this, SLOT( doAccept()));
    connect(pushButtonCancel, SIGNAL( clicked() ), this, SLOT( close()));
    connect(comboBoxCommodities, SIGNAL( activated(int) ), this, SLOT( comboBoxCommoditiesChanged(int)));
    connect(listViewCommodities, SIGNAL(clicked(QModelIndex)), &widgetMapper_, SLOT(setCurrentModelIndex(QModelIndex)));
    connect(comboBoxChosenNetPrice, SIGNAL(currentIndexChanged(int)), this, SLOT( comboBoxChosenNetPriceChanged(int) ) );
    connect(spinBoxDiscount, SIGNAL( valueChanged(int) ), this, SLOT( updateNetVal() ) );
    connect(doubleSpinBoxAmount, SIGNAL( valueChanged(const QString&) ), this, SLOT( updateNetVal() ) );
}


CommodityListDialog::~CommodityListDialog()
{
    db_->modelCommodity()->setFilter("");
}

// ***************************** SLOTS START *****************************************

/** Slot
 *  spinBox netto numbers changed
 */
void CommodityListDialog::comboBoxChosenNetPriceChanged(const int i)
{
    const QModelIndex current(listViewCommodities->selectionModel()->currentIndex());
    if(current.isValid())
    {
        switch(i)
        {
        case 0:
            netVal_ = doubleSpinBoxPriceNet1->value();
            break;
        case 1:
            netVal_ = doubleSpinBoxPriceNet2->value();
            break;
        case 2:
            netVal_ = doubleSpinBoxPriceNet3->value();
            break;
        case 3:
            netVal_ = doubleSpinBoxPriceNet4->value();
            break;
        }
        updateNetVal();
    }
}

/** Slot
 *  Accept and close
 */
void CommodityListDialog::doAccept() {
    if (doubleSpinBoxAmount->text().isEmpty() || doubleSpinBoxAmount->value() < 0.001)
    {
        QMessageBox::information(this, qApp->applicationName(), trUtf8("Podaj ilość"), QMessageBox::Ok);
		return;
	}

    if (!lineEditName->text().isEmpty())
    {
        const QModelIndex current(listViewCommodities->selectionModel()->currentIndex());
        ret.discount = QString("%1").arg(spinBoxDiscount->value());
        ret.id = db_->modelCommodity()->data(db_->modelCommodity()->index(current.row(), CommodityFields::ID)).toString();
        ret.name = db_->modelCommodity()->data(db_->modelCommodity()->index(current.row(), CommodityFields::NAME)).toString();
        switch(comboBoxChosenNetPrice->currentIndex())
        {
        case 0:
            ret.net = db_->modelCommodity()->data(db_->modelCommodity()->index(current.row(), CommodityFields::NET1)).toString();
            break;
        case 1:
            ret.net = db_->modelCommodity()->data(db_->modelCommodity()->index(current.row(), CommodityFields::NET2)).toString();
            break;
        case 2:
            ret.net = db_->modelCommodity()->data(db_->modelCommodity()->index(current.row(), CommodityFields::NET3)).toString();
            break;
        case 3:
            ret.net = db_->modelCommodity()->data(db_->modelCommodity()->index(current.row(), CommodityFields::NET4)).toString();
            break;
        }

        ret.pkwiu = db_->modelCommodity()->data(db_->modelCommodity()->index(current.row(), CommodityFields::PKWIU)).toString();
        ret.quantity = QString("%1").arg(doubleSpinBoxAmount->value());
        ret.type = db_->modelCommodity()->data(db_->modelCommodity()->index(current.row(), CommodityFields::TYPE_ID)).toString();
        ret.unit = db_->modelCommodity()->data(db_->modelCommodity()->index(current.row(), CommodityFields::UNIT_ID)).toString();
        ret.vat = db_->modelCommodity()->data(db_->modelCommodity()->index(current.row(), CommodityFields::VAT)).toString();

		accept();
    }
    else
    {
        QMessageBox::information(this, qApp->applicationName(), trUtf8("Wskaż towar"), QMessageBox::Ok);
	}
}



/** Slot
 *  Calulate Netto
 */
void CommodityListDialog::updateNetVal()
{
    const double totalNetPrice = netVal_ * doubleSpinBoxAmount->value();
    const double discountedNetPrice = totalNetPrice - totalNetPrice * spinBoxDiscount->value() * 0.01;
    labelNetVal->setText(sett().numberToString(discountedNetPrice, 'f', 2));
}



void CommodityListDialog::comboBoxCommoditiesChanged(int val)
{
    db_->modelCommodity()->setFilter(QString("`type_id` = %1").arg(val + 1));
}


// ***************************** SLOTS END *****************************************



///** Remove unnecessary zeros 1,000 = 1
// */
//QString CommodityListDialog::trimZeros(const QString &in)
//{
//	// code to remove unncessery zeros
//    const QStringList quan(in.split(sett().getDecimalPointStr()));
//    QString quantity(in);
//    if (quan[1].compare("000") == 0)
//    {
//		quantity = quan[0];
//	}
//	return quantity;
//}
