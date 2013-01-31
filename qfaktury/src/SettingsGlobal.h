#ifndef SETTINGSGLOBAL_H
#define SETTINGSGLOBAL_H

#include <QSettings>
#include <QDate>
#include <QDir>
#include <QString>
#include <QLocale>
#include <QDebug>

// Singleton class to store all the settings and static values

/**
 * @brief
 *
 */
class SettingsGlobal: public QSettings
{
    Q_OBJECT

public:
    enum KEYS {VAT_RATES, LANG, CURRENCIES, ADDIT_TEXT, EDIT, EDIT_NAME, EDIT_SYMBOL,
              NUMBER_OF_COPIES, TAXID_MASK, ACCOUNT_MASK, FIRST_RUN, UNITS, LOGO, PAYMENT_TYPE, CORRECTION_REASON,
              ORDER_NUMBER, NAME, CODE, PKWIU, QUANTITY, INTERNAT_UNIT, UNIT_PRICE, NET_VAL, DISCOUNT, DISCOUNT_VAL,
              NET_AFTER, VAT_VAL, VAT_PRICE, GROSS_VAL, USER_NAME, USER_LOCATION, USER_ADDRESS, USER_ACCOUNT,
              USER_TAXID, USER_PHONE, USER_MAIL, USER_WWW, WORKING_DIR, CSS};

    static QString keyName(const int key)
    {
        switch(key)
        {
        case VAT_RATES: return QString("stawki");
        case LANG: return QString("lang");
        case CURRENCIES: return QString("waluty");
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
        case ORDER_NUMBER: return QString("Lp");
        case NAME: return QString("Nazwa");
        case CODE: return QString("Kod");
        case PKWIU: return QString("pkwiu");
        case QUANTITY: return QString("ilosc");
        case INTERNAT_UNIT: return QString("jm");
        case UNIT_PRICE: return QString("cenajedn");
        case NET_VAL: return QString("wartnetto");
        case DISCOUNT: return QString("rabatperc");
        case DISCOUNT_VAL: return QString("rabatval");
        case NET_AFTER: return QString("nettoafter");
        case VAT_VAL: return QString("vatval");
        case VAT_PRICE: return QString("vatprice");
        case GROSS_VAL: return QString("bruttoval");
        case USER_NAME: return QString("usernazwa");
        case USER_LOCATION: return QString("usermiejscowosc");
        case USER_ADDRESS: return QString("useradres");
        case USER_ACCOUNT: return QString("userkonto");
        case USER_TAXID: return QString("usernip");
        case USER_PHONE: return QString("userphone");
        case USER_MAIL: return QString("usermail");
        case USER_WWW: return QString("userwww");
        case WORKING_DIR: return QString("working_dir");
        case CSS: return QString("css");
        default:
            qDebug() << "SettingsGlobal::keyName(): improper value of the argument key: " << key;
            return QString();
        }
    }

public:
    // returns date format used for all dates
    /**
     * @brief
     *
     * @return QString
     */
    QString getDateFormat() const
    {
        // it's better to have a full year... so
        return dateFormat;
    }


    /** Reset all settings to default values
     */
    /**
     * @brief
     *
     */
    void resetSettings()
    {
        beginGroup("General");
        setValue(keyName(LANG), tr("pl"));
        setValue(keyName(CURRENCIES), tr("PLN"));
        endGroup();

        setValue(keyName(ADDIT_TEXT), trUtf8("towar odebrałem zgodnie z fakturą"));
        setValue(keyName(EDIT), "false");
        setValue(keyName(EDIT_NAME), "false");
        setValue(keyName(EDIT_SYMBOL), "false");
        setValue(keyName(NUMBER_OF_COPIES), 1);
        setValue(keyName(TAXID_MASK), "999-999-99-99;");
        setValue(keyName(ACCOUNT_MASK), "99-9999-9999-9999-9999-9999-9999;");
        setValue(keyName(FIRST_RUN), false);
        setValue(keyName(UNITS), tr("szt.|kg.|g.|m.|km.|godz."));
        setValue(keyName(LOGO), "");
        setValue(keyName(PAYMENT_TYPE), trUtf8("gotówka|przelew|zaliczka"));
        setValue(keyName(CORRECTION_REASON), trUtf8("zmiana ilości") );
        setValue(keyName(VAT_RATES), tr("23|8|5|0|zw."));
        setValue(keyName(WORKING_DIR), QString("%1/elinux").arg(QDir::homePath()));
        setValue(keyName(CSS), QString("style.css"));

        // here we could add special code for Rachunek
        beginGroup("faktury_pozycje");
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
        endGroup();

        beginGroup("printpos");
        setValue(keyName(USER_NAME), "true");
        setValue(keyName(USER_LOCATION), "true");
        setValue(keyName(USER_ADDRESS), "true");
        setValue(keyName(USER_ACCOUNT), "true");
        setValue(keyName(USER_TAXID), "true");
        setValue(keyName(USER_PHONE), "true");
        setValue(keyName(USER_MAIL), "true");
        setValue(keyName(USER_WWW), "true");
        endGroup();
    }


    // returns working directory
    /**
     * @brief
     *
     * @return QString
     */
    QString getWorkingDir() const
    {
        return value(keyName(WORKING_DIR)).toString();
    }

    // returns templates directory
    /**
     * @brief
     *
     * @return QString
     */
    QString getTemplate() const
    {
        const QString style = value(keyName(CSS)).toString();
        QString ret = getWorkingDir() + "/templates/" + style;

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
    QString getDataDir() const
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
    QString getDecimalPointStr() const
    {
        return QString(locale.decimalPoint());
    }

    /**
     * @brief
     *
     * @return QString
     */
    QString getTPointStr() const
    {
        return QString(locale.groupSeparator());
    }

    /**
     * @brief
     *
     * @param i
     * @param f
     * @param prec
     * @return QString
     */
    QString numberToString(const double i, const char f = 'f', const int prec = 2) const
    {
        return locale.toString(i, f, prec);
    }

    /**
     * @brief
     *
     * @param i
     * @return QString
     */
    QString numberToString(const int i) const
    {
        return locale.toString(i);
    }

    /**
     * @brief
     *
     * @param s
     * @return double
     */
    double stringToDouble(const QString &s) const
    {
        return locale.toDouble(s);
    }

private:
    QString dateFormat; /**< TODO */
    QLocale locale; /**< TODO */

/**
 * @brief
 *
 */
    SettingsGlobal()
    {
        dateFormat = "dd/MM/yyyy";
    }

/**
 * @brief
 *
 * @param
 */
    SettingsGlobal(const SettingsGlobal&):QSettings() {}

    /**
     * @brief
     *
     * @return SettingsGlobal
     */
    friend SettingsGlobal& sett() {
        static SettingsGlobal sett;
        return sett;
    }
};

/**
 * @brief
 *
 * @return SettingsGlobal
 */
SettingsGlobal& sett();

#endif
