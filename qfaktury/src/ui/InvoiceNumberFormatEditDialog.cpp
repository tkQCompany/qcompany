#include <QMessageBox>

#include "InvoiceNumberFormatEditDialog.h"
#include "ui_InvoiceNumberFormatEditDialog.h"
#include "InvoiceNumberFormatData.h"
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
    for(int i = InvoiceNumberFormatData::NR; i <= InvoiceNumberFormatData::HYPHEN; ++i)
    {
        ui_->comboBoxFields->addItem(QString("%1 : %2").arg(InvoiceNumberFormatData::FieldName(i))
                                    .arg(InvoiceNumberFormatData::FieldDescription(i)));
    }

    connect(ui_->pushButtonAddField, SIGNAL(clicked()), this, SLOT(fieldAdd_()));
    connect(ui_->pushButtonChangeField, SIGNAL(clicked()), this, SLOT(fieldChange_()));
    connect(ui_->pushButtonRemoveField, SIGNAL(clicked()), this, SLOT(fieldRemove_()));
    connect(ui_->pushButtonShowExamples, SIGNAL(clicked()), this, SLOT(showExamples_()));
}


void InvoiceNumberFormatEditDialog::initList_(const QString &format)
{
    if(format.isEmpty())
        return;

    QString label;
    QVector<int> vint(InvoiceNumberFormatData::Parse(format));

    ui_->listWidgetFields->clear();
    for(int i = 0; i < vint.size(); ++i)
    {
        label = InvoiceNumberFormatData::FieldName(vint.at(i));
        ui_->listWidgetFields->insertItem(ui_->listWidgetFields->count(), label);
    }
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

    ui_->listWidgetFields->addItem(InvoiceNumberFormatData::FieldName(index));
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
    ui_->listWidgetFields->item(index)->setText(InvoiceNumberFormatData::FieldName(InvoiceNumberFormatData::FieldID(field)));
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
