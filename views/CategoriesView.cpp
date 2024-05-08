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
