#include "ModelVat.h"
#include "SettingsGlobal.h"

ModelVat::ModelVat(QObject *parent) : QStringListModel(parent)
{
    SettingsGlobal s;
    listVat_.append(s.value(s.VAT_RATES).toString().split(QChar('|')));
    setStringList(listVat_);
}


QStringList ModelVat::listVAT() const
{
    return listVat_;
}
