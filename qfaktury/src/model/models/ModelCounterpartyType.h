#ifndef MODELCOUNTERPARTYTYPE_H
#define MODELCOUNTERPARTYTYPE_H

#include <QSqlTableModel>

/**
 * @brief
 *
 */
class ModelCounterpartyType : public QSqlTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelCounterpartyType(QObject *parent);
    bool addType(const QString &type);
    bool delType(const QString &type);
    void setMyCompanyVisible(const bool yes, const bool onlyMyCompany = false);

private:
    bool addDelType_(const QString &type, const bool addType);
};

#endif // MODELCOUNTERPARTYTYPE_H
