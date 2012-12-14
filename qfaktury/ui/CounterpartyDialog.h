#ifndef COUNTERPARTYDIALOG_H
#define COUNTERPARTYDIALOG_H

#include <QMessageBox>
#include <QDataWidgetMapper>

#include "ui_CounterpartyDialog.h"
#include "../ui/CountryDialog.h"
#include "../ui/CounterpartyTypeDialog.h"
#include "../ui/EmailDialog.h"
#include "../ui/PhoneDialog.h"
#include "../src/model/Database.h"
#include "../src/SettingsGlobal.h"

/**
 * @brief
 *
 */
class CounterpartyDialog: public QDialog, public Ui::CounterpartyDialog
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param parent
 * @param db
 * @param id
 */
    CounterpartyDialog(QWidget *parent, Database *db, const QModelIndex &id = QModelIndex());
    /**
     * @brief
     *
     */
    ~CounterpartyDialog();

private slots:
    /**
     * @brief
     *
     */
    void editCounterpartyTypeList_();
    /**
     * @brief
     *
     */
    void editCountryList_();
    /**
     * @brief
     *
     */
    void editEmailList_();
    /**
     * @brief
     *
     */
    void editPhoneList_();
    /**
     * @brief
     *
     */
    void okClick_();
private:
    /**
     * @brief
     *
     */
    void init();
    /**
     * @brief
     *
     * @param
     * @return QString
     */
    QString isEmpty_(const QString &);
    /**
     * @brief
     *
     * @return bool
     */
    bool validate_();
    /**
     * @brief
     *
     * @param
     * @return bool
     */
    bool validateForm_(QString &);
private:
    Database *db_; /**< TODO */
    const QModelIndex id_; /**< TODO */
    QDataWidgetMapper mapper_; /**< TODO */
};
#endif
