#include <QSqlRelationalDelegate>
#include <QSqlError>
#include <QWebView>
#include <QTextStream>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QTranslator>
#include <QFile>
#include <QMessageBox>

#include "InvoiceDialogImpl.h"
#include "ui_InvoiceDialog.h"
#include "Database.h"
#include "CounterpartyInfoDialog.h"
#include "CommodityDialog.h"
#include "CommodityListDialog.h"
#include "Money_t.h"
#include "CustomPaymentDialog.h"
#include "CounterpartyDialog.h"
#include "ModelInvoice.h"
#include "ModelCounterparty.h"
#include "SettingsGlobal.h"
#include "CounterpartyData.h"
#include "ModelInvoiceType.h"
#include "ModelPaymentType.h"
#include "ModelCurrency.h"
#include "ModelCommodity.h"
#include "ModelVat.h"
#include "PaymentTypeData.h"
#include "CurrencyData.h"
#include "CustomPaymData.h"
#include "InvoiceComposer.h"


InvoiceDialogImpl::InvoiceDialogImpl(QWidget *parent, Database *database)
    : QObject(parent), parent_((QDialog*)parent), ui(new Ui::InvoiceDialog()),
      db(database), custPaymDataPtr(0), isLoaded_(false)
{
}

InvoiceDialogImpl::~InvoiceDialogImpl()
{
    delete ui;
}


/**
 * @brief
 *
 * @param commodities
 */
void InvoiceDialogImpl::tableCommodityFill(const QList<CommodityVisualData> &commodities)
{
    ui->tableWidgetCommodities->setRowCount(commodities.size());
    for(int row = 0; row < commodities.size(); ++row)
    {
        tableCommoditySetItems(commodities.at(row), row);
    }
}


void InvoiceDialogImpl::tableCommoditySetItems(const CommodityVisualData &cvd, const int rowNum)
{
    const int precision = 2;
    const int precisionQuantity = 3;
    ui->tableWidgetCommodities->setItem(rowNum, CommodityVisualFields::ID, new QTableWidgetItem(QString("%1").arg(cvd.ID())));
    ui->tableWidgetCommodities->setItem(rowNum, CommodityVisualFields::NAME, new QTableWidgetItem(cvd.name()));
    ui->tableWidgetCommodities->setItem(rowNum, CommodityVisualFields::QUANTITY, new QTableWidgetItem(cvd.quantity().toString(precisionQuantity)));
    ui->tableWidgetCommodities->setItem(rowNum, CommodityVisualFields::UNIT, new QTableWidgetItem(cvd.unit()));
    ui->tableWidgetCommodities->setItem(rowNum, CommodityVisualFields::PKWIU, new QTableWidgetItem(cvd.pkwiu()));
    ui->tableWidgetCommodities->setItem(rowNum, CommodityVisualFields::NET, new QTableWidgetItem(cvd.net().toString(precision)));
    ui->tableWidgetCommodities->setItem(rowNum, CommodityVisualFields::VAT, new QTableWidgetItem(DecVal::removeTrailingZeros(cvd.vat().toString(precision))));
    ui->tableWidgetCommodities->setItem(rowNum, CommodityVisualFields::TYPE, new QTableWidgetItem(cvd.type()));
    ui->tableWidgetCommodities->setItem(rowNum, CommodityVisualFields::DISCOUNT, new QTableWidgetItem(cvd.discount().toString(precision)));
}


/**
 * @brief
 *
 * @return QList<CommodityVisualData>
 */
QList<CommodityVisualData> InvoiceDialogImpl::getCommoditiesVisualData() const
{
    QList<CommodityVisualData> ret;

    for(int r = 0; r < ui->tableWidgetCommodities->rowCount(); ++r)
    {
        CommodityVisualData d;
        d.setID(ui->tableWidgetCommodities->item(r, CommodityVisualFields::ID)->data(Qt::DisplayRole).toLongLong());
        d.setName(ui->tableWidgetCommodities->item(r, CommodityVisualFields::NAME)->data(Qt::DisplayRole).toString());
        d.setQuantity(DecVal(ui->tableWidgetCommodities->item(r, CommodityVisualFields::QUANTITY)->data(Qt::DisplayRole).toString()));
        d.setUnit(ui->tableWidgetCommodities->item(r, CommodityVisualFields::UNIT)->data(Qt::DisplayRole).toString());
        d.setPkwiu(ui->tableWidgetCommodities->item(r, CommodityVisualFields::PKWIU)->data(Qt::DisplayRole).toString());
        d.setNet(Money_t(ui->tableWidgetCommodities->item(r, CommodityVisualFields::NET)->data(Qt::DisplayRole).toString()));
        d.setVat(DecVal(ui->tableWidgetCommodities->item(r, CommodityVisualFields::VAT)->data(Qt::DisplayRole).toString()));
        d.setType(ui->tableWidgetCommodities->item(r, CommodityVisualFields::TYPE)->data(Qt::DisplayRole).toString());
        d.setDiscount(DecVal(ui->tableWidgetCommodities->item(r, CommodityVisualFields::DISCOUNT)->data(Qt::DisplayRole).toString()));

        ret.append(d);
    }

    return ret;
}



/**
 * @brief
 *
 * @return InvoiceData
 */
InvoiceData InvoiceDialogImpl::getInvoiceData() const
{
    InvoiceData ret;
    ret.setAdditText(ui->lineEditAdditionalText->text());
    ret.setCounterpartyID((CounterpartyTypeData::CounterpartyType)db->modelCounterparty()->
                 data(db->modelCounterparty()->index(ui->comboBoxCounterparties->currentIndex(),
                            CounterpartyFields::ID)).toLongLong());
    ret.setCurrency(CurrencyData::stringToCurrencyCode(db->modelCurrency()->data(
                                                           db->modelCurrency()->
                                                           index(ui->comboBoxCurrency->currentIndex(), CurrencyFields::CODE)).toString()));
    ret.setDiscount(DecVal(ui->spinBoxDiscount->value()));
    ret.setInvNumber(ui->lineEditInvNumber->text());
    ret.setIssuanceDate(ui->dateEditDateOfIssuance->date());
    ret.setPaymentDate(ui->dateEditDayOfPayment->date());
    ret.setPaymentType(PaymentTypeData::stringToPaymentType(db->modelPaymentType()->data(db->modelPaymentType()->
                                                  index(ui->comboBoxPayment->currentIndex(),
                                                        PaymentTypeFields::TYPE)).toString()));
    ret.setSellingDate(ui->dateEditDateOfSell->date());
    ret.setType(InvoiceTypeData::stringToInvoiceType(db->modelInvoiceType()->data(db->modelInvoiceType()->
                                               index(ui->comboBoxInvoiceType->currentIndex(),
                                                     InvoiceTypeFields::INVOICE_TYPE)).toString()));
    return ret;
}


/**
 * @brief
 *
 */
void InvoiceDialogImpl::calculateSum()
{
    Money_t discountTotal, netTotal, grossTotal;

    const DecVal onePercent(DecVal(1)/DecVal(100));
    for (int i = 0; i < ui->tableWidgetCommodities->rowCount(); ++i)
    {
        const DecVal quantity(ui->tableWidgetCommodities->item(i, CommodityVisualFields::QUANTITY)->text());
        const Money_t netValue(ui->tableWidgetCommodities->item(i, CommodityVisualFields::NET)->text());
        bool ok = false;
        const QString vatRateStr(ui->tableWidgetCommodities->item(i, CommodityVisualFields::VAT)->text());
        const double vatConvRet = vatRateStr.toDouble(&ok); //handling non-numeric forms of vat values (e.g. "zw." in polish version)
        const DecVal vatRate = onePercent * (ok? DecVal(vatConvRet) : DecVal(0.0));
        const DecVal discountRate = onePercent * DecVal(ui->tableWidgetCommodities->item(i, CommodityVisualFields::DISCOUNT)->text());
        const Money_t grossValue = netValue + netValue * vatRate;
        const Money_t discountValue = grossValue * discountRate;

        netTotal += netValue * quantity;
        discountTotal += discountValue * quantity;
        grossTotal += (grossValue - discountValue) * quantity;
    }

    const DecVal discountPerInvoiceRate(DecVal(ui->spinBoxDiscount->value()) * onePercent);
    const Money_t discountPerInvoiceVal = grossTotal * discountPerInvoiceRate;
    discountTotal += discountPerInvoiceVal;
    grossTotal -= discountPerInvoiceVal;

    const int precision = 2;
    ui->labelSumNetVal->setText(netTotal.toString(precision));
    ui->labelDiscountVal->setText(discountTotal.toString(precision));
    ui->labelSumGrossVal->setText(grossTotal.toString(precision));
}


void InvoiceDialogImpl::init(InvoiceTypeData::Type invoiceType, const QModelIndex &idEdit)
{
    ui->dateEditDateOfSell->setDate(QDate::currentDate());
    ui->dateEditDateOfIssuance->setDate(QDate::currentDate());
    ui->dateEditDayOfPayment->setDate(QDate::currentDate());

    connect(ui->pushButtonAddCommodity, SIGNAL(clicked()), this, SLOT(addCommodity()));
    connect(ui->pushButtonAddNewCommodity, SIGNAL(clicked()), this, SLOT(addNewCommodity()));
    connect(ui->pushButtonMoreInfo, SIGNAL(clicked()), this, SLOT(counterpartyMoreInfo()));
    connect(ui->pushButtonRemoveCommodity, SIGNAL(clicked()), this, SLOT(delCommodity()));
    connect(ui->pushButtonEditCommodity, SIGNAL(clicked()), this, SLOT(editCommodity()));
    connect(ui->pushButtonClose, SIGNAL(clicked()), this, SLOT(canQuit()));
    connect(ui->pushButtonSave, SIGNAL(clicked()), this, SLOT(saveInvoice()));
    connect(ui->pushButtonPrint, SIGNAL(clicked()), this, SLOT(printInvoice()));
    connect(ui->pushButtonAddCounterparty, SIGNAL(clicked()), this, SLOT(counterpartyAdd()));
    connect(ui->tableWidgetCommodities, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editCommodity()));
    connect(ui->tableWidgetCommodities, SIGNAL(itemActivated(QTableWidgetItem *)), this, SLOT(tableActivated(QTableWidgetItem *)));
    connect(ui->tableWidgetCommodities, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(tableActivated(QTableWidgetItem *)));
    connect(ui->lineEditAdditionalText, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
    connect(ui->comboBoxPayment, SIGNAL(currentIndexChanged (QString)), this, SLOT(payTextChanged(QString)));
    connect(ui->comboBoxCurrency, SIGNAL(currentIndexChanged (QString)), this, SLOT(textChanged(QString)));
    connect(ui->comboBoxCounterparties, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateInvoiceNumberFormat()));
    connect(ui->comboBoxInvoiceType, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateInvoiceNumberFormat()));
    connect(ui->dateEditDateOfSell, SIGNAL(dateChanged(QDate)), this, SLOT(dateChanged(QDate)));
    connect(ui->dateEditDateOfIssuance, SIGNAL(dateChanged(QDate)), this, SLOT(dateChanged(QDate)));
    connect(ui->dateEditDateOfIssuance, SIGNAL(dateChanged(QDate)), this, SLOT(updateInvoiceNumberFormat()));
    connect(ui->dateEditDayOfPayment, SIGNAL(dateChanged(QDate)), this, SLOT(dateChanged(QDate)));
    connect(ui->spinBoxDiscount, SIGNAL(valueChanged(int)), this, SLOT(discountChange()));
    connect(ui->checkBoxDiscount, SIGNAL(stateChanged(int)), this, SLOT(discountConstChange()));

    ui->pushButtonMoreInfo->setEnabled(false);
    ui->pushButtonRemoveCommodity->setEnabled(false);
    ui->pushButtonEditCommodity->setEnabled(false);
    ui->dateEditDayOfPayment->setEnabled(false);

    ui->comboBoxInvoiceType->setModel(db->modelInvoiceType());
    ui->comboBoxInvoiceType->setModelColumn(InvoiceTypeFields::INVOICE_TYPE);

    ui->comboBoxCounterparties->setModel(db->modelCounterparty());
    ui->comboBoxCounterparties->setModelColumn(CounterpartyFields::NAME);
    ui->comboBoxCounterparties->setInsertPolicy(QComboBox::InsertAtBottom);
    ui->comboBoxPayment->setModel(db->modelPaymentType());
    ui->comboBoxPayment->setModelColumn(PaymentTypeFields::TYPE);
    ui->comboBoxCurrency->setModel(db->modelCurrency());
    ui->comboBoxCurrency->setModelColumn(CurrencyFields::CODE);

    mapper.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper.setItemDelegate(new QSqlRelationalDelegate(this));
    mapper.setModel(db->modelInvoice());
    mapper.addMapping(ui->lineEditInvNumber, InvoiceFields::INV_NUMBER);
    mapper.addMapping(ui->dateEditDateOfSell, InvoiceFields::SELLING_DATE);
    mapper.addMapping(ui->comboBoxInvoiceType, InvoiceFields::TYPE_ID);
    mapper.addMapping(ui->comboBoxCounterparties, InvoiceFields::COUNTERPARTY_ID);
    mapper.addMapping(ui->dateEditDateOfIssuance, InvoiceFields::ISSUANCE_DATE);
    mapper.addMapping(ui->dateEditDayOfPayment, InvoiceFields::PAYMENT_DATE);
    mapper.addMapping(ui->comboBoxPayment, InvoiceFields::PAYMENT_ID);
    mapper.addMapping(ui->comboBoxCurrency, InvoiceFields::CURRENCY_ID);
    mapper.addMapping(ui->lineEditAdditionalText, InvoiceFields::ADDIT_TEXT);
    mapper.addMapping(ui->spinBoxDiscount, InvoiceFields::DISCOUNT);

    ui->tableWidgetCommodities->setColumnCount(CommodityVisualFields::DISCOUNT - CommodityVisualFields::ID + 1);
    setHeaders();

    retranslateUi();

    if(idEdit.isValid())
    {
        mapper.setCurrentIndex(idEdit.row());
        tableCommodityFill(db->commodities(idEdit.data().toLongLong()));
        ui->pushButtonSave->setEnabled(false); //TODO: sprawdzić czy ustawienia z Settings mają grać tu rolę
        ui->pushButtonMoreInfo->setEnabled(true);
        parent_->setWindowTitle(trUtf8("Edycja dokumentu - %1[*]").arg(InvoiceTypeData::name(invoiceType)));
        calculateSum();
    }
    else
    {
        db->modelInvoice()->insertRow(db->modelInvoice()->rowCount());
        mapper.toLast();

        SettingsGlobal s;
        const QString invoiceTypeStr(InvoiceTypeData::name(invoiceType));
        setInitialComboBoxIndexes(invoiceTypeStr, PaymentTypeData::name(PaymentTypeData::CASH),
                                  CurrencyData::codeName((CurrencyData::Currencies)s.value(s.DEFAULT_CURRENCY).value<int>()));
        parent_->setWindowTitle(trUtf8("Nowy dokument - %1 [*]").arg(invoiceTypeStr));

        updateInvoiceNumber();

        const QString additText(s.value(s.ADDIT_TEXT).toString());
        if(!additText.isEmpty())
        {
            ui->lineEditAdditionalText->setText(additText);
        }
    }

    isLoaded_ = true;
}



/**
 * @brief
 *
 */
void InvoiceDialogImpl::setInitialComboBoxIndexes(const QString &invoiceType,
                                               const QString &paymentType,
                                               const QString &defaultCurrency)
{
    ui->comboBoxInvoiceType->setCurrentIndex(ui->comboBoxInvoiceType->findText(invoiceType));
    ui->comboBoxPayment->setCurrentIndex(ui->comboBoxPayment->findText(paymentType));
    ui->comboBoxCurrency->setCurrentIndex(ui->comboBoxCurrency->findText(defaultCurrency));
}


void InvoiceDialogImpl::setHeaders()
{
    QStringList headers;
    for(int c = CommodityVisualFields::ID; c <= CommodityVisualFields::DISCOUNT; ++c)
    {
        headers.append(CommodityVisualData::header((CommodityVisualFields::Field)c));
    }
    ui->tableWidgetCommodities->setHorizontalHeaderLabels(headers);
    ui->tableWidgetCommodities->hideColumn(CommodityVisualFields::ID);
    ui->tableWidgetCommodities->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidgetCommodities->resizeColumnsToContents();
}


void InvoiceDialogImpl::updateInvoiceNumber()
{
    const QString counterpartyName(ui->comboBoxCounterparties->currentText());
    if( (!ui->lineEditInvNumFormat->text().isEmpty()) &&
            ui->dateEditDateOfIssuance->date().isValid() &&
            (ui->comboBoxInvoiceType->currentIndex() != -1))
    {
        SettingsGlobal s;
        const QString invNumFormat(ui->lineEditInvNumFormat->text().isEmpty()
                                   ?s.value(s.DEFAULT_INV_NUM_FORMAT).toString()
                                  :ui->lineEditInvNumFormat->text());

        const bool defaultInvNumFormat = (s.value(s.DEFAULT_INV_NUM_FORMAT).toString() == invNumFormat);
        const ModelInvoice::DBData dbData(*db->modelInvoice()->getLastExistingNumberDateFromDB(defaultInvNumFormat,
                                                                                                counterpartyName).get());
        const QString invNum(db->modelInvoice()->generateInvoiceNumber(*(InvoiceNumberFormat_t::Parse(invNumFormat).get()),
                                                                         dbData.invNumStr,
                                                                         ui->dateEditDateOfIssuance->date(),
                                                                         dbData.gotIssuanceDate,
                                                                         (InvoiceTypeData::Type)ui->comboBoxInvoiceType->currentIndex()));
        ui->lineEditInvNumber->setText(invNum);
    }
}


void InvoiceDialogImpl::updateInvoiceNumberFormat()
{
    ui->lineEditInvNumFormat->setText(db->modelInvoice()->
                                   getInvoiceNumberFormat(ui->
                                    comboBoxCounterparties->currentText()));
    if(isLoaded_)
    {
        parent_->setWindowModified(true);
    }
}



/**
 * @brief
 *
 * @return bool
 */
bool InvoiceDialogImpl::validateForm()
{
    if(ui->lineEditInvNumber->text().isEmpty())
    {
        QMessageBox::warning(parent_, qApp->applicationName(), trUtf8("Brak numeru kolejnego faktury"),
                              QMessageBox::Ok);
        return false;
    }

    if(ui->comboBoxInvoiceType->currentIndex() == -1)
    {
        QMessageBox::warning(parent_, qApp->applicationName(), trUtf8("Wybierz typ faktury."),
                              QMessageBox::Ok);
        return false;
    }

    if(ui->comboBoxCounterparties->currentIndex() == -1)
    {
        QMessageBox::warning(parent_, qApp->applicationName(), trUtf8("Wybierz kontrahenta."),
                              QMessageBox::Ok);
        return false;
    }

    if(ui->tableWidgetCommodities->rowCount() == 0)
    {
        QMessageBox::warning(parent_, qApp->applicationName(), trUtf8("Wybierz towary lub usługi do sprzedania."),
                              QMessageBox::Ok);
        return false;
    }


    if(ui->comboBoxPayment->currentIndex() == -1)
    {
        QMessageBox::warning(parent_, qApp->applicationName(), trUtf8("Wybierz rodzaj płatności."),
                              QMessageBox::Ok);
        return false;
    }


    if(ui->comboBoxCurrency->currentIndex() == -1)
    {
        QMessageBox::warning(parent_, qApp->applicationName(), trUtf8("Wybierz walutę rozliczeniową."),
                              QMessageBox::Ok);
        return false;
    }

    return true;
}



/**
 * @brief
 *
 */
void InvoiceDialogImpl::counterpartyAdd()
{
    QScopedPointer<CounterpartyDialog> dialog(new CounterpartyDialog(parent_, db));
    counterpartyDialogPtr = dialog.data();
    if(dialog->exec() == QDialog::Accepted)
    {
        if(db->modelCounterparty()->submitAll())
        {
            ui->comboBoxCounterparties->setCurrentIndex(db->modelCounterparty()->rowCount() - 1);
        }
        else
        {
            QMessageBox::warning(parent_, trUtf8("Błąd dodawania kontrahenta"), db->modelCounterparty()->lastError().text());
        }
    }

    if(db->modelCounterparty()->rowCount() > 0)
    {
        ui->pushButtonMoreInfo->setEnabled(true);
    }
}



/**
 * @brief
 *
 */
void InvoiceDialogImpl::counterpartyMoreInfo()
{
    if(ui->comboBoxCounterparties->currentIndex() != -1)
    {
        CounterpartyInfoDialog dialog(parent_, db, ui->comboBoxCounterparties->
                                      model()->index(ui->comboBoxCounterparties->
                                            currentIndex(), CounterpartyFields::ID));
        dialog.exec();
    }
}


/**
 * @brief
 *
 * @param QDate
 */
void InvoiceDialogImpl::dateChanged(QDate )
{
    ui->pushButtonSave->setEnabled(true);
    if(isLoaded_)
    {
        parent_->setWindowModified(true);
    }
}


/**
 * @brief Removes the commodity from the list
 *
 */
void InvoiceDialogImpl::delCommodity()
{
    const qlonglong idCommodity(ui->tableWidgetCommodities->item(
                            ui->tableWidgetCommodities->currentRow(),
                            CommodityVisualFields::ID)->data(Qt::DisplayRole).toLongLong());
    const DecVal changeAmount = ui->tableWidgetCommodities->item(
                ui->tableWidgetCommodities->currentRow(),
                CommodityVisualFields::QUANTITY)->data(Qt::DisplayRole).value<DecVal>();
    db->modelCommodity()->changeAmount(idCommodity, changeAmount);
    ui->tableWidgetCommodities->removeRow(ui->tableWidgetCommodities->currentRow());
    calculateSum();
    ui->pushButtonSave->setEnabled(true);
    if(isLoaded_)
    {
        parent_->setWindowModified(true);
    }
}




/**
 * @brief Allows to edit selected product. Opens ChangeAmountDialog window.
 *
 */
void InvoiceDialogImpl::editCommodity()
{
    // we can only modify quantity
//    ChangeAmountDialog changeAmount(this);
//    changeAmount.lineEditCommodityName->setText(ui_->tableWidgetCommodities-> item(ui_->tableWidgetCommodities->currentRow(), 1)->text());
//    changeAmount.lineEditCommodityCode->setText(ui_->tableWidgetCommodities-> item(ui_->tableWidgetCommodities->currentRow(), 2)->text());
//    changeAmount.spinBoxCommodityAmount->setValue(ui_->tableWidgetCommodities-> item(ui_->tableWidgetCommodities->currentRow(), 4)->text().toInt());
//    if (changeAmount.exec() == QDialog::Accepted)
//    {
//        const int currentRow = ui_->tableWidgetCommodities->currentRow();
//        ui_->tableWidgetCommodities->item(currentRow,
//                     CommodityVisualFields::QUANTITY)->
//                        setText(changeAmount.spinBoxCommodityAmount->cleanText());
//        ui_->pushButtonSave->setEnabled(true);
//        unsaved = true;
//        calculateSum();
//    }
}


void InvoiceDialogImpl::printPaintRequested(QPrinter *printer)
{
    QWebView doc;
    doc.setHtml(docHTML);
    doc.print(printer);
}


void InvoiceDialogImpl::retranslateUi()
{
    QTranslator appTranslator;
    SettingsGlobal s;
    appTranslator.load(QString(":/res/translations/qfaktury_") + s.value(s.LANG).toString());
    qApp->installTranslator(&appTranslator);
    ui->retranslateUi((QDialog*)parent_);
}


/**
 * @brief
 *
 * @param item
 */
void InvoiceDialogImpl::tableActivated(QTableWidgetItem * item)
{
    if (item != 0 && !item->text().isEmpty())
    {
        ui->pushButtonRemoveCommodity->setEnabled(true);
        ui->pushButtonEditCommodity->setEnabled(true);
    }
}


/**
 * @brief
 *
 */
void InvoiceDialogImpl::discountConstChange()
{
    if(ui->checkBoxDiscount->isChecked())
    {
        ui->pushButtonSave->setEnabled(true);
    }
}
/**
 * @brief Used to recalculate discount when spinBox arrows are pressed.
 *
 */
void InvoiceDialogImpl::discountChange()
{
    if(isLoaded_)
    {
        calculateSum();
        ui->pushButtonSave->setEnabled(true);
        parent_->setWindowModified(true);
    }
}



/**
 * @brief
 *
 */
void InvoiceDialogImpl::addCommodity()
{
    QScopedPointer<CommodityListDialog> dialog(new CommodityListDialog(parent_, db));
    commodityListDialogPtr = dialog.data();
    if(dialog->exec() == QDialog::Accepted)
    {
        const int rowNum = ui->tableWidgetCommodities->rowCount();
        ui->tableWidgetCommodities->setRowCount(rowNum + 1);
        tableCommoditySetItems(dialog->ret, rowNum);

        ui->pushButtonSave->setEnabled(true);
        parent_->setWindowModified(true);
        calculateSum();

        db->modelCommodity()->changeAmount(dialog->ret.ID(), -dialog->ret.quantity());
    }
}


/**
 * @brief
 *
 */
void InvoiceDialogImpl::addNewCommodity()
{
    QScopedPointer<CommodityDialog> dialog(new CommodityDialog(parent_, db));
    commodityDialogPtr = dialog.data();
    if(dialog->exec() == QDialog::Accepted)
    {
        if(! db->modelCommodity()->submitAll())
        {
            QMessageBox::warning(parent_, trUtf8("Błąd dodawania towaru"), db->modelCommodity()->lastError().text());
        }
    }
}


/**
 * @brief Asks the user if save the invoice
 *
 */
void InvoiceDialogImpl::canQuit()
{
    if (((QWidget*)parent())->isWindowModified())
    {
        if(QMessageBox::warning(parent_, qApp->applicationName(),
                         trUtf8("Dane zostały zmienione, czy chcesz zapisać?"),
                         QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)
           == QMessageBox::No)
        {
            parent_->reject();
        }
        else
        {
            saveInvoice();
            parent_->accept();
        }
    }
    else
    {
        parent_->accept();
    }
}


/**
 * @brief
 *
 * @param text
 */
void InvoiceDialogImpl::payTextChanged(QString text)
{
    if(ui->comboBoxPayment->currentIndex() > 0)
    {
        ui->dateEditDayOfPayment->setEnabled(true);
    }
    else
    {
        ui->dateEditDayOfPayment->setEnabled(false);
    }

    if(ui->comboBoxPayment->currentIndex() == (ui->comboBoxPayment->count() - 1))
    {
        SettingsGlobal settings;
        if (settings.stringToDouble(ui->labelSumGrossVal->text()) == 0)
        {
            QMessageBox::warning(parent_, qApp->applicationName(), trUtf8("Taki sposób płatności nie może zostać wybrany, ponieważ kwota do zapłaty wynosi 0."));
            ui->comboBoxPayment->setCurrentIndex(0);
            return;
        }
        CustomPaymentDialog cp;
        cp.setInvoiceAmount(settings.stringToDouble(ui->labelSumGrossVal->text()));
        if (cp.exec() ==  QDialog::Accepted)
        {
            custPaymDataPtr = cp.custPaymData;
            ui->dateEditDayOfPayment->setEnabled(false);
        }
        else
        {
            ui->comboBoxPayment->setCurrentIndex(0);
        }
    }

    textChanged(text);
}


/**
 * @brief
 *
 * @param QString
 */
void InvoiceDialogImpl::textChanged(QString)
{
    if(isLoaded_)
    {
        ui->pushButtonSave->setEnabled(true);
        parent_->setWindowModified(true);
    }
}



/**
 * @brief
 *
 * @return bool
 */
bool InvoiceDialogImpl::saveInvoice()
{
    if (!validateForm()) return false;

    bool result = mapper.submit();
    if(result)
    {
        result = db->invoiceWithCommoditiesInsertTransact(getInvoiceData(), getCommoditiesVisualData());
        if(result)
        {
            parent_->setWindowModified(false);
            ui->pushButtonSave->setEnabled(false);
            ui->pushButtonRemoveCommodity->setEnabled(false);
            ui->pushButtonEditCommodity->setEnabled(false);
        }
        else
        {
            qDebug("InvoiceDialog::saveInvoice() - lastError: %s", qPrintable(db->modelInvoice()->lastError().text()));
        }
    }
    else
    {
        qDebug("InvoiceDialog::saveInvoice() - mapper_submit - lastError: %s",
                 qPrintable(db->modelInvoice()->lastError().text()));
    }

    return result;
}

/** Slot makeInvoice
 *  Generate html with invoice and show Print Preview dialog
 */
/**
 * @brief
 *
 */
void InvoiceDialogImpl::printInvoice()
{
    if(!validateForm())
        return;

    SettingsGlobal s;

    CounterpartyData customer;
    ModelCounterparty *model = static_cast<ModelCounterparty *>(ui->comboBoxCounterparties->model());

    const int currIndex = ui->comboBoxCounterparties->currentIndex();
    customer.setAccountName(model->data(model->index(currIndex, CounterpartyFields::ACCOUNT_NAME)).toString());
    customer.setCountry(model->data(model->index(currIndex, CounterpartyFields::COUNTRY)).toString());
    customer.setId(model->data(model->index(currIndex, CounterpartyFields::ID)).toLongLong());
    customer.setLocation(model->data(model->index(currIndex, CounterpartyFields::LOCATION)).toString());
    customer.setName(model->data(model->index(currIndex, CounterpartyFields::NAME)).toString());
    customer.setStreet(model->data(model->index(currIndex, CounterpartyFields::STREET)).toString());
    customer.setTax_ident(model->data(model->index(currIndex, CounterpartyFields::TAX_IDENT)).toString());

    CounterpartyData seller;
    model->setOnlyMyCompanyVisible(true);
    constexpr int sellerIndex = 0;
    seller.setAccountName(model->data(model->index(sellerIndex, CounterpartyFields::ACCOUNT_NAME)).toString());
    seller.setCountry(model->data(model->index(sellerIndex, CounterpartyFields::COUNTRY)).toString());
    seller.setId(model->data(model->index(sellerIndex, CounterpartyFields::ID)).toLongLong());
    seller.setLocation(model->data(model->index(sellerIndex, CounterpartyFields::LOCATION)).toString());
    seller.setName(model->data(model->index(sellerIndex, CounterpartyFields::NAME)).toString());
    seller.setStreet(model->data(model->index(sellerIndex, CounterpartyFields::STREET)).toString());
    seller.setTax_ident(model->data(model->index(sellerIndex, CounterpartyFields::TAX_IDENT)).toString());
    db->modelCounterparty()->setOnlyMyCompanyVisible(false);


    InvoiceComposer ic;
    ic.setData(getInvoiceData(), Money_t(ui->labelSumNetVal->text()), Money_t(ui->labelSumGrossVal->text()),
               DecVal(0), customer, seller, getCommoditiesVisualData());

    docHTML = ic.getInvoiceHtml();

    QFile file;
    QTextStream stream;
    file.setFileName("debugInvoice.html");
    file.open(QIODevice::WriteOnly);
    stream.setDevice(&file);
    stream << docHTML;
    file.close();

    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, parent_);
    preview.setWindowFlags(Qt::Window);
    preview.setWindowTitle(trUtf8("%1 - Podgląd wydruku").arg(ui->comboBoxInvoiceType->currentText()));
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(printPaintRequested(QPrinter*)));
    preview.exec();
}


/**
 * @brief
 *
 * @param isAllowed
 */
//void InvoiceDialogImpl::setIsEditAllowed(const bool isAllowed)
//{
//    ui->lineEditInvNumber->setEnabled(isAllowed);
//    ui->dateEditDateOfSell->setEnabled(isAllowed);
//    ui->dateEditDateOfIssuance->setEnabled(isAllowed);
//    ui->tableWidgetCommodities->setEnabled(isAllowed);
//    ui->spinBoxDiscount->setEnabled(isAllowed);
//    ui->labelDiscount1->setEnabled(isAllowed);
//    ui->comboBoxPayment->setEnabled(isAllowed);
//    ui->dateEditDayOfPayment->setEnabled(isAllowed);
//    ui->lineEditAdditionalText->setEnabled(isAllowed);
//    ui->pushButtonAddCommodity->setEnabled(isAllowed);
//    ui->pushButtonRemoveCommodity->setEnabled(isAllowed);
//    ui->pushButtonEditCommodity->setEnabled(isAllowed);
//    ui->checkBoxDiscount->setEnabled(isAllowed);
//    ui->comboBoxCounterparties->setEnabled(isAllowed);
//    ui->comboBoxCurrency->setEnabled(isAllowed);
//    ui->pushButtonSave->setEnabled(isAllowed);
//    ui->pushButtonAddCounterparty->setEnabled(isAllowed);
//    ui->dateEditDayOfPayment->setEnabled(isAllowed);
//    if (!isAllowed || ui->spinBoxDiscount->value() == 0) {
//        ui->checkBoxDiscount->setChecked(false);
//        ui->spinBoxDiscount->setEnabled(false);
//    } else {
//        ui->checkBoxDiscount->setChecked(true);
//        ui->spinBoxDiscount->setEnabled(true);
//    }
//    if (isAllowed && (ui->comboBoxPayment->currentIndex() > 0)) {
//        ui->dateEditDayOfPayment->setEnabled(true);
//    } else {
//        ui->dateEditDayOfPayment->setEnabled(false);
//    }
//}


/**
 * @brief
 *
 * @return QString
 */
QString InvoiceDialogImpl::getGroupedSums()
{
    QStringList out;
    SettingsGlobal s;
    const QStringList rates = s.value(s.VAT_RATES).toString().split("|");

    QMap<int, double> rateNet;
    QMap<int, double> rateVat;
    QMap<int, double> ratesGross;
    // every currency in int value is equal vat currency

    const int ssize = rates.size();

    // lp, nazwa, kod, pkwiu, ilosc, jm, rabat, cena jm., netto, vat, brutto
    for (int i = 0; i < ui->tableWidgetCommodities->rowCount(); ++i)
    {
        for (int y = 0; y < ssize; ++y)
        {
            if (rates[y] == ui->tableWidgetCommodities->item(i, CommodityVisualFields::VAT)->text())
            {
                rateNet[y] += s.stringToDouble(ui->tableWidgetCommodities->item(i, CommodityVisualFields::NET)->text());
                ratesGross[y] += 0;//s.stringToDouble(tableWidgetCommodities->item(i, CommodityVisualFields::)->text());
                rateVat[y] = ratesGross[y] - rateNet[y];
            }
            else
            {
                rateNet[y] += 0;
                ratesGross[y] += 0;
                rateVat[y] += 0;
            }
        }
    }

    for (int y = 0; y < ssize; ++y)
    {
        out += "<tr class=\"stawki\">";
        out += "<td width=\"30%\">" + s.numberToString(rateNet[y], 'f', 2) + "</td>"; // netto
        out += "<td width=\"10%\">" + rates[y] + "</td>"; // stawka
        out += "<td width=\"30%\">" + s.numberToString(rateVat[y], 'f', 2) + "</td>"; // podatek
        out += "<td width=\"30%\">" + s.numberToString(ratesGross[y], 'f', 2) + "</td>"; // brutto
        out += "</tr>";

    }
    return out.join(" ");
}
