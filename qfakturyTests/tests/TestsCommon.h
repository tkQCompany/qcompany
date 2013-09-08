#ifndef TESTSCOMMON_H
#define TESTSCOMMON_H

#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QDir>

#include "SettingsGlobal.h"

namespace TestsCommon
{

void setAppData()
{
    QCoreApplication::setApplicationName("QFaktury");
    QCoreApplication::setOrganizationName("www.e-linux.pl");
    QCoreApplication::setOrganizationDomain("www.e-linux.pl");
    QCoreApplication::setApplicationVersion(APP_VERSION);
}


void removeDBFile()
{
    const QString dbFilename(QString("%1-%2.db3").arg(QCoreApplication::applicationName()).arg(APP_VERSION));
    if(QFile::exists(dbFilename))
    {
        QDir dir;
        dir.remove(dbFilename);
    }
}

}

#endif
