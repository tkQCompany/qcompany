#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>

#include "ui_SettingsDialog.h"
#include "../SettingsGlobal.h"


/**
 * @brief
 *
 */
class SettingsDialog: public QDialog, public Ui::SettingsDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param
 */
    SettingsDialog(QWidget *);
private slots:
    /**
     * @brief
     *
     */
    void apply();
    /**
     * @brief
     *
     */
    void okButtonClick();
    /**
     * @brief
     *
     */
    void addLogoBtnClick();
    /**
     * @brief
     *
     */
    void currAddBtnClick();
    /**
     * @brief
     *
     */
    void currDelBtnClick();
    /**
     * @brief
     *
     */
    void corrAddBtnClick();
    /**
     * @brief
     *
     */
    void corrDelBtnClick();
    /**
     * @brief
     *
     */
    void vatAddBtnClick();
    /**
     * @brief
     *
     */
    void vatDelBtnClick();
    /**
     * @brief
     *
     */
    void vatUpBtnClick();
    /**
     * @brief
     *
     */
    void vatDownBtnClick();
    /**
     * @brief
     *
     */
    void currencyAddBtnClick();
    /**
     * @brief
     *
     */
    void currencyDelBtnClick();
    /**
     * @brief
     *
     */
    void paymUpBtnClick();
    /**
     * @brief
     *
     */
    void paymDownBtnClick();
    /**
     * @brief
     *
     */
    void paymDelBtnClick();
    /**
     * @brief
     *
     */
    void zastBtnEnable();
    /**
     * @brief
     *
     */
    void paymAddBtnClick();
    /**
     * @brief
     *
     */
    void defTextBtnClick();
    /**
     * @brief
     *
     */
    void setDefaultClick();
    /**
     * @brief
     *
     */
    void maskHelpClick();
    /**
     * @brief
     *
     */
    void workingDirBtnClick();
private:    
    /**
     * @brief
     *
     */
    void init();
    /**
     * @brief
     *
     */
    void sumSize();
    /**
     * @brief
     *
     */
    void saveSettings();
    /**
     * @brief
     *
     */
    void readSettings();
    /**
     * @brief
     *
     */
    void getEncodings();
    /**
     * @brief
     *
     * @return QStringList
     */
    QStringList getTemplates();
    /**
     * @brief
     *
     * @return QStringList
     */
    QStringList getTranslations() const;
    /**
     * @brief
     *
     * @param
     * @return QString
     */
    QString getItemsToString(const QListWidget *);
private:
    bool read; /**< TODO */
    QList<QTextCodec *> codecs; /**< TODO */
};
#endif
