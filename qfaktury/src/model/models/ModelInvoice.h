#ifndef MODELINVOICE_H
#define MODELINVOICE_H

#include <QSqlRelationalTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegExp>

#include "../models_data/InvoiceData.h"
#include "../models_data/InvoiceTypeData.h"
#include "../models_data/InvoiceNumberFormatData.h"

/**
 * @brief
 *
 */
class ModelInvoice : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelInvoice(QObject *parent);
    /**
     * @brief
     *
     * @param section
     * @param orientation
     * @param role
     * @return QVariant
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /**
     * @brief
     *
     * @return QString
     */
    QString generateInvoiceNumber(const QString& invoiceNumFormat, const QDate &issuanceDate, const QString& invoiceTypeName, const QString& counterpartyName = QString()) const;
    QString getInvoiceNumberFormat(const QString &counterpartyName) const;
    void setDataRange(const QDate &from, const QDate &to);

private:
    QString getNextInvNumberFromDB_(const QString& invoiceNumFormat, const QDate &issuanceDate, const int periodId, const int periodLocationInFormat, const QString& counterpartyName = QString()) const;
    static QString increaseField_(const QString &invNumber, const QString& regExpStr, const int numberLocationInFormat, const int increase);
};

#endif // MODELINVOICE_H
