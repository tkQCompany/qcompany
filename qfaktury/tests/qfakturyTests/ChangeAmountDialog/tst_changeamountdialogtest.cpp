#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

class ChangeAmountDialogTest : public QObject
{
    Q_OBJECT
    
public:
    ChangeAmountDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

ChangeAmountDialogTest::ChangeAmountDialogTest()
{
}

void ChangeAmountDialogTest::initTestCase()
{
}

void ChangeAmountDialogTest::cleanupTestCase()
{
}

void ChangeAmountDialogTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void ChangeAmountDialogTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(ChangeAmountDialogTest)

#include "tst_changeamountdialogtest.moc"
