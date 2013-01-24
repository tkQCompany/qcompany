#include "InvoiceNumberFormatEditDialog.h"
#include "ui_InvoiceNumberFormatEditDialog.h"

InvoiceNumberFormatEditDialog::InvoiceNumberFormatEditDialog(QWidget *parent, const QString &format) :
    QDialog(parent),
    ui(new Ui::InvoiceNumberFormatEditDialog)
{
    ui->setupUi(this);
    init_();
    initList_(format);
}

InvoiceNumberFormatEditDialog::~InvoiceNumberFormatEditDialog()
{
    delete ui;
}

void InvoiceNumberFormatEditDialog::init_()
{
    for(int i = InvoiceNumberFormatData::NUMBER; i <= InvoiceNumberFormatData::F_QUARTER; ++i)
    {
        ui->comboBoxFields->addItem(QString("%1 - %2").arg(InvoiceNumberFormatData::FieldName(i))
                                    .arg(InvoiceNumberFormatData::FieldDescription(i)));
    }

    for(int i = InvoiceNumberFormatData::SLASH; i <= InvoiceNumberFormatData::HYPHEN; ++i)
    {
        ui->comboBoxSeparator->addItem(InvoiceNumberFormatData::SeparatorName(i));
    }

    for(int i = InvoiceNumberFormatData::WEEK; i <= InvoiceNumberFormatData::WHOLE_LIFE; ++i)
    {
        ui->comboBoxPeriod->addItem(InvoiceNumberFormatData::ChosenPeriodName(i), i);
    }

    connect(ui->pushButtonAddField, SIGNAL(clicked()), this, SLOT(fieldAdd_()));
    connect(ui->pushButtonChangeField, SIGNAL(clicked()), this, SLOT(fieldChange_()));
    connect(ui->pushButtonRemoveField, SIGNAL(clicked()), this, SLOT(fieldRemove_()));
    connect(ui->pushButtonAddSeparator, SIGNAL(clicked()), this, SLOT(separatorAdd_()));
    connect(ui->pushButtonChangeSeparator, SIGNAL(clicked()), this, SLOT(separatorChange_()));
}


void InvoiceNumberFormatEditDialog::initList_(const QString &format)
{
    if(format.isEmpty())
        return;

    int from = 0, to = 0;
    const QChar left('{'), right('}');

    ui->listWidgetFields->clear();
    while( (from = format.indexOf(left, from)) != -1)
    {
        if( (to = format.indexOf(right, from)) != -1)
        {
            const QString fieldName(InvoiceNumberFormatData::FieldName(InvoiceNumberFormatData::FieldID(format.mid(from, to - from + 1))));
            ui->listWidgetFields->insertItem(ui->listWidgetFields->count(), fieldName);
            from = to + 1;
            if(from >= format.count())
                break;
        }
        else
        {
            break;
        }

        if(format.at(from) != left)
        {//it should be a separator now. If not, then it's an error
            to = format.indexOf(left, from);
            if(to == -1)
            {
                to = format.count() - 1;
            }

            const QChar separator(format.at(from));
            for(int i = InvoiceNumberFormatData::SLASH; i <= InvoiceNumberFormatData::HYPHEN; ++i)
            {
                if(separator == InvoiceNumberFormatData::SeparatorName(i))
                {
                    ui->listWidgetFields->insertItem(ui->listWidgetFields->count(), separator);
                    break;
                }
            }
        }
    }
}


void InvoiceNumberFormatEditDialog::accept()
{
    invNumFormat_.clear();
    const int rowMax = ui->listWidgetFields->count();
    for(int row = 0; row < rowMax; ++row)
    {
        invNumFormat_.append(ui->listWidgetFields->item(row)->text());
    }
    done(QDialog::Accepted);
}


void InvoiceNumberFormatEditDialog::fieldAdd_()
{
    const int index = ui->comboBoxFields->currentIndex();
    if(index == -1)
    {
        QMessageBox::information(this, trUtf8("Wybierz pole"), trUtf8("Wybierz najpierw pożądane pole numeru faktur."), QMessageBox::Ok);
        return;
    }

    ui->listWidgetFields->addItem(InvoiceNumberFormatData::FieldName(index));
}


void InvoiceNumberFormatEditDialog::fieldChange_()
{
    if(ui->comboBoxFields->currentIndex() == -1)
    {
        QMessageBox::information(this, trUtf8("Wybierz pole"), trUtf8("Wybierz najpierw pożądane pole numeru faktur."), QMessageBox::Ok);
        return;
    }

    const int index = ui->listWidgetFields->currentIndex().row();
    if(index == -1)
    {
        QMessageBox::information(this, trUtf8("Wybierz pole"), trUtf8("Wybierz najpierw pole do zmiany."), QMessageBox::Ok);
        return;
    }
    ui->listWidgetFields->item(index)->setText(InvoiceNumberFormatData::FieldName(ui->comboBoxFields->currentIndex()));
}


void InvoiceNumberFormatEditDialog::fieldRemove_()
{
    const int index = ui->listWidgetFields->currentIndex().row();
    if(index == -1)
    {
        QMessageBox::information(this, trUtf8("Wybierz pole"), trUtf8("Wybierz najpierw pole lub separator do skasowania."), QMessageBox::Ok);
        return;
    }

    ui->listWidgetFields->takeItem(index);
}


void InvoiceNumberFormatEditDialog::separatorAdd_()
{
    if(ui->comboBoxSeparator->currentIndex() == -1)
    {
        QMessageBox::information(this, trUtf8("Wybierz separator"), trUtf8("Wybierz najpierw pożądany separator."), QMessageBox::Ok);
        return;
    }

    ui->listWidgetFields->addItem(InvoiceNumberFormatData::SeparatorName(ui->comboBoxSeparator->currentIndex()));
}


void InvoiceNumberFormatEditDialog::separatorChange_()
{
    const int index = ui->listWidgetFields->currentIndex().row();
    if(index == -1)
    {
        QMessageBox::information(this, trUtf8("Wybierz separator"), trUtf8("Wybierz najpierw separator do zmiany."), QMessageBox::Ok);
        return;
    }

    ui->listWidgetFields->item(index)->setText(InvoiceNumberFormatData::SeparatorName(ui->comboBoxSeparator->currentIndex()));
}



void InvoiceNumberFormatEditDialog::showExamples_()
{

}
