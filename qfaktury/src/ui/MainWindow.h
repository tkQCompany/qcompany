#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPointer>

#include "ui_MainWindow.h"
#include "Database.h"
#include "InvoiceTypeData.h"
#include "CommodityData.h"
#include "CounterpartyData.h"
#include "CommodityDialog.h"
#include "CounterpartyDialog.h"


namespace CMF = CommodityFields;
namespace CPF = CounterpartyFields;
namespace IVF = InvoiceFields;


/**
 * @brief
 *
 */
class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

public slots:
    void editMyCompanyInfo();

protected slots:
    void tabChanged_(int index);
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

protected:
    void createInvoice_(const InvoiceTypeData::Type type);
    Database* database();
    void init_();
    void setActions_(const bool counterpartiesEditEnabled, const bool counterpartiesRemoveEnable, const bool, const bool, const bool historyEditEnabled, const bool historyRemoveEnabled);
    virtual void loadPlugins();

protected:
    Ui::MainWindow *ui;
    QPointer<CommodityDialog> commodityDialogPtr;
    QPointer<CounterpartyDialog> counterpartyDialogPtr;

private:
    Database db_; /**< An object representing the application's database */
    QMap<int, QString> customActions;
};
#endif
