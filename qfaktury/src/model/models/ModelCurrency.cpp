#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>
#include <QRegExp>
#include <QObject>
#include <QDebug>
#include <QDomDocument>
#include <QSqlQuery>
#include <QSqlError>

#include "SettingsGlobal.h"
#include "ModelCurrency.h"

ModelCurrency::ModelCurrency(QObject *parent) :
    QSqlTableModel(parent, QSqlDatabase::database())
{
    setTable("currency");
}


void ModelCurrency::updateCurrenciesRates()
{
    netMgr_ = new QNetworkAccessManager(this);
    const QNetworkReply *reply(netMgr_->get(QNetworkRequest(QUrl("http://www.nbp.pl/kursy/xml/dir.txt"))));
    connect(reply, SIGNAL(finished()), this, SLOT(netReplyFinished_()));
}


void ModelCurrency::netReplyFinished_()
{
    QString line;
    QStringList matchedLines;
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    const QRegExp regexp(QString("(a)(\\d\\d\\d)(z)(%1)(%2)(\\d\\d)")
                   .arg(QDate::currentDate().toString("yy"))
                   .arg(QDate::currentDate().toString("MM")));

    disconnect(netMgr_);

    while(!reply->atEnd())
    {
        line = reply->readLine().trimmed();
        if(line.contains(regexp))
        {
            matchedLines.append(line);
        }
    }

    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "ModelCurrency::netReplyFinished_(): error: " << reply->errorString();
    }
    else
    {
        if(!matchedLines.isEmpty())
        {
            const QString ratesFileXml(matchedLines.at(matchedLines.size() - 1)
                                       + ".xml");
            const QString urlStr(QString("http://www.nbp.pl/kursy/xml/%1").arg(ratesFileXml));
            const QNetworkReply *reply2 = netMgr_->get(QNetworkRequest(QUrl(urlStr)));
            connect(reply2, SIGNAL(finished()), this,
                    SLOT(updateDBsCurrenciesRates_()));

        }
        else
        {
            qDebug() << "ModelCurrency::netReplyFinished_(): no matched lines with currency rates";
        }
    }
    reply->deleteLater();
}


void ModelCurrency::updateDBsCurrenciesRates_()
{
    QDomDocument xmlDoc;
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    {
        QString errorStr;
        if(!xmlDoc.setContent(reply->readAll(), &errorStr))
        {
            qDebug() << "Can't read XML from the network: " << errorStr;
            qDebug() << "URL XML: " <<  reply->url().toString();
            qDebug() << "XML Doc: " << xmlDoc.toString();
            return;
        }
    }

    bool ret = true;
    int currencyUnit;
    double currencyRate;
    QString currencyName, currencyCode;
    const QString lastUpdateElementName("data_publikacji");
    const QDomNodeList lastUpdate(xmlDoc.documentElement()
                                  .elementsByTagName(lastUpdateElementName));
    if(lastUpdate.length() > 0)
    {
        const QDomNodeList lastUpdateChild(lastUpdate.at(0).childNodes());
        if(lastUpdateChild.length() > 0)
        {
            lastUpdateByCentralBank_ = QDate::fromString(lastUpdateChild.at(0).nodeValue(), "yyyy-MM-dd");
        }
        else
        {
            qDebug() << "Can't extract node value from '" << lastUpdateElementName << "' element.";
        }
    }
    else
    {
        qDebug() << "Can't find '" << lastUpdateElementName << "' element.";
    }

    const QDomNodeList elements(xmlDoc.documentElement()
                                .elementsByTagName("pozycja"));
    SettingsGlobal s;
    for(int i = 0; i < elements.size(); ++i)
    {
        const QDomNodeList children(elements.item(i).childNodes());
        for(int j = 0; j < children.size(); ++j)
        {
            if(children.item(j).nodeName() == "nazwa_waluty")
            {
                QDomElement e = children.item(j).toElement();
                currencyName = e.firstChild().nodeValue();
            }
            else
            if(children.item(j).nodeName() == "przelicznik")
            {
                QDomElement e = children.item(j).toElement();
                currencyUnit = e.firstChild().nodeValue().toInt();
            }
            else
            if(children.item(j).nodeName() == "kod_waluty")
            {
                QDomElement e = children.item(j).toElement();
                currencyCode = e.firstChild().nodeValue();
            }
            else
            if(children.item(j).nodeName() == "kurs_sredni")
            {
                QDomElement e = children.item(j).toElement();
                currencyRate = s.stringToDouble(e.firstChild().nodeValue());
            }
        }
        QSqlQuery q(query());
        const QString updateSqlTemplate("UPDATE `currency` SET `code_unit` = %1, `exchange_rate_pln` = %2, `localized_name` = '%4' WHERE `code` = '%3'");
        database().transaction();
        q.exec(updateSqlTemplate.arg(currencyUnit).arg(currencyRate).arg(currencyCode).arg(currencyName));
        if(!q.isActive())
        {
            qDebug() << "ModelCurrency::updateDBsCurrenciesRates_(): " << q.lastError().text();
            ret = false;
            break;
        }

        if(!database().commit())
        {
            ret = false;
            break;
        }
    }
    if(ret)
        emit updatingCurrenciesRatesFinished();
    reply->deleteLater();
}
