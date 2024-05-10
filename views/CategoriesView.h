#ifndef CATEGORIESVIEW_H
#define CATEGORIESVIEW_H

#include <QMainWindow>
#include <QListWidgetItem>

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
  void on_helpButton_clicked();
  void on_newFilterButton_clicked();
  void on_newCategoryButton_clicked();
  void on_deleteCategoriesButton_clicked();
  void on_deleteFiltersButton_clicked();
  void on_categorysList_currentRowChanged(int currentRow);
  void on_categorysList_itemClicked(QListWidgetItem *item);
  void on_categorysList_itemSelectionChanged();

  void deleteCategories();
  void loadCategories();
  void loadFilters(QString category);
  void clearFilters();

  private:
  Ui::CategoriesView *ui;
};

#endif // CATEGORIESVIEW_H