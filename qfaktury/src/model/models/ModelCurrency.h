#ifndef MODELCURRENCY_H
#define MODELCURRENCY_H

#include <QSqlTableModel>
#include <QDate>


class QNetworkAccessManager;

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

    const QDate lastUpdateByCentralBank() const {return lastUpdateByCentralBank_;}
    void updateCurrenciesRates();

signals:
    void updatingCurrenciesRatesFinished();

private slots:
    void netReplyFinished_();
    void updateDBsCurrenciesRates_();

private:
    QDate lastUpdateByCentralBank_;
    QNetworkAccessManager *netMgr_;
};

#endif // MODELCURRENCY_H
