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
    case PRINT_FLAGS: return QString("print_fields");
    case SETTINGS: return QString("settings");
    case LISTS: return QString("lists");
    default:
        qDebug("SettingsGlobal::categoryName(): improper value of the argument key: %d", key);
        return QString();
    }
}


const QString SettingsGlobal::keyName(const SettingsGlobal::KEYS key)
{
    switch(key)
    {
    case VAT_RATES: return QString("%1/vat_rates").arg(categoryName(LISTS));
    case LANG: return QString("%1/language").arg(categoryName(SETTINGS));
    case ADDIT_TEXT: return QString("%1/additional_text").arg(categoryName(SETTINGS));
    case CAN_EDIT: return QString("%1/edit").arg(categoryName(SETTINGS));
    case CAN_EDIT_NAME: return QString("%1/edit_name").arg(categoryName(SETTINGS));
    case CAN_EDIT_SYMBOL: return QString("%1/edit_symbol").arg(categoryName(SETTINGS));
    case NUMBER_OF_COPIES: return QString("%1/number_of_copies").arg(categoryName(SETTINGS));
    case TAXID_MASK: return QString("%1/tax_id_mask").arg(categoryName(SETTINGS));
    case ACCOUNT_NUM_MASK: return QString("%1/account_num_mask").arg(categoryName(SETTINGS));
    case FIRST_RUN: return QString("%1/first_run").arg(categoryName(SETTINGS));
    case UNITS: return QString("%1/units").arg(categoryName(LISTS));
    case LOGO_PATH: return QString("%1/logo").arg(categoryName(SETTINGS));
    case PAYMENT_TYPE: return QString("%1/payment_type").arg(categoryName(LISTS));
    case CORRECTION_REASONS: return QString("correction_reasons").arg(categoryName(LISTS));
    case ORDER_NUMBER: return QString("%1/order_number").arg(categoryName(INVOICE_FIELDS));
    case NAME: return QString("%1/name").arg(categoryName(INVOICE_FIELDS));
    case CODE: return QString("%1/code").arg(categoryName(INVOICE_FIELDS));
    case PKWIU: return QString("%1/pkwiu").arg(categoryName(INVOICE_FIELDS));
    case QUANTITY: return QString("%1/quantity").arg(categoryName(INVOICE_FIELDS));
    case INTERNAT_UNIT: return QString("%1/international_unit").arg(categoryName(LISTS));
    case UNIT_PRICE: return QString("%1/unit_price").arg(categoryName(INVOICE_FIELDS));
    case NET_VAL: return QString("%1/net_value").arg(categoryName(INVOICE_FIELDS));
    case DISCOUNT: return QString("%1/discount_percent").arg(categoryName(INVOICE_FIELDS));
    case DISCOUNT_VAL: return QString("%1/discount_val").arg(categoryName(INVOICE_FIELDS));
    case NET_AFTER: return QString("%1/net_after").arg(categoryName(INVOICE_FIELDS));
    case VAT_VAL: return QString("%1/vat_value").arg(categoryName(INVOICE_FIELDS));
    case VAT_PRICE: return QString("%1/vat_price").arg(categoryName(INVOICE_FIELDS));
    case GROSS_VAL: return QString("%1/gross_value").arg(categoryName(INVOICE_FIELDS));
    case DISPLAY_SELLER_NAME: return QString("%1/display_seller_name").arg(categoryName(PRINT_FLAGS));
    case DISPLAY_SELLER_LOCATION: return QString("%1/display_seller_location").arg(categoryName(PRINT_FLAGS));
    case DISPLAY_SELLER_ADDRESS: return QString("%1/display_seller_address").arg(categoryName(PRINT_FLAGS));
    case DISPLAY_SELLER_ACCOUNT: return QString("%1/display_seller_account").arg(categoryName(PRINT_FLAGS));
    case DISPLAY_SELLER_TAXID: return QString("%1/display_seller_taxid").arg(categoryName(PRINT_FLAGS));
    case DISPLAY_SELLER_PHONE: return QString("%1/display_seller_phone").arg(categoryName(PRINT_FLAGS));
    case DISPLAY_SELLER_MAIL: return QString("%1/display_seller_email").arg(categoryName(PRINT_FLAGS));
    case DISPLAY_SELLER_WWW: return QString("%1/display_seller_www").arg(categoryName(PRINT_FLAGS));
    case DISPLAY_BUYER_NAME: return QString("%1/display_buyer_name").arg(categoryName(PRINT_FLAGS));
    case DISPLAY_BUYER_LOCATION: return QString("%1/display_buyer_location").arg(categoryName(PRINT_FLAGS));
    case DISPLAY_BUYER_ADDRESS: return QString("%1/display_buyer_address").arg(categoryName(PRINT_FLAGS));
    case DISPLAY_BUYER_ACCOUNT: return QString("%1/display_buyer_account").arg(categoryName(PRINT_FLAGS));
    case DISPLAY_BUYER_TAXID: return QString("%1/display_buyer_taxid").arg(categoryName(PRINT_FLAGS));
    case DISPLAY_BUYER_PHONE: return QString("%1/display_buyer_phone").arg(categoryName(PRINT_FLAGS));
    case DISPLAY_BUYER_MAIL: return QString("%1/display_buyer_email").arg(categoryName(PRINT_FLAGS));
    case DISPLAY_BUYER_WWW: return QString("%1/display_buyer_www").arg(categoryName(PRINT_FLAGS));
    case CSS: return QString("%1/css").arg(categoryName(SETTINGS));
    case DEFAULT_INV_NUM_FORMAT: return QString("%1/default_inv_num_format").arg(categoryName(SETTINGS));
    case DEFAULT_CURRENCY: return QString("%1/default_currency").arg(categoryName(SETTINGS));
    case COUNTRY: return QString("%1/country").arg(categoryName(SETTINGS));
    case TEXT1: return QString("%1/inv_num_text1").arg(categoryName(SETTINGS));
    case TEXT2: return QString("%1/inv_num_text2").arg(categoryName(SETTINGS));
    case TEXT3: return QString("%1/inv_num_text3").arg(categoryName(SETTINGS));
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
    setValue(keyName(CAN_EDIT), "false");
    setValue(keyName(CAN_EDIT_NAME), "false");
    setValue(keyName(CAN_EDIT_SYMBOL), "false");
    setValue(keyName(NUMBER_OF_COPIES), 1);
    setValue(keyName(TAXID_MASK), "999-999-99-99; ");
    setValue(keyName(ACCOUNT_NUM_MASK), "99-9999-9999-9999-9999-9999-9999; ");
    setValue(keyName(UNITS), trUtf8("szt.|kg.|g.|m.|km.|godz."));
    setValue(keyName(LOGO_PATH), "");
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

    setValue(keyName(DISPLAY_BUYER_NAME), true);
    setValue(keyName(DISPLAY_BUYER_LOCATION), true);
    setValue(keyName(DISPLAY_BUYER_ADDRESS), true);
    setValue(keyName(DISPLAY_BUYER_ACCOUNT), true);
    setValue(keyName(DISPLAY_BUYER_TAXID), true);
    setValue(keyName(DISPLAY_BUYER_PHONE), true);
    setValue(keyName(DISPLAY_BUYER_MAIL), true);
    setValue(keyName(DISPLAY_BUYER_WWW), true);
}
