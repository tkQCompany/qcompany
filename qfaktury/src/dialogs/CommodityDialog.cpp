#include "CommodityDialog.h"

/** Constructor
 */
CommodityDialog::CommodityDialog(QWidget *parent, Database *db,
                                 const QModelIndex &id_edit):
    QDialog(parent), db_(db)
{
    setupUi(this);
    init();

    if(id_edit.isValid())
    {
        setWindowTitle(trUtf8("Edytuj towar/usługę"));
        comboBoxType->setEnabled(false);

        mapper_->setCurrentIndex(id_edit.row());
        if(id_edit.row() == 0)
        {
            addSuffix(comboBoxMeasureUnit->currentText());
        }
    }
    else
    {
        db_->modelCommodity()->insertRow(db_->modelCommodity()->rowCount());
        mapper_->toLast();
        comboBoxType->setCurrentIndex(0);
        comboBoxMeasureUnit->setCurrentIndex(0);
    }
}


CommodityDialog::~CommodityDialog()
{
    db_->modelCommodity()->revertAll();
}


/** Init
 */
void CommodityDialog::init()
{
    comboBoxType->setModel(db_->modelCommodityType());
    comboBoxType->setModelColumn(CommodityTypeFields::TYPE);

    comboBoxMeasureUnit->setModel(db_->modelUnit());
    comboBoxMeasureUnit->setModelColumn(UnitFields::NAME);

    comboBoxVat->setModel(db_->modelVat());
    comboBoxVat->setInsertPolicy(QComboBox::NoInsert);

    mapper_ = new QDataWidgetMapper(this);
    mapper_->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper_->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper_->setModel(db_->modelCommodity());
    mapper_->addMapping(lineEditName, CommodityFields::NAME);
    mapper_->addMapping(lineEditAbbreviation, CommodityFields::ABBREV);
    mapper_->addMapping(lineEditPKWIU, CommodityFields::PKWIU);
    mapper_->addMapping(comboBoxType, CommodityFields::TYPE_ID);
    mapper_->addMapping(comboBoxMeasureUnit, CommodityFields::UNIT_ID);
    mapper_->addMapping(lineEditNet1, CommodityFields::NET1);
    mapper_->addMapping(lineEditNet2, CommodityFields::NET2);
    mapper_->addMapping(lineEditNet3, CommodityFields::NET3);
    mapper_->addMapping(lineEditNet4, CommodityFields::NET4);
    mapper_->addMapping(comboBoxVat, CommodityFields::VAT, "currentText");
    mapper_->addMapping(doubleSpinBoxQuantity, CommodityFields::QUANTITY);

    validator_.setBottom(0.0);
    validator_.setDecimals(2);
    lineEditNet1->setValidator(&validator_);
    lineEditNet2->setValidator(&validator_);
    lineEditNet3->setValidator(&validator_);
    lineEditNet4->setValidator(&validator_);

    connect(pushButtonOK, SIGNAL(clicked()), this, SLOT(okClick()));
    connect(pushButtonCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(toolButtonPKWIU, SIGNAL(clicked()), this, SLOT(pkwiuGet()));
    connect(comboBoxMeasureUnit, SIGNAL(currentIndexChanged(QString)), this, SLOT(addSuffix(QString)));
}

/******************** SLOTS START ***************************/


/** Slot
 *  save data to XML file and returns row for products table
 */
void CommodityDialog::okClick()
{
    if (lineEditName->text().isEmpty())
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
    doubleSpinBoxQuantity->setSuffix(QChar(' ') + suffix);
}

/******************** SLOTS END ***************************/
