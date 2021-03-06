#ifndef MODELCOUNTERPARTY_H
#define MODELCOUNTERPARTY_H

#include <QSqlRelationalTableModel>

/**
 * @brief
 *
 */
class ModelCounterparty : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelCounterparty(QObject *parent);
    /**
     * @brief
     *
     * @param section
     * @param orientation
     * @param role
     * @return QVariant
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool isInvNumFormatEmpty(const QModelIndex &idCounterparty) const;

    void setOnlyMyCompanyVisible(const bool yes);
};

#endif
