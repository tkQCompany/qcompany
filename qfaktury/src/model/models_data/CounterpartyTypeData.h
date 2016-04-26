#ifndef COUNTERPARTYTYPEDATA_H
#define COUNTERPARTYTYPEDATA_H

#include <QObject>


namespace CounterpartyTypeFields
{
    /**
     * @brief
     *
     */
    enum Fields {ID, TYPE};
}

/**
 * @brief
 *
 */
struct CounterpartyTypeData
{
    /**
     * @brief Default values for the first start of the application. Later these values can change with the exception for MY_COMPANY.
     *
     */
    enum CounterpartyType {MY_COMPANY, COMPANY, OFFICE};

    /**
     * @brief
     *
     * @param type
     * @return QString
     */
    static QString name(const CounterpartyType type)
    {
        switch(type)
        {
        case MY_COMPANY: return QObject::trUtf8("Moja firma");
        case COMPANY: return QObject::trUtf8("Firma");
        case OFFICE: return QObject::trUtf8("Urząd");
        default:
            qDebug("CounterpartyTypeData::name(): Unknown commodity's type.");
            return QString();
        }
    }
};

Q_DECLARE_METATYPE(CounterpartyTypeData::CounterpartyType)

#endif // COUNTERPARTYTYPEDATA_H
