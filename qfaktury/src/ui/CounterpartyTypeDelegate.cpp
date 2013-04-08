#include "CounterpartyTypeDelegate.h"

CounterpartyTypeDelegate::CounterpartyTypeDelegate(QObject *parent) :
    QSqlRelationalDelegate(parent)
{
}


void CounterpartyTypeDelegate::setModelData(QWidget *editor,
                                            QAbstractItemModel *model,
                                            const QModelIndex &index) const
{
    QComboBox *comboBox = dynamic_cast<QComboBox*>(editor);
    if(comboBox && (comboBox->objectName().compare("comboBoxType") == 0) && (comboBox->count() > 0))
    {
        const int myCompany = ( (comboBox->count() > 1) ? 1 : 0) + 1;//skips my company and adds 1 because SQL starts from 1
        model->setData(index, comboBox->currentIndex() + myCompany, Qt::EditRole);
    }
    else
    {
        QSqlRelationalDelegate::setModelData(editor, model, index);
    }
}
