#ifndef PAYMENTTYPEDATA_H
#define PAYMENTTYPEDATA_H


namespace PaymentTypeFields
{
/**
 * @brief
 *
 */
enum Fields {ID_PAYMENT_TYPE, TYPE};
}

/**
 * @brief
 *
 */
struct PaymentTypeData
{
    /**
     * @brief
     *
     */
    enum PaymentTypes {CASH, PREPAYMENT, TRANSFER};

    /**
     * @brief
     *
     * @param paymentType
     * @return QString
     */
    static QString name(const PaymentTypes paymentType)
    {
        switch(paymentType)
        {
        case CASH: return QObject::trUtf8("gotówka");
        case PREPAYMENT: return QObject::trUtf8("zaliczka");
        case TRANSFER: return QObject::trUtf8("przelew");
        default:
            qDebug("Unknown case in PaymentTypeData::name(): %d\n", paymentType);
            return QString();
        }
    }

    static PaymentTypes stringToPaymentType(const QString &strType)
    {
        int ret = CASH;
        do
        {
        } while( (strType != PaymentTypeData::name((PaymentTypes)ret)) && (++ret < TRANSFER));

        return (PaymentTypes)ret;
    }
};

#endif // PAYMENTTYPEDATA_H
