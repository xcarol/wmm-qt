#include "mainwindow.h"
#include "./build/wmm_autogen/include/ui_mainwindow.h"
#include "databasesettingsdialog.h"
#include "importfile.h"
#include "database.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setCentralWidget(new ImportFile());
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionDatabase_triggered() {
  DatabaseSettingsDialog dialog = DatabaseSettingsDialog();
  Database database = Database();

  dialog.init(database.getHostname(),
              database.getDatabase(),
              database.getUsername(),
              database.getUserpass());

  bool success = dialog.exec();
  if (success) {
    database.setHostname(dialog.hostname);
    database.setDatabase(dialog.database);
    database.setUsername(dialog.username);
    database.setUserpass(dialog.userpass);
  }
}
