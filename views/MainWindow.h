#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "BrowseDataView.h"
#include "CategorizeView.h"
#include "DuplicatesView.h"
#include "ImportFileView.h"
#include "SqlCommandView.h"
#include "CategoriesView.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

#define CURRENT_VIEW "CurrentView"
#define IMPORT_FILE_VIEW "ImportFileView"
#define CATEGORIZE_VIEW "CategorizeView"
#define DUPLICATES_VIEW "DuplicatesView"
#define BROWSE_DATA_VIEW "BrowseDataView"
#define SQL_COMMAND_VIEW "SqlCommandView"

#define IMPORT_FILE_VIEW_INDEX 0
#define CATEGORIZE_VIEW_INDEX 1
#define DUPLICATES_VIEW_INDEX 2
#define BROWSE_DATA_VIEW_INDEX 3
#define SQL_COMMAND_VIEW_INDEX 4
#define CATEGORIES_VIEW_INDEX 5

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_actionDatabase_triggered();
  void on_actionImport_triggered();
  void on_actionCategorize_triggered();
  void on_actionBrowse_triggered();
  void on_actionSql_Command_triggered();
  void on_actionDuplicates_triggered();
  void on_actionCategories_triggered();

  signals:
  void statusBarUpdated(const QString &message);

private:
  Ui::MainWindow *ui;
  QSettings settings = QSettings("com.xicra", "wmm");

  BrowseDataView browseDataView = BrowseDataView();
  CategoriesView categoriesView = CategoriesView();
  CategorizeView categorizeView = CategorizeView();
  DuplicatesView duplicatesView = DuplicatesView();
  ImportFileView importFileView = ImportFileView();
  SqlCommandView sqlCommandView = SqlCommandView();
};
#endif // MAINWINDOW_H
