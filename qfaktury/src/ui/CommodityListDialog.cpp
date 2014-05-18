#include <QMessageBox>
#include <QDataWidgetMapper>

#include "CommodityListDialog.h"
#include "ui_CommodityListDialog.h"
#include "Database.h"
#include "ModelCommodity.h"
#include "ModelCommodityType.h"
#include "CommodityData.h"
#include "CommodityTypeData.h"
#include "SettingsGlobal.h"


CommodityListDialog::CommodityListDialog(QWidget *parent, Database *db):
    QDialog(parent), ui(new Ui::CommodityListDialog), db(db), widgetMapper(new QDataWidgetMapper)
{
    ui->setupUi(this);
    init();
}


void CommodityListDialog::init()
{
    ui->listViewCommodities->setModel(db->modelCommodity());
    ui->listViewCommodities->setModelColumn(CommodityFields::NAME);
    ui->comboBoxCommodities->setModel(db->modelCommodityType());
    ui->comboBoxCommodities->setModelColumn(CommodityTypeFields::TYPE);

    widgetMapper->setModel(db->modelCommodity());
    comboBoxCommoditiesChanged(CommodityTypeData::GOODS);
    widgetMapper->addMapping(ui->comboBoxCommodities, CommodityFields::TYPE_ID);
    widgetMapper->addMapping(ui->lineEditName, CommodityFields::NAME);
    widgetMapper->addMapping(ui->lineEditPriceNet1, CommodityFields::NET1);
    widgetMapper->addMapping(ui->lineEditPriceNet2, CommodityFields::NET2);
    widgetMapper->addMapping(ui->lineEditPriceNet3, CommodityFields::NET3);
    widgetMapper->addMapping(ui->lineEditPriceNet4, CommodityFields::NET4);
    widgetMapper->toFirst();
    ui->doubleSpinBoxAmount->setValue(0.0);

    ui->listViewCommodities->selectionModel()->setCurrentIndex(db->modelCommodity()->index(0, 0), QItemSelectionModel::Rows);
    comboBoxChosenNetPriceChanged(ui->comboBoxChosenNetPrice->currentIndex());

    SettingsGlobal s;    
    ui->lineEditName->setEnabled(s.value(s.EDIT_NAME).toBool());

	// connects
    connect(ui->pushButtonOK, SIGNAL( clicked() ), this, SLOT( doAccept()));
    connect(ui->pushButtonCancel, SIGNAL( clicked() ), this, SLOT( close()));
    connect(ui->comboBoxCommodities, SIGNAL( activated(int) ), this, SLOT( comboBoxCommoditiesChanged(int)));
    connect(ui->listViewCommodities, SIGNAL(clicked(QModelIndex)), widgetMapper, SLOT(setCurrentModelIndex(QModelIndex)));
    connect(ui->comboBoxChosenNetPrice, SIGNAL(currentIndexChanged(int)), this, SLOT( comboBoxChosenNetPriceChanged(int) ) );
    connect(ui->spinBoxDiscount, SIGNAL( valueChanged(int) ), this, SLOT( updateNetVal() ) );
    connect(ui->doubleSpinBoxAmount, SIGNAL( valueChanged(const QString&) ), this, SLOT( updateNetVal() ) );
    connect(ui->listViewCommodities, SIGNAL(clicked(QModelIndex)), this, SLOT(updateNetVal()));

    validator.setBottom(0.0);
    validator.setDecimals(2);
    ui->lineEditPriceNet1->setValidator(&validator);
    ui->lineEditPriceNet2->setValidator(&validator);
    ui->lineEditPriceNet3->setValidator(&validator);
    ui->lineEditPriceNet4->setValidator(&validator);
}


CommodityListDialog::~CommodityListDialog()
{
    delete widgetMapper;
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
            netVal.setValue(ui->lineEditPriceNet1->text().remove(' '));
            break;
        case 1:
            netVal.setValue(ui->lineEditPriceNet2->text().remove(' '));
            break;
        case 2:
            netVal.setValue(ui->lineEditPriceNet3->text().remove(' '));
            break;
        case 3:
            netVal.setValue(ui->lineEditPriceNet4->text().remove(' '));
            break;
        default:
            qDebug("Unexpected value in a switch() in CommodityListDialog::comboBoxChosenNetPriceChanged()");
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
        const qlonglong id_commodity = db->modelCommodity()->data(db->modelCommodity()->index(current.row(), CommodityFields::ID_COMMODITY)).toLongLong();
        const DecVal amountOnStock(db->modelCommodity()->amount(id_commodity));
        const QString currentCommodityTypeStr(db->modelCommodity()->data(db->modelCommodity()->index(current.row(), CommodityFields::TYPE_ID)).toString());
        const DecVal demandedAmount(ui->doubleSpinBoxAmount->value());

        if( (amountOnStock >= demandedAmount)  || (currentCommodityTypeStr == CommodityTypeData::name(CommodityTypeData::SERVICES)))
        {
            ret.discount = DecVal(ui->spinBoxDiscount->value());
            ret.id = id_commodity;
            ret.name = db->modelCommodity()->data(db->modelCommodity()->index(current.row(), CommodityFields::NAME)).toString();

            CommodityFields::Field netIndex;
            switch(ui->comboBoxChosenNetPrice->currentIndex())
            {
            case 0:
                netIndex = CommodityFields::NET1;
                break;
            case 1:
                netIndex = CommodityFields::NET2;
                break;
            case 2:
                netIndex = CommodityFields::NET3;
                break;
            case 3:
                netIndex = CommodityFields::NET4;
                break;
            }
            ret.net = Money_t(db->modelCommodity()->data(db->modelCommodity()->index(current.row(), netIndex)).toString());

            ret.pkwiu = db->modelCommodity()->data(db->modelCommodity()->index(current.row(), CommodityFields::PKWIU)).toString();
            ret.quantity = demandedAmount;
            ret.type = currentCommodityTypeStr;
            ret.unit = db->modelCommodity()->data(db->modelCommodity()->index(current.row(), CommodityFields::UNIT_ID)).toString();
            ret.vat = DecVal(db->modelCommodity()->data(db->modelCommodity()->index(current.row(), CommodityFields::VAT)).toDouble());

            accept();
        }
        else
        {
            QMessageBox::information(this, qApp->applicationName(),
                                     trUtf8("Nie ma wystarczającej ilości towaru na stanie. Ilość towaru: %1")
                                     .arg(db->modelCommodity()->amount(db->modelCommodity()->data(db->modelCommodity()->index(current.row(), CommodityFields::ID_COMMODITY)).toLongLong()).toString()), QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this, qApp->applicationName(), trUtf8("Wskaż towar"), QMessageBox::Ok);
	}
}



void CommodityListDialog::updateNetVal()
{
    const Money_t totalNetPrice = netVal * DecVal(ui->doubleSpinBoxAmount->value());
    const DecVal onePercent(DecVal(1)/DecVal(100));
    const Money_t discountedNetPrice = totalNetPrice - (totalNetPrice * onePercent * DecVal(ui->spinBoxDiscount->value()));
    const int precision = 2;
    ui->labelNetVal->setText(discountedNetPrice.toString(precision));
}



void CommodityListDialog::comboBoxCommoditiesChanged(int val)
{
    db->modelCommodity()->setFilter(QString("`type_id` = %1").arg(val + 1));
}
