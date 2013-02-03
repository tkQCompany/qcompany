#ifndef INVOICENUMBERFORMATEDITDIALOG_H
#define INVOICENUMBERFORMATEDITDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "model/models_data/InvoiceNumberFormatData.h"
#include "InvoiceNumberFormatExamplesDialog.h"

namespace Ui {
class InvoiceNumberFormatEditDialog;
}

class InvoiceNumberFormatEditDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QString invNumFormat_ READ format)
    
public:
    explicit InvoiceNumberFormatEditDialog(QWidget *parent = 0, const QString &format = QString());
    ~InvoiceNumberFormatEditDialog();
    QString format() const {return invNumFormat_;}

public slots:
    void accept();

private slots:
    void fieldAdd_();
    void fieldChange_();
    void fieldRemove_();

    void showExamples_();

private:
    void init_();
    void initList_(const QString &);
    QString listToString_() const;
    
private:
    Ui::InvoiceNumberFormatEditDialog *ui;
    QString invNumFormat_;
};

#endif // INVOICENUMBERFORMATEDITDIALOG_H
