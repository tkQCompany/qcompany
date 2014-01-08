#include <QTest>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#include <QString>

#include "../TestsCommon/TestsCommon.h"
#include "CounterpartyDialog.h"
#include "Database.h"
#include "SettingsGlobal.h"
#include "ui_CounterpartyDialog.h"
#include "CounterpartyData.h"
#include "CounterpartyTypeData.h"
#include "ModelCounterparty.h"
#include "ModelCountry.h"


struct CounterpartyDialogPublic : public CounterpartyDialog
{
    CounterpartyDialogPublic(QWidget *parent, Database *db, const QModelIndex &id = QModelIndex(), const bool myCompany = false)
        : CounterpartyDialog(parent, db, id, myCompany)
    {
    }

    Ui::CounterpartyDialog *ui() {return CounterpartyDialog::ui; }
};


class CounterpartyDialogTest : public QObject
{
    Q_OBJECT

private:
    void fail_(const int lineNum, const QSqlError &errSql, const QString &lastQuery);
    
private Q_SLOTS:
    void initTestCase();
    void testCaseInitialValues();
    void testCaseGUIAddNewCounterparty();
    void testCaseGUIAddNewCounterparty_data();
};


void CounterpartyDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
    SettingsGlobal s;
    s.setFirstRun(true);
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
    Database db;
    CounterpartyDialogPublic dialog(0, &db, QModelIndex());

    QFETCH(QString, name);
    QFETCH(CounterpartyTypeData::CounterpartyType, typeId);
    QFETCH(QString, country);
    QFETCH(QString, location);
    QFETCH(QString, postalCode);
    QFETCH(QString, street);
    QFETCH(QString, taxID);
    QFETCH(QString, taxID_formal);
    QFETCH(QString, accountNum);
    QFETCH(QString, accountNum_formal);
    QFETCH(QString, www);
    QFETCH(QString, primaryEmail);
    QFETCH(QString, primaryPhone);

    QSqlQuery q(db.modelCounterparty()->query());
    if(!q.prepare("SELECT id_counterparty, name, type_id, country, location, postal_code, street, tax_ident, account_name, www, primary_email, primary_phone FROM counterparty WHERE name = :name"))
    {
        qCritical() << "Preparation of SQL query has failed at line: " << __LINE__ << " (" << q.lastError().text() << ")";
    }
    q.bindValue(":name", name);
    QVERIFY(q.exec());
    if(q.isActive())
    {
        QCOMPARE(q.next(), false);
    }
    else
    {
        fail_(__LINE__, q.lastError(), q.lastQuery());
    }

    QTest::keyClicks(dialog.ui()->lineEditName, name);
    dialog.ui()->comboBoxType->setCurrentIndex(dialog.ui()->comboBoxType->findText(CounterpartyTypeData::name(typeId)));
    dialog.ui()->comboBoxCountry->setCurrentIndex(dialog.ui()->comboBoxCountry->findText(country));
    QTest::keyClicks(dialog.ui()->lineEditLocation, location);
    QTest::keyClicks(dialog.ui()->lineEditPostalCode, postalCode);
    QTest::keyClicks(dialog.ui()->lineEditAddress, street);
    dialog.ui()->lineEditTaxID->setText(taxID);
    dialog.ui()->lineEditAccountName->setText(accountNum);
    QTest::keyClicks(dialog.ui()->lineEditWWW, www);
    QTest::keyClicks(dialog.ui()->lineEditPrimaryEmail, primaryEmail);
    QTest::keyClicks(dialog.ui()->lineEditPrimaryPhone, primaryPhone);
    QTest::mouseClick(dialog.ui()->pushButtonOK, Qt::LeftButton);
    QVERIFY(db.modelCounterparty()->submitAll());
    QCOMPARE(db.modelCounterparty()->lastError().type(), QSqlError::NoError);

    QVERIFY(q.exec());
    if(q.isActive())
    {
        QCOMPARE(q.next(), true);
        const long long correct_id = 2LL; //ID==1 is for our own company
        QCOMPARE(q.value(0).toLongLong(), correct_id);
        QCOMPARE(q.value(1).toString(), name);
        QCOMPARE(q.value(2).toInt(), (int)typeId + 1);//SQL starts from 1 and MY COMPANY is skipped
        QCOMPARE(q.value(3).toString(), country);
        QCOMPARE(q.value(4).toString(), location);
        QCOMPARE(q.value(5).toString(), postalCode);
        QCOMPARE(q.value(6).toString(), street);
        QCOMPARE(q.value(7).toString(), taxID_formal);
        QCOMPARE(q.value(8).toString(), accountNum_formal);
        QCOMPARE(q.value(9).toString(), www);
        QCOMPARE(q.value(10).toString(), primaryEmail);
        QCOMPARE(q.value(11).toString(), primaryPhone);
    }
    else
    {
        fail_(__LINE__, q.lastError(), q.lastQuery());
    }
}



void CounterpartyDialogTest::testCaseGUIAddNewCounterparty_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<CounterpartyTypeData::CounterpartyType>("typeId");
    QTest::addColumn<QString>("country");
    QTest::addColumn<QString>("location");
    QTest::addColumn<QString>("postalCode");
    QTest::addColumn<QString>("street");
    QTest::addColumn<QString>("taxID");
    QTest::addColumn<QString>("taxID_formal");
    QTest::addColumn<QString>("accountNum");
    QTest::addColumn<QString>("accountNum_formal");
    QTest::addColumn<QString>("www");
    QTest::addColumn<QString>("primaryEmail");
    QTest::addColumn<QString>("primaryPhone");

    SettingsGlobal s;

    QTest::newRow("") << QString("name")
                      << CounterpartyTypeData::COMPANY
                      << s.value(s.COUNTRY).toString()
                      << QString("location")
                      << QString("postal_code")
                      << QString("street 123/1234")
                      << QString("0123456789")
                      << QString("012-345-67-89")
                      << QString("99999999999999999999999999")
                      << QString("99-9999-9999-9999-9999-9999-9999")
                      << QString("www.test.com")
                      << QString("email@test.com")
                      << QString("0123456789");
}



void CounterpartyDialogTest::fail_(const int lineNum, const QSqlError &errSql, const QString &lastQuery)
{
    QFAIL(qPrintable(QString("Error(line: %1): Errnum: %2\nError: %3\nLast query: %4")
                     .arg(lineNum)
                     .arg(errSql.number())
                     .arg(errSql.text())
                     .arg(lastQuery)));
}

Q_DECLARE_METATYPE(CounterpartyTypeData::CounterpartyType)

QTEST_MAIN(CounterpartyDialogTest)

#include "tst_counterpartydialogtest.moc"
