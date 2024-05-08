#include <QDesktopServices>
#include <QUrl>

#include "../lib/Database.h"
#include "CategoriesView.h"
#include "ui_CategoriesView.h"

CategoriesView::CategoriesView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CategoriesView) {
  ui->setupUi(this);
  loadCategories();
}

CategoriesView::~CategoriesView() { delete ui; }

void CategoriesView::on_helpButton_clicked() {
  QDesktopServices::openUrl(QUrl(regexpHelpUrl));
}

void CategoriesView::on_newFilterButton_clicked() {}

void CategoriesView::on_newCategoryButton_clicked() {}

void CategoriesView::on_deleteCategoriesButton_clicked() {}

void CategoriesView::on_deleteFiltersButton_clicked() {}

void CategoriesView::loadCategories() {
  Database database = Database();

  QStringList categories = database.getCategoryNames();

  foreach (QString category, categories) {
    ui->categorysList->addItem(category);
  }

  if (categories.length() > 0) {
    ui->categorysList->setCurrentRow(0);
  }
}

void CategoriesView::loadFilters(QString category) {
  Database database = Database();

  QStringList filters = database.getFilterNames(category);

  foreach (QString filter, filters) {
    ui->filterList->addItem(filter);
  }
}
