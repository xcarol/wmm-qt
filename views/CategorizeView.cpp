#include "CategorizeView.h"
#include "ui_CategorizeView.h"

CategorizeView::CategorizeView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CategorizeView)
{
    ui->setupUi(this);
}

CategorizeView::~CategorizeView()
{
    delete ui;
}
