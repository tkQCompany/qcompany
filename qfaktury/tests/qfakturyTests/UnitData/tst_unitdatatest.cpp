#include <QtCore/QString>
#include <QtTest/QtTest>

class UnitDataTest : public QObject
{
    Q_OBJECT
    
public:
    UnitDataTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

UnitDataTest::UnitDataTest()
{
}

void UnitDataTest::initTestCase()
{
}

void UnitDataTest::cleanupTestCase()
{
}

void UnitDataTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void UnitDataTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(UnitDataTest)

#include "tst_unitdatatest.moc"
