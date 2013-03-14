#ifndef MODELCURRENCY_H
#define MODELCURRENCY_H

#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QDate>
#include <QRegExp>
#include <QStringList>
#include <QDomDocument>

#include "../models_data/CurrencyData.h"

/**
 * @brief
 *
 */
class ModelCurrency : public QSqlTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelCurrency(QObject *parent);

    void updateCurrenciesRates();

signals:
    void updatingCurrenciesRatesFinished();

private slots:
    void netReplyFinished_();
    void updateDBsCurrenciesRates_();

private:
    QNetworkAccessManager *netMgr_;
};

#endif // MODELCURRENCY_H
