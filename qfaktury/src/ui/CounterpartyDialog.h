#ifndef COUNTERPARTYDIALOG_H
#define COUNTERPARTYDIALOG_H

#include <QDataWidgetMapper>
#include <QDialog>
#include <QSortFilterProxyModel>


namespace Ui
{
class CounterpartyDialog;
}

class Database;

/**
 * @brief
 *
 */
class CounterpartyDialog: public QDialog
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
    virtual ~CounterpartyDialog();

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
protected:
    Ui::CounterpartyDialog *ui_;
    Database *db_; /**< TODO */
    const QModelIndex id_; /**< TODO */
    QDataWidgetMapper mapper_; /**< TODO */
    QSortFilterProxyModel proxyModelCounterpartyType_;

};
#endif
