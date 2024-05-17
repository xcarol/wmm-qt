#ifndef SQLCOMMANDVIEW_H
#define SQLCOMMANDVIEW_H

#include <QMainWindow>
#include <QProgressDialog>
#include <QSqlQuery>
#include <QSettings>

namespace Ui {
class SqlCommandView;
}

class SqlCommandView : public QMainWindow {
  Q_OBJECT

public:
  explicit SqlCommandView(QWidget *parent = nullptr);
  ~SqlCommandView();

private slots:
  void on_clearHistoryButton_clicked();
  void on_execButton_clicked();
  void on_backupButton_clicked();
  void on_restoreButton_clicked();
  void on_historyComboBox_currentIndexChanged(int index);

  private:
  Ui::SqlCommandView *ui;

  QString settingsName = "sqlCommandHistory";
  QSettings settings = QSettings("com.xicra", "wmm");

  void fillResultTable(QList<QSqlRecord> records,
                       std::function<void(int)> functionObject);
};

#endif // SQLCOMMANDVIEW_H
