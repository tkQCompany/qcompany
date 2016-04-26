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
    enum CATEGORY_OF_KEYS {INVOICE_FIELDS, PRINT_FIELDS};

    enum KEYS {VAT_RATES, LANG, ADDIT_TEXT, EDIT, EDIT_NAME,
               EDIT_SYMBOL, NUMBER_OF_COPIES, TAXID_MASK, ACCOUNT_NUM_MASK, FIRST_RUN,
               UNITS, LOGO, PAYMENT_TYPE, CORRECTION_REASONS, ORDER_NUMBER,
               NAME, CODE, PKWIU, QUANTITY, INTERNAT_UNIT,
               UNIT_PRICE, NET_VAL, DISCOUNT, DISCOUNT_VAL, NET_AFTER,
               VAT_VAL, VAT_PRICE, GROSS_VAL, DISPLAY_SELLER_NAME, DISPLAY_SELLER_LOCATION,
               DISPLAY_SELLER_ADDRESS, DISPLAY_SELLER_ACCOUNT, DISPLAY_SELLER_TAXID, DISPLAY_SELLER_PHONE, DISPLAY_SELLER_MAIL,
               DISPLAY_SELLER_WWW, DISPLAY_BUYER_NAME, DISPLAY_BUYER_LOCATION,
               DISPLAY_BUYER_ADDRESS, DISPLAY_BUYER_ACCOUNT, DISPLAY_BUYER_TAXID, DISPLAY_BUYER_PHONE, DISPLAY_BUYER_MAIL,
               DISPLAY_BUYER_WWW, CSS, DEFAULT_INV_NUM_FORMAT, DEFAULT_CURRENCY,
               COUNTRY, TEXT1, TEXT2, TEXT3,
               LAST_UPDATE_EXCHANGE_RATES, LAST_UPDATE_EXCHANGE_RATES_CENTRAL_BANK};

    SettingsGlobal();
    static const QString categoryName(const CATEGORY_OF_KEYS key);
    static const QString keyName(const KEYS key);

    /**
     * @brief Reset all settings to default values
     *
     */
    void resetSettings();

    /**
     * @brief Getter for the current date format used for dates presented to the user.
     *
     * @return QString Current date format.
     */
    const QString dateFormatExternal() const
    {
        return dateFormatExternal_;
    }


    /**
     * @brief Getter for the current date format used for dates NOT presented to the user.
     *
     * @return QString Current date format.
     */
    const QString dateFormatInternal() const
    {
        return dateFormatInternal_;
    }


    /**
     * @brief Returns decimal point.
     *
     * @return QChar
     */
    const QChar decimalPoint() const
    {
        return locale_.decimalPoint();
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
