#ifndef MODELINVOICE_H
#define MODELINVOICE_H

#include <QSqlRelationalTableModel>
#include <QDate>
#include <QString>
#include <memory>

#include "InvoiceTypeData.h"
#include "InvoiceNumberFormat_t.h"

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
    QString generateInvoiceNumber(const InvoiceNumberFormat_t& invoiceNumFormat, const QString &prevInvNum, const QDate &issuanceDate, const QDate &prevIssuanceDate, const InvoiceTypeData::Type invoiceType) const;
    QString getInvoiceNumberFormat(const QString &counterpartyName) const;
    struct DBData
    {
        QDate gotIssuanceDate;
        QString invNumStr;
    };
    std::unique_ptr<DBData> getLastExistingNumberDateFromDB(const bool defaultInvNumFormat, const QString &counterpartyName) const;
    void setDataRange(const QDate &from, const QDate &to);
    QStringList simulateConsecutiveInvoiceNumbers(const InvoiceNumberFormat_t &invoiceNumFormat, const QDate &firstIssuanceDate, const InvoiceTypeData::Type invoiceType, const int invNumCounts = 1) const;

private:
    static long increaseNumber_(const InvoiceNumberFormat_t& invoiceNumFormat, const QString &prevInvNum, const QDate &issuanceDate, const QDate &prevIssuanceDate, const InvoiceNumberFormat_t::Field periodId, const int position);
};

#endif // MODELINVOICE_H
