#ifndef CATEGORIESVIEW_H
#define CATEGORIESVIEW_H

#include <QMainWindow>
#include <QListWidget>

namespace Ui {
class CategoriesView;
}

class CategoriesView : public QMainWindow {
  Q_OBJECT

public:
  explicit CategoriesView(QWidget *parent = nullptr);
  ~CategoriesView();

private:
  QString regexpHelpUrl = QString("https://dev.mysql.com/doc/refman/8.0/en/regexp.html");

private slots:
  void on_applyCategoryButton_clicked();
  void on_applyFilterButton_clicked();
  void on_deleteCategoriesButton_clicked();
  void on_deleteFiltersButton_clicked();
  void on_categorysList_currentRowChanged(int currentRow);
  void on_categorysList_itemClicked(QListWidgetItem *item);
  void on_categorysList_itemSelectionChanged();
  void on_filterList_itemClicked(QListWidgetItem *item);
  void on_helpButton_triggered(QAction *arg1);
  void on_newCategoryButton_clicked();
  void on_newFilterButton_clicked();
  void on_newCategoryEdit_textChanged(const QString &arg1);
  void on_newCategoryEdit_returnPressed();
  void on_newFilterEdit_textChanged(const QString &arg1);
  void on_newFilterEdit_returnPressed();
  void on_renameButton_clicked();
  void on_renameCurrentEdit_textChanged(const QString &arg1);
  void on_renameNewEdit_textChanged(const QString &arg1);

  void addCategory();
  void addFilter();
  void applyCategory();
  void clearFilters();
  void deleteCategories();
  void deleteFilters();
  void loadCategories(int selectRow = 0);
  void loadFilters(QString category);
  void renameCategory(QString category, QString newName);
  void updateCategoryButtons();
  void updateFilterButtons();

  private:
  Ui::CategoriesView *ui;
};

#endif // CATEGORIESVIEW_H
