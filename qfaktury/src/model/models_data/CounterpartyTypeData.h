#ifndef COUNTERPARTYTYPEDATA_H
#define COUNTERPARTYTYPEDATA_H


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
     * @brief
     *
     */
    enum CounterpartyType {MY_COMPANY, COMPANY, OFFICE};

    /**
     * @brief
     *
     * @param type
     * @return QString
     */
    static QString name(const int type)
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

#endif // COUNTERPARTYTYPEDATA_H
