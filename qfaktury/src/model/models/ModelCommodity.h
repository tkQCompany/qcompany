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

    /**
     * @brief
     *
     * @param section
     * @param orientation
     * @param role
     * @return QVariant
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // MODELCOMMODITY_H
