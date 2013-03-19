#ifndef SETTINGSGLOBAL_H
#define SETTINGSGLOBAL_H

#include <QSettings>
#include <QDate>
#include <QDir>
#include <QString>
#include <QLocale>
#include <QDebug>


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
               UNITS, LOGO, PAYMENT_TYPE, CORRECTION_REASON, ORDER_NUMBER,
               NAME, CODE, PKWIU, QUANTITY, INTERNAT_UNIT,
               UNIT_PRICE, NET_VAL, DISCOUNT, DISCOUNT_VAL, NET_AFTER,
               VAT_VAL, VAT_PRICE, GROSS_VAL, SELLER_NAME, SELLER_LOCATION,
               SELLER_ADDRESS, SELLER_ACCOUNT, SELLER_TAXID, SELLER_PHONE, SELLER_MAIL,
               SELLER_WWW, WORKING_DIR, CSS, DEFAULT_INV_NUM_FORMAT, DEFAULT_CURRENCY,
               COUNTRY, TEXT1, TEXT2, TEXT3,
               LAST_UPDATE_EXCHANGE_RATES, LAST_UPDATE_EXCHANGE_RATES_CENTRAL_BANK};

    SettingsGlobal() : dateFormatInternal_("yyyy-MM-dd"),
        dateFormatExternal_("dd/MM/yyyy")
    {
    }

    static const QString keyName(const int key)
    {
        switch(key)
        {
        case VAT_RATES: return QString("stawki");
        case LANG: return QString("lang");
        case ADDIT_TEXT: return QString("addText");
        case EDIT: return QString("edit");
        case EDIT_NAME: return QString("editName");
        case EDIT_SYMBOL: return QString("editSymbol");
        case NUMBER_OF_COPIES: return QString("numberOfCopies");
        case TAXID_MASK: return QString("nipMask");
        case ACCOUNT_MASK: return QString("accountMask");
        case FIRST_RUN: return QString("firstrun");
        case UNITS: return QString("jednostki");
        case LOGO: return QString("logo");
        case PAYMENT_TYPE: return QString("payments");
        case CORRECTION_REASON: return QString("pkorekty");
        case ORDER_NUMBER: return QString("faktury_pozycje/Lp");
        case NAME: return QString("faktury_pozycje/Nazwa");
        case CODE: return QString("faktury_pozycje/Kod");
        case PKWIU: return QString("faktury_pozycje/pkwiu");
        case QUANTITY: return QString("faktury_pozycje/ilosc");
        case INTERNAT_UNIT: return QString("faktury_pozycje/jm");
        case UNIT_PRICE: return QString("faktury_pozycje/cenajedn");
        case NET_VAL: return QString("faktury_pozycje/wartnetto");
        case DISCOUNT: return QString("faktury_pozycje/rabatperc");
        case DISCOUNT_VAL: return QString("faktury_pozycje/rabatval");
        case NET_AFTER: return QString("faktury_pozycje/nettoafter");
        case VAT_VAL: return QString("faktury_pozycje/vatval");
        case VAT_PRICE: return QString("faktury_pozycje/vatprice");
        case GROSS_VAL: return QString("faktury_pozycje/bruttoval");
        case SELLER_NAME: return QString("printpos/display_seller_name");
        case SELLER_LOCATION: return QString("printpos/display_seller_location");
        case SELLER_ADDRESS: return QString("printpos/display_seller_address");
        case SELLER_ACCOUNT: return QString("printpos/display_seller_account");
        case SELLER_TAXID: return QString("printpos/display_seller_taxid");
        case SELLER_PHONE: return QString("printpos/display_seller_phone");
        case SELLER_MAIL: return QString("printpos/display_seller_email");
        case SELLER_WWW: return QString("printpos/display_seller_www");
        case WORKING_DIR: return QString("working_dir");
        case CSS: return QString("css");
        case DEFAULT_INV_NUM_FORMAT: return QString("default_inv_num_format");
        case DEFAULT_CURRENCY: return QString("default_currency");
        case COUNTRY: return QString("country");
        case TEXT1: return QString("inv_num_text1");
        case TEXT2: return QString("inv_num_text2");
        case TEXT3: return QString("inv_num_text3");
        case LAST_UPDATE_EXCHANGE_RATES: return QString("last_update_exchange_rates");
        case LAST_UPDATE_EXCHANGE_RATES_CENTRAL_BANK: return QString("last_update_exchange_rates_central_bank");
        default:
            qDebug() << "SettingsGlobal::keyName(): improper value of the argument key: " << key;
            return QString();
        }
    }

public:
    /** Reset all settings to default values
     */
    /**
     * @brief
     *
     */
    void resetSettings() //TODO: make sure it won't damage sth (like individual invoice numbering scheme)
    {
        setValue(keyName(LANG), trUtf8("pl"));
        setValue(keyName(ADDIT_TEXT), trUtf8("Towar odebrałem zgodnie z fakturą"));
        setValue(keyName(EDIT), "false");
        setValue(keyName(EDIT_NAME), "false");
        setValue(keyName(EDIT_SYMBOL), "false");
        setValue(keyName(NUMBER_OF_COPIES), 1);
        setValue(keyName(TAXID_MASK), "999-999-99-99;");
        setValue(keyName(ACCOUNT_MASK), "99-9999-9999-9999-9999-9999-9999;");
        setValue(keyName(FIRST_RUN), false);
        setValue(keyName(UNITS), trUtf8("szt.|kg.|g.|m.|km.|godz."));
        setValue(keyName(LOGO), "");
        setValue(keyName(PAYMENT_TYPE), trUtf8("gotówka|przelew|zaliczka"));
        setValue(keyName(CORRECTION_REASON), trUtf8("zmiana ilości") );
        setValue(keyName(VAT_RATES), trUtf8("23|8|5|0|zw."));
        setValue(keyName(WORKING_DIR), QString("%1/elinux").arg(QDir::homePath()));
        setValue(keyName(CSS), QString("style.css"));
        setValue(keyName(DEFAULT_INV_NUM_FORMAT), trUtf8("{TEKST1}/{R}-{M}-{D}/{NR_R}"));
        setValue(keyName(DEFAULT_CURRENCY), trUtf8("PLN"));
        setValue(keyName(COUNTRY), trUtf8("Polska"));
        setValue(keyName(TEXT1), trUtf8("F"));
        setValue(keyName(TEXT2), trUtf8(""));
        setValue(keyName(TEXT3), trUtf8(""));
        setValue(keyName(LAST_UPDATE_EXCHANGE_RATES), QString(""));
        setValue(keyName(LAST_UPDATE_EXCHANGE_RATES_CENTRAL_BANK), QString(""));

        setValue(keyName(ORDER_NUMBER), true);
        setValue(keyName(NAME), true);
        setValue(keyName(CODE), true);
        setValue(keyName(PKWIU), true);
        setValue(keyName(QUANTITY), true);
        setValue(keyName(INTERNAT_UNIT), true);
        setValue(keyName(UNIT_PRICE), true);
        setValue(keyName(NET_VAL), true);
        setValue(keyName(DISCOUNT), true);
        setValue(keyName(DISCOUNT_VAL), true);
        setValue(keyName(NET_AFTER), true);
        setValue(keyName(VAT_VAL), true);
        setValue(keyName(VAT_PRICE), true);
        setValue(keyName(GROSS_VAL), true);

        setValue(keyName(SELLER_NAME), "true");
        setValue(keyName(SELLER_LOCATION), "true");
        setValue(keyName(SELLER_ADDRESS), "true");
        setValue(keyName(SELLER_ACCOUNT), "true");
        setValue(keyName(SELLER_TAXID), "true");
        setValue(keyName(SELLER_PHONE), "true");
        setValue(keyName(SELLER_MAIL), "true");
        setValue(keyName(SELLER_WWW), "true");
    }


    // returns working directory
    /**
     * @brief
     *
     * @return QString
     */
    const QString workingDir() const
    {
        return value(WORKING_DIR).toString();
    }

    // returns templates directory
    /**
     * @brief
     *
     * @return QString
     */
    const QString templateDir() const
    {
        const QString style(value(CSS).toString());
        QString ret = workingDir() + "/templates/" + style;

        QFile f;
        f.setFileName(ret);
        if (!f.exists()) {
            ret = QDir::currentPath() + "/templates/" + style;
        }

        f.setFileName(ret);
        if (!f.exists()) {
            ret = "/usr/local/share/qfaktury/templates/style.css";
        }

        return ret;
    }


    // return invoices dir
    /**
     * @brief
     *
     * @return QString
     */
    const QString dataDir() const
    {
        //TODO: Changed name of the folder to avoid overwriting the files.
        //TODO: This may require conversion script.
        return "/invoices";
    }


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
        if(default_val.isValid())
        {
            return QSettings::value(keyName(key), default_val);
        }
        else
        {
            return QSettings::value(keyName(key));
        }
    }

    /**
     * @brief
     *
     * @param i
     * @param f
     * @param prec
     * @return QString
     */
    const QString numberToString(const double i, const char f = 'f', const int prec = 2) const
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

private:
    const QString dateFormatInternal_; /**< TODO */
    const QString dateFormatExternal_; /**< TODO */
    QLocale locale_; /**< TODO */
};

#endif
