#ifndef DATABASE_H
#define DATABASE_H

#include <QMessageBox>
#include <QCoreApplication>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "models/ModelCommodity.h"
#include "models/ModelCommodityType.h"
#include "models/ModelCounterparty.h"
#include "models/ModelCounterpartyType.h"
#include "models/ModelCountry.h"
#include "models/ModelCurrency.h"
#include "models/ModelEmail.h"
#include "models/ModelInvoice.h"
#include "models/ModelInvoiceType.h"
#include "models/ModelInvoiceWithCommodities.h"
#include "models/ModelPaymentType.h"
#include "models/ModelPhone.h"
#include "models/ModelUnit.h"
#include "models/ModelVat.h"
#include "../../ui/CommodityVisualData.h"

/**
 * @brief
 *
 */
class Database: public QObject
{
    Q_DECLARE_TR_FUNCTIONS(Database)
public:
/**
 * @brief
 *
 * @param parent
 */
    Database(QObject *parent = 0);
    /**
     * @brief
     *
     */
    ~Database();

    const QString dbFileName() const;

    /**
     * @brief
     *
     * @return ModelCommodity
     */
    ModelCommodity* modelCommodity() const;
    /**
     * @brief
     *
     * @return ModelCommodityType
     */
    ModelCommodityType* modelCommodityType() const;
    /**
     * @brief
     *
     * @return ModelCounterparty
     */
    ModelCounterparty* modelCounterparty() const;
    /**
     * @brief
     *
     * @return ModelCounterpartyType
     */
    ModelCounterpartyType* modelCounterpartyType() const;
    /**
     * @brief
     *
     * @return ModelCountry
     */
    ModelCountry* modelCountry() const;
    /**
     * @brief
     *
     * @return ModelCurrency
     */
    ModelCurrency* modelCurrency() const;
    /**
     * @brief
     *
     * @return ModelEmail
     */
    ModelEmail* modelEmail() const;
    /**
     * @brief
     *
     * @return ModelInvoice
     */
    ModelInvoice* modelInvoice() const;
    /**
     * @brief
     *
     * @return ModelInvoiceType
     */
    ModelInvoiceType* modelInvoiceType() const;
    /**
     * @brief
     *
     * @return ModelInvoiceWithCommodities
     */
    ModelInvoiceWithCommodities* modelInvoiceWithCommodities() const;
    /**
     * @brief
     *
     * @return ModelPaymentType
     */
    ModelPaymentType* modelPaymentType() const;
    /**
     * @brief
     *
     * @return ModelPhone
     */
    ModelPhone* modelPhone() const;
    /**
     * @brief
     *
     * @return ModelUnit
     */
    ModelUnit* modelUnit() const;
    /**
     * @brief
     *
     * @return ModelVat
     */
    ModelVat* modelVat() const;

    /**
     * @brief
     *
     * @param id_invoice
     * @return QList<CommodityVisualData>
     */
    QList<CommodityVisualData> commodities(const qint64 id_invoice);
    /**
     * @brief
     *
     * @param invoice
     * @param commodities
     * @return bool
     */
    bool invoiceWithCommoditiesInsertTransact(const InvoiceData &invoice, const QList<CommodityVisualData> &commodities);
    /**
     * @brief
     *
     * @param id_invoice
     * @return bool
     */
    bool invoiceWithCommoditiesDeleteTransact(const qint64 &id_invoice);

private:
    /**
     * @brief
     *
     * @return bool
     */
    bool createTablesIfNotExist_();

    /**
     * @brief
     *
     */
    void initModels_();

    /**
     * @brief
     *
     * @return bool
     */
    bool insertDataIfNotInserted_();

    /**
     * @brief
     *
     * @param sqlQuery
     * @param line
     * @return bool
     */
    bool sqlExecute_(const QString &sqlQuery, const int line);
    /**
     * @brief
     *
     * @param table
     * @param column
     * @param condition
     * @param line
     * @return bool
     */
    bool sqlInsertIf_(const QString &table, const QString &column, const QString &condition, const int line);
private:
    QSqlDatabase db_; /**< TODO */
    ModelCommodity *modelCommodity_; /**< TODO */
    ModelCommodityType *modelCommodityType_; /**< TODO */
    ModelCounterparty *modelCounterparty_; /**< TODO */
    ModelCounterpartyType *modelCounterpartyType_; /**< TODO */
    ModelCountry *modelCountry_; /**< TODO */
    ModelCurrency *modelCurrency_; /**< TODO */
    ModelEmail *modelEmail_; /**< TODO */
    ModelInvoice *modelInvoice_; /**< TODO */
    ModelInvoiceType *modelInvoiceType_; /**< TODO */
    ModelInvoiceWithCommodities *modelInvoiceWithCommodities_; /**< TODO */
    ModelPaymentType *modelPaymentType_; /**< TODO */
    ModelPhone *modelPhone_; /**< TODO */
    ModelUnit *modelUnit_; /**< TODO */
    ModelVat *modelVat_; /**< TODO */
};

#endif // DATABASE_H
