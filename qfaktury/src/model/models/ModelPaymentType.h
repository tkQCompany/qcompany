#ifndef MODELPAYMENTTYPE_H
#define MODELPAYMENTTYPE_H

#include <QSqlTableModel>

/**
 * @brief
 *
 */
class ModelPaymentType : public QSqlTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelPaymentType(QObject *parent = 0);
};

#endif // MODELPAYMENTTYPE_H
