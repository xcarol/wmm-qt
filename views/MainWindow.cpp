#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "../dialogs/DatabaseSettings.h"
#include "ImportFileView.h"
#include "../helpers/Database.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setCentralWidget(new ImportFileView());
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionDatabase_triggered() {
  DatabaseSettings dialog = DatabaseSettings();
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
