#ifndef COUNTERPARTYTYPEDELEGATE_H
#define COUNTERPARTYTYPEDELEGATE_H

#include <QSqlRelationalDelegate>


class CounterpartyTypeDelegate : public QSqlRelationalDelegate
{
    Q_OBJECT
public:
    explicit CounterpartyTypeDelegate(QObject *parent = 0);

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
        
};

#endif // COUNTERPARTYTYPEDELEGATE_H
