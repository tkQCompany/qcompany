#ifndef SETTINGSGLOBAL_H
#define SETTINGSGLOBAL_H

#include <QSettings>
#include <QString>
#include <QLocale>

/**
 * @brief
 *
 */
class SettingsGlobal: public QSettings
{
    Q_OBJECT

public:
    enum KEYS {VAT_RATES, LANG, ADDIT_TEXT, EDIT, EDIT_NAME,
               EDIT_SYMBOL, NUMBER_OF_COPIES, TAXID_MASK, ACCOUNT_MASK, FIRST_RUN,
               UNITS, LOGO, PAYMENT_TYPE, CORRECTION_REASONS, ORDER_NUMBER,
               NAME, CODE, PKWIU, QUANTITY, INTERNAT_UNIT,
               UNIT_PRICE, NET_VAL, DISCOUNT, DISCOUNT_VAL, NET_AFTER,
               VAT_VAL, VAT_PRICE, GROSS_VAL, SELLER_NAME, SELLER_LOCATION,
               SELLER_ADDRESS, SELLER_ACCOUNT, SELLER_TAXID, SELLER_PHONE, SELLER_MAIL,
               SELLER_WWW, CSS, DEFAULT_INV_NUM_FORMAT, DEFAULT_CURRENCY,
               COUNTRY, TEXT1, TEXT2, TEXT3,
               LAST_UPDATE_EXCHANGE_RATES, LAST_UPDATE_EXCHANGE_RATES_CENTRAL_BANK};

    SettingsGlobal();
    static const QString keyName(const int key);

    /** Reset all settings to default values
     */
    /**
     * @brief
     *
     */
    void resetSettings();

    /**
     * @brief
     *
     * @return QString
     */
    const QString dateFormatExternal() const
    {
        return dateFormatExternal_;
    }


    /**
     * @brief
     *
     * @return QString
     */
    const QString dateFormatInternal() const
    {
        return dateFormatInternal_;
    }


    /**
     * @brief
     *
     * @return QString
     */
    const QString decimalPointStr() const
    {
        return QString(locale_.decimalPoint());
    }

    /**
     * @brief
     *
     * @return QString
     */
    const QString tPointStr() const
    {
        return QString(locale_.groupSeparator());
    }



    using QSettings::value;

    const QVariant value(KEYS key, const QVariant &default_val = QVariant()) const
    {
        return QSettings::value(keyName(key), default_val);
    }

    /**
     * @brief
     *
     * @param i
     * @param f
     * @param prec
     * @return QString
     */
    const QString numberToString(const double i, const char f, const int prec) const
    {
        return locale_.toString(i, f, prec);
    }
    /**
     * @brief
     *
     * @param i
     * @return QString
     */
    const QString numberToString(const int i) const
    {
        return locale_.toString(i);
    }

    /**
     * @brief
     *
     * @param s
     * @return double
     */
    double stringToDouble(const QString &s) const
    {
        return locale_.toDouble(s);
    }

    bool firstRun() const
    {
        return value(keyName(FIRST_RUN)).toBool();
    }

    void setFirstRun(const bool firstRun)
    {
        setValue(keyName(FIRST_RUN), firstRun);
    }

private:
    const QString dateFormatInternal_; /**< TODO */
    const QString dateFormatExternal_; /**< TODO */
    QLocale locale_; /**< TODO */
};

#endif
