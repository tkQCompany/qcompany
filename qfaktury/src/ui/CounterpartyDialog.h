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

protected slots:
    /**
     * @brief
     *
     */
    void editCounterpartyTypeList();
    /**
     * @brief
     *
     */
    void editEmailList();
    /**
     * @brief
     *
     */
    void editPhoneList();

    void editFormat();
    /**
     * @brief
     *
     */
    void okClick();

    void showExamples();
private:
    /**
     * @brief
     *
     */
    void init_();
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
    QString validateForm_();
protected:
    Ui::CounterpartyDialog *ui;
    Database *db; /**< TODO */
    const QModelIndex id; /**< TODO */
    QDataWidgetMapper mapper; /**< TODO */
    QSortFilterProxyModel proxyModelCounterpartyType;

};
#endif
