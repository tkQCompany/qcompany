#include "CounterpartyInfoDialog.h"
#include "ui_CounterpartyInfoDialog.h"


/** Constructor
 */
CounterpartyInfoDialog::CounterpartyInfoDialog(QWidget *parent, Database *db, const QModelIndex &id_edit):
    QDialog(parent), ui_(new Ui::CounterpartyInfoDialog), db_(db)
{
    ui_->setupUi(this);
    init();

    if(id_edit.isValid())
    {
        widgetMapper_->setCurrentIndex(id_edit.row());
    }
    else
    {
        widgetMapper_->toFirst();
    }
}


CounterpartyInfoDialog::~CounterpartyInfoDialog()
{
    delete ui_;
}


/** Init
 */
void CounterpartyInfoDialog::init()
{
    widgetMapper_ = new QDataWidgetMapper(this);
    widgetMapper_->setModel(db_->modelCounterparty());

    const QByteArray labelProp("text");
    widgetMapper_->addMapping(ui_->labelTypeVal, CounterpartyFields::TYPE_ID, labelProp);
    widgetMapper_->addMapping(ui_->labelNameE, CounterpartyFields::NAME, labelProp);
    widgetMapper_->addMapping(ui_->labelAddressE, CounterpartyFields::STREET, labelProp);
    widgetMapper_->addMapping(ui_->labelCityE, CounterpartyFields::LOCATION, labelProp);
    widgetMapper_->addMapping(ui_->labelTicE, CounterpartyFields::TAX_IDENT, labelProp);
    widgetMapper_->addMapping(ui_->labelAccountE, CounterpartyFields::ACCOUNT_NAME, labelProp);
    widgetMapper_->addMapping(ui_->pushButtonWWW, CounterpartyFields::WWW, labelProp);

	// connects
    connect(ui_->pushButtonOK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui_->pushButtonWWW, SIGNAL(clicked()), this, SLOT(openCustomerWWW()));
}

// *************************** SLOTS START *************************************



/** Slot
 *  Opens the WEBSITE of the customer
 */
void CounterpartyInfoDialog::openCustomerWWW()
{
    QDesktopServices::openUrl(QUrl(ui_->pushButtonWWW->text()));
}

// *************************** SLOTS END *************************************
