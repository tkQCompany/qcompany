#ifndef MODELPHONE_H
#define MODELPHONE_H

#include <QSqlRelationalTableModel>

/**
 * @brief
 *
 */
class ModelPhone : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelPhone(QObject *parent);
    /**
     * @brief
     *
     * @param idCounterparty
     */
    void setIDCounterparty(const QString &idCounterparty);
};

#endif // MODELPHONE_H
