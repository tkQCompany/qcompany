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
private slots:
    void tabChanged_(QWidget * widget);
    void aboutQt_();
    void about_();
    void addCommodity_();
    void delCommodity_();
    void editCommodity_();
    void addCounterparty_();
    void delCounterparty_();
    void editCounterparty_();
    void editInvoice_();
    void delInvoice_();
    void editCompanyInfo_();
    void editSettings_();
    void newInvoice_();
    void newProFormaInvoice_();
    void newCorrection_();
    void newDuplicate_();
    void newGrossInvoice_();
    void newBill_();
    void help_() const;
    void reportBug_() const;
    void mainUpdateStatus_(QModelIndex index);
    void showTableMenuGoods_(const QPoint &) const;
    void showTableMenuCounterparties_(const QPoint&) const;
    void showTableMenuHistory_(const QPoint &) const;
    void pluginSlot_();
    void pluginInfoSlot_();
    void reReadInvHistory_();
    void retranslateUi_();
    void keyPressEvent(QKeyEvent*);

private:
    void createInvoice_(const InvoiceTypeData::Type type);
    bool firstRun_() const;
    void init_();
    void setActions_(const bool counterpartiesEditEnabled, const bool counterpartiesRemoveEnable, const bool, const bool, const bool historyEditEnabled, const bool historyRemoveEnabled);
    void setupDir_() const;

private:
    Database db_; /**< An object representing the application's database */
    QMap<int, QString> customActions;

protected:
    virtual void loadPlugins();
};
#endif
