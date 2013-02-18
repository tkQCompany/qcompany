#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>
#include <QTranslator>

#include "ui_SettingsDialog.h"
#include "../SettingsGlobal.h"
#include "Database.h"
#include "InvoiceNumberFormatEditDialog.h"
#include "InvoiceNumberFormatExamplesDialog.h"


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
    SettingsDialog(QWidget *parent, Database *db);
private slots:
    /**
     * @brief
     *
     */
    void apply_();
    /**
     * @brief
     *
     */
    void okButtonClick_();
    /**
     * @brief
     *
     */
    void addLogoBtnClick_();
    /**
     * @brief
     *
     */
    void currAddBtnClick_();
    /**
     * @brief
     *
     */
    void currDelBtnClick_();
    /**
     * @brief
     *
     */
    void corrAddBtnClick_();
    /**
     * @brief
     *
     */
    void corrDelBtnClick_();
    /**
     * @brief
     *
     */
    void vatAddBtnClick_();
    /**
     * @brief
     *
     */
    void vatDelBtnClick_();
    /**
     * @brief
     *
     */
    void vatUpBtnClick_();
    /**
     * @brief
     *
     */
    void vatDownBtnClick_();
    /**
     * @brief
     *
     */
    void currencyAddBtnClick_();
    /**
     * @brief
     *
     */
    void currencyDelBtnClick_();
    /**
     * @brief
     *
     */
    void paymUpBtnClick_();
    /**
     * @brief
     *
     */
    void paymDownBtnClick_();
    /**
     * @brief
     *
     */
    void paymDelBtnClick_();
    /**
     * @brief
     *
     */
    void applyBtnEnable_();
    /**
     * @brief
     *
     */
    void paymAddBtnClick_();

    /**
     * @brief
     *
     */
    void setDefaultClick_();

    void showExamples_();
    /**
     * @brief
     *
     */
    void maskHelpClick_();
    /**
     * @brief
     *
     */
    void workingDirBtnClick_();
    void editFormat_();
private:    
    /**
     * @brief
     *
     */
    void init_();
    /**
     * @brief
     *
     */
    void sumSize_();
    /**
     * @brief
     *
     */
    void saveSettings_();
    /**
     * @brief
     *
     */
    void readSettings_();

    /**
     * @brief
     *
     * @return QStringList
     */
    QStringList getTemplates_();
    /**
     * @brief
     *
     * @return QStringList
     */
    QStringList getTranslations_() const;
    /**
     * @brief
     *
     * @param
     * @return QString
     */
    QString getItemsToString_(const QListWidget *);

    void addToListWidget_(QListWidget *listTo, QLineEdit *lineEditFrom);
    void delFromListWidget_(QListWidget *list);
    void moveUpElement_(QListWidget *list);
    void moveDownElement_(QListWidget *list);

private:
    Database *db_;
};
#endif
