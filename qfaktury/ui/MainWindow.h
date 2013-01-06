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
namespace IVF = InvoiceFields;


/**
 * @brief
 *
 */
class MainWindow: public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void tabChanged(QWidget * widget);
    void aboutQt();
    void about();
    void addCommodity();
    void closeEvent(QCloseEvent *event);
    void delCommodity();
    void editCommodity();
    void addCounterparty();
    void delCounterparty();
    void editCounterparty();
    void editInvoice();
    void delInvoice();
    void editCompanyInfo();
    void editSettings();
    void newInvoice();
    void newProFormaInvoice();
    void newCorrection();
    void newDuplicate();
    void newGrossInvoice();
    void newBill();
    void help() const;
    void reportBug() const;
    void mainUpdateStatus(QModelIndex index);
    void showTableMenuGoods(const QPoint &) const;
    void showTableMenuCounterparties(const QPoint&) const;
    void showTableMenuHistory(const QPoint &) const;
    void pluginSlot();
    void pluginInfoSlot();
    void keyPressEvent(QKeyEvent*);

private:
    void createInvoice(const InvoiceTypeData::Type type);
    bool firstRun();
    void init();
    void saveAllSettings();
    void saveColumnWidth();
    void setActions(const bool counterpartiesEditEnabled, const bool counterpartiesRemoveEnable, const bool, const bool, const bool historyEditEnabled, const bool historyRemoveEnabled);
    void setupDir() const;

private:
    Database db_; /**< An object representing the application's database */
    QMap<int, QString> customActions;

protected:
    virtual void loadPlugins();
};
#endif
