#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "../dialogs/DatabaseSettings.h"
#include "../helpers/Database.h"
#include "CategorizeView.h"
#include "ImportFileView.h"
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  QSettings settings = QSettings("com.xicra", "wmm");
  if (settings.value(CURRENT_VIEW).toString() == CATEGORIZE_VIEW) {
    setCentralWidget(new CategorizeView());
  } else {
    setCentralWidget(new ImportFileView());
  }
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionDatabase_triggered() {
  DatabaseSettings dialog = DatabaseSettings();
  Database database = Database();

  dialog.init(database.getHostname(), database.getDatabase(),
              database.getUsername(), database.getUserpass());

  bool success = dialog.exec();
  if (success) {
    database.setHostname(dialog.hostname);
    database.setDatabase(dialog.database);
    database.setUsername(dialog.username);
    database.setUserpass(dialog.userpass);
  }
}

void MainWindow::on_actionImport_triggered() {
  QSettings settings = QSettings("com.xicra", "wmm");
  settings.setValue(CURRENT_VIEW, IMPORT_VIEW);
  setCentralWidget(new ImportFileView());
}

void MainWindow::on_actionCategorize_triggered() {
  QSettings settings = QSettings("com.xicra", "wmm");
  settings.setValue(CURRENT_VIEW, CATEGORIZE_VIEW);
  setCentralWidget(new CategorizeView());
}
