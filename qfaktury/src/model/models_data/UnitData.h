#ifndef UNITDATA_H
#define UNITDATA_H


namespace UnitFields
{
/**
 * @brief
 *
 */
enum Fields {ID_UNIT, UNIT_NAME};
}

class QString;
class QObject;


/**
 * @brief
 *
 */
struct UnitData
{
    /**
     * @brief Default values when the app is launched for the first time. Later they can be useless.
     *
     */
    enum Name {UNIT, KG, G, M, KM, HOUR, PACKAGE};

    /**
     * @brief
     *
     * @param unitName
     * @return QString
     */
    static QString name(const Name unitName)
    {
        switch(unitName)
        {
        case UNIT: return QObject::trUtf8("szt.");
        case KG: return QObject::trUtf8("kg");
        case G: return QObject::trUtf8("g");
        case M: return QObject::trUtf8("m");
        case KM: return QObject::trUtf8("km");
        case HOUR: return QObject::trUtf8("godz.");
        case PACKAGE: return QObject::trUtf8("kmpl.");
        default:
            qDebug("UnitData::name() - unknown case: %d", unitName);
            return QString();
        }
    }
};

#endif // UNITDATA_H
