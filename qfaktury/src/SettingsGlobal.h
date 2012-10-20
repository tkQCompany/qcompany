#ifndef SETTINGSGLOBAL_H
#define SETTINGSGLOBAL_H

#include <QSettings>
#include <QDate>
#include <QDir>
#include <QString>
#include <QLocale>

// Singleton class to store all the settings and static values

/**
 * @brief
 *
 */
class SettingsGlobal: public QSettings
{
    Q_OBJECT
public:
    const QString KEY_VAT_RATES; /**< TODO */
public:

    // get date from settings as QDate
    /**
     * @brief
     *
     * @param val
     * @return QDate
     */
    QDate getValueAsDate(QString val) {
        QString date = value(val).toString();
        QLocale locale;
        return locale.toDate( date, getDateFormat());
    }

    // returns date format used for all dates
    /**
     * @brief
     *
     * @return QString
     */
    QString getDateFormat() {
        // it's better to have a full year... so
        return dateFormat;
    }

    // returns date format used while saving the file
    /**
     * @brief
     *
     * @return QString
     */
    QString getFnameDateFormat() {
        return fileNameDateFormat;
    }


    /**
     * validate the settings and set them to default values if required.
     */
    /**
     * @brief
     *
     */
    void checkSettings() {
        if (value("browser_name").toString().compare("") == 0)
            setValue("browser_name", "");
        if (value("default_browser").toString().compare("") == 0)
            setValue("default_browser", "true");
        if (value("lang").toString().compare("") == 0)
            setValue("lang", tr("pl"));
        if (value("localEnc").toString().compare("") == 0)
            setValue("localEnc", tr("UTF-8"));


        if (value("addText").toString().compare("") == 0)
            setValue("addText",
                     trUtf8("towar odebrałem zgodnie z fakturą"));
        if (value("chars_in_symbol").toString().compare("") == 0)
            setValue("chars_in_symbol", tr("0"));
        if (value("day").toString().compare("") == 0)
            setValue("day", "false");
        if (value("edit").toString().compare("") == 0)
            setValue("edit", "false");
        if (value("editName").toString().compare("") == 0)
            setValue("editName", "false");
        if (value("editSymbol").toString().compare("") == 0)
            setValue("editSymbol", "false");
        if (value("editSymbol").toString().compare("") == 0)
            setValue("editSymbol", "false");
        if (value("editSymbol").toString().compare("") == 0)
            setValue("editSymbol", "false");
        if (value("filtrEnd").toString().compare("") == 0)
            setValue("filtrEnd",
                     QDate::currentDate().toString(Qt::ISODate));
        if (value("filtrStart").toString().compare("") == 0)
            setValue("filtrStart", QDate::currentDate().toString(
                         Qt::ISODate));
        if (value("firstrun").toString().compare("") == 0)
            setValue("firstrun", false);
        if (value("jednostki").toString().compare("") == 0)
            setValue("jednostki", tr("szt.|kg.|g.|m|km.|godz."));
        if (value("korNr").toString().compare("") == 0)
            setValue("korNr", "1");
        if (value("logo").toString().compare("") == 0)
            setValue("logo", "");
        if (value("margLeftPrinter").toString().compare("") == 0)
            setValue("margLeftPrinter", "10");
        if (value("margTopPrinter").toString().compare("") == 0)
            setValue("margTopPrinter", "10");
        if (value("margDownPrinter").toString().compare("") == 0)
            setValue("margDownPrinter", "10");
        if (value("margRightPrinter").toString().compare("") == 0)
            setValue("margRightPrinter", "10");
        if (value("month").toString().compare("") == 0)
            setValue("month", "false");
        if (value("paym1").toString().compare("") == 0)
            setValue("paym1", tr("gotówka"));
        if (value("payments").toString().compare("") == 0)
            setValue("payments", tr("gotówka|przelew|zaliczka"));
        if (value("pdfQuality").toString().compare("") == 0)
            setValue("pdfQuality", "1");
        if (value("pkorekty").toString().compare("") == 0)
            setValue("pkorekty", tr("zmiana ilości"));
        if (value("prefix").toString().compare("") == 0)
            setValue("prefix", "");
        if (value("renamed").toString().compare("") == 0)
            setValue("renamed", "tak");
        if (value("shortYear").toString().compare("") == 0)
            setValue("shortYear", "false");
        if (value("stawki").toString().compare("") == 0)
            setValue("stawki", tr("23|8|5|0|zw."));
        if (value("sufix").toString().compare("") == 0)
            setValue("sufix", "");
        if (value("waluty").toString().compare("") == 0)
            setValue("waluty", tr("PLN|EUR|USD"));
        if (value("year").toString().compare("") == 0)
            setValue("year", "false");
        if (value("numberOfCopies").toString().compare("") == 0)
            setValue("numberOfCopies", 1);


        // here we could add special code for Rachunek
        beginGroup("faktury_pozycje");
        if (value("Lp").toString().compare("") == 0)
            setValue("Lp", true);
        if (value("Nazwa").toString().compare("") == 0)
            setValue("Nazwa", true);
        if (value("Kod").toString().compare("") == 0)
            setValue("Kod", true);
        if (value("pkwiu").toString().compare("") == 0)
            setValue("pkwiu", true);
        if (value("ilosc").toString().compare("") == 0)
            setValue("ilosc", true);
        if (value("jm").toString().compare("") == 0)
            setValue("jm", true);
        if (value("cenajedn").toString().compare("") == 0)
            setValue("cenajedn", true);
        if (value("wartnetto").toString().compare("") == 0)
            setValue("wartnetto", true);
        if (value("rabatperc").toString().compare("") == 0)
            setValue("rabatperc", true);
        if (value("rabatval").toString().compare("") == 0)
            setValue("rabatval", true);
        if (value("nettoafter").toString().compare("") == 0)
            setValue("nettoafter", true);
        if (value("vatval").toString().compare("") == 0)
            setValue("vatval", true);
        if (value("vatprice").toString().compare("") == 0)
            setValue("vatprice", true);
        if (value("bruttoval").toString().compare("") == 0)
            setValue("bruttoval", true);
        endGroup();

        beginGroup("formatki");
        if (value("chAmount_top").toString().compare("") == 0)
            setValue("chAmount_top", "50");
        if (value("chAmount_left").toString().compare("") == 0)
            setValue("chAmount_left", "50");
        if (value("chAmount_width").toString().compare("") == 0)
            setValue("chAmount_width", "288");
        if (value("chAmount_height").toString().compare("") == 0)
            setValue("chAmount_height", "184");
        if (value("form1_top").toString().compare("") == 0)
            setValue("form1_top", "50");
        if (value("form1_left").toString().compare("") == 0)
            setValue("form1_left", "50");
        if (value("form1_width").toString().compare("") == 0)
            setValue("form1_width", "748");
        if (value("form1_height").toString().compare("") == 0)
            setValue("form1_height", "507");
        if (value("form2_top").toString().compare("") == 0)
            setValue("form2_top", "200");
        if (value("form2_left").toString().compare("") == 0)
            setValue("form2_left", "200");
        if (value("form2_width").toString().compare("") == 0)
            setValue("form2_width", "388");
        if (value("form2_height").toString().compare("") == 0)
            setValue("form2_height", "350");
        if (value("form4_top").toString().compare("") == 0)
            setValue("form4_top", "50");
        if (value("form4_left").toString().compare("") == 0)
            setValue("form4_left", "50");
        if (value("form4_width").toString().compare("") == 0)
            setValue("form4_width", "423");
        if (value("form4_height").toString().compare("") == 0)
            setValue("form4_height", "358");
        if (value("form7_top").toString().compare("") == 0)
            setValue("form7_top", "50");
        if (value("form7_left").toString().compare("") == 0)
            setValue("form7_left", "50");
        if (value("form7_width").toString().compare("") == 0)
            setValue("form7_width", "636");
        if (value("form7_height").toString().compare("") == 0)
            setValue("form7_height", "600");
        if (value("formfra_top").toString().compare("") == 0)
            setValue("formfra_top", "50");
        if (value("formfra_left").toString().compare("") == 0)
            setValue("formfra_left", "50");
        if (value("formfra_width").toString().compare("") == 0)
            setValue("formfra_width", "546");
        if (value("formfra_height").toString().compare("") == 0)
            setValue("formfra_height", "650");
        if (value("kontlist_top").toString().compare("") == 0)
            setValue("kontlist_top", "50");
        if (value("kontlist_left").toString().compare("") == 0)
            setValue("kontlist_left", "50");
        if (value("kontlist_width").toString().compare("") == 0)
            setValue("kontlist_width", "300");
        if (value("kontlist_height").toString().compare("") == 0)
            setValue("kontlist_height", "300");
        if (value("korform_top").toString().compare("") == 0)
            setValue("korform_top", "50");
        if (value("korform_left").toString().compare("") == 0)
            setValue("korform_left", "50");
        if (value("korform_width").toString().compare("") == 0)
            setValue("korform_width", "537");
        if (value("korform_height").toString().compare("") == 0)
            setValue("korform_height", "677");
        if (value("korprintpreview_top").toString().compare("") == 0)
            setValue("korprintpreview_top", "50");
        if (value("korprintpreview_left").toString().compare("") == 0)
            setValue("korprintpreview_left", "50");
        if (value("korprintpreview_width").toString().compare("") == 0)
            setValue("korprintpreview_width", "900");
        if (value("korprintpreview_height").toString().compare("") == 0)
            setValue("korprintpreview_height", "600");
        if (value("printpreview_top").toString().compare("") == 0)
            setValue("printpreview_top", "50");
        if (value("printpreview_left").toString().compare("") == 0)
            setValue("printpreview_left", "50");
        if (value("printpreview_width").toString().compare("") == 0)
            setValue("printpreview_width", "900");
        if (value("printpreview_height").toString().compare("") == 0)
            setValue("printpreview_height", "600");
        if (value("towadd_top").toString().compare("") == 0)
            setValue("towadd_top", "50");
        if (value("towadd_left").toString().compare("") == 0)
            setValue("towadd_left", "50");
        if (value("towadd_width").toString().compare("") == 0)
            setValue("towadd_width", "334");
        if (value("towadd_height").toString().compare("") == 0)
            setValue("towadd_height", "286");
        if (value("towlist_top").toString().compare("") == 0)
            setValue("towlist_top", "50");
        if (value("towlist_left").toString().compare("") == 0)
            setValue("towlist_left", "50");
        if (value("towlist_width").toString().compare("") == 0)
            setValue("towlist_width", "300");
        if (value("towlist_height").toString().compare("") == 0)
            setValue("towlist_height", "300");
        endGroup();

        beginGroup("printpos");
        if (value("usernazwa").toString().compare("") == 0)
            setValue("usernazwa", "true");
        if (value("usermiejscowosc").toString().compare("") == 0)
            setValue("usermiejscowosc", "true");
        if (value("useradres").toString().compare("") == 0)
            setValue("useradres", "true");
        if (value("userkonto").toString().compare("") == 0)
            setValue("userkonto", "true");
        if (value("usernip").toString().compare("") == 0)
            setValue("usernip", "true");
        if (value("userphone").toString().compare("") == 0)
            setValue("userphone", "true");
        if (value("usermail").toString().compare("") == 0)
            setValue("usermail", "true");
        if (value("userwww").toString().compare("") == 0)
            setValue("userwww", "true");
        if (value("clientnazwa").toString().compare("") == 0)
            setValue("clientnazwa", "true");
        if (value("clientmiejscowosc").toString().compare("") == 0)
            setValue("clientmiejscowosc", "true");
        if (value("clientadres").toString().compare("") == 0)
            setValue("clientadres", "true");
        if (value("clientkonto").toString().compare("") == 0)
            setValue("clientkonto", "true");
        if (value("clientnip").toString().compare("") == 0)
            setValue("clientnip", "true");
        if (value("clientphone").toString().compare("") == 0)
            setValue("clientphone", "true");
        if (value("clientmail").toString().compare("") == 0)
            setValue("clientmail", "true");
        if (value("clientwww").toString().compare("") == 0)
            setValue("clientwww", "true");
        endGroup();

        beginGroup("wydruki");
        if (value("col1").toString().compare("") == 0)
            setValue("col1", "10");
        if (value("col2").toString().compare("") == 0)
            setValue("col2", "25");
        if (value("col3").toString().compare("") == 0)
            setValue("col3", "12");
        if (value("col4").toString().compare("") == 0)
            setValue("col4", "12");
        if (value("col5").toString().compare("") == 0)
            setValue("col5", "10");
        if (value("col6").toString().compare("") == 0)
            setValue("col6", "9");
        if (value("col7").toString().compare("") == 0)
            setValue("col7", "11");
        if (value("col8").toString().compare("") == 0)
            setValue("col8", "11");
        if (value("col9").toString().compare("") == 0)
            setValue("col9", "10");
        if (value("col10").toString().compare("") == 0)
            setValue("col10", "12");
        if (value("col11").toString().compare("") == 0)
            setValue("col11", "12");
        if (value("col12").toString().compare("") == 0)
            setValue("col12", "12");
        if (value("col13").toString().compare("") == 0)
            setValue("col13", "12");
        if (value("col14").toString().compare("") == 0)
            setValue("col14", "12");

        endGroup();

        sync();
    }

    /** Reset all settings to default values
     */
    /**
     * @brief
     *
     */
    void resetSettings() {
        beginGroup("General");
        setValue("browser_name", "");
        setValue("default_browser", "true");
        setValue("lang", tr("pl"));
        setValue("waluty", tr("PLN"));
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
        setValue(KEY_VAT_RATES, tr("22|7|0|zw."));
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
    QString getWorkingDir() {
        return value("working_dir", QDir::homePath()).toString() + "/elinux";
    }

    // returns templates directory
    /**
     * @brief
     *
     * @return QString
     */
    QString getTemplate() {


        QString style = value("css", "style.css").toString();
        if (style.compare("") == 0) {
            style = "style.css";
        }

        QString ret = getWorkingDir() + "/templates/" + style;
        QString path = getWorkingDir() + "/templates/";

        QFile f;
        f.setFileName(ret);
        if (!f.exists()) {
            ret = QDir::currentPath() + "/templates/" + style;
        }

        f.setFileName(ret);
        if (!f.exists()) {
            ret = "/usr/local/share/qfaktury/templates/style.css";
        }

        // qDebug() << ret;
        return ret;
    }


    // return invoices dir
    /**
     * @brief
     *
     * @return QString
     */
    QString getDataDir() {
        // Changed name of the folder to avoid overwriting the files.
        // This may require conversion script.
        return "/invoices";
    }

    // return invoices dir
    /**
     * @brief
     *
     * @return QString
     */
    QString getInvoicesDir() {
        return getWorkingDir() + getDataDir() + "/";
    }

    // return customers xml
    /**
     * @brief
     *
     * @return QString
     */
    QString getCustomersXml() {
        return getWorkingDir() + "/customers.xml";
    }

    // return customers xml
    /**
     * @brief
     *
     * @return QString
     */
    QString getProductsXml() {
        return getWorkingDir() + "/products.xml";
    }

    // returns inoice doc name stored as a DOCTYPE
    /**
     * @brief
     *
     * @return QString
     */
    QString getInoiveDocName() {
        return "invoice";
    }

    // returns correction doc name stored as a DOCTYPE
    /**
     * @brief
     *
     * @return QString
     */
    QString getCorrDocName() {
        return "correction";
    }


    // returns customers doc name stored as a DOCTYPE
    /**
     * @brief
     *
     * @return QString
     */
    QString getCustomersDocName() {
        return "customers";
    }

    // returns products doc name stored as a DOCTYPE
    /**
     * @brief
     *
     * @return QString
     */
    QString getProdutcsDocName() {
        return "products";
    }


    /**
     * @brief
     *
     * @return QString
     */
    QString getCompanyName() {
        return "company";
    }

    /**
     * @brief
     *
     * @return QString
     */
    QString getOfficeName() {
        return "office";
    }

    /**
     * @brief
     *
     * @return QString
     */
    QString getCompanyNameTr() {
        return trUtf8("Firma");
    }

    /**
     * @brief
     *
     * @return QString
     */
    QString getOfficeNameTr() {
        return trUtf8("Urząd");
    }

    /**
     * @brief
     *
     * @return QString
     */
    QString getProductName() {
        return "product";
    }

    /**
     * @brief
     *
     * @return QString
     */
    QString getServiceName() {
        return "service";
    }

    // Adds Data to input string
    /**
     * @brief
     *
     * @param in
     * @return QString
     */
    QString getNameWithData(QString in) {
        return in + trUtf8("DATA");
    }


    /**
     * @brief
     *
     * @return QByteArray
     */
    QByteArray getCodecName() {
        return "UTF-8";
    }

    /**
     * @brief
     *
     * @return QString
     */
    QString getDecimalPointStr() {
        QChar decimalPoint = locale.decimalPoint ();
        return QString(decimalPoint);
    }

    /**
     * @brief
     *
     * @return QString
     */
    QString getTPointStr() {
        QChar tPoint = locale.groupSeparator();
        return QString(tPoint);
    }

    /**
     * @brief
     *
     * @param i
     * @param f
     * @param prec
     * @return QString
     */
    QString numberToString(double i, char f = 'f', int prec = 2) {
        return locale.toString(i, f, prec);
    }

    /**
     * @brief
     *
     * @param i
     * @return QString
     */
    QString numberToString(int i) {
        return locale.toString(i);
    }

    /**
     * @brief
     *
     * @param s
     * @return double
     */
    double stringToDouble(QString s) {
        return locale.toDouble(s);
    }
private:
    QString dateFormat; /**< TODO */
    QString fileNameDateFormat; /**< TODO */
    QLocale locale; /**< TODO */

    // constr
/**
 * @brief
 *
 */
    SettingsGlobal(): KEY_VAT_RATES("stawki")
    {
        dateFormat = "dd/MM/yyyy";
        fileNameDateFormat = "yyyy-MM-dd";
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
