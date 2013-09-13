#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

#include "ModelUnit.h"

ModelUnit::ModelUnit(QObject *parent) :
    QSqlTableModel(parent, QSqlDatabase::database())
{
    setTable("unit");
}


bool ModelUnit::addUnit(const QString &unitName)
{
    QSqlQuery q(query());
    database().transaction();
    q.exec(QString("INSERT INTO unit(`unit_name`) VALUES(\"%1\")").arg(unitName));
    if(q.isActive())
    {
        database().commit();
        select();
        return true;
    }
    else
    {
        QMessageBox::warning(0, trUtf8("Błąd w bazie danych"),
                             trUtf8("Błąd podczas dodawania jednostki: %1")
                             .arg(lastError().text()));
        database().rollback();
        return false;
    }
}


bool ModelUnit::deleteUnit(const QString &unitName)
{
    QSqlQuery q(query());
    database().transaction();
    q.exec(QString("SELECT `id_unit` FROM `unit` WHERE `unit_name`='%1'").arg(unitName));
    if(q.isActive())
    {
        if(q.next())
        {
            q.exec(QString("DELETE FROM `unit` WHERE `id_unit` = %1").arg(q.value(0).toUInt()));
            const bool ret = database().commit();
            select();
            return ret;
        }
        else
        {
            qDebug() << "ModelUnit::deleteUnit(): " << "Jednostka już nie istnieje w bazie danych";
            qDebug() << q.lastQuery();
        }
    }
    else
    {
        QMessageBox::warning(0, trUtf8("Błąd w bazie danych"),
                             trUtf8("Błąd podczas usuwania jednostki: %1")
                             .arg(lastError().text()));
    }
    database().rollback();
    return false;
}
