#include <QString>
#include <QtTest>
#include <QCoreApplication>

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
}

void SettingsDialogTest::cleanupTestCase()
{
}

void SettingsDialogTest::testGUIChangeLanguage()
{
    QString currLang, nextLang;

    {
        SettingsDialog sdialog;

        const int langCount = sdialog.comboBoxLanguage->count();
        QVERIFY(langCount > 0);

        currLang = sdialog.comboBoxLanguage->currentText();
        const int currInd = sdialog.comboBoxLanguage->currentIndex();
        sdialog.comboBoxLanguage->setCurrentIndex( (currInd + 1 ) % langCount); //next language
        nextLang = sdialog.comboBoxLanguage->currentText();
        QVERIFY(currLang != nextLang);

        QTest::mouseClick(sdialog.pushButtonApply,Qt::LeftButton);
    }
    SettingsDialog sdialog;
    QVERIFY(sdialog.comboBoxLanguage->count() > 0);
    QCOMPARE(sdialog.comboBoxLanguage->currentText(), nextLang);
}

QTEST_MAIN(SettingsDialogTest)

#include "tst_settingsdialogtest.moc"
