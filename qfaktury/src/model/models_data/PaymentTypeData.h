#ifndef PAYMENTTYPEDATA_H
#define PAYMENTTYPEDATA_H

#include <QString>

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
    static QString name(const int paymentType)
    {
        switch(paymentType)
        {
        case CASH: return QObject::trUtf8("gotówka");
        case PREPAYMENT: return QObject::trUtf8("zaliczka");
        case TRANSFER: return QObject::trUtf8("przelew");
        default:
            qDebug("Unknown case in PaymentTypeData::name(): %d", paymentType);
            return QString();
        }
    }
};

#endif // PAYMENTTYPEDATA_H
