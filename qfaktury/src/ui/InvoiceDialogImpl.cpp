#include <QSqlRelationalDelegate>
#include <QSqlError>
#include <QTextDocument>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QTranslator>
#include <QFile>
#include <QMessageBox>

#include "InvoiceDialogImpl.h"
#include "ui_InvoiceDialog.h"
#include "Database.h"
#include "CounterpartyInfoDialog.h"
#include "CommodityListDialog.h"
#include "ConvertAmount.h"
#include "CustomPaymentDialog.h"
#include "CounterpartyDialog.h"
#include "ModelInvoice.h"
#include "ModelCounterparty.h"
#include "SettingsGlobal.h"
#include "CounterpartyData.h"
#include "ModelInvoiceType.h"
#include "ModelPaymentType.h"
#include "ModelCurrency.h"
#include "PaymentTypeData.h"
#include "CurrencyData.h"
#include "CustomPaymData.h"


InvoiceDialogImpl::InvoiceDialogImpl(QWidget *parent, Database *database)
    : QDialog(parent), ui(new Ui::InvoiceDialog()), db(database)
{
}

InvoiceDialogImpl::~InvoiceDialogImpl()
{
    db->modelInvoice()->revertAll();
    delete ui;
}


/**
 * @brief
 *
 * @param commodities
 */
void InvoiceDialogImpl::fillTableCommodity(const QList<CommodityVisualData> &commodities)
{
    ui->tableWidgetCommodities->setColumnCount(CommodityVisualFields::DISCOUNT - CommodityVisualFields::ID + 1);
    ui->tableWidgetCommodities->setRowCount(commodities.size());
    for(int r = 0; r < commodities.size(); ++r)
    {
        for(int c = CommodityVisualFields::ID; c <= CommodityVisualFields::DISCOUNT; ++c)
        {
            ui->tableWidgetCommodities->setItem(r, c, new QTableWidgetItem(commodities.at(r).field(c)));
        }
    }

    QStringList headers;
    for(int c = CommodityVisualFields::ID; c <= CommodityVisualFields::DISCOUNT; ++c)
    {
        headers.append(CommodityVisualData::header(c));
    }
    ui->tableWidgetCommodities->setHorizontalHeaderLabels(headers);
    ui->tableWidgetCommodities->hideColumn(CommodityVisualFields::ID);
    ui->tableWidgetCommodities->resizeColumnsToContents();
}


void InvoiceDialogImpl::genInvoiceNumber(const QString& invNumFormat, const QDate& issuanceDate, const int invoiceType, const QString& counterpartyName)
{
    const QString invNum(db->modelInvoice()->generateInvoiceNumber(invNumFormat, issuanceDate,
                                                                    InvoiceTypeData::name(invoiceType),
                                                                    counterpartyName));
    ui->lineEditInvNumber->setText(invNum);
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
        for(int c = CommodityVisualFields::ID; c <= CommodityVisualFields::DISCOUNT; ++c)
        {
            d.setField(c, ui->tableWidgetCommodities->item(r, c)->data(Qt::DisplayRole));
        }
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
    ret.setField(InvoiceFields::ADDIT_TEXT, ui->lineEditAdditionalText->text());
    ret.setField(InvoiceFields::COUNTERPARTY_ID, db->modelCounterparty()->
                 data(db->modelCounterparty()->index(ui->comboBoxCounterparties->currentIndex(),
                            CounterpartyFields::ID)).toLongLong());
    ret.setField(InvoiceFields::CURRENCY_ID, ui->comboBoxCurrency->currentIndex() + 1);
    ret.setField(InvoiceFields::DISCOUNT, ui->spinBoxDiscount->value());
    ret.setField(InvoiceFields::INV_NUMBER, ui->lineEditInvNumber->text());
    ret.setField(InvoiceFields::ISSUANCE_DATE, ui->dateEditDateOfIssuance->date());
    ret.setField(InvoiceFields::PAYMENT_DATE, ui->dateEditDayOfPayment->date());
    ret.setField(InvoiceFields::PAYMENT_ID, ui->comboBoxPayment->currentIndex() + 1);
    ret.setField(InvoiceFields::SELLING_DATE, ui->dateEditDateOfSell->date());
    ret.setField(InvoiceFields::TYPE_ID, ui->comboBoxInvoiceType->currentIndex() + 1);

    return ret;
}


/**
 * @brief
 *
 */
void InvoiceDialogImpl::calculateSum()
{
    SettingsGlobal s;
    netTotal = discountTotal = grossTotal = 0.0;

    for (int i = 0; i < ui->tableWidgetCommodities->rowCount(); ++i)
    {
        const double quantity = s.stringToDouble(ui->tableWidgetCommodities->item(i, CommodityVisualFields::QUANTITY)->text());
        const double netVal = s.stringToDouble(ui->tableWidgetCommodities->item(i, CommodityVisualFields::NET)->text());
        const double vatRate = 0.01 * s.stringToDouble(ui->tableWidgetCommodities->item(i, CommodityVisualFields::VAT)->text());
        const double discountRate = 0.01 * s.stringToDouble(ui->tableWidgetCommodities->item(i, CommodityVisualFields::DISCOUNT)->text());

        const double grossVal = netVal + netVal * vatRate;
        const double discountValue = discountRate * grossVal;

        netTotal += netVal * quantity;
        discountTotal += discountValue * quantity;
        grossTotal += (grossVal - discountValue) * quantity;
    }

    ui->labelSumNetVal->setText(s.numberToString(netTotal, 'f', 2));
    ui->labelDiscountVal->setText(s.numberToString(discountTotal, 'f', 2));
    ui->labelSumGrossVal->setText(s.numberToString(grossTotal, 'f', 2));
}


void InvoiceDialogImpl::init(InvoiceTypeData::Type invoiceType, const QModelIndex &idEdit)
{
    ui->dateEditDateOfSell->setDate(QDate::currentDate());
    ui->dateEditDateOfIssuance->setDate(QDate::currentDate());
    ui->dateEditDayOfPayment->setDate(QDate::currentDate());

    QObject::connect(ui->pushButtonAddCommodity, SIGNAL(clicked()), this, SLOT(addCommodity()));
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
    connect(ui->lineEditInvNumFormat, SIGNAL(textChanged(QString)), this, SLOT(updateInvoiceNumber()));
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

    ui->pushButtonRemoveCommodity->setEnabled(false);
    ui->pushButtonEditCommodity->setEnabled(false);
    ui->dateEditDayOfPayment->setEnabled(false);
    ui->spinBoxDiscount->setEnabled(false);

    ui->comboBoxInvoiceType->setModel(db->modelInvoiceType());
    ui->comboBoxInvoiceType->setModelColumn(InvoiceTypeFields::TYPE);

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
    mapper.addMapping(ui->comboBoxInvoiceType, InvoiceFields::TYPE_ID, "currentIndex");
    mapper.addMapping(ui->comboBoxCounterparties, InvoiceFields::COUNTERPARTY_ID, "currentIndex");
    mapper.addMapping(ui->dateEditDateOfIssuance, InvoiceFields::ISSUANCE_DATE);
    mapper.addMapping(ui->dateEditDayOfPayment, InvoiceFields::PAYMENT_DATE);
    mapper.addMapping(ui->comboBoxPayment, InvoiceFields::PAYMENT_ID);
    mapper.addMapping(ui->comboBoxCurrency, InvoiceFields::CURRENCY_ID, "currentIndex");
    mapper.addMapping(ui->lineEditAdditionalText, InvoiceFields::ADDIT_TEXT);
    mapper.addMapping(ui->spinBoxDiscount, InvoiceFields::DISCOUNT);

    if(idEdit.isValid())
    {
        mapper.setCurrentIndex(idEdit.row());
        fillTableCommodity(db->commodities(idEdit.data().toLongLong()));
        ui->pushButtonSave->setEnabled(false);
    }
    else
    {
        db->modelInvoice()->insertRow(db->modelInvoice()->rowCount());
        mapper.toLast();

        SettingsGlobal s;
        const QString invoiceTypeStr(InvoiceTypeData::name(invoiceType));
        setInitialComboBoxIndexes(invoiceTypeStr, PaymentTypeData::name(PaymentTypeData::CASH), s.value(s.DEFAULT_CURRENCY).toString());
        setWindowTitle(invoiceTypeStr);

        updateInvoiceNumber();

        const QString additText(s.value(s.ADDIT_TEXT).toString());
        if(!additText.isEmpty())
        {
            ui->lineEditAdditionalText->setText(additText);
        }
    }

    retranslateUi();
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


void InvoiceDialogImpl::updateInvoiceNumber()
{
    if( (!ui->lineEditInvNumFormat->text().isEmpty()) &&
            ui->dateEditDateOfIssuance->date().isValid() &&
            (ui->comboBoxInvoiceType->currentIndex() != -1) &&
            (ui->comboBoxCounterparties->currentIndex() != -1))
    {
        genInvoiceNumber(ui->lineEditInvNumFormat->text(),
                          ui->dateEditDateOfIssuance->date(),
                          InvoiceTypeData::StringToInvoiceType(ui->comboBoxInvoiceType->currentText()),
                          ui->comboBoxCounterparties->currentText());
    }
}


void InvoiceDialogImpl::updateInvoiceNumberFormat()
{
    ui->lineEditInvNumFormat->setText(db->modelInvoice()->
                                       getInvoiceNumberFormat(ui->
                                        comboBoxCounterparties->currentText()));
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
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Brak numeru kolejnego faktury"),
                              QMessageBox::Ok);
        saveFailed = true;
        return false;
    }

    if(ui->comboBoxInvoiceType->currentIndex() == -1)
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Wybierz typ faktury."),
                              QMessageBox::Ok);
        saveFailed = true;
        return false;
    }

    if(ui->comboBoxCounterparties->currentIndex() == -1)
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Wybierz kontrahenta."),
                              QMessageBox::Ok);
        saveFailed = true;
        return false;
    }

    if(ui->tableWidgetCommodities->rowCount() == 0)
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Wybierz towary lub usługi do sprzedania."),
                              QMessageBox::Ok);
        saveFailed = true;
        return false;
    }


    if(ui->comboBoxPayment->currentIndex() == -1)
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Wybierz rodzaj płatności."),
                              QMessageBox::Ok);
        saveFailed = true;
        return false;
    }


    if(ui->comboBoxCurrency->currentIndex() == -1)
    {
        QMessageBox::warning(this, qApp->applicationName(), trUtf8("Wybierz walutę rozliczeniową."),
                              QMessageBox::Ok);
        saveFailed = true;
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
    CounterpartyDialog dialog(this, db);
    if (dialog.exec() == QDialog::Accepted)
    {
        if(db->modelCounterparty()->submitAll())
        {
            ui->comboBoxCounterparties->setCurrentIndex(db->modelCounterparty()->rowCount() - 1);
        }
        else
        {
            QMessageBox::warning(this, trUtf8("Błąd dodawania kontrahenta"), db->modelCounterparty()->lastError().text());
        }
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
        CounterpartyInfoDialog dialog(this, db, ui->comboBoxCounterparties->
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
    unsaved = true;
}


/**
 * @brief Removes the commodity from the list
 *
 */
void InvoiceDialogImpl::delCommodity()
{
    ui->tableWidgetCommodities->removeRow(ui->tableWidgetCommodities->currentRow());
    calculateSum();
    ui->pushButtonSave->setEnabled(true);
    unsaved = true;
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
    QTextDocument doc(this);
    doc.setHtml(docHTML);
    doc.print(printer);
}


void InvoiceDialogImpl::retranslateUi()
{
    QTranslator appTranslator;
    SettingsGlobal s;
    appTranslator.load(QString("translations/qfaktury_") + s.value(s.LANG).toString());
    qApp->installTranslator(&appTranslator);
    ui->retranslateUi(this);
}


/**
 * @brief
 *
 * @param item
 */
void InvoiceDialogImpl::tableActivated(QTableWidgetItem * item)
{
    if (item != nullptr && !item->text().isEmpty())
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
        ui->spinBoxDiscount->setEnabled(true);
        ui->pushButtonSave->setEnabled(true);
        calculateSum();
    }
    else
    {
        ui->spinBoxDiscount->setValue(0);
        ui->spinBoxDiscount->setEnabled(false);
    }
}
/**
 * @brief Used to recalculate discount when spinBox arrows are pressed.
 *
 */
void InvoiceDialogImpl::discountChange()
{
    calculateSum();
    ui->pushButtonSave->setEnabled(true);
    unsaved = true;
}



/**
 * @brief
 *
 */
void InvoiceDialogImpl::addCommodity()
{
    CommodityListDialog dialog(this, db);
    if (dialog.exec() == QDialog::Accepted)
    {
        const int rowNum = ui->tableWidgetCommodities->rowCount() == 0 ? 0 : ui->tableWidgetCommodities->rowCount() - 1;
        ui->tableWidgetCommodities->insertRow(rowNum);

        ui->tableWidgetCommodities->setColumnCount(CommodityVisualFields::DISCOUNT - CommodityVisualFields::ID + 1);

        QStringList headers;
        for(int i = CommodityVisualFields::ID; i <= CommodityVisualFields::DISCOUNT; ++i)
        {
            ui->tableWidgetCommodities->setItem(rowNum, i, new QTableWidgetItem(dialog.ret.field(i)));
            headers.push_back(CommodityVisualData::header(i));
        }

        ui->tableWidgetCommodities->hideColumn(CommodityVisualFields::ID);
        ui->tableWidgetCommodities->setHorizontalHeaderLabels(headers);
        ui->tableWidgetCommodities->resizeColumnsToContents();

        ui->pushButtonSave->setEnabled(true);
        unsaved = true;
        calculateSum();
    }
}


/**
 * @brief Asks the user if save the invoice
 *
 */
void InvoiceDialogImpl::canQuit()
{
    // canClose == false -> data changed
    if (unsaved || QMessageBox::warning(this, qApp->applicationName(), trUtf8("Dane zostały zmienione, czy chcesz zapisać?"),
                                         QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)
            == QMessageBox::No)
    {
        reject();
    }
    else
    {
        saveInvoice();
        if (!saveFailed)
        {
            accept();
        }
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

    if(ui->comboBoxPayment->currentIndex() == ui->comboBoxPayment->count() - 1)
    {
        SettingsGlobal settings;
        if (settings.stringToDouble(ui->labelSumGrossVal->text()) == 0)
        {
            QMessageBox::warning(this, qApp->applicationName(), trUtf8("Taki sposób płatności nie może zostać wybrany, ponieważ kwota do zapłaty wynosi 0."));
            ui->comboBoxPayment->setCurrentIndex(0);
            return;
        }
        CustomPaymentDialog cp;
        cp.setInvoiceAmount(settings.stringToDouble(ui->labelSumGrossVal->text()));
        if (cp.exec() ==  QDialog::Accepted)
        {
            custPaymData = cp.custPaymData;
            ui->dateEditDayOfPayment->setEnabled(false);
        }
        else
        {
            ui->comboBoxPayment->setCurrentIndex(0);
        }
    }

    textChanged(text);
}

/** Slot textChanged
 *  Activates buttons
 */
/**
 * @brief
 *
 * @param QString
 */
void InvoiceDialogImpl::textChanged(QString) {
    ui->pushButtonSave->setEnabled(true);
    unsaved = true;
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
        if(!result)
        {
            qDebug() << "InvoiceDialog::saveInvoice() - lastError: " << db->modelInvoice()->lastError().text();
            saveFailed = true;
            unsaved = true;
        }
        ui->pushButtonSave->setEnabled(false);
        ui->pushButtonRemoveCommodity->setEnabled(false);
        ui->pushButtonEditCommodity->setEnabled(false);
    }
    else
    {
        qDebug() << "InvoiceDialog::saveInvoice() - mapper_submit - lastError: "
                 << db->modelInvoice()->lastError().text();
        saveFailed = true;
        unsaved = true;
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
    QTextStream stream;
    QString invoiceHTMLTemplate, styleCSS;

    QFile file(":/res/templates/template.html");
    if(file.open(QIODevice::ReadOnly))
    {
        stream.setDevice(&file);
        invoiceHTMLTemplate = stream.readAll();
        file.close();

        file.setFileName("style.css");
        if(file.open(QIODevice::ReadOnly))
        {
            stream.setDevice(&file);
            styleCSS = stream.readAll();
            file.close();
        }
        else
        {
            qDebug() << "File " << file.fileName() << " couldn't be opened.";
        }

        const QString logo(s.value(s.LOGO).toString());
        const QString stampStr(logo.isEmpty() ? trUtf8("Pieczęć wystawcy") : QString("<img src=\"%1\">").arg(logo));

        QString sellerAttrList;

        s.beginGroup("printpos");
        if(s.value(s.SELLER_NAME).toBool())
            sellerAttrList += QString("<li>%1</li>").arg(s.value(s.SELLER_NAME).toString());
        if(s.value(s.SELLER_ADDRESS).toBool())
            sellerAttrList += QString("<li>%1</li>").arg(s.value(s.SELLER_ADDRESS).toString());
        if(s.value(s.SELLER_LOCATION).toBool())
            sellerAttrList += QString("<li>%1</li>").arg(s.value(s.SELLER_LOCATION).toString());
        if(s.value(s.SELLER_TAXID).toBool())
            sellerAttrList += trUtf8("<li>NIP: %1</li>").arg(s.value(s.SELLER_TAXID).toString());
        if(s.value(s.SELLER_ACCOUNT).toBool())
            sellerAttrList += trUtf8("<li>Nr konta: %1</li>").arg(s.value(s.SELLER_ACCOUNT).toString().replace("-", " "));
        s.endGroup();
        const QString sellerHTML(QString("<h1>Sprzedawca:</h1><ul>%1</ul>").arg(sellerAttrList));

        const QList<CommodityVisualData> products(getCommoditiesVisualData());
        QString productsHTML;
        foreach(CommodityVisualData cvd, products)
        {
            productsHTML += "<tr>";
            if(s.contains(s.keyName(s.ORDER_NUMBER)))
            {
                productsHTML += QString("<td>%1</td>").arg(cvd.field(CommodityVisualFields::ID));
            }

            if(s.contains(s.keyName(s.NAME)))
            {
                productsHTML += QString("<td>%1</td>").arg(cvd.field(CommodityVisualFields::NAME));
            }

            if(s.contains(s.keyName(s.PKWIU)))
            {
                productsHTML += QString("<td>%1</td>").arg(cvd.field(CommodityVisualFields::PKWIU));
            }

            if(s.contains(s.keyName(s.QUANTITY)))
            {
                productsHTML += QString("<td>%1</td>").arg(cvd.field(CommodityVisualFields::QUANTITY));
            }

            if(s.contains(s.keyName(s.INTERNAT_UNIT)))
            {
                productsHTML += QString("<td>%1</td>").arg(cvd.field(CommodityVisualFields::UNIT));
            }

            if(s.contains(s.keyName(s.NET_VAL)))
            {
                productsHTML += QString("<td>%1</td>").arg(cvd.field(CommodityVisualFields::NET));
            }

            if(s.contains(s.keyName(s.DISCOUNT)))
            {
                productsHTML += QString("<td>%1</td>").arg(cvd.field(CommodityVisualFields::DISCOUNT));
            }

            if(s.contains(s.keyName(s.VAT_VAL)))
            {
                productsHTML += QString("<td>%1</td>").arg(cvd.field(CommodityVisualFields::VAT));
            }

            productsHTML += "</tr>";
        }

        QString summaryHTML(QString("<span>%1 %2 %3</span>").arg(trUtf8("Do zapłaty:")).arg(ui->labelSumGrossVal->text()).arg(ui->comboBoxCurrency->currentText()));
        summaryHTML += QString("%1 %2").arg(trUtf8("słownie:")).arg(ConvertAmount::convertPL(ui->labelSumGrossVal->text(), ui->comboBoxCurrency->currentText()));

        if(ui->comboBoxPayment->currentIndex() == 0)
        {
            summaryHTML += trUtf8("forma płatności: ") + ui->comboBoxPayment->currentText() + "<b>";
            summaryHTML += trUtf8("Zapłacono gotówką");
        }
        else if((ui->comboBoxPayment->currentIndex() == ui->comboBoxPayment->count() -1) && (custPaymData != nullptr))
        {
            summaryHTML += "<span style=\"toPay\">";
            summaryHTML += QString(trUtf8("Zapłacono: ") + custPaymData->payment1 + ": "
                                  +  s.numberToString(custPaymData->amount1) + " " + ui->comboBoxCurrency->currentText() + " "
                                  + custPaymData->date1.toString(s.dateFormatExternal()) + "<br>");
            summaryHTML += QString(trUtf8("Zaległości: ") + custPaymData->payment2 + ": "
                                  +  s.numberToString(custPaymData->amount2) + " " + ui->comboBoxCurrency->currentText() + " "
                                  + custPaymData->date2.toString(s.dateFormatExternal()));
            summaryHTML += "</span>";
        }
        else
        {
            summaryHTML += trUtf8("forma płatności: ") + ui->comboBoxPayment->currentText() + "<b>";
            summaryHTML += "<span style=\"payDate\">";
            summaryHTML += trUtf8("termin płatności: ") + ui->dateEditDayOfPayment->date().toString(s.dateFormatExternal());
            summaryHTML += "</span></b>";
        }

        summaryHTML += "<span class=\"additionalText\">"	+ ui->lineEditAdditionalText->text() + "</span>";
        summaryHTML += "</td>";
        summaryHTML += "<td width=\"3%\">&nbsp;</td>";
        summaryHTML += "<td width=\"48%\" valign=\"top\">";

        summaryHTML += "<table width=\"90%\" border=\"0\">";
        summaryHTML += "<tr class=\"stawkiHeader\"><td colspan=\"4\">";
        summaryHTML += trUtf8("Ogółem stawkami:");
        summaryHTML += "</td></tr>";
        summaryHTML += getGroupedSums();
        summaryHTML += "<tr>";
        summaryHTML += "<td>&nbsp;</td>"; // netto
        summaryHTML += "<td>&nbsp;</td>"; // stawka
        summaryHTML += "<td>&nbsp;</td>"; // podatek
        summaryHTML += "<td>&nbsp;</td>"; // brutto
        summaryHTML += "</tr>";
        summaryHTML += "</table>";
        summaryHTML += "</td>";
        summaryHTML += "</tr>";
        summaryHTML += "</table>";
        summaryHTML += "</td></tr>";


        docHTML = invoiceHTMLTemplate.arg(s.value(s.LANG).toString())
                                  .arg(ui->comboBoxInvoiceType->currentText())
                                  .arg(styleCSS)
                                  .arg(stampStr)
                                  .arg(ui->comboBoxInvoiceType->currentText())
                                  .arg(trUtf8("Nr: %1").arg(ui->lineEditInvNumber->text()))
                                  .arg(trUtf8("Data wystawienia: %1").arg(ui->dateEditDateOfIssuance->date().toString(s.dateFormatExternal())))
                                  .arg(trUtf8("Data sprzedaży: %1").arg(ui->dateEditDateOfSell->date().toString(s.dateFormatExternal())))
                                  .arg(trUtf8("ORYGINAŁ"))
                                  .arg(sellerHTML)
                                  .arg(trUtf8("<h1>Nabywca:</h1><ul><li>%1</li></ul>").arg(ui->comboBoxCounterparties->currentText()) )
                                  .arg(productsHTML)
                                  .arg(trUtf8("Wartość Netto"))
                                  .arg(trUtf8("Kwota VAT"))
                                  .arg(trUtf8("Wartość Brutto"))
                                  .arg(trUtf8("Razem:"))
                                  .arg(ui->labelSumNetVal->text())
                                  .arg(s.numberToString(s.stringToDouble(ui->labelSumGrossVal->text()) - s.stringToDouble(ui->labelSumNetVal->text()), 'f', 2))
                                  .arg(ui->labelSumGrossVal->text())
                                  .arg(summaryHTML);

        file.setFileName("debugInvoice.html");
        file.open(QIODevice::WriteOnly);
        stream.setDevice(&file);
        stream << docHTML;
        file.close();

        QPrinter printer(QPrinter::HighResolution);
        QPrintPreviewDialog preview(&printer, this);
        preview.setWindowFlags(Qt::Window);
        preview.setWindowTitle(trUtf8("%1 - Podgląd wydruku").arg(ui->comboBoxInvoiceType->currentText()));
        connect(&preview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(printPaintRequested(QPrinter*)));
        preview.exec();
    }
    else
    {
        qDebug() << "File " << file.fileName() << " couldn't be opened.";
    }

    //invStrList.clear();
    //makeInvoiceHeaderHTML(comboBoxInvoiceType->currentIndex() + 1);

    //makeInvoiceHeader(comboBoxInvoiceType->currentIndex() + 1, true, false, true);
    //makeInvoiceBody();
    //makeInvoiceProducts();
    //makeInvoiceSumm();
    //makeInvoiceSummAll();
    //makeInvoiceFooter();

    const int numberOfCopies = s.value(s.NUMBER_OF_COPIES, 2).toInt();
    for (int i = 0; i < numberOfCopies; ++i)
    {
        // print copy
        //makeInvoiceHeader(comboBoxInvoiceType->currentIndex() + 1, true, true, false);
        //makeInvoiceBody();
        //makeInvoiceProducts();
        //makeInvoiceSumm();
        //makeInvoiceSummAll();
        //makeInvoiceFooter();
    }

    //makeInvoiceFooterHtml();
    //print();
    //pushButtonSave->setFocus();
}


/**
 * @brief
 *
 * @param isAllowed
 */
void InvoiceDialogImpl::setIsEditAllowed(const bool isAllowed)
{
    ui->lineEditInvNumber->setEnabled(isAllowed);
    ui->dateEditDateOfSell->setEnabled(isAllowed);
    ui->dateEditDateOfIssuance->setEnabled(isAllowed);
    ui->tableWidgetCommodities->setEnabled(isAllowed);
    ui->spinBoxDiscount->setEnabled(isAllowed);
    ui->labelDiscount1->setEnabled(isAllowed);
    ui->comboBoxPayment->setEnabled(isAllowed);
    ui->dateEditDayOfPayment->setEnabled(isAllowed);
    ui->lineEditAdditionalText->setEnabled(isAllowed);
    ui->pushButtonAddCommodity->setEnabled(isAllowed);
    ui->pushButtonRemoveCommodity->setEnabled(isAllowed);
    ui->pushButtonEditCommodity->setEnabled(isAllowed);
    ui->checkBoxDiscount->setEnabled(isAllowed);
    ui->comboBoxCounterparties->setEnabled(isAllowed);
    ui->comboBoxCurrency->setEnabled(isAllowed);
    ui->pushButtonSave->setEnabled(isAllowed);
    ui->pushButtonAddCounterparty->setEnabled(isAllowed);
    ui->dateEditDayOfPayment->setEnabled(isAllowed);
    if (!isAllowed || ui->spinBoxDiscount->value() == 0) {
        ui->checkBoxDiscount->setChecked(false);
        ui->spinBoxDiscount->setEnabled(false);
    } else {
        ui->checkBoxDiscount->setChecked(true);
        ui->spinBoxDiscount->setEnabled(true);
    }
    if (isAllowed && (ui->comboBoxPayment->currentIndex() > 0)) {
        ui->dateEditDayOfPayment->setEnabled(true);
    } else {
        ui->dateEditDayOfPayment->setEnabled(false);
    }
}


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
