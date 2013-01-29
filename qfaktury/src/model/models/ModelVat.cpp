#include "ModelVat.h"

ModelVat::ModelVat(QObject *parent) : QStringListModel(parent)
{
    listVat_.append(sett().value(SettingsGlobal::keyName(SettingsGlobal::VAT_RATES)).toString().split(QChar('|')));
    setStringList(listVat_);
}
