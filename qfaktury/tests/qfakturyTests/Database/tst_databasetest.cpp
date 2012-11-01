#include <QtCore/QString>
#include <QtTest/QtTest>

class DatabaseTest : public QObject
{
    Q_OBJECT
    
public:
    DatabaseTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

DatabaseTest::DatabaseTest()
{
}

void DatabaseTest::initTestCase()
{
}

void DatabaseTest::cleanupTestCase()
{
}

void DatabaseTest::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void DatabaseTest::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(DatabaseTest)

#include "tst_databasetest.moc"
