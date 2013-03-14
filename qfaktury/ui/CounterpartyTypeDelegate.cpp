#include "CounterpartyTypeDelegate.h"
#include <QDebug>

CounterpartyTypeDelegate::CounterpartyTypeDelegate(QObject *parent) :
    QSqlRelationalDelegate(parent)
{
}


void CounterpartyTypeDelegate::setModelData(QWidget *editor,
                                            QAbstractItemModel *model,
                                            const QModelIndex &index) const
{
    QComboBox *comboBox = dynamic_cast<QComboBox*>(editor);
    if(comboBox && (comboBox->objectName().compare("comboBoxType") == 0) )
    {
        model->setData(index, index.row()+2, Qt::EditRole); //+2 - skips my company and adds 1 because SQL starts from 1 TODO: maybe better code is possible?
        return;
    }
    QSqlRelationalDelegate::setModelData(editor, model, index);
}
