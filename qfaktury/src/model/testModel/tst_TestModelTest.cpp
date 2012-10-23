#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

#include "../Database.h"

class TestModelTest : public QObject
{
    Q_OBJECT

    Database *db_;
    
public:
    TestModelTest();

private:
    InvoiceData createRandomInvoice();
    QList<CommodityData> createRandomListOfCommodities(const int size);
    QList<CommodityVisualData> convertToVisual(const QList<CommodityData> &listOrig);
    void deleteAllCommodities();
    void deleteAllInvoices();
    void insertCommodities(const QList<CommodityData> &list);
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseInsertInvoiceWithItems();
    void testPerfInsertInvoiceWithItems();
};

TestModelTest::TestModelTest()
{
}

void TestModelTest::initTestCase()
{
    db_ = new Database(this);
}

void TestModelTest::cleanupTestCase()
{
    QFile file(db_->dbFileName());
    delete db_;
    if(file.exists())
    {
        file.remove();
    }
}


InvoiceData TestModelTest::createRandomInvoice()
{
    InvoiceData d;
    QString str;
    d.additText = str.fill('t', qrand()%200);
    d.counterpartyID = 1;
    d.currencyID = 1;
    d.discount = qrand()%100;
    d.id = 1;
    d.invNumber = QString("1/2012");
    d.issuanceDate = QDate::currentDate();
    d.paymentDate = QDate::currentDate();
    d.paymentID = 1;
    d.sellingDate = QDate::currentDate();
    d.typeID = 1;

    return d;
}

QList<CommodityData> TestModelTest::createRandomListOfCommodities(const int size)
{
    QList<CommodityData> list;

    for(int i = 0; i < size; ++i)
    {
        CommodityData cd;
        QString tmp;

        cd.abbrev = QString("%1").arg(qrand()%10 + 1);
        cd.id = i + 1;
        cd.name = tmp.fill('n', qrand()%20 + 1);
        cd.net1 = qrand()%40000;
        cd.net2 = qrand()%40000;
        cd.net3 = qrand()%40000;
        cd.net4 = qrand()%40000;
        cd.pkwiu = tmp.fill('p', qrand()%20 + 1);
        cd.quantity = qrand()%3 + 1;
        cd.type_id = ((qrand()%2) ? CommodityTypeData::GOODS: CommodityTypeData::SERVICES);
        cd.unit_id = (qrand() % (UnitData::PACKAGE - UnitData::UNIT + 1)) + 1;
        cd.vat = QString("%1").arg(qrand() % 100);

        list.append(cd);
    }
    return list;
}



QList<CommodityVisualData> TestModelTest::convertToVisual(const QList<CommodityData> &listOrig)
{
    QList<CommodityVisualData> listRet;

    foreach(CommodityData cd, listOrig)
    {
        CommodityVisualData cvd;
        cvd.discount = QString("%1").arg(qrand() % 100);
        cvd.id = QString("%1").arg(cd.id);
        cvd.name = cd.name;

        double net = 0.0;
        switch(qrand()%4)
        {
        case 0:
            net = cd.net1;
            break;
        case 1:
            net = cd.net2;
            break;
        case 2:
            net = cd.net3;
            break;
        case 3:
            net = cd.net4;
            break;
        }
        cvd.net = QString("%1").arg(net);

        cvd.pkwiu = cd.pkwiu;
        cvd.quantity = QString("%1").arg(cd.quantity);
        cvd.type = CommodityTypeData::names(cd.type_id);
        cvd.unit = UnitData::name(cd.unit_id);
        cvd.vat = cd.vat;

        listRet.append(cvd);
    }
    return listRet;
}


void TestModelTest::deleteAllCommodities()
{
    QSqlQuery query(db_->modelCommodity()->query());
    query.exec("DELETE FROM commodity");
}


void TestModelTest::deleteAllInvoices()
{
    QSqlQuery query(db_->modelInvoice()->query());
    query.exec("DELETE FROM invoice");
}


void TestModelTest::insertCommodities(const QList<CommodityData> &list)
{
    foreach(const CommodityData v, list)
    {
        QSqlRecord rec(db_->modelCommodity()->record());
        for(int i = CommodityFields::ID; i <= CommodityFields::QUANTITY; ++i)
        {
            rec.setValue(i, v.field(i));
        }

        if(!db_->modelCommodity()->insertRecord(-1, rec))
        {
            qCritical() << "TestModelTest::insertCommodities(): record inserting failed.";
        }
    }
    if(!db_->modelCommodity()->submitAll())
    {
        qCritical() << "TestModelTest::insertCommodities(): submitting all data wasn't succeed: " << db_->modelCommodity()->lastError().text();
    }
}


void TestModelTest::testCaseInsertInvoiceWithItems()
{
    const InvoiceData invoice(createRandomInvoice());
    const QList<CommodityData> listCommodOrig(createRandomListOfCommodities(1));

    deleteAllInvoices();
    deleteAllCommodities();

    insertCommodities(listCommodOrig);

    const QList<CommodityVisualData> listCommodVisualOrig(convertToVisual(listCommodOrig));
    if(db_->invoiceWithCommoditiesInsertTransact(invoice, listCommodVisualOrig))
    {
        const QList<CommodityVisualData> listCommodVisualReturned(db_->commodities(invoice.id));
        QCOMPARE(listCommodVisualReturned.size(), listCommodVisualOrig.size());

        for(int i = 0; i < listCommodVisualReturned.size(); ++i)
        {
            for(int j = CommodityVisualFields::ID; j <= CommodityVisualFields::DISCOUNT; ++j)
            {
                QVERIFY2(listCommodVisualReturned.at(i).field(j) == listCommodVisualOrig.at(i).field(j), QString("j = %1").arg(j).toAscii());
            }
        }
    }
}


void TestModelTest::testPerfInsertInvoiceWithItems()
{
    const InvoiceData invData(createRandomInvoice());
    const QList<CommodityData> listCommodOrig(createRandomListOfCommodities(1));

    deleteAllInvoices();
    deleteAllCommodities();

    insertCommodities(listCommodOrig);
    const QList<CommodityVisualData> listCommodVisualOrig(convertToVisual(listCommodOrig));

    QBENCHMARK
    {
        db_->invoiceWithCommoditiesInsertTransact(invData, listCommodVisualOrig);
        deleteAllInvoices();
        deleteAllCommodities();
    }
}

QTEST_MAIN(TestModelTest)

#include "tst_TestModelTest.moc"
