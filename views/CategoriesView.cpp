#include <QDesktopServices>
#include <QUrl>

#include "CategoriesView.h"
#include "ui_CategoriesView.h"

CategoriesView::CategoriesView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CategoriesView)
{
    ui->setupUi(this);
}

CategoriesView::~CategoriesView()
{
    delete ui;
}

void CategoriesView::on_helpButton_clicked()
{
    QDesktopServices::openUrl(QUrl("https://dev.mysql.com/doc/refman/8.0/en/regexp.html"));
}


void CategoriesView::on_newFilterButton_clicked()
{

}


void CategoriesView::on_newCategoryButton_clicked()
{

}


void CategoriesView::on_deleteCategoriesButton_clicked()
{

}


void CategoriesView::on_deleteFiltersButton_clicked()
{

}

