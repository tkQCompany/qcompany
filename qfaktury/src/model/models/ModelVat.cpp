#include "ModelVat.h"

ModelVat::ModelVat(QObject *parent) : QStringListModel(parent)
{
    SettingsGlobal s;
    listVat_.append(s.value(s.keyName(s.VAT_RATES)).toString().split(QChar('|')));
    setStringList(listVat_);
}
