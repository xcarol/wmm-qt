#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

#define CURRENT_VIEW "CurrentView"
#define IMPORT_FILE_VIEW "ImportFileView"
#define CATEGORIZE_VIEW "CategorizeView"
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

signals:
  void statusBarUpdated(const QString &message);

private:
  Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
