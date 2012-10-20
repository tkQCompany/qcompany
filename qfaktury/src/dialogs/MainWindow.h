#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QScopedPointer>
#include <QMessageBox>
#include <QKeyEvent>
#include <QSqlRelationalDelegate>

#include "ui_MainWindow.h"
#include "CorrectiveInvoiceDialog.h"
#include "CorrectiveInvoiceGrossDialog.h"
#include "DuplicateDialog.h"
#include "InvoiceGrossDialog.h"
#include "BillDialog.h"
#include "../SettingsGlobal.h"
#include "SettingsDialog.h"
#include "CommodityDialog.h"
#include "../model/Database.h"


namespace CMF = CommodityFields;
namespace CPF = CounterpartyFields;
namespace IVF = InvoiceDataFields;


/**
 * @brief
 *
 */
class MainWindow: public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
public:
/**
 * @brief
 *
 * @param parent
 */
    MainWindow(QWidget *parent = 0);
    /**
     * @brief
     *
     */
    ~MainWindow();
private slots:
    /**
     * @brief
     *
     * @param tab
     */
    void tableClear(QTableWidget *tab) {tab->setRowCount(0);}
    /**
     * @brief
     *
     * @param widget
     */
    void tabChanged(QWidget * widget);
    /**
     * @brief
     *
     */
    void aboutQt();
    /**
     * @brief
     *
     */
    void about();
    /**
     * @brief
     *
     */
    void addCommodity();
    /**
     * @brief
     *
     * @param event
     */
    void closeEvent(QCloseEvent *event);
    /**
     * @brief
     *
     */
    void delCommodity();
    /**
     * @brief
     *
     */
    void editCommodity();
    /**
     * @brief
     *
     */
    void addCounterparty();
    /**
     * @brief
     *
     */
    void delCounterparty();
    /**
     * @brief
     *
     */
    void editCounterparty();
    /**
     * @brief
     *
     */
    void editInvoice();
    /**
     * @brief
     *
     */
    void delInvoice();
    /**
     * @brief
     *
     */
    void editCompanyInfo();
    /**
     * @brief
     *
     */
    void editSettings();
    /**
     * @brief
     *
     */
    void newInvoice();
    /**
     * @brief
     *
     */
    void newProFormaInvoice();
    /**
     * @brief
     *
     */
    void newCorrection();
    /**
     * @brief
     *
     */
    void newDuplicate();
    /**
     * @brief
     *
     */
    void newGrossInvoice();
    /**
     * @brief
     *
     */
    void newBill();
    /**
     * @brief
     *
     */
    void help() const;
    /**
     * @brief
     *
     */
    void reportBug() const;
    /**
     * @brief
     *
     * @param index
     */
    void mainUpdateStatus(QModelIndex index);
    /**
     * @brief
     *
     * @param
     */
    void showTableMenuGoods(const QPoint &) const;
    /**
     * @brief
     *
     * @param
     */
    void showTableMenuCounterparties(const QPoint&) const;
    /**
     * @brief
     *
     * @param
     */
    void showTableMenuHistory(const QPoint &) const;
    /**
     * @brief
     *
     */
    void pluginSlot();
    /**
     * @brief
     *
     */
    void pluginInfoSlot();
    /**
     * @brief
     *
     * @param
     */
    void keyPressEvent(QKeyEvent*);

private:
    /**
     * @brief
     *
     * @param type
     */
    void createInvoice(const InvoiceTypeData::Type type);
    /**
     * @brief
     *
     * @return bool
     */
    bool firstRun();
    /**
     * @brief
     *
     */
    void init();
    /**
     * @brief
     *
     */
    void saveAllSettings();
    /**
     * @brief
     *
     */
    void saveColumnWidth();
    /**
     * @brief
     *
     * @param counterpartiesEditEnabled
     * @param counterpartiesRemoveEnable
     * @param bool
     * @param bool
     * @param historyEditEnabled
     * @param historyRemoveEnabled
     */
    void setActions(const bool counterpartiesEditEnabled, const bool counterpartiesRemoveEnable, const bool, const bool, const bool historyEditEnabled, const bool historyRemoveEnabled);
    /**
     * @brief
     *
     */
    void setupDir() const;

private:
    Database db_; /**< TODO */
    QMap<int, QString> customActions; /**< TODO */

protected:
    /**
     * @brief
     *
     */
    virtual void loadPlugins();
};
#endif
