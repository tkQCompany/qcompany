#include <QMessageBox>

#include "InvoiceNumberFormatEditDialog.h"
#include "ui_InvoiceNumberFormatEditDialog.h"
#include "InvoiceNumberFormat_t.h"
#include "InvoiceNumberFormatExamplesDialog.h"

InvoiceNumberFormatEditDialog::InvoiceNumberFormatEditDialog(QWidget *parent, Database *db, const QString &format) :
    QDialog(parent),
    ui_(new Ui::InvoiceNumberFormatEditDialog),
    db_(db)
{
    ui_->setupUi(this);
    init_();
    initList_(format);
}

InvoiceNumberFormatEditDialog::~InvoiceNumberFormatEditDialog()
{
    delete ui_;
}

void InvoiceNumberFormatEditDialog::init_()
{
    for(int i = InvoiceNumberFormat_t::NR; i <= InvoiceNumberFormat_t::HYPHEN; ++i)
    {
        ui_->comboBoxFields->addItem(QString("%1 : %2").arg(InvoiceNumberFormat_t::FieldName((InvoiceNumberFormat_t::Field)i))
                                    .arg(InvoiceNumberFormat_t::FieldDescription((InvoiceNumberFormat_t::Field)i)));
    }

    connect(ui_->pushButtonAddField, SIGNAL(clicked()), this, SLOT(fieldAdd_()));
    connect(ui_->pushButtonChangeField, SIGNAL(clicked()), this, SLOT(fieldChange_()));
    connect(ui_->pushButtonRemoveField, SIGNAL(clicked()), this, SLOT(fieldRemove_()));
    connect(ui_->pushButtonShowExamples, SIGNAL(clicked()), this, SLOT(showExamples_()));
}


void InvoiceNumberFormatEditDialog::initList_(const QString &formatStr)
{
    if(formatStr.isEmpty())
        return;

    ui_->listWidgetFields->clear();
    ui_->listWidgetFields->insertItems(0, InvoiceNumberFormat_t::Parse(formatStr)->fieldStrList());
}


QString InvoiceNumberFormatEditDialog::listToString_() const
{
    QString ret;
    const int rowMax = ui_->listWidgetFields->count();
    for(int row = 0; row < rowMax; ++row)
    {
        ret.append(ui_->listWidgetFields->item(row)->text());
    }

    return ret;
}

void InvoiceNumberFormatEditDialog::accept()
{
    invNumFormat_ = listToString_();
    done(QDialog::Accepted);
}


void InvoiceNumberFormatEditDialog::fieldAdd_()
{
    const int index = ui_->comboBoxFields->currentIndex();
    if(index == -1)
    {
        QMessageBox::information(this, trUtf8("Wybierz pole"), trUtf8("Wybierz najpierw pożądane pole numeru faktur."), QMessageBox::Ok);
        return;
    }

    ui_->listWidgetFields->addItem(InvoiceNumberFormat_t::FieldName((InvoiceNumberFormat_t::Field)index));
}


void InvoiceNumberFormatEditDialog::fieldChange_()
{
    if(ui_->comboBoxFields->currentIndex() == -1)
    {
        QMessageBox::information(this, trUtf8("Wybierz pole"), trUtf8("Wybierz najpierw pożądane pole numeru faktur."), QMessageBox::Ok);
        return;
    }

    const int index = ui_->listWidgetFields->currentIndex().row();
    if(index == -1)
    {
        QMessageBox::information(this, trUtf8("Wybierz pole"), trUtf8("Wybierz najpierw pole do zmiany."), QMessageBox::Ok);
        return;
    }

    const QString field(ui_->comboBoxFields->currentText().left(ui_->comboBoxFields->currentText().indexOf(QChar(':'))).trimmed());
    ui_->listWidgetFields->item(index)->setText(InvoiceNumberFormat_t::FieldName(InvoiceNumberFormat_t::FieldID(field)));
}


void InvoiceNumberFormatEditDialog::fieldRemove_()
{
    const int index = ui_->listWidgetFields->currentIndex().row();
    if(index == -1)
    {
        QMessageBox::information(this, trUtf8("Wybierz pole"), trUtf8("Wybierz najpierw pole lub separator do skasowania."), QMessageBox::Ok);
        return;
    }

    ui_->listWidgetFields->takeItem(index);
}


void InvoiceNumberFormatEditDialog::showExamples_()
{
    InvoiceNumberFormatExamplesDialog dialog(this, db_, listToString_());
    dialog.exec();
}
