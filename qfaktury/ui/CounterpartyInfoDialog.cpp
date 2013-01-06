#include "CounterpartyInfoDialog.h"


/** Constructor
 */
CounterpartyInfoDialog::CounterpartyInfoDialog(QWidget *parent, Database *db, const QModelIndex &id_edit):
    QDialog(parent), db_(db)
{
    setupUi(this);
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


/** Init
 */
void CounterpartyInfoDialog::init()
{
    widgetMapper_ = new QDataWidgetMapper(this);
    widgetMapper_->setModel(db_->modelCounterparty());

    const QByteArray labelProp("text");
    widgetMapper_->addMapping(labelTypeVal, CounterpartyFields::TYPE_ID, labelProp);
    widgetMapper_->addMapping(labelNameE, CounterpartyFields::NAME, labelProp);
    widgetMapper_->addMapping(labelAddressE, CounterpartyFields::STREET, labelProp);
    widgetMapper_->addMapping(labelCityE, CounterpartyFields::LOCATION, labelProp);
    widgetMapper_->addMapping(labelTicE, CounterpartyFields::TAX_IDENT, labelProp);
    widgetMapper_->addMapping(labelAccountE, CounterpartyFields::ACCOUNT_NAME, labelProp);
    widgetMapper_->addMapping(pushButtonWWW, CounterpartyFields::WWW, labelProp);

	// connects
    connect(pushButtonOK, SIGNAL(clicked()), this, SLOT(accept()));
    connect(pushButtonWWW, SIGNAL(clicked()), this, SLOT(openCustomerWWW()));
}

// *************************** SLOTS START *************************************



/** Slot
 *  Opens the WEBSITE of the customer
 */
void CounterpartyInfoDialog::openCustomerWWW()
{
    QDesktopServices::openUrl(QUrl(pushButtonWWW->text()));
}

// *************************** SLOTS END *************************************
