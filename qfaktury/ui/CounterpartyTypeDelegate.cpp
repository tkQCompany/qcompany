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
        const int myCompany = ( (comboBox->count() > 1) ? 1 : 0) + 1;//skips my company and adds 1 because SQL starts from 1
        model->setData(index, comboBox->currentIndex() + myCompany, Qt::EditRole); //TODO: make sure it won't be possible to have only My_COMPANY in the combobox
        return;
    }
    QSqlRelationalDelegate::setModelData(editor, model, index);
}
