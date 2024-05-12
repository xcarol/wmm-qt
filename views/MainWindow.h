#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

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
};
#endif // MAINWINDOW_H
