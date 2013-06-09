#include <QString>
#include <QtTest>

#include "CurrencyData.h"
#include "Money_t.h"

class Money_tTest : public QObject
{
    Q_OBJECT
    
public:
    Money_tTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCasePLN();
    void testCasePLN_data();
};

Money_tTest::Money_tTest()
{
}

void Money_tTest::initTestCase()
{
}

void Money_tTest::cleanupTestCase()
{
}

void Money_tTest::testCasePLN()
{
    QFETCH(QString, arg);
    QFETCH(CurrencyData::Currencies, currency);
    QFETCH(QString, ret);

    Money_t m(arg);
    m.setCurrency(currency);
    QCOMPARE(m.verballyPL(), ret);
}

void Money_tTest::testCasePLN_data()
{
    QTest::addColumn<QString>("arg");
    QTest::addColumn<CurrencyData::Currencies>("currency");
    QTest::addColumn<QString>("ret");
    const CurrencyData::Currencies currency = CurrencyData::PLN;

    const int radix = 10;
    const int fieldWidth = 2;
    const QChar fillChar(QChar('0'));
    for(int i = 0; i < 100; ++i)
    {
        const QString textNumberFormatDot(QString("0.%1").arg(i, fieldWidth, radix, fillChar));
        QTest::newRow(textNumberFormatDot.toAscii()) << textNumberFormatDot << currency
                                                     << QString("zero złotych %1/100 PLN").arg(i, fieldWidth, radix, fillChar);
    }
    QTest::newRow("1,00") << "1.0" << currency << QString("jeden złoty 00/100 PLN");
    QTest::newRow("2,00") << "2.0" << currency << QString("dwa złote 00/100 PLN");
    QTest::newRow("3,00") << "3.0" << currency << QString("trzy złote 00/100 PLN");
    QTest::newRow("4,00") << "4.0" << currency << QString("cztery złote 00/100 PLN");
    QTest::newRow("5,00") << "5.0" << currency << QString("pięć złotych 00/100 PLN");
    QTest::newRow("6,00") << "6.0" << currency << QString("sześć złotych 00/100 PLN");
    QTest::newRow("7,00") << "7.0" << currency << QString("siedem złotych 00/100 PLN");
    QTest::newRow("8,00") << "8.0" << currency << QString("osiem złotych 00/100 PLN");
    QTest::newRow("9,00") << "9.0" << currency << QString("dziewięć złotych 00/100 PLN");

    QTest::newRow("10,00") << "10.0" << currency << QString("dziesięć złotych 00/100 PLN");
    QTest::newRow("20,00") << "20.0" << currency << QString("dwadzieścia złotych 00/100 PLN");
    QTest::newRow("30,00") << "30.0" << currency << QString("trzydzieści złotych 00/100 PLN");
    QTest::newRow("40,00") << "40.0" << currency << QString("czterdzieści złotych 00/100 PLN");
    QTest::newRow("50,00") << "50.0" << currency << QString("pięćdziesiąt złotych 00/100 PLN");
    QTest::newRow("60,00") << "60.0" << currency << QString("sześćdziesiąt złotych 00/100 PLN");
    QTest::newRow("70,00") << "70.0" << currency << QString("siedemdziesiąt złotych 00/100 PLN");
    QTest::newRow("80,00") << "80.0" << currency << QString("osiemdziesiąt złotych 00/100 PLN");
    QTest::newRow("90,00") << "90.0" << currency << QString("dziewięćdziesiąt złotych 00/100 PLN");

    QTest::newRow("100,00") << "100.0" << currency << QString("sto złotych 00/100 PLN");
    QTest::newRow("200,00") << "200.0" << currency << QString("dwieście złotych 00/100 PLN");
    QTest::newRow("300,00") << "300.0" << currency << QString("trzysta złotych 00/100 PLN");
    QTest::newRow("400,00") << "400.0" << currency << QString("czterysta złotych 00/100 PLN");
    QTest::newRow("500,00") << "500.0" << currency << QString("pięćset złotych 00/100 PLN");
    QTest::newRow("600,00") << "600.0" << currency << QString("sześćset złotych 00/100 PLN");
    QTest::newRow("700,00") << "700.0" << currency << QString("siedemset złotych 00/100 PLN");
    QTest::newRow("800,00") << "800.0" << currency << QString("osiemset złotych 00/100 PLN");
    QTest::newRow("900,00") << "900.0" << currency << QString("dziewięćset złotych 00/100 PLN");

    QTest::newRow("1000,00") << "1000.0" << currency << QString("tysiąc złotych 00/100 PLN");
    QTest::newRow("2000,00") << "2000.0" << currency << QString("dwa tysiące złotych 00/100 PLN");
    QTest::newRow("3000,00") << "3000.0" << currency << QString("trzy tysiące złotych 00/100 PLN");
    QTest::newRow("4000,00") << "4000.0" << currency << QString("cztery tysiące złotych 00/100 PLN");
    QTest::newRow("5000,00") << "5000.0" << currency << QString("pięć tysięcy złotych 00/100 PLN");
    QTest::newRow("6000,00") << "6000.0" << currency << QString("sześć tysięcy złotych 00/100 PLN");
    QTest::newRow("7000,00") << "7000.0" << currency << QString("siedem tysięcy złotych 00/100 PLN");
    QTest::newRow("8000,00") << "8000.0" << currency << QString("osiem tysięcy złotych 00/100 PLN");
    QTest::newRow("9000,00") << "9000.0" << currency << QString("dziewięć tysięcy złotych 00/100 PLN");

    QTest::newRow("10000,00") << "10000.0" << currency << QString("dziesięć tysięcy złotych 00/100 PLN");
    QTest::newRow("11000,00") << "11000.0" << currency << QString("jedenaście tysięcy złotych 00/100 PLN");

    QTest::newRow("232,33") << "232.33" << currency << QString("dwieście trzydzieści dwa złote 33/100 PLN");
    QTest::newRow("23299,33") << "23299.33" << currency << QString("dwadzieścia trzy tysiące dwieście dziewięćdziesiąt dziewięć złotych 33/100 PLN");
    QTest::newRow("999999999999999999999,99") << "999999999999999999999.99" << currency << QString("dziewięćset dziewięćdziesiąt dziewięć trylionów dziewięćset dziewięćdziesiąt dziewięć biliardów dziewięćset dziewięćdziesiąt dziewięć bilionów dziewięćset dziewięćdziesiąt dziewięć miliardów dziewięćset dziewięćdziesiąt dziewięć milionów dziewięćset dziewięćdziesiąt dziewięć tysięcy dziewięćset dziewięćdziesiąt dziewięć złotych 99/100 PLN");
}

QTEST_APPLESS_MAIN(Money_tTest)

#include "tst_money_ttest.moc"
