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

void CategoriesView::on_deleteCategoriesButton_clicked() {
  QList<QListWidgetItem *> items = ui->categorysList->selectedItems();
  foreach (QListWidgetItem *item, items) {
    qDebug() << item->text();
  }
}

void CategoriesView::on_deleteFiltersButton_clicked() {}

void CategoriesView::on_categorysList_currentRowChanged(int currentRow) {
  loadFilters(ui->categorysList->currentItem()->text());
}

void CategoriesView::on_categorysList_itemSelectionChanged() {
  int selectedItems = ui->categorysList->selectedItems().length();
  if (selectedItems == 1) {
    ui->deleteCategoriesButton->setEnabled(true);
  } else if (selectedItems > 1) {
    clearFilters();
  }
}

void CategoriesView::loadCategories() {
  Database database = Database();
  QStringList categories = database.getCategoryNames();

  ui->categorysList->clear();

  foreach (QString category, categories) {
    ui->categorysList->addItem(category);
  }

  if (categories.length() > 0) {
    ui->categorysList->setCurrentRow(0);
  }
}

void CategoriesView::clearFilters() { ui->filterList->clear(); }

void CategoriesView::loadFilters(QString category) {
  Database database = Database();
  QStringList filters = database.getFilterNames(category);

  clearFilters();
  if (filters.isEmpty()) {
    filters = database.getDescriptionsByCategory(category);
    database.updateCategoryFilters(category, filters);
  }

  foreach (QString filter, filters) {
    ui->filterList->addItem(filter);
  }
}
