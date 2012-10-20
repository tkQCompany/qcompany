#ifndef CURRENCYDATA_H
#define CURRENCYDATA_H

namespace CurrencyFields
{
/**
 * @brief
 *
 */
enum Fields {ID_CURRENCY, NAME};
}

/**
 * @brief
 *
 */
struct CurrencyData
{
    /**
     * @brief
     *
     */
    enum Currencies {EUR = 1, PLN, USD}; // "= 1" because SQL starts counting from 1

    /**
     * @brief
     *
     * @param currency
     * @return QString
     */
    static QString name(const int currency)
    {
        switch(currency)
        {
        case EUR: return QObject::trUtf8("EUR");
        case PLN: return QObject::trUtf8("PLN");
        case USD: return QObject::trUtf8("USD");
        default:
            qDebug("Unknown case in CurrencyData::name()");
            return QString();
        }
    }
};

#endif // CURRENCYDATA_H
