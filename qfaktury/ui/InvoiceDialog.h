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

    void printPaintRequested_(QPrinter *printer);
    /**
     * @brief
     *
     * @param item
     */
    void tableActivated_(QTableWidgetItem *item);

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
    virtual void updateInvoiceNumberFormat();
    /**
     * @brief
     *
     */
    virtual void printInvoice();
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
    //virtual void makeInvoiceBody();
    /**
     * @brief
     *
     */
    //void makeInvoiceFooter();
    /**
     * @brief
     *
     */
    //virtual void makeInvoiceFooterHtml();
    /**
     * @brief
     *
     * @param bool
     * @param bool
     * @param bool
     */
    //virtual void makeInvoiceHeader(const int invoiceType, const bool, const bool, const bool);
    /**
     * @brief
     *
     */
    //virtual void makeInvoiceHeaderHTML(const int invoiceType);
    /**
     * @brief
     *
     */
    //virtual void makeInvoiceProducts();
    /**
     * @brief
     *
     */
    //virtual void makeInvoiceProductsHeader();
    /**
     * @brief
     *
     */
    //virtual void makeInvoiceSumm();
    /**
     * @brief
     *
     */
    //virtual void makeInvoiceSummAll();

    /**
     * @brief
     *
     * @param bool
     */
    virtual void setIsEditAllowed(const bool);
protected:
    bool unsaved, saveFailed; /**< TODO */
    CustomPaymData *custPaymData; /**< TODO */
    double discountTotal, netTotal, grossTotal; /**< TODO */
    QString docHTML;
    Database *db_; /**< TODO */

private:
    /**
     * @brief
     *
     */
    void init_();
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
