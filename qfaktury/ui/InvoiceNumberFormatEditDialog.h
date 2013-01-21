#ifndef INVOICENUMBERFORMATEDITDIALOG_H
#define INVOICENUMBERFORMATEDITDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "model/models_data/InvoiceNumberFormatData.h"

namespace Ui {
class InvoiceNumberFormatEditDialog;
}

class InvoiceNumberFormatEditDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QString invNumFormat_ READ format)
    
public:
    explicit InvoiceNumberFormatEditDialog(QWidget *parent = 0);
    ~InvoiceNumberFormatEditDialog();
    QString format() const {return invNumFormat_;}

private slots:
    void fieldAdd_();
    void fieldChange_();
    void fieldRemove_();

    void separatorAdd_();
    void separatorChange_();

    void showExamples_();

private:
    void init_();
    
private:
    Ui::InvoiceNumberFormatEditDialog *ui;
    QString invNumFormat_;
};

#endif // INVOICENUMBERFORMATEDITDIALOG_H
