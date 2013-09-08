#include <QTest>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>

#include "../TestsCommon.h"
#include "CounterpartyDialog.h"
#include "Database.h"
#include "SettingsGlobal.h"
#include "ui_CounterpartyDialog.h"
#include "CounterpartyData.h"
#include "CounterpartyTypeData.h"
#include "ModelCounterparty.h"


struct CounterpartyDialogPublic : public CounterpartyDialog
{
    CounterpartyDialogPublic(QWidget *parent, Database *db, const QModelIndex &id = QModelIndex(), const bool myCompany = false)
        : CounterpartyDialog(parent, db, id, myCompany)
    {
    }

    Ui::CounterpartyDialog *ui() {return ui_; }
};


class CounterpartyDialogTest : public QObject
{
    Q_OBJECT
    
public:
    CounterpartyDialogTest();

private:
    void fail_(const int lineNum, const QSqlError &errSql, const QString &lastQuery);
    
private Q_SLOTS:
    void initTestCase();
    void init();
    void testCaseInitialValues();
    void testCaseGUIAddNewCounterparty();
};

CounterpartyDialogTest::CounterpartyDialogTest()
{
}

void CounterpartyDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    SettingsGlobal s;
    s.setFirstRun(true);
}

void CounterpartyDialogTest::init()
{
    TestsCommon::removeDBFile();
}


void CounterpartyDialogTest::testCaseInitialValues()
{
    SettingsGlobal s;
    Database db;
    CounterpartyDialogPublic dialog(0, &db, QModelIndex());
    QCOMPARE(dialog.ui()->comboBoxType->currentText(), CounterpartyTypeData::name(CounterpartyTypeData::COMPANY));
    QCOMPARE(dialog.ui()->comboBoxCountry->currentText(), s.value(s.COUNTRY).toString());
}

void CounterpartyDialogTest::testCaseGUIAddNewCounterparty()
{
    SettingsGlobal s;
    Database db;
    CounterpartyDialogPublic dialog(0, &db, QModelIndex());

    const QString counterpartyName("Kontr1");
    QSqlQuery q(db.modelCounterparty()->query());
    if(!q.prepare("SELECT id_counterparty FROM counterparty WHERE name = :name"))
    {
        qCritical() << "Preparation of SQL query has failed at line: " << __LINE__ << " (" << q.lastError().text() << ")";
    }
    q.bindValue(":name", counterpartyName);
    QVERIFY(q.exec());
    if(q.isActive())
    {
        QCOMPARE(q.next(), false);
    }
    else
    {
        fail_(__LINE__, q.lastError(), q.lastQuery());
    }

    QTest::keyClicks(dialog.ui()->lineEditName, "Kontr1");
    QTest::keyClicks(dialog.ui()->lineEditLocation, counterpartyName + "LOCATION");
    QTest::keyClicks(dialog.ui()->lineEditPostalCode, counterpartyName + "POSTALCODE");
    QTest::keyClicks(dialog.ui()->lineEditAddress, counterpartyName + "ADDRESS");
    //QTest::keyClicks(dialog.ui()->lineEditTaxID, QString("012-345-67-89"));
    //QTest::keyClicks(dialog.ui()->lineEditAccountName, QString("99-9999-9999-9999-9999-9999-9999"));
    QTest::mouseClick(dialog.ui()->pushButtonOK, Qt::LeftButton);
    QVERIFY(db.modelCounterparty()->submitAll());
    QCOMPARE(db.modelCounterparty()->lastError().type(), QSqlError::NoError);

    QVERIFY(q.exec());
    if(q.isActive())
    {
        QCOMPARE(q.next(), true);
        const long long correct_id = 2LL; //ID==1 is for our own company
        QCOMPARE(q.value(0).toLongLong(), correct_id);
    }
    else
    {
        fail_(__LINE__, q.lastError(), q.lastQuery());
    }
}

void CounterpartyDialogTest::fail_(const int lineNum, const QSqlError &errSql, const QString &lastQuery)
{
    QFAIL(qPrintable(QString("Error(line: %1): Errnum: %2\nError: %3\nLast query: %4")
                     .arg(lineNum)
                     .arg(errSql.number())
                     .arg(errSql.text())
                     .arg(lastQuery)));
}


QTEST_MAIN(CounterpartyDialogTest)

#include "tst_counterpartydialogtest.moc"
