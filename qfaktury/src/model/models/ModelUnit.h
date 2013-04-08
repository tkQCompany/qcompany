#ifndef MODELUNIT_H
#define MODELUNIT_H

#include <QSqlTableModel>

/**
 * @brief
 *
 */
class ModelUnit : public QSqlTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelUnit(QObject *parent);

    bool addUnit(const QString &unitName);
    bool deleteUnit(const QString &unitName);
};

#endif // MODELUNIT_H
