#include "CommodityDelegate.h"

CommodityDelegate::CommodityDelegate(QObject *parent) :
    QSqlRelationalDelegate(parent)
{
}


void CommodityDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = dynamic_cast<QComboBox*>(editor);
    if(comboBox && (comboBox->objectName().compare("comboBoxMeasureUnit") == 0) && (comboBox->count() > 0))
    {
        model->setData(index, comboBox->currentIndex() + 1, Qt::EditRole); //adds 1 because SQL starts from 1
    }
    else
    {
        QSqlRelationalDelegate::setModelData(editor, model, index);
    }
}
