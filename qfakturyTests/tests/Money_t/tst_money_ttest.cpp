#include <QString>
#include <QTest>

#include "CurrencyData.h"
#include "Money_t.h"
#include "SettingsGlobal.h"
#include "../../TestsCommon/TestsCommon.h"


class Money_tTest : public QObject
{
    Q_OBJECT
    
private Q_SLOTS:
    void initTestCase();
    void testCaseDefaultValues();
    void testCaseEqualityOperators();
    void testCaseEqualityOperators_data();
    void testCasePLN();
    void testCasePLN_data();
};


void Money_tTest::initTestCase()
{
    TestsCommon::setAppData();
    SettingsGlobal s;
    s.setFirstRun(true);
}


void Money_tTest::testCaseDefaultValues()
{
    Money_t m;
    SettingsGlobal s;
    QCOMPARE(m.toString(2), QString("%0%1%2").arg(0).arg(s.decimalPoint()).arg("00"));
    QCOMPARE((int)m.currency(), (int)s.value(s.keyName(s.DEFAULT_CURRENCY)).value<int>());
}


void Money_tTest::testCaseEqualityOperators()
{
    QFETCH(Money_t, first_arg);
    QFETCH(Money_t, second_arg);
    QFETCH(bool, result);

    QCOMPARE(first_arg == second_arg, result);
    QCOMPARE(first_arg != second_arg, !result);
}


void Money_tTest::testCaseEqualityOperators_data()
{
    QTest::addColumn<Money_t>("first_arg");
    QTest::addColumn<Money_t>("second_arg");
    QTest::addColumn<bool>("result");

    Money_t m_0_0_PLN; m_0_0_PLN.setCurrency(CurrencyData::PLN);
    Money_t m_0_0_AUD; m_0_0_AUD.setCurrency(CurrencyData::AUD);
    QTest::newRow("1") << m_0_0_PLN << m_0_0_PLN << true;
    QTest::newRow("2") << m_0_0_PLN << m_0_0_AUD << false;


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

    SettingsGlobal s;
    const QChar decimPoint(s.decimalPoint());
    for(int i = 0; i < 100; ++i)
    {
        const QString textNumberFormatDot(QString("0%1%2").arg(decimPoint).arg(i, fieldWidth, radix, fillChar));
        QTest::newRow(textNumberFormatDot.toLatin1()) << textNumberFormatDot << currency
                                                     << QString("zero złotych %1/100 PLN").arg(i, fieldWidth, radix, fillChar);
    }
    QTest::newRow("1,00") << QString("%1%2%3").arg(1).arg(decimPoint).arg(0) << currency << QString("jeden złoty 00/100 PLN");
    QTest::newRow("2,00") << QString("%1%2%3").arg(2).arg(decimPoint).arg(0) << currency << QString("dwa złote 00/100 PLN");
    QTest::newRow("3,00") << QString("%1%2%3").arg(3).arg(decimPoint).arg(0) << currency << QString("trzy złote 00/100 PLN");
    QTest::newRow("4,00") << QString("%1%2%3").arg(4).arg(decimPoint).arg(0) << currency << QString("cztery złote 00/100 PLN");
    QTest::newRow("5,00") << QString("%1%2%3").arg(5).arg(decimPoint).arg(0) << currency << QString("pięć złotych 00/100 PLN");
    QTest::newRow("6,00") << QString("%1%2%3").arg(6).arg(decimPoint).arg(0) << currency << QString("sześć złotych 00/100 PLN");
    QTest::newRow("7,00") << QString("%1%2%3").arg(7).arg(decimPoint).arg(0) << currency << QString("siedem złotych 00/100 PLN");
    QTest::newRow("8,00") << QString("%1%2%3").arg(8).arg(decimPoint).arg(0) << currency << QString("osiem złotych 00/100 PLN");
    QTest::newRow("9,00") << QString("%1%2%3").arg(9).arg(decimPoint).arg(0) << currency << QString("dziewięć złotych 00/100 PLN");

    QTest::newRow("10,00") << QString("%1%2%3").arg(10).arg(decimPoint).arg(0) << currency << QString("dziesięć złotych 00/100 PLN");
    QTest::newRow("20,00") << QString("%1%2%3").arg(20).arg(decimPoint).arg(0) << currency << QString("dwadzieścia złotych 00/100 PLN");
    QTest::newRow("30,00") << QString("%1%2%3").arg(30).arg(decimPoint).arg(0) << currency << QString("trzydzieści złotych 00/100 PLN");
    QTest::newRow("40,00") << QString("%1%2%3").arg(40).arg(decimPoint).arg(0) << currency << QString("czterdzieści złotych 00/100 PLN");
    QTest::newRow("50,00") << QString("%1%2%3").arg(50).arg(decimPoint).arg(0) << currency << QString("pięćdziesiąt złotych 00/100 PLN");
    QTest::newRow("60,00") << QString("%1%2%3").arg(60).arg(decimPoint).arg(0) << currency << QString("sześćdziesiąt złotych 00/100 PLN");
    QTest::newRow("70,00") << QString("%1%2%3").arg(70).arg(decimPoint).arg(0) << currency << QString("siedemdziesiąt złotych 00/100 PLN");
    QTest::newRow("80,00") << QString("%1%2%3").arg(80).arg(decimPoint).arg(0) << currency << QString("osiemdziesiąt złotych 00/100 PLN");
    QTest::newRow("90,00") << QString("%1%2%3").arg(90).arg(decimPoint).arg(0) << currency << QString("dziewięćdziesiąt złotych 00/100 PLN");

    QTest::newRow("100,00") << QString("%1%2%3").arg(100).arg(decimPoint).arg(0) << currency << QString("sto złotych 00/100 PLN");
    QTest::newRow("200,00") << QString("%1%2%3").arg(200).arg(decimPoint).arg(0) << currency << QString("dwieście złotych 00/100 PLN");
    QTest::newRow("300,00") << QString("%1%2%3").arg(300).arg(decimPoint).arg(0) << currency << QString("trzysta złotych 00/100 PLN");
    QTest::newRow("400,00") << QString("%1%2%3").arg(400).arg(decimPoint).arg(0) << currency << QString("czterysta złotych 00/100 PLN");
    QTest::newRow("500,00") << QString("%1%2%3").arg(500).arg(decimPoint).arg(0) << currency << QString("pięćset złotych 00/100 PLN");
    QTest::newRow("600,00") << QString("%1%2%3").arg(600).arg(decimPoint).arg(0) << currency << QString("sześćset złotych 00/100 PLN");
    QTest::newRow("700,00") << QString("%1%2%3").arg(700).arg(decimPoint).arg(0) << currency << QString("siedemset złotych 00/100 PLN");
    QTest::newRow("800,00") << QString("%1%2%3").arg(800).arg(decimPoint).arg(0) << currency << QString("osiemset złotych 00/100 PLN");
    QTest::newRow("900,00") << QString("%1%2%3").arg(900).arg(decimPoint).arg(0) << currency << QString("dziewięćset złotych 00/100 PLN");

    QTest::newRow("1000,00") << QString("%1%2%3").arg(1000).arg(decimPoint).arg(0) << currency << QString("tysiąc złotych 00/100 PLN");
    QTest::newRow("2000,00") << QString("%1%2%3").arg(2000).arg(decimPoint).arg(0) << currency << QString("dwa tysiące złotych 00/100 PLN");
    QTest::newRow("3000,00") << QString("%1%2%3").arg(3000).arg(decimPoint).arg(0) << currency << QString("trzy tysiące złotych 00/100 PLN");
    QTest::newRow("4000,00") << QString("%1%2%3").arg(4000).arg(decimPoint).arg(0) << currency << QString("cztery tysiące złotych 00/100 PLN");
    QTest::newRow("5000,00") << QString("%1%2%3").arg(5000).arg(decimPoint).arg(0) << currency << QString("pięć tysięcy złotych 00/100 PLN");
    QTest::newRow("6000,00") << QString("%1%2%3").arg(6000).arg(decimPoint).arg(0) << currency << QString("sześć tysięcy złotych 00/100 PLN");
    QTest::newRow("7000,00") << QString("%1%2%3").arg(7000).arg(decimPoint).arg(0) << currency << QString("siedem tysięcy złotych 00/100 PLN");
    QTest::newRow("8000,00") << QString("%1%2%3").arg(8000).arg(decimPoint).arg(0) << currency << QString("osiem tysięcy złotych 00/100 PLN");
    QTest::newRow("9000,00") << QString("%1%2%3").arg(9000).arg(decimPoint).arg(0) << currency << QString("dziewięć tysięcy złotych 00/100 PLN");

    QTest::newRow("10000,00") << QString("%1%2%3").arg(10000).arg(decimPoint).arg(0) << currency << QString("dziesięć tysięcy złotych 00/100 PLN");
    QTest::newRow("11000,00") << QString("%1%2%3").arg(11000).arg(decimPoint).arg(0) << currency << QString("jedenaście tysięcy złotych 00/100 PLN");

    QTest::newRow("232,33")   << QString("%1%2%3").arg(232).arg(decimPoint).arg(33) << currency << QString("dwieście trzydzieści dwa złote 33/100 PLN");
    QTest::newRow("23299,33") << QString("%1%2%3").arg(23299).arg(decimPoint).arg(33) << currency << QString("dwadzieścia trzy tysiące dwieście dziewięćdziesiąt dziewięć złotych 33/100 PLN");
    QTest::newRow("999999999999999999999,99") << QString("%1%2%3").arg("999999999999999999999").arg(decimPoint).arg(99) << currency << QString("dziewięćset dziewięćdziesiąt dziewięć trylionów dziewięćset dziewięćdziesiąt dziewięć biliardów dziewięćset dziewięćdziesiąt dziewięć bilionów dziewięćset dziewięćdziesiąt dziewięć miliardów dziewięćset dziewięćdziesiąt dziewięć milionów dziewięćset dziewięćdziesiąt dziewięć tysięcy dziewięćset dziewięćdziesiąt dziewięć złotych 99/100 PLN");
}


QTEST_GUILESS_MAIN(Money_tTest)

#include "tst_money_ttest.moc"
