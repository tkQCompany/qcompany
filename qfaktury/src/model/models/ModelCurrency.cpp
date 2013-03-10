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

    int currencyUnit;
    double currencyRate;
    QString currencyName, currencyCode;
    const QString updateSqlTemplate("UPDATE `currency` SET `code_unit` = %1, exchange_rate_pln = %2 WHERE `code` = '%3'");
    const QDomNodeList elements(xmlDoc.documentElement()
                                .elementsByTagName("pozycja"));
    for(size_t i = 0; i < elements.length(); ++i)
    {
        const QDomNodeList children(elements.item(i).childNodes());
        for(size_t j = 0; j < children.length(); ++j)
        {
            if(children.item(j).nodeName() == "nazwa_waluty")
            {
                QDomElement e = children.item(j).toElement();
                currencyName = e.firstChild().nodeValue();
                qDebug() << e.nodeName() << ": " <<
                            currencyName;
            }
            else
            if(children.item(j).nodeName() == "przelicznik")
            {
                QDomElement e = children.item(j).toElement();
                currencyUnit = e.firstChild().nodeValue().toInt();
                qDebug() << e.nodeName() << ": " <<
                            currencyUnit;
            }
            else
            if(children.item(j).nodeName() == "kod_waluty")
            {
                QDomElement e = children.item(j).toElement();
                currencyCode = e.firstChild().nodeValue();
                qDebug() << e.nodeName() << ": " <<
                            currencyCode;
            }
            else
            if(children.item(j).nodeName() == "kurs_sredni")
            {
                QDomElement e = children.item(j).toElement();
                currencyRate = e.firstChild().nodeValue().toDouble();
                qDebug() << e.nodeName() << ": " <<
                            currencyRate;
            }
        }
        QSqlQuery q(query());
        database().transaction();
        q.exec(updateSqlTemplate.arg(currencyUnit).arg(currencyRate).arg(currencyCode));
        if(!q.isActive())
        {
            qDebug() << "ModelCurrency::updateDBsCurrenciesRates_(): " << q.lastError().text();
            break;
        }
        database().commit();
    }
    reply->deleteLater();
}
