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
#include "CommodityDelegate.h"

/** Constructor
 */
CommodityDialog::CommodityDialog(QWidget *parent, Database *db,
                                 const QModelIndex &id_edit):
    QDialog(parent), ui(new Ui::CommodityDialog), db(db)
{
    ui->setupUi(this);
    init_();

    if(id_edit.isValid())
    {
        setWindowTitle(trUtf8("Edytuj towar/usługę"));
        ui->comboBoxType->setEnabled(false);

        mapper.setCurrentIndex(id_edit.row());
        if(id_edit.row() == 0)
        {
            addSuffix(ui->comboBoxMeasureUnit->currentText());
        }
    }
    else
    {
        db->modelCommodity()->insertRow(db->modelCommodity()->rowCount());
        mapper.toLast();
        ui->comboBoxType->setCurrentIndex(ui->comboBoxType->findText(CommodityTypeData::name(CommodityTypeData::GOODS)));
        ui->comboBoxMeasureUnit->setCurrentIndex(ui->comboBoxMeasureUnit->findText(UnitData::name(UnitData::UNIT)));
    }
    connect(ui->pushButtonOK, SIGNAL(clicked()), this, SLOT(okClick()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->toolButtonPKWIU, SIGNAL(clicked()), this, SLOT(pkwiuGet()));
    connect(ui->comboBoxMeasureUnit, SIGNAL(currentIndexChanged(QString)), this, SLOT(addSuffix(QString)));
    connect(ui->comboBoxType, SIGNAL(currentIndexChanged(QString)), this, SLOT(servicesCantBeCounted(QString)));
}


CommodityDialog::~CommodityDialog()
{
    db->modelCommodity()->revertAll();
    delete ui;
}


/** Init
 */
void CommodityDialog::init_()
{
    ui->comboBoxType->setModel(db->modelCommodityType());
    ui->comboBoxType->setModelColumn(CommodityTypeFields::TYPE);

    ui->comboBoxMeasureUnit->setModel(db->modelUnit());
    ui->comboBoxMeasureUnit->setModelColumn(UnitFields::UNIT_NAME);

    ui->comboBoxVat->setModel(db->modelVat());
    ui->comboBoxVat->setInsertPolicy(QComboBox::NoInsert);

    mapper.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper.setItemDelegate(new CommodityDelegate(this));
    mapper.setModel(db->modelCommodity());
    mapper.addMapping(ui->lineEditName, CommodityFields::NAME);
    mapper.addMapping(ui->lineEditAbbreviation, CommodityFields::ABBREV);
    mapper.addMapping(ui->lineEditPKWIU, CommodityFields::PKWIU);
    mapper.addMapping(ui->comboBoxType, CommodityFields::TYPE_ID);
    mapper.addMapping(ui->comboBoxMeasureUnit, CommodityFields::UNIT_ID);
    mapper.addMapping(ui->lineEditNet1, CommodityFields::NET1);
    mapper.addMapping(ui->lineEditNet2, CommodityFields::NET2);
    mapper.addMapping(ui->lineEditNet3, CommodityFields::NET3);
    mapper.addMapping(ui->lineEditNet4, CommodityFields::NET4);
    mapper.addMapping(ui->comboBoxVat, CommodityFields::VAT, "currentText");
    mapper.addMapping(ui->doubleSpinBoxQuantity, CommodityFields::QUANTITY);

    validator.setBottom(0.0);
    validator.setDecimals(2);
    ui->lineEditNet1->setValidator(&validator);
    ui->lineEditNet2->setValidator(&validator);
    ui->lineEditNet3->setValidator(&validator);
    ui->lineEditNet4->setValidator(&validator);
}

/******************** SLOTS START ***************************/


/** Slot
 *  save data to XML file and returns row for products table
 */
void CommodityDialog::okClick()
{
    if (ui->lineEditName->text().isEmpty())
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Musisz podać nazwę"));
        return;
    }

    if(mapper.submit() && (db->modelCommodity()->lastError().type() == QSqlError::NoError))
    {
        accept();
    }
    else
    {
        QMessageBox::warning(this, trUtf8("Błąd w podanych danych"), trUtf8("Niekompletne lub błędne dane:\n%1")
                             .arg(db->modelCommodity()->lastError().text()));
    }
}



/** Slot
 *  Find PKWIU code on the net
 */
void CommodityDialog::pkwiuGet()
{
    QDesktopServices::openUrl(QUrl(tr("http://www.klasyfikacje.pl/")));
}


void CommodityDialog::servicesCantBeCounted(const QString &name)
{
    if(name == CommodityTypeData::name(CommodityTypeData::SERVICES))
    {
        ui->labelQuantity->setVisible(false);
        ui->doubleSpinBoxQuantity->setVisible(false);
    }
    else
    {
        ui->labelQuantity->setVisible(true);
        ui->doubleSpinBoxQuantity->setVisible(true);
    }
}


void CommodityDialog::addSuffix(const QString &suffix)
{
    ui->doubleSpinBoxQuantity->setSuffix(QChar(' ') + suffix);
}

/******************** SLOTS END ***************************/
