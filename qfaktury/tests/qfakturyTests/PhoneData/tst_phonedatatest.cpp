#include <QtCore/QString>
#include <QtTest/QtTest>

class PhoneDataTest : public QObject
{
    Q_OBJECT
    
public:
    PhoneDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

PhoneDataTest::PhoneDataTest()
{
}

void PhoneDataTest::initTestCase()
{
}

void PhoneDataTest::cleanupTestCase()
{
}

void PhoneDataTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void PhoneDataTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(PhoneDataTest)

#include "tst_phonedatatest.moc"
