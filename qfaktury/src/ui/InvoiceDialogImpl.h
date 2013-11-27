#ifndef INVOICEDIALOGIMPL_H
#define INVOICEDIALOGIMPL_H

#include <QDataWidgetMapper>
#include <QDialog>
#include <QPointer>

#include "InvoiceData.h"
#include "InvoiceTypeData.h"
#include "CommodityVisualData.h"
#include "InvoiceNumberFormat_t.h"


class QTableWidgetItem;
class CustomPaymData;
class CommodityDialog;
class CommodityListDialog;
class CounterpartyDialog;
class QPrinter;

namespace Ui
{
class InvoiceDialog;
}


class Database;

class InvoiceDialogImpl: public QObject
{
    Q_OBJECT

public:
    InvoiceDialogImpl(QWidget *parent, Database *database);
    virtual ~InvoiceDialogImpl();

    /**
     * @brief
     *
     */
    void calculateSum();

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
    void init(InvoiceTypeData::Type invoiceType, const QModelIndex &idEdit);

    /**
     * @brief
     *
     * @param commodities
     */
    void fillTableCommodity(const QList<CommodityVisualData> &commodities);
    /**
     * @brief
     *
     * @return InvoiceData
     */
    InvoiceData getInvoiceData() const;
    /**
     * @brief
     *
     * @return QList<CommodityVisualData>
     */
    QList<CommodityVisualData> getCommoditiesVisualData() const;
    /**
     * @brief
     *
     */
    void setInitialComboBoxIndexes(const QString &invoiceType,
                                    const QString &paymentType,
                                    const QString &defaultCurrency);

    void setHeaders();

    /**
     * @brief
     *
     * @return bool
     */
    bool validateForm();

    //virtual void setIsEditAllowed(const bool);

public slots:
    /**
     * @brief
     *
     */
    void counterpartyAdd();

    /**
     * @brief
     *
     */
    void counterpartyMoreInfo();

    /**
     * @brief
     *
     * @param QDate
     */
    void dateChanged(QDate);
    /**
     * @brief
     *
     */
    void delCommodity();
    /**
     * @brief
     *
     */
    void editCommodity();

    void printPaintRequested(QPrinter *printer);
    void retranslateUi();

    /**
     * @brief
     *
     */
    virtual void addCommodity();

    /**
     * @brief
     *
     */
    virtual void addNewCommodity();
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

    /**
     * @brief
     *
     * @param item
     */
    void tableActivated(QTableWidgetItem *item);

    void updateInvoiceNumber();
    void updateInvoiceNumberFormat();

public:
    QDialog *parent_;
    Ui::InvoiceDialog *ui;
    Database *db; /**< TODO */
    QDataWidgetMapper mapper; /**< TODO */
    CustomPaymData *custPaymDataPtr; /**< TODO */
    QPointer<CommodityDialog> commodityDialogPtr;
    QPointer<CommodityListDialog> commodityListDialogPtr;
    QPointer<CounterpartyDialog> counterpartyDialogPtr;
    bool isLoaded_;
    double discountTotal, netTotal, grossTotal; /**< TODO */
    QString docHTML;
};

#endif // INVOICEDIALOGIMPL_H
