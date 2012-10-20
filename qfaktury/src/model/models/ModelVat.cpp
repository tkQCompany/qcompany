#include "ModelVat.h"

ModelVat::ModelVat(QObject *parent) : QStringListModel(parent)
{
    listVat_.append(sett().value("stawki").toString().split(QChar('|')));
    setStringList(listVat_);
}
