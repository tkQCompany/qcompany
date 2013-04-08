#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>
#include <QSqlError>
#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>

#include "CommodityDialog.h"
#include "ui_CommodityDialog.h"
#include "Database.h"
#include "ModelCommodity.h"
#include "ModelCommodityType.h"
#include "CommodityTypeData.h"
#include "ModelUnit.h"
#include "ModelCountry.h"
#include "ModelVat.h"
#include "UnitData.h"
#include "CommodityData.h"

/** Constructor
 */
CommodityDialog::CommodityDialog(QWidget *parent, Database *db,
                                 const QModelIndex &id_edit):
    QDialog(parent), ui_(new Ui::CommodityDialog), db_(db)
{
    ui_->setupUi(this);
    init();

    if(id_edit.isValid())
    {
        setWindowTitle(trUtf8("Edytuj towar/usługę"));
        ui_->comboBoxType->setEnabled(false);

        mapper_->setCurrentIndex(id_edit.row());
        if(id_edit.row() == 0)
        {
            addSuffix(ui_->comboBoxMeasureUnit->currentText());
        }
    }
    else
    {
        db_->modelCommodity()->insertRow(db_->modelCommodity()->rowCount());
        mapper_->toLast();
        ui_->comboBoxType->setCurrentIndex(0);
        ui_->comboBoxMeasureUnit->setCurrentIndex(0);
    }
}


CommodityDialog::~CommodityDialog()
{
    db_->modelCommodity()->revertAll();
    delete ui_;
}


/** Init
 */
void CommodityDialog::init()
{
    ui_->comboBoxType->setModel(db_->modelCommodityType());
    ui_->comboBoxType->setModelColumn(CommodityTypeFields::TYPE);

    ui_->comboBoxMeasureUnit->setModel(db_->modelUnit());
    ui_->comboBoxMeasureUnit->setModelColumn(UnitFields::NAME);

    ui_->comboBoxVat->setModel(db_->modelVat());
    ui_->comboBoxVat->setInsertPolicy(QComboBox::NoInsert);

    mapper_ = new QDataWidgetMapper(this);
    mapper_->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper_->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper_->setModel(db_->modelCommodity());
    mapper_->addMapping(ui_->lineEditName, CommodityFields::NAME);
    mapper_->addMapping(ui_->lineEditAbbreviation, CommodityFields::ABBREV);
    mapper_->addMapping(ui_->lineEditPKWIU, CommodityFields::PKWIU);
    mapper_->addMapping(ui_->comboBoxType, CommodityFields::TYPE_ID);
    mapper_->addMapping(ui_->comboBoxMeasureUnit, CommodityFields::UNIT_ID);
    mapper_->addMapping(ui_->lineEditNet1, CommodityFields::NET1);
    mapper_->addMapping(ui_->lineEditNet2, CommodityFields::NET2);
    mapper_->addMapping(ui_->lineEditNet3, CommodityFields::NET3);
    mapper_->addMapping(ui_->lineEditNet4, CommodityFields::NET4);
    mapper_->addMapping(ui_->comboBoxVat, CommodityFields::VAT, "currentText");
    mapper_->addMapping(ui_->doubleSpinBoxQuantity, CommodityFields::QUANTITY);

    validator_.setBottom(0.0);
    validator_.setDecimals(2);
    ui_->lineEditNet1->setValidator(&validator_);
    ui_->lineEditNet2->setValidator(&validator_);
    ui_->lineEditNet3->setValidator(&validator_);
    ui_->lineEditNet4->setValidator(&validator_);

    connect(ui_->pushButtonOK, SIGNAL(clicked()), this, SLOT(okClick()));
    connect(ui_->pushButtonCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui_->toolButtonPKWIU, SIGNAL(clicked()), this, SLOT(pkwiuGet()));
    connect(ui_->comboBoxMeasureUnit, SIGNAL(currentIndexChanged(QString)), this, SLOT(addSuffix(QString)));
}

/******************** SLOTS START ***************************/


/** Slot
 *  save data to XML file and returns row for products table
 */
void CommodityDialog::okClick()
{
    if (ui_->lineEditName->text().isEmpty())
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Musisz podać nazwę"));
        return;
    }

    if(mapper_->submit())
    {
        accept();
    }
    else
    {
        QMessageBox::warning(this, trUtf8("Błąd w podanych danych"), trUtf8("Niekompletne lub błędne dane:\n%1")
                             .arg(db_->modelCommodity()->lastError().text()));
    }
}



/** Slot
 *  Find PKWIU code on the net
 */
void CommodityDialog::pkwiuGet()
{
    QDesktopServices::openUrl(QUrl(tr("http://www.klasyfikacje.pl/")));
}


void CommodityDialog::addSuffix(const QString &suffix)
{
    ui_->doubleSpinBoxQuantity->setSuffix(QChar(' ') + suffix);
}

/******************** SLOTS END ***************************/
