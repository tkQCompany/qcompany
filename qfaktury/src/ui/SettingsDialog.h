#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>


namespace Ui
{
class SettingsDialog;
}


class Database;
class QListWidget;
class QLineEdit;


/**
 * @brief
 *
 */
class SettingsDialog: public QDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param
 */
    SettingsDialog(QWidget *parent, Database *db);
    ~SettingsDialog();

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
    void unitAddBtnClick_();
    /**
     * @brief
     *
     */
    void unitDelBtnClick_();
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
    void updateCurrenciesRates_();
    void updateCurrenciesTableView_();

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

    bool addToListWidget_(QListWidget *listTo, QLineEdit *lineEditFrom);
    void delFromListWidget_(QListWidget *list);
    void moveUpElement_(QListWidget *list);
    void moveDownElement_(QListWidget *list);

private:
    Ui::SettingsDialog *ui_;
    Database *db_;
};
#endif
