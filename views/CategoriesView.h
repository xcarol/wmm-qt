#ifndef CATEGORIESVIEW_H
#define CATEGORIESVIEW_H

#include <QMainWindow>

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

  void loadCategories();
  void loadFilters(QString category);

private:
  Ui::CategoriesView *ui;
};

#endif // CATEGORIESVIEW_H
