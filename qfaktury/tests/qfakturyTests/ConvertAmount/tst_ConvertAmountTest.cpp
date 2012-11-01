#include <QtCore/QString>
#include <QtTest/QtTest>

class ConvertAmountTest : public QObject
{
    Q_OBJECT
    
public:
    ConvertAmountTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

ConvertAmountTest::ConvertAmountTest()
{
}

void ConvertAmountTest::initTestCase()
{
}

void ConvertAmountTest::cleanupTestCase()
{
}

void ConvertAmountTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void ConvertAmountTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(ConvertAmountTest)

#include "tst_ConvertAmountTest.moc"
