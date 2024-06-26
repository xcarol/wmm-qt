#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>

#include "../lib/Database.h"
#include "../widgets/MessageBox.h"
#include "CategoriesView.h"
#include "ui_CategoriesView.h"

CategoriesView::CategoriesView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CategoriesView) {
  ui->setupUi(this);
  loadCategories();
}

CategoriesView::~CategoriesView() { delete ui; }

void CategoriesView::on_applyCategoryButton_clicked() { applyCategory(); }

void CategoriesView::on_applyFilterButton_clicked() {
  QListWidgetItem *categoryItem = ui->categorysList->currentItem();
  QListWidgetItem *filterItem = ui->filterList->currentItem();

  if (categoryItem == NULL || filterItem == NULL) {
    return;
  }

  QString category = categoryItem->text();
  QString filter = filterItem->text();

  QMessageBox::StandardButton res = QMessageBox::question(
      QApplication::topLevelWidgets().first(), QString(tr("UPDATE")),
      QString(
          tr("You're about to apply category '%1' to the transactions which "
             "description meets the '%2' RegEx"
             "\nAre you sure?"))
          .arg(category)
          .arg(filter));
  if (res == QMessageBox::StandardButton::No) {
    return;
  }

  Database database = Database();
  int updatedRows = database.updateRowsCategory(filter, category);
  QString sqlError = database.getLastErrorText();

  if (sqlError.isEmpty()) {
    MessageBox::DatabaseSuccess(
        QString(tr("A total of %1 transactions updated.")).arg(updatedRows));
  } else {
    MessageBox::DatabaseError(sqlError);
  }
}

void CategoriesView::on_deleteCategoriesButton_clicked() { deleteCategories(); }

void CategoriesView::on_deleteFiltersButton_clicked() { deleteFilters(); }

void CategoriesView::on_categorysList_currentRowChanged(int currentRow) {
  if (currentRow >= 0) {
    loadFilters(ui->categorysList->currentItem()->text());
  }
}

void CategoriesView::on_categorysList_itemClicked(QListWidgetItem *item) {
  int selectedItems = ui->categorysList->selectedItems().length();

  ui->renameCurrentEdit->setText(
      selectedItems == 1 ? ui->categorysList->currentItem()->text() : "");
  ui->renameNewEdit->clear();

  updateCategoryButtons();
}

void CategoriesView::on_categorysList_itemSelectionChanged() {
  int selectedItems = ui->categorysList->selectedItems().length();

  if (selectedItems > 1) {
    clearFilters();
  }
}

void CategoriesView::on_filterList_itemClicked(QListWidgetItem *item) {
  updateFilterButtons();
}

void CategoriesView::on_helpButton_triggered(QAction *arg1) {
  QDesktopServices::openUrl(QUrl(regexpHelpUrl));
}

void CategoriesView::on_newCategoryButton_clicked() { addCategory(); }

void CategoriesView::on_newFilterButton_clicked() { addFilter(); }

void CategoriesView::on_newCategoryEdit_textChanged(const QString &arg1) {
  ui->newCategoryButton->setEnabled(arg1.length() > 0);
}

void CategoriesView::on_newCategoryEdit_returnPressed() { addCategory(); }

void CategoriesView::on_newFilterEdit_textChanged(const QString &arg1) {
  updateFilterButtons();
}

void CategoriesView::on_newFilterEdit_returnPressed() { addFilter(); }

void CategoriesView::on_renameButton_clicked() {
  renameCategory(ui->renameCurrentEdit->text(), ui->renameNewEdit->text());
}

void CategoriesView::on_renameCurrentEdit_textChanged(const QString &arg1) {
  updateCategoryButtons();
}

void CategoriesView::on_renameNewEdit_textChanged(const QString &arg1) {
  updateCategoryButtons();
}

void CategoriesView::addCategory() {
  QString category = ui->newCategoryEdit->text();
  if (category.length()) {
    ui->categorysList->addItem(category);
  }
}

void CategoriesView::addFilter() {
  QString category = ui->categorysList->currentItem()->text();
  QString filter = ui->newFilterEdit->text();

  if (category.length() && filter.length()) {
    Database database = Database();

    database.addFilter(category, filter);

    QString error = database.getLastErrorText();
    if (!error.isEmpty()) {
      MessageBox::DatabaseError(database.getLastErrorText());
      return;
    }

    loadFilters(category);
  }
}

void CategoriesView::applyCategory() {
  QString category = ui->categorysList->currentItem()->text();

  QMessageBox::StandardButton res = QMessageBox::question(
      QApplication::topLevelWidgets().first(), QString(tr("APPLY")),
      QString(
          tr("You're about to apply category '%1' to the uncategorized "
             "transactions that match this category filters.\nAre you sure?"))
          .arg(category));
  if (res == QMessageBox::StandardButton::No) {
    return;
  }

  Database database = Database();
  QString sqlError;

  int updatedRows = database.updateRowsCategory(category);
  sqlError = database.getLastErrorText();
  if (sqlError.isEmpty()) {
    MessageBox::DatabaseSuccess(
        QString(tr("A total of %1 transactions updated.")).arg(updatedRows));
  } else {
    MessageBox::DatabaseError(database.getLastErrorText());
  }
}

void CategoriesView::clearFilters() {
  ui->filterList->clear();
  ui->newFilterEdit->clear();
}

void CategoriesView::deleteCategories() {
  QStringList categories;
  QList<QListWidgetItem *> items = ui->categorysList->selectedItems();

  std::function<QString(QStringList)> categoriesAsList =
      [](QStringList categories) -> QString {
    QString list;
    foreach (QString category, categories) {
      list.append("- ").append(category).append("\n");
    }
    return list;
  };

  foreach (QListWidgetItem *item, items) {
    categories.append(item->text());
  }

  QMessageBox::StandardButton res = QMessageBox::question(
      QApplication::topLevelWidgets().first(), QString(tr("DELETE")),
      QString(tr("You're about to delete the following categories"
                 ":\n%2\n\nAre you sure?"))
          .arg(categoriesAsList(categories)));
  if (res == QMessageBox::StandardButton::No) {
    return;
  }

  res = QMessageBox::question(
      QApplication::topLevelWidgets().first(), QString(tr("DELETE")),
      QString(tr("It will also delete the filters of the selected categories.\n"
                 "And it will also remove this categories from the "
                 "corresponding transactions."
                 "\n\nAre you really really sure?")));
  if (res == QMessageBox::StandardButton::No) {
    return;
  }

  Database database = Database();
  QString sqlError;

  int deletedRows = database.deleteCategories(categories);
  sqlError = database.getLastErrorText();
  if (!sqlError.isEmpty()) {
    MessageBox::DatabaseError(database.getLastErrorText());
    return;
  }

  int updatedRows = database.resetRowsCategories(categories);
  sqlError = database.getLastErrorText();
  if (!sqlError.isEmpty()) {
    MessageBox::DatabaseError(database.getLastErrorText());
    return;
  }

  MessageBox::DatabaseSuccess(
      QString(tr("A total of %1 categories deleted.\n"
                 "A total of %2 filters deleted.\n"
                 "A total of %3 transactions updated."))
          .arg(categories.length())
          .arg(deletedRows)
          .arg(updatedRows));

  loadCategories();
}

void CategoriesView::deleteFilters() {
  QStringList filters;
  QList<QListWidgetItem *> items = ui->filterList->selectedItems();

  foreach (QListWidgetItem *item, items) {
    filters.append(item->text());
  }

  QMessageBox::StandardButton res = QMessageBox::question(
      QApplication::topLevelWidgets().first(), QString(tr("DELETE")),
      QString(tr("You're about to delete %1 filters.\nAre you sure?"))
          .arg(filters.length()));
  if (res == QMessageBox::StandardButton::No) {
    return;
  }

  Database database = Database();
  QString sqlError;

  int deletedRows = database.deleteFilters(filters);
  sqlError = database.getLastErrorText();
  if (!sqlError.isEmpty()) {
    MessageBox::DatabaseError(database.getLastErrorText());
    return;
  }

  loadCategories(ui->categorysList->currentRow());
}

void CategoriesView::loadCategories(int selectRow) {
  Database database = Database();
  QStringList categories = database.getCategoryNames();

  ui->categorysList->clear();

  foreach (QString category, categories) {
    ui->categorysList->addItem(category);
  }

  if (categories.length() > 0) {
    int row = 0;

    if (selectRow < categories.length()) {
      row = selectRow;
    }

    ui->categorysList->setCurrentRow(row);
  }
}

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

  updateFilterButtons();
}

void CategoriesView::renameCategory(QString category, QString newName) {
  Database database = Database();

  database.renameCategory(category, newName);

  QString error = database.getLastErrorText();
  if (!error.isEmpty()) {
    MessageBox::DatabaseError(database.getLastErrorText());
    return;
  }

  loadCategories();
}

void CategoriesView::updateCategoryButtons() {
  int selectedCategories = ui->categorysList->selectedItems().length();
  ui->applyCategoryButton->setEnabled(selectedCategories == 1);
  ui->deleteCategoriesButton->setEnabled(selectedCategories);
  ui->renameButton->setEnabled(
      ui->renameCurrentEdit->text().isEmpty() == false &&
      ui->renameNewEdit->text().isEmpty() == false && selectedCategories == 1);
}

void CategoriesView::updateFilterButtons() {
  int selectedFilters = ui->filterList->selectedItems().length();
  ui->deleteFiltersButton->setEnabled(selectedFilters);
  ui->applyFilterButton->setEnabled(selectedFilters == 1);
  ui->newFilterButton->setEnabled(ui->newFilterEdit->text().length() &&
                                  ui->categorysList->selectedItems().length() ==
                                      1);
}
