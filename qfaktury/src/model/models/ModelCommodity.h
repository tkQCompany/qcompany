#ifndef MODELCOMMODITY_H
#define MODELCOMMODITY_H

#include <QSqlRelationalTableModel>

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

    double amount(const QString &id);

    /**
     * @brief
     *
     * @param section
     * @param orientation
     * @param role
     * @return QVariant
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    bool changeAmount(const QString &id, const double change);
};

#endif // MODELCOMMODITY_H
