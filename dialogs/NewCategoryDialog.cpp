#include "NewCategoryDialog.h"
#include "ui_NewCategoryDialog.h"
#include <qdialogbuttonbox.h>
#include <qpushbutton.h>

NewCategoryDialog::NewCategoryDialog(QString defaultCategory, QString defaultFilter, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewCategoryDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->categoryName->setText(defaultCategory);
    ui->filterName->setText(defaultFilter);
}

NewCategoryDialog::~NewCategoryDialog()
{
    delete ui;
}

void NewCategoryDialog::on_buttonBox_accepted()
{
    category = ui->categoryName->text();
    filter = ui->filterName->text();
}


void NewCategoryDialog::on_categoryName_textChanged(const QString &arg1)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ui->categoryName->text().isEmpty() == false && ui->filterName->text().isEmpty() == false);
}


void NewCategoryDialog::on_filterName_textChanged(const QString &arg1)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(ui->categoryName->text().isEmpty() == false && ui->filterName->text().isEmpty() == false);
}

