#ifndef COUNTERPARTYDIALOG_H
#define COUNTERPARTYDIALOG_H

#include <QMessageBox>
#include <QDataWidgetMapper>
#include <QSortFilterProxyModel>

#include "ui_CounterpartyDialog.h"
#include "../ui/CounterpartyTypeDialog.h"
#include "../ui/EmailDialog.h"
#include "../ui/InvoiceNumberFormatEditDialog.h"
#include "../ui/InvoiceNumberFormatExamplesDialog.h"
#include "../ui/PhoneDialog.h"
#include "../src/model/Database.h"
#include "../src/SettingsGlobal.h"
#include "../ui/CounterpartyTypeDelegate.h"

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
    CounterpartyDialog(QWidget *parent, Database *db, const QModelIndex &id = QModelIndex(), const bool myCompany = false);
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
    void editEmailList_();
    /**
     * @brief
     *
     */
    void editPhoneList_();

    void editFormat_();
    /**
     * @brief
     *
     */
    void okClick_();

    void showExamples_();
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
    QSortFilterProxyModel proxyModelCounterpartyType_;

};
#endif
