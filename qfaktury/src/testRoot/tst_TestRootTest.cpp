#include <QTest>
#include <QtCore/QCoreApplication>
#include <QTextCodec>

#include "../ConvertAmount.h"

class TestRootTest : public QObject
{
    Q_OBJECT
    
public:
    TestRootTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseConvertAmount();
    void testCaseConvertAmount_data();
};

TestRootTest::TestRootTest()
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
}

void TestRootTest::initTestCase()
{
}

void TestRootTest::cleanupTestCase()
{
}

void TestRootTest::testCaseConvertAmount()
{
    QFETCH(QString, input_number);
    QFETCH(QString, currency);
    QFETCH(QString, output);

    QCOMPARE(ConvertAmount::convertPL(input_number, currency), output);
}

void TestRootTest::testCaseConvertAmount_data()
{
    QTest::addColumn<QString>("input_number");
    QTest::addColumn<QString>("currency");
    QTest::addColumn<QString>("output");
    QTest::newRow("ConvertAmount 0.00 PL") << QString("0.00") << QString("PLN") << QString("zero złotych");
}

QTEST_MAIN(TestRootTest)

#include "tst_TestRootTest.moc"
