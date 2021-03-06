#ifndef INVOICENUMBERFORMATEDITDIALOG_H
#define INVOICENUMBERFORMATEDITDIALOG_H

#include <QDialog>

namespace Ui {
class InvoiceNumberFormatEditDialog;
}


class Database;


class InvoiceNumberFormatEditDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QString invNumFormat_ READ format)
    
public:
    explicit InvoiceNumberFormatEditDialog(QWidget *parent, Database *db, const QString &format = QString());
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
    void initList_(const QString &formatStr);
    QString listToString_() const;
    
private:
    Ui::InvoiceNumberFormatEditDialog *ui_;
    QString invNumFormat_;
    Database *db_;
};

#endif // INVOICENUMBERFORMATEDITDIALOG_H
