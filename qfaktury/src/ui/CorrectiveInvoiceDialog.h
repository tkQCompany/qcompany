#ifndef CORRECTIVEINVOICEDIALOG_H
#define CORRECTIVEINVOICEDIALOG_H

#include <QDialog>
#include <QModelIndex>

#include "InvoiceTypeData.h"

class Database;

/**
 * @brief
 *
 */
class CorrectiveInvoiceDialog: public QDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param
 * @param db
 */
    CorrectiveInvoiceDialog(QWidget *parent, Database *db, InvoiceTypeData::Type invoiceType, const QModelIndex &idInvoice = QModelIndex());
    /**
     * @brief
     *
     */
    virtual ~CorrectiveInvoiceDialog();

protected slots:
    /**
     * @brief
     *
     */
    //void printInvoice();
    /**
     * @brief
     *
     */
    void canQuit();
protected:
    /**
     * @brief
     *
     * @param bool
     */
    void setIsEditAllowed(bool);
    /**
     * @brief
     *
     */
    void calculateDiscount();
    /**
     * @brief
     *
     */
    void calculateSum();
    /**
     * @brief
     *
     * @param int
     */
    void calculateOneDiscount(const int);

    /**
     * @brief
     *
     */
    //void makeInvoiceSummAll();
    /**
     * @brief
     *
     */
    //void makeInvoiceSumm();
private:
    /**
     * @brief
     *
     */
    void createOriginalInv();
    /**
     * @brief
     *
     */
    //void makeBeforeCorrProducts();

    /**
     * @brief
     *
     */
    void init_();

    /**
     * @brief
     *
     */
    //void makeBeforeCorrSumm();

protected:
    class CorrectiveInvoiceDialogImpl;
    CorrectiveInvoiceDialogImpl *pImpl_;
    double origDiscTotal, origNettTotal, origGrossTotal, diffTotal; /**< TODO */
};
#endif
