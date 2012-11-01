#include <QtCore/QString>
#include <QtTest/QtTest>

class EmailDataTest : public QObject
{
    Q_OBJECT
    
public:
    EmailDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

EmailDataTest::EmailDataTest()
{
}

void EmailDataTest::initTestCase()
{
}

void EmailDataTest::cleanupTestCase()
{
}

void EmailDataTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void EmailDataTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(EmailDataTest)

#include "tst_emaildatatest.moc"
