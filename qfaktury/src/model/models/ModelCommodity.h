#ifndef MODELCOMMODITY_H
#define MODELCOMMODITY_H

#include <QSqlRelationalTableModel>

#include <Money_t.h>

/**
 * @brief
 *
 */
class ModelCommodity : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelCommodity(QObject *parent);

    Money_t::val_t amount(const qlonglong &id);

    /**
     * @brief
     *
     * @param section
     * @param orientation
     * @param role
     * @return QVariant
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    bool changeAmount(const qlonglong &id, const Money_t::val_t &change);
};

#endif // MODELCOMMODITY_H
