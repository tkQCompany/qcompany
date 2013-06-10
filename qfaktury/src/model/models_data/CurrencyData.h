#ifndef CURRENCYDATA_H
#define CURRENCYDATA_H

namespace CurrencyFields
{
/**
 * @brief
 *
 */
enum Fields {ID_CURRENCY, CODE, CODE_UNIT, EXCHANGE_RATE_PLN, LOCALIZED_NAME};
}

class CurrencyData
{
public:
enum Currencies{AUD, BGN, BRL, CAD, CHF, CNY, CZK, DKK, EUR, GBP, HKD, HRK, ILS,
                LTL, LVL, MXN, MYR, NOK, NZD, PHP, PLN, RON, RUB, SEK, SGD, THB,
                TRY, UAH, USD, XDR, ZAR, CLP, HUF, INR, ISK, JPY, KRW, IDR};

static QString codeName(const Currencies currency_code)
{
    switch(currency_code)
    {
    case AUD: return QString("AUD");
    case BGN: return QString("BGN");
    case BRL: return QString("BRL");
    case CAD: return QString("CAD");
    case CHF: return QString("CHF");
    case CNY: return QString("CNY");
    case CZK: return QString("CZK");
    case DKK: return QString("DKK");
    case EUR: return QString("EUR");
    case GBP: return QString("GBP");
    case HKD: return QString("HKD");
    case HRK: return QString("HRK");
    case ILS: return QString("ILS");
    case LTL: return QString("LTL");
    case LVL: return QString("LVL");
    case MXN: return QString("MXN");
    case MYR: return QString("MYR");
    case NOK: return QString("NOK");
    case NZD: return QString("NZD");
    case PHP: return QString("PHP");
    case PLN: return QString("PLN");
    case RON: return QString("RON");
    case RUB: return QString("RUB");
    case SEK: return QString("SEK");
    case SGD: return QString("SGD");
    case THB: return QString("THB");
    case TRY: return QString("TRY");
    case UAH: return QString("UAH");
    case USD: return QString("USD");
    case XDR: return QString("XDR");
    case ZAR: return QString("ZAR");
    case CLP: return QString("CLP");
    case HUF: return QString("HUF");
    case INR: return QString("INR");
    case ISK: return QString("ISK");
    case JPY: return QString("JPY");
    case KRW: return QString("KRW");
    case IDR: return QString("IDR");
    default:
        qDebug("Bad currency_code in CurrencyData::codeName(): %d\n", currency_code);
    }
    return QString();
}


static QString name(const Currencies currency_code)
{
    switch(currency_code)
    {
    case AUD: return QObject::trUtf8("dolar australijski");
    case BGN: return QObject::trUtf8("lew bułgarski");
    case BRL: return QObject::trUtf8("real brazylijski");
    case CAD: return QObject::trUtf8("dolar kanadyjski");
    case CHF: return QObject::trUtf8("frank szwajcarski");
    case CNY: return QObject::trUtf8("yuan renminbi (Chiny)");
    case CZK: return QObject::trUtf8("korona czeska");
    case DKK: return QObject::trUtf8("korona duńska");
    case EUR: return QObject::trUtf8("euro");
    case GBP: return QObject::trUtf8("funt szterling");
    case HKD: return QObject::trUtf8("dolar Hongkongu");
    case HRK: return QObject::trUtf8("kuna chorwacka");
    case ILS: return QObject::trUtf8("nowy izraelski szekel");
    case LTL: return QObject::trUtf8("lit litewski");
    case LVL: return QObject::trUtf8("łat łotewski");
    case MXN: return QObject::trUtf8("peso meksykańskie");
    case MYR: return QObject::trUtf8("ringgit malezyjski");
    case NOK: return QObject::trUtf8("korona norweska");
    case NZD: return QObject::trUtf8("dolar nowozelandzki");
    case PHP: return QObject::trUtf8("peso filipinskie");
    case PLN: return QObject::trUtf8("złoty polski");
    case RON: return QObject::trUtf8("lej rumuński");
    case RUB: return QObject::trUtf8("rubel rosyjski");
    case SEK: return QObject::trUtf8("korona szwedzka");
    case SGD: return QObject::trUtf8("dolar singapurski");
    case THB: return QObject::trUtf8("bat (Tajlandia)");
    case TRY: return QObject::trUtf8("lira turecka");
    case UAH: return QObject::trUtf8("hrywna (Ukraina)");
    case USD: return QObject::trUtf8("dolar amerykański");
    case XDR: return QObject::trUtf8("SDR (MFW)");
    case ZAR: return QObject::trUtf8("rand (RPA)");
    case CLP: return QObject::trUtf8("peso chilijskie");
    case HUF: return QObject::trUtf8("forint (Węgry)");
    case INR: return QObject::trUtf8("rupia indyjska");
    case ISK: return QObject::trUtf8("korona islandzka");
    case JPY: return QObject::trUtf8("jen (Japonia)");
    case KRW: return QObject::trUtf8("won (Korea Południowa)");
    case IDR: return QObject::trUtf8("rupia (Indonezja)");
    default:
        qDebug("Bad currency_code in CurrencyData::codeName(): %d\n", currency_code);
    }
    return QString();
}

};

Q_DECLARE_METATYPE(CurrencyData::Currencies)

#endif // CURRENCYDATA_H
