#ifndef INVOICEDIALOG_H
#define INVOICEDIALOG_H

#include <QDataWidgetMapper>
#include <QProcess>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QTextDocument>
#include <QTranslator>
#include <QDebug>

#include "CounterpartyInfoDialog.h"
#include "CommodityListDialog.h"
#include "ChangeAmountDialog.h"
#include "../src/ConvertAmount.h"
#include "CustomPaymentDialog.h"
#include "CounterpartyDialog.h"
#include "ui_InvoiceDialog.h"
#include "../src/model/Database.h"
#include "InvoiceNumberFormatEditDialog.h"


/**
 * @brief
 *
 */
class InvoiceDialog: public QDialog, public Ui::InvoiceDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param parent
 * @param db
 * @param id_edit
 */
    InvoiceDialog(QWidget *parent, Database *db, const QModelIndex &id_edit = QModelIndex());
    /**
     * @brief
     *
     */
    virtual ~InvoiceDialog();

    /**
     * @brief
     *
     */
    virtual void init_();
    /**
     * @brief
     *
     * @param bool
     */
    virtual void setIsEditAllowed(const bool);

public:    
    InvoiceData ret; /**< TODO */

signals:
    void genInvoiceNumberParamsChanged();

private slots:
    /**
     * @brief
     *
     */
    void counterpartyAdd_();
    /**
     * @brief
     *
     * @param QDate
     */
    void dateChanged_ (QDate);
    /**
     * @brief
     *
     */
    void delCommodity_();
    /**
     * @brief
     *
     */
    void editCommodity_();
    /**
     * @brief
     *
     */
    void counterpartyMoreInfo_();
    /**
     * @brief
     *
     * @param item
     */
    void tableActivated_(QTableWidgetItem *item);
    /**
     * @brief
     *
     * @param
     */
    void printSlot_(QPrinter*) const;

    void retranslateUi_();

protected slots:    
    /**
     * @brief
     *
     */
    virtual void addCommodity();
    /**
     * @brief
     *
     */
    virtual void canQuit();

    /**
     * @brief
     *
     */
    virtual void discountChange();
    /**
     * @brief
     *
     */
    virtual void discountConstChange();

    virtual void updateInvoiceNumber();
    /**
     * @brief
     *
     */
    virtual void makeInvoice();
    /**
     * @brief
     *
     * @param QString
     */
    virtual void payTextChanged(QString);
    /**
     * @brief
     *
     * @return bool
     */
    virtual bool saveInvoice();
    /**
     * @brief
     *
     * @param QString
     */
    virtual void textChanged(QString);
protected:
    /**
     * @brief
     *
     */
    virtual void calculateSum();

    /**
     * @brief
     *
     * @return QString
     */
    QString getGroupedSums();
    /**
     * @brief
     *
     */
    virtual void makeInvoiceBody();
    /**
     * @brief
     *
     */
    void makeInvoiceFooter();
    /**
     * @brief
     *
     */
    virtual void makeInvoiceFooterHtml();
    /**
     * @brief
     *
     * @param bool
     * @param bool
     * @param bool
     */
    virtual void makeInvoiceHeader(const bool, const bool, const bool);
    /**
     * @brief
     *
     */
    virtual void makeInvoiceHeaderHTML();
    /**
     * @brief
     *
     */
    virtual void makeInvoiceProducts();
    /**
     * @brief
     *
     */
    virtual void makeInvoiceProductsHeader();
    /**
     * @brief
     *
     */
    virtual void makeInvoiceSumm();
    /**
     * @brief
     *
     */
    virtual void makeInvoiceSummAll();


    /**
     * @brief
     *
     */
    virtual void print();
protected:
    bool unsaved, isEdit, saveFailed; /**< TODO */
    InvoiceTypeData::Type invType; /**< TODO */
    CustomPaymData *custPaymData; /**< TODO */
    double discountTotal, netTotal, grossTotal; /**< TODO */
    QStringList invStrList; /**< TODO */
    Database *db_; /**< TODO */

private:
    /**
     * @brief
     *
     * @param commodities
     */
    void fillTableCommodity_(const QList<CommodityVisualData> &commodities);
    void genInvoiceNumber_(const QString& invNumFormat, const QDate& issuanceDate, const int invoiceType, const QString& counterpartyName);
    /**
     * @brief
     *
     * @return InvoiceData
     */
    InvoiceData getInvoiceData_() const;
    /**
     * @brief
     *
     * @return QList<CommodityVisualData>
     */
    QList<CommodityVisualData> getCommoditiesVisualData_() const;
    /**
     * @brief
     *
     */
    void setInitialComboBoxIndexes_();
    /**
     * @brief
     *
     * @return bool
     */
    bool validateForm_();
private:
    QDataWidgetMapper mapper_; /**< TODO */
};
#endif
