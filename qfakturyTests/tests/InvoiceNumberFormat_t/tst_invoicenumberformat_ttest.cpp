#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "SettingsGlobal.h"
#include "InvoiceNumberFormat_t.h"

struct AppendTest
{
    void clearAll() {fields_.clear(); expectedValues_.clear();}
    QString expectedValue(const int pos) const {return expectedValues_.at(pos);}
    InvoiceNumberFormat_t::Field field(const int pos) const {return fields_.at(pos);}
    void setContent(const QList<InvoiceNumberFormat_t::Field> &fields, const QStringList &expectedValues)
    {
        fields_ = fields;
        expectedValues_ = expectedValues;
    }

    int size() const {return fields_.size();}
    QString toString() const
    {
        QString ret;
        foreach(const QString val, expectedValues_)
        {
            ret.append(val);
        }
        return ret;
    }

private:
    QList<InvoiceNumberFormat_t::Field> fields_;
    QStringList expectedValues_;
};

class InvoiceNumberFormat_tTest : public QObject
{
    Q_OBJECT
    
public:
    InvoiceNumberFormat_tTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCaseAppend();
    void testCaseAppend_data();
private:
    SettingsGlobal s_;
    InvoiceNumberFormat_t testedObj_;
    AppendTest appendTest_;
};

InvoiceNumberFormat_tTest::InvoiceNumberFormat_tTest()
{
}

void InvoiceNumberFormat_tTest::initTestCase()
{
}

void InvoiceNumberFormat_tTest::cleanupTestCase()
{
}

void InvoiceNumberFormat_tTest::testCaseAppend()
{
    QFETCH(InvoiceNumberFormat_t::Field, field);
    QFETCH(int, position);
    QFETCH(QString, expectedValue);

    QCOMPARE(testedObj_.size(), position);
    QCOMPARE(testedObj_.fieldList().size(),position);
    QCOMPARE(testedObj_.fieldStrList().size(), position);
    testedObj_.append(field, s_);
    QCOMPARE(testedObj_.size(), position+1);
    QCOMPARE(testedObj_.fieldList().size(), position+1);
    QCOMPARE(testedObj_.fieldStrList().size(), position+1);
    QCOMPARE(testedObj_.cap(appendTest_.toString(), position), expectedValue);
}

void InvoiceNumberFormat_tTest::testCaseAppend_data()
{
    testedObj_.clearAll();
    appendTest_.clearAll();

    QList<InvoiceNumberFormat_t::Field> fields;
    QStringList expectedValues;

    //"1-2-3-4-5"
    fields << InvoiceNumberFormat_t::NR   << InvoiceNumberFormat_t::HYPHEN
           << InvoiceNumberFormat_t::NR_Y << InvoiceNumberFormat_t::HYPHEN
           << InvoiceNumberFormat_t::NR_M << InvoiceNumberFormat_t::HYPHEN
           << InvoiceNumberFormat_t::NR_D << InvoiceNumberFormat_t::HYPHEN
           << InvoiceNumberFormat_t::NR_Q;
    expectedValues << "1" << "-" << "2" << "-" << "3" << "-" << "4" << "-" << "5";

    fields << InvoiceNumberFormat_t::HYPHEN;
    expectedValues << "-";

    //"1/2/3/4/5"
    fields << InvoiceNumberFormat_t::NR   << InvoiceNumberFormat_t::SLASH
           << InvoiceNumberFormat_t::NR_Y << InvoiceNumberFormat_t::SLASH
           << InvoiceNumberFormat_t::NR_M << InvoiceNumberFormat_t::SLASH
           << InvoiceNumberFormat_t::NR_D << InvoiceNumberFormat_t::SLASH
           << InvoiceNumberFormat_t::NR_Q;
    expectedValues << "1" << "/" << "2" << "/" << "3" << "/" << "4" << "/" << "5";

    fields << InvoiceNumberFormat_t::HYPHEN;
    expectedValues << "-";

    //"1\\2\\3\\4\\5"
    fields << InvoiceNumberFormat_t::NR   << InvoiceNumberFormat_t::BACKSLASH
           << InvoiceNumberFormat_t::NR_Y << InvoiceNumberFormat_t::BACKSLASH
           << InvoiceNumberFormat_t::NR_M << InvoiceNumberFormat_t::BACKSLASH
           << InvoiceNumberFormat_t::NR_D << InvoiceNumberFormat_t::BACKSLASH
           << InvoiceNumberFormat_t::NR_Q;
    expectedValues << "1" << "\\" << "2" << "\\" << "3" << "\\" << "4" << "\\" << "5";

    fields << InvoiceNumberFormat_t::HYPHEN;
    expectedValues << "-";

    //"1-2/3\\4-5"
    fields << InvoiceNumberFormat_t::NR   << InvoiceNumberFormat_t::HYPHEN
           << InvoiceNumberFormat_t::NR_Y << InvoiceNumberFormat_t::SLASH
           << InvoiceNumberFormat_t::NR_M << InvoiceNumberFormat_t::BACKSLASH
           << InvoiceNumberFormat_t::NR_D << InvoiceNumberFormat_t::HYPHEN
           << InvoiceNumberFormat_t::NR_Q;
    expectedValues << "1" << "-" << "2" << "/" << "3" << "\\" << "4" << "-" << "5";

    appendTest_.setContent(fields, expectedValues);

    QTest::addColumn<InvoiceNumberFormat_t::Field>("field");
    QTest::addColumn<int>("position");
    QTest::addColumn<QString>("expectedValue");

    for(int i = 0; i < appendTest_.size(); ++i)
    {
        QTest::newRow(QString("%1").arg(i).toLatin1()) << appendTest_.field(i)   << i << appendTest_.expectedValue(i);
    }
}

QTEST_MAIN(InvoiceNumberFormat_tTest)

#include "tst_invoicenumberformat_ttest.moc"
