#include "CommodityListDialog.h"
#include "ui_CommodityListDialog.h"

/** Constructor
 */
CommodityListDialog::CommodityListDialog(QWidget *parent, Database *db):
    QDialog(parent), ui(new Ui::CommodityListDialog), db(db), netVal(0.0)
{
    ui->setupUi(this);
    init();
}

/** Init
 */
void CommodityListDialog::init()
{
    ui->listViewCommodities->setModel(db->modelCommodity());
    ui->listViewCommodities->setModelColumn(CommodityFields::NAME);
    ui->comboBoxCommodities->setModel(db->modelCommodityType());
    ui->comboBoxCommodities->setModelColumn(CommodityTypeFields::TYPE);

    widgetMapper.setModel(db->modelCommodity());
    db->modelCommodity()->setFilter(QString("`type_id` = %1").arg(CommodityTypeData::GOODS + 1));
    widgetMapper.addMapping(ui->comboBoxCommodities, CommodityFields::TYPE_ID);
    widgetMapper.addMapping(ui->lineEditName, CommodityFields::NAME);
    widgetMapper.addMapping(ui->doubleSpinBoxPriceNet1, CommodityFields::NET1);
    widgetMapper.addMapping(ui->doubleSpinBoxPriceNet2, CommodityFields::NET2);
    widgetMapper.addMapping(ui->doubleSpinBoxPriceNet3, CommodityFields::NET3);
    widgetMapper.addMapping(ui->doubleSpinBoxPriceNet4, CommodityFields::NET4);
    widgetMapper.addMapping(ui->doubleSpinBoxAmount, CommodityFields::QUANTITY);
    //widgetMapper_.addMapping(spinBoxDiscount, CommodityFields::DISCOUNT);
    widgetMapper.toFirst();

    ui->listViewCommodities->selectionModel()->setCurrentIndex(db->modelCommodity()->index(0, 0), QItemSelectionModel::Rows);
    comboBoxChosenNetPriceChanged(ui->comboBoxChosenNetPrice->currentIndex());

    SettingsGlobal s;
    if(s.value(s.EDIT_NAME).toBool())
    {
        ui->lineEditName->setEnabled(true);
    }
    else
    {
        ui->lineEditName->setEnabled(false);
	}

	// connects
    connect(ui->pushButtonOK, SIGNAL( clicked() ), this, SLOT( doAccept()));
    connect(ui->pushButtonCancel, SIGNAL( clicked() ), this, SLOT( close()));
    connect(ui->comboBoxCommodities, SIGNAL( activated(int) ), this, SLOT( comboBoxCommoditiesChanged(int)));
    connect(ui->listViewCommodities, SIGNAL(clicked(QModelIndex)), &widgetMapper, SLOT(setCurrentModelIndex(QModelIndex)));
    connect(ui->comboBoxChosenNetPrice, SIGNAL(currentIndexChanged(int)), this, SLOT( comboBoxChosenNetPriceChanged(int) ) );
    connect(ui->spinBoxDiscount, SIGNAL( valueChanged(int) ), this, SLOT( updateNetVal() ) );
    connect(ui->doubleSpinBoxAmount, SIGNAL( valueChanged(const QString&) ), this, SLOT( updateNetVal() ) );
}


CommodityListDialog::~CommodityListDialog()
{
    db->modelCommodity()->setFilter("");
    delete ui;
}

// ***************************** SLOTS START *****************************************

/** Slot
 *  spinBox netto numbers changed
 */
void CommodityListDialog::comboBoxChosenNetPriceChanged(const int i)
{
    const QModelIndex current(ui->listViewCommodities->selectionModel()->currentIndex());
    if(current.isValid())
    {
        switch(i)
        {
        case 0:
            netVal = ui->doubleSpinBoxPriceNet1->value();
            break;
        case 1:
            netVal = ui->doubleSpinBoxPriceNet2->value();
            break;
        case 2:
            netVal = ui->doubleSpinBoxPriceNet3->value();
            break;
        case 3:
            netVal = ui->doubleSpinBoxPriceNet4->value();
            break;
        }
        updateNetVal();
    }
}

/** Slot
 *  Accept and close
 */
void CommodityListDialog::doAccept() {
    if (ui->doubleSpinBoxAmount->text().isEmpty() || ui->doubleSpinBoxAmount->value() < 0.001)
    {
        QMessageBox::information(this, qApp->applicationName(), trUtf8("Podaj ilość"), QMessageBox::Ok);
		return;
	}

    if (!ui->lineEditName->text().isEmpty())
    {
        const QModelIndex current(ui->listViewCommodities->selectionModel()->currentIndex());
        ret.discount = QString("%1").arg(ui->spinBoxDiscount->value());
        ret.id = db->modelCommodity()->data(db->modelCommodity()->index(current.row(), CommodityFields::ID)).toString();
        ret.name = db->modelCommodity()->data(db->modelCommodity()->index(current.row(), CommodityFields::NAME)).toString();
        switch(ui->comboBoxChosenNetPrice->currentIndex())
        {
        case 0:
            ret.net = db->modelCommodity()->data(db->modelCommodity()->index(current.row(), CommodityFields::NET1)).toString();
            break;
        case 1:
            ret.net = db->modelCommodity()->data(db->modelCommodity()->index(current.row(), CommodityFields::NET2)).toString();
            break;
        case 2:
            ret.net = db->modelCommodity()->data(db->modelCommodity()->index(current.row(), CommodityFields::NET3)).toString();
            break;
        case 3:
            ret.net = db->modelCommodity()->data(db->modelCommodity()->index(current.row(), CommodityFields::NET4)).toString();
            break;
        }

        ret.pkwiu = db->modelCommodity()->data(db->modelCommodity()->index(current.row(), CommodityFields::PKWIU)).toString();
        ret.quantity = QString("%1").arg(ui->doubleSpinBoxAmount->value());
        ret.type = db->modelCommodity()->data(db->modelCommodity()->index(current.row(), CommodityFields::TYPE_ID)).toString();
        ret.unit = db->modelCommodity()->data(db->modelCommodity()->index(current.row(), CommodityFields::UNIT_ID)).toString();
        ret.vat = db->modelCommodity()->data(db->modelCommodity()->index(current.row(), CommodityFields::VAT)).toString();

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
    const double totalNetPrice = netVal * ui->doubleSpinBoxAmount->value();
    const double discountedNetPrice = totalNetPrice - totalNetPrice * ui->spinBoxDiscount->value() * 0.01;

    SettingsGlobal s;
    ui->labelNetVal->setText(s.numberToString(discountedNetPrice, 'f', 2));
}



void CommodityListDialog::comboBoxCommoditiesChanged(int val)
{
    db->modelCommodity()->setFilter(QString("`type_id` = %1").arg(val + 1));
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
