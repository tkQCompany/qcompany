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
    enum KEYS {VAT_RATES, BROWSER_NAME, DEFAULT_BROWSER, LANG, CURRENCIES};

    static QString keyName(const int key)
    {
        switch(key)
        {
        case VAT_RATES: return QString("stawki");
        case BROWSER_NAME: return QString("browser_name");
        case DEFAULT_BROWSER: return QString("default_browser");
        case LANG: return QString("lang");
        case CURRENCIES: return QString("waluty");
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
    QString getDateFormat() const {
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
        setValue(keyName(BROWSER_NAME), "");
        setValue(keyName(DEFAULT_BROWSER), "true");
        setValue(keyName(LANG), tr("pl"));
        setValue(keyName(CURRENCIES), tr("PLN"));
        endGroup();

        setValue("addText", trUtf8("towar odebrałem zgodnie z fakturą"));
        setValue("chars_in_symbol", tr("0"));
        setValue("day", "false");
        setValue("edit", "false");
        setValue("editName", "false");
        setValue("editSymbol", "false");
        setValue("numberOfCopies", 1);
        setValue("nipMask", "999-99-999-99; ");
        setValue("accountMask", "99-9999-9999-9999-9999-9999-9999; ");
        //      setValue ("filtrEnd", QDate::currentDate ().toString (Qt::ISODate));
        setValue("filtrStart", QDate::currentDate().toString(getDateFormat()));
        setValue("firstrun", false);
        setValue("jednostki", tr("szt.|kg.|g.|m.|km.|godz."));
        setValue("korNr", "1");
        setValue("logo", "");
        /*
         setValue("margLeft","15");
         setValue("margTop","15");
         setValue("margDown","15");
         setValue("margRight","15");
         */
        setValue("margLeftPrinter", "10");
        setValue("margTopPrinter", "10");
        setValue("margDownPrinter", "10");
        setValue("margRightPrinter", "10");
        setValue("month", "false");
        setValue("paym1", trUtf8("gotówka") );
        setValue("payments", trUtf8("gotówka|przelew|zaliczka") );
        setValue("pdfQuality", "1");
        setValue("pkorekty", trUtf8("zmiana ilości") );
        setValue("prefix", "");
        setValue("renamed", "tak");
        setValue("shortYear", "false");
        setValue(keyName(VAT_RATES), tr("22|7|0|zw."));
        setValue("sufix", "");
        setValue("waluty", tr("PLN|EUR|USD"));
        setValue("year", "false");

        // here we could add special code for Rachunek
        beginGroup("faktury_pozycje");
        setValue("Lp", true);
        setValue("Nazwa", true);
        setValue("Kod", true);
        setValue("pkwiu", true);
        setValue("ilosc", true);
        setValue("jm", true);
        setValue("cenajedn", true);
        setValue("wartnetto", true);
        setValue("rabatperc", true);
        setValue("rabatval", true);
        setValue("nettoafter", true);
        setValue("vatval", true);
        setValue("vatprice", true);
        setValue("bruttoval", true);
        endGroup();

        beginGroup("formatki");
        setValue("chAmount_top", "50");
        setValue("chAmount_left", "50");
        setValue("chAmount_width", "288");
        setValue("chAmount_height", "184");
        setValue("form1_top", "50");
        setValue("form1_left", "50");
        setValue("form1_width", "748");
        setValue("form1_height", "507");
        setValue("form2_top", "200");
        setValue("form2_left", "200");
        setValue("form2_width", "388");
        setValue("form2_height", "350");
        setValue("form4_top", "50");
        setValue("form4_left", "50");
        setValue("form4_width", "423");
        setValue("form4_height", "358");
        setValue("form7_top", "50");
        setValue("form7_left", "50");
        setValue("form7_width", "636");
        setValue("form7_height", "600");
        setValue("formfra_top", "50");
        setValue("formfra_left", "50");
        setValue("formfra_width", "546");
        setValue("formfra_height", "650");
        setValue("kontlist_top", "50");
        setValue("kontlist_left", "50");
        setValue("kontlist_width", "300");
        setValue("kontlist_height", "300");
        setValue("korform_top", "50");
        setValue("korform_left", "50");
        setValue("korform_width", "537");
        setValue("korform_height", "677");
        setValue("korprintpreview_top", "50");
        setValue("korprintpreview_left", "50");
        setValue("korprintpreview_width", "900");
        setValue("korprintpreview_height", "600");
        setValue("printpreview_top", "50");
        setValue("printpreview_left", "50");
        setValue("printpreview_width", "900");
        setValue("printpreview_height", "600");
        setValue("towadd_top", "50");
        setValue("towadd_left", "50");
        setValue("towadd_width", "334");
        setValue("towadd_height", "286");
        setValue("towlist_top", "50");
        setValue("towlist_left", "50");
        setValue("towlist_width", "300");
        setValue("towlist_height", "300");
        endGroup();

        beginGroup("printpos");
        setValue("usernazwa", "true");
        setValue("usermiejscowosc", "true");
        setValue("useradres", "true");
        setValue("userkonto", "true");
        setValue("usernip", "true");
        setValue("userphone", "true");
        setValue("usermail", "true");
        setValue("userwww", "true");
        setValue("clientnazwa", "true");
        setValue("clientmiejscowosc", "true");
        setValue("clientadres", "true");
        setValue("clientkonto", "true");
        setValue("clientnip", "true");
        setValue("clientphone", "true");
        setValue("clientmail", "true");
        setValue("clientwww", "true");
        endGroup();

        beginGroup("wydruki");
        setValue("col1", "10");
        setValue("col2", "25");
        setValue("col3", "12");
        setValue("col4", "12");
        setValue("col5", "12");
        setValue("col6", "9");
        setValue("col7", "11");
        setValue("col8", "11");
        setValue("col9", "10");
        setValue("col10", "12");
        setValue("col11", "12");
        setValue("col12", "12");
        setValue("col13", "12");
        setValue("col14", "12");
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
        return value("working_dir", QDir::homePath()).toString() + QString("/elinux");
    }

    // returns templates directory
    /**
     * @brief
     *
     * @return QString
     */
    QString getTemplate() const
    {
        QString style = value("css", "style.css").toString();
        if (style.isEmpty())
        {
            style = "style.css";
        }

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

    // constr
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
