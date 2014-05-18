#ifndef MODELVAT_H
#define MODELVAT_H

#include <QStringListModel>

/**
 * @brief
 *
 */
class ModelVat : public QStringListModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelVat(QObject *parent);
    QStringList listVAT() const;
private:
    QStringList listVat_; /**< TODO */
};

#endif // MODELVAT_H
