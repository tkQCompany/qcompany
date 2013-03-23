#include <QtTest/QtTest>

#include "ConvertAmount.h"

class ConvertAmountTest : public QObject
{
    Q_OBJECT
    
public:
    ConvertAmountTest();
    
private Q_SLOTS:
    void testCasePLN();
    void testCasePLN_data();
};

ConvertAmountTest::ConvertAmountTest()
{
}

void ConvertAmountTest::testCasePLN()
{
    QFETCH(QString, arg);
    QFETCH(QString, currency);
    QFETCH(QString, ret);
    QCOMPARE(ConvertAmount::convertPL(arg, currency), ret);
}

void ConvertAmountTest::testCasePLN_data()
{
    QTest::addColumn<QString>("arg");
    QTest::addColumn<QString>("currency");
    QTest::addColumn<QString>("ret");
    const QString currency("PLN");
    for(int i = 0; i < 100; ++i)
    {
        const QString textNumberFormatDot(QString("0,%1").arg(i, 2, 10, QChar('0')));
        QTest::newRow(textNumberFormatDot.toAscii()) << textNumberFormatDot << currency << QString("%1/100 PLN").arg(i, 2, 10, QChar('0'));
    }
    QTest::newRow("1,00") << QString("1,00") << currency << QString("jeden 00/100 PLN");
    QTest::newRow("2,00") << QString("2,00") << currency << QString("dwa 00/100 PLN");
    QTest::newRow("3,00") << QString("3,00") << currency << QString("trzy 00/100 PLN");
    QTest::newRow("4,00") << QString("4,00") << currency << QString("cztery 00/100 PLN");
    QTest::newRow("5,00") << QString("5,00") << currency << QString("pięć 00/100 PLN");
    QTest::newRow("6,00") << QString("6,00") << currency << QString("sześć 00/100 PLN");
    QTest::newRow("7,00") << QString("7,00") << currency << QString("siedem 00/100 PLN");
    QTest::newRow("8,00") << QString("8,00") << currency << QString("osiem 00/100 PLN");
    QTest::newRow("9,00") << QString("9,00") << currency << QString("dziewięć 00/100 PLN");

    QTest::newRow("10,00") << QString("10,00") << currency << QString("dziesięć 00/100 PLN");
    QTest::newRow("20,00") << QString("20,00") << currency << QString("dwadzieścia 00/100 PLN");
    QTest::newRow("30,00") << QString("30,00") << currency << QString("trzydzieści 00/100 PLN");
    QTest::newRow("40,00") << QString("40,00") << currency << QString("czterdzieści 00/100 PLN");
    QTest::newRow("50,00") << QString("50,00") << currency << QString("pięćdziesiąt 00/100 PLN");
    QTest::newRow("60,00") << QString("60,00") << currency << QString("sześćdziesiąt 00/100 PLN");
    QTest::newRow("70,00") << QString("70,00") << currency << QString("siedemdziesiąt 00/100 PLN");
    QTest::newRow("80,00") << QString("80,00") << currency << QString("osiemdziesiąt 00/100 PLN");
    QTest::newRow("90,00") << QString("90,00") << currency << QString("dziewięćdziesiąt 00/100 PLN");

    QTest::newRow("100,00") << QString("100,00") << currency << QString("sto 00/100 PLN");
    QTest::newRow("200,00") << QString("200,00") << currency << QString("dwieście 00/100 PLN");
    QTest::newRow("300,00") << QString("300,00") << currency << QString("trzysta 00/100 PLN");
    QTest::newRow("400,00") << QString("400,00") << currency << QString("czterysta 00/100 PLN");
    QTest::newRow("500,00") << QString("500,00") << currency << QString("pięćset 00/100 PLN");
    QTest::newRow("600,00") << QString("600,00") << currency << QString("sześćset 00/100 PLN");
    QTest::newRow("700,00") << QString("700,00") << currency << QString("siedemset 00/100 PLN");
    QTest::newRow("800,00") << QString("800,00") << currency << QString("osiemset 00/100 PLN");
    QTest::newRow("900,00") << QString("900,00") << currency << QString("dziewięćset 00/100 PLN");

    QTest::newRow("1000,00") << QString("1000,00") << currency << QString("tysiąc 00/100 PLN");

    QTest::newRow("232,33") << QString("232,33") << currency << QString("dwieście trzydzieści dwa 33/100 PLN");
}

QTEST_APPLESS_MAIN(ConvertAmountTest)

#include "tst_ConvertAmountTest.moc"
