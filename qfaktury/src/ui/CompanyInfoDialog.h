#ifndef COMPANYINFODIALOG_H
#define COMPANYINFODIALOG_H

#include "../SettingsGlobal.h"
#include "ui_CompanyInfoDialog.h"
#include "../model/Database.h"

class CompanyInfoDialog: public QDialog, public Ui::CompanyInfoDialog
{
    Q_OBJECT
public:
    CompanyInfoDialog(QWidget *, Database *db);
private slots:
    void init();
    void okClick();
private:
    Database *db_;
};
#endif
