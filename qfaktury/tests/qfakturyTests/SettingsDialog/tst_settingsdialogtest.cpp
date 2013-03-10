#include <QtTest>

#include "../TestsCommon.h"
#include "SettingsDialog.h"

class SettingsDialogTest : public QObject
{
    Q_OBJECT
    
public:
    SettingsDialogTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testGUIChangeLanguage();
};

SettingsDialogTest::SettingsDialogTest()
{

}

void SettingsDialogTest::initTestCase()
{
    TestsCommon::setAppData();
    TestsCommon::removeDBFile();
}

void SettingsDialogTest::cleanupTestCase()
{
}

void SettingsDialogTest::testGUIChangeLanguage()
{
    QString currLang, nextLang;
    int currInd = 0;
    Database db;

    {
        SettingsDialog sdialog(0, &db);

        const int langCount = sdialog.comboBoxLanguage->count();
        QVERIFY(langCount > 0);

        currLang = sdialog.comboBoxLanguage->currentText();
        currInd = sdialog.comboBoxLanguage->currentIndex();
        sdialog.comboBoxLanguage->setCurrentIndex( (currInd + 1 ) % langCount); //next language
        nextLang = sdialog.comboBoxLanguage->currentText();
        QVERIFY(currLang != nextLang);

        QTest::mouseClick(sdialog.pushButtonApply,Qt::LeftButton);
    }
    SettingsDialog sdialog(0, &db);
    QVERIFY(sdialog.comboBoxLanguage->count() > 0);
    QCOMPARE(sdialog.comboBoxLanguage->currentText(), nextLang);

    sdialog.comboBoxLanguage->setCurrentIndex(currInd); //restoring
}



QTEST_MAIN(SettingsDialogTest)

#include "tst_settingsdialogtest.moc"
