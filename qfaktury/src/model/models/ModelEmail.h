#ifndef MODELEMAIL_H
#define MODELEMAIL_H

#include <QSqlRelationalTableModel>

#include "../models_data/EmailData.h"

/**
 * @brief
 *
 */
class ModelEmail : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     * @param id_counterparty
     */
    explicit ModelEmail(QObject *parent, const QString &id_counterparty);
};

#endif // MODELEMAIL_H
