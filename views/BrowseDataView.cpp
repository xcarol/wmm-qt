#include "BrowseDataView.h"
#include "ui_BrowseDataView.h"

BrowseDataView::BrowseDataView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::BrowseDataView) {
  ui->setupUi(this);
  ui->tabWidget->addTab(&bankView, banksLabel);
  ui->tabWidget->addTab(&categoriesView, categoriesLabel);
}

BrowseDataView::~BrowseDataView() { delete ui; }

void BrowseDataView::on_tabWidget_currentChanged(int index) {
  if (ui->tabWidget->tabText(index) == banksLabel) {
    bankView.update();
  }
}
