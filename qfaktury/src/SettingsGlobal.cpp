#include "SettingsGlobal.h"
#include "CurrencyData.h"


SettingsGlobal::SettingsGlobal() : dateFormatInternal_("yyyy-MM-dd"),
    dateFormatExternal_("dd/MM/yyyy")
{
    if(firstRun())
    {
        resetSettings();
    }
}


const QString SettingsGlobal::categoryName(const CATEGORY_OF_KEYS key)
{
    switch(key)
    {
    case INVOICE_FIELDS: return QString("invoice_fields");
    case PRINT_FIELDS: return QString("print_fields");
    default:
        qDebug("SettingsGlobal::categoryName(): improper value of the argument key: %d", key);
        return QString();
    }
}


const QString SettingsGlobal::keyName(const SettingsGlobal::KEYS key)
{
    switch(key)
    {
    case VAT_RATES: return QString("vat_rates");
    case LANG: return QString("language");
    case ADDIT_TEXT: return QString("additional_text");
    case EDIT: return QString("edit");
    case EDIT_NAME: return QString("edit_name");
    case EDIT_SYMBOL: return QString("edit_symbol");
    case NUMBER_OF_COPIES: return QString("number_of_copies");
    case TAXID_MASK: return QString("tax_id_mask");
    case ACCOUNT_NUM_MASK: return QString("account_num_mask");
    case FIRST_RUN: return QString("first_run");
    case UNITS: return QString("units");
    case LOGO: return QString("logo");
    case PAYMENT_TYPE: return QString("payment_type");
    case CORRECTION_REASONS: return QString("correction_reasons");
    case ORDER_NUMBER: return QString("%1/order_number").arg(categoryName(INVOICE_FIELDS));
    case NAME: return QString("%1/name").arg(categoryName(INVOICE_FIELDS));
    case CODE: return QString("%1/code").arg(categoryName(INVOICE_FIELDS));
    case PKWIU: return QString("%1/pkwiu").arg(categoryName(INVOICE_FIELDS));
    case QUANTITY: return QString("%1/quantity").arg(categoryName(INVOICE_FIELDS));
    case INTERNAT_UNIT: return QString("%1/international_unit").arg(categoryName(INVOICE_FIELDS));
    case UNIT_PRICE: return QString("%1/unit_price").arg(categoryName(INVOICE_FIELDS));
    case NET_VAL: return QString("%1/net_value").arg(categoryName(INVOICE_FIELDS));
    case DISCOUNT: return QString("%1/discount_percent").arg(categoryName(INVOICE_FIELDS));
    case DISCOUNT_VAL: return QString("%1/discount_val").arg(categoryName(INVOICE_FIELDS));
    case NET_AFTER: return QString("%1/net_after").arg(categoryName(INVOICE_FIELDS));
    case VAT_VAL: return QString("%1/vat_value").arg(categoryName(INVOICE_FIELDS));
    case VAT_PRICE: return QString("%1/vat_price").arg(categoryName(INVOICE_FIELDS));
    case GROSS_VAL: return QString("%1/gross_value").arg(categoryName(INVOICE_FIELDS));
    case DISPLAY_SELLER_NAME: return QString("%1/display_seller_name").arg(categoryName(PRINT_FIELDS));
    case DISPLAY_SELLER_LOCATION: return QString("%1/display_seller_location").arg(categoryName(PRINT_FIELDS));
    case DISPLAY_SELLER_ADDRESS: return QString("%1/display_seller_address").arg(categoryName(PRINT_FIELDS));
    case DISPLAY_SELLER_ACCOUNT: return QString("%1/display_seller_account").arg(categoryName(PRINT_FIELDS));
    case DISPLAY_SELLER_TAXID: return QString("%1/display_seller_taxid").arg(categoryName(PRINT_FIELDS));
    case DISPLAY_SELLER_PHONE: return QString("%1/display_seller_phone").arg(categoryName(PRINT_FIELDS));
    case DISPLAY_SELLER_MAIL: return QString("%1/display_seller_email").arg(categoryName(PRINT_FIELDS));
    case DISPLAY_SELLER_WWW: return QString("%1/display_seller_www").arg(categoryName(PRINT_FIELDS));
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
        qDebug("SettingsGlobal::keyName(): improper value of the argument key: %d", key);
        return QString();
    }
}


void SettingsGlobal::resetSettings()
{
    setValue(keyName(LANG), trUtf8("pl"));
    setValue(keyName(ADDIT_TEXT), trUtf8("Towar odebrałem zgodnie z fakturą"));
    setValue(keyName(EDIT), "false");
    setValue(keyName(EDIT_NAME), "false");
    setValue(keyName(EDIT_SYMBOL), "false");
    setValue(keyName(NUMBER_OF_COPIES), 1);
    setValue(keyName(TAXID_MASK), "999-999-99-99; ");
    setValue(keyName(ACCOUNT_NUM_MASK), "99-9999-9999-9999-9999-9999-9999; ");
    setValue(keyName(UNITS), trUtf8("szt.|kg.|g.|m.|km.|godz."));
    setValue(keyName(LOGO), "");
    setValue(keyName(PAYMENT_TYPE), trUtf8("gotówka|przelew|zaliczka"));
    setValue(keyName(CORRECTION_REASONS), trUtf8("Pomyłka w rabacie|"
                                                "Podwyższenie ceny|"
                                                "Pomyłka w cenie|"
                                                "Pomyłka w stawce VAT|"
                                                "Pomyłka w kwocie VAT|"
                                                "Pomyłka w miarze i ilości (liczbie) dostarczonych towarów lub zakresie wykonanych usług|"
                                                "Pomyłka w cenie jednostkowej towaru lub usługi bez kwoty podatku (cenie jednostkowej netto)|"
                                                "Pomyłka w kwocie wszelkich rabatów, w tym za wcześniejsze otrzymanie należności, o ile nie zostały one uwzględnione w cenie jednostkowej netto|"
                                                "Pomyłka w wartości dostarczonych towarów lub wykonanych usług, objętych transakcją, bez kwoty podatku (wartość sprzedaży netto)|"
                                                "Pomyłka w sumie wartości sprzedaży netto z podziałem na sprzedaż objętą poszczególnymi stawkami podatku i sprzedaż zwolnioną od podatku|"
                                                "Pomyłka w kwocie należności ogółem|"
                                                 "Pomyłka w numerze faktury|"
                                                 "Pomyłka w dacie sprzedaży|"
                                                 "Pomyłka w dacie wystawienia|"
                                                 "Pomyłka w typie faktury|"
                                                 "Pomyłka w typie płatności|"
                                                 "Pomyłka w terminie płatności|"
                                                 "Pomyłka w wybranej walucie|"
                                                 "Pomyłka w uwagach") );
    setValue(keyName(VAT_RATES), trUtf8("23|8|5|0|zw."));
    setValue(keyName(CSS), QString("style.css"));
    setValue(keyName(DEFAULT_INV_NUM_FORMAT), trUtf8("{TEKST1}/{R}-{M}-{D}/{NR_R}"));
    setValue(keyName(DEFAULT_CURRENCY), CurrencyData::PLN);
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

    setValue(keyName(DISPLAY_SELLER_NAME), true);
    setValue(keyName(DISPLAY_SELLER_LOCATION), true);
    setValue(keyName(DISPLAY_SELLER_ADDRESS), true);
    setValue(keyName(DISPLAY_SELLER_ACCOUNT), true);
    setValue(keyName(DISPLAY_SELLER_TAXID), true);
    setValue(keyName(DISPLAY_SELLER_PHONE), true);
    setValue(keyName(DISPLAY_SELLER_MAIL), true);
    setValue(keyName(DISPLAY_SELLER_WWW), true);
}
