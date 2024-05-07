#include "MainWindow.h"
#include "../dialogs/DatabaseSettings.h"
#include "../lib/Database.h"
#include "BrowseDataView.h"
#include "CategorizeView.h"
#include "DuplicatesView.h"
#include "ImportFileView.h"
#include "SqlCommandView.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  if (settings.value(CURRENT_VIEW).toString() == CATEGORIZE_VIEW) {
    setCentralWidget(new CategorizeView());
  } else if (settings.value(CURRENT_VIEW).toString() == DUPLICATES_VIEW) {
    setCentralWidget(new DuplicatesView());
  } else if (settings.value(CURRENT_VIEW).toString() == IMPORT_FILE_VIEW) {
    setCentralWidget(new ImportFileView());
  } else if (settings.value(CURRENT_VIEW).toString() == BROWSE_DATA_VIEW) {
    setCentralWidget(new BrowseDataView());
  } else if (settings.value(CURRENT_VIEW).toString() == SQL_COMMAND_VIEW) {
    setCentralWidget(new SqlCommandView());
  } else {
    setCentralWidget(new MainWindow());
  }
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionDatabase_triggered() {
  DatabaseSettings dialog = DatabaseSettings();
  Database database = Database();

  dialog.init(database.getHostname(), database.getPort(),
              database.getDatabase(), database.getUsername(),
              database.getUserpass());

  bool success = dialog.exec();
  if (success) {
    database.setHostname(dialog.hostname);
    database.setPort(dialog.port);
    database.setDatabase(dialog.database);
    database.setUsername(dialog.username);
    database.setUserpass(dialog.userpass);
  }
}

void MainWindow::on_actionImport_triggered() {
  settings.setValue(CURRENT_VIEW, IMPORT_FILE_VIEW);
  setCentralWidget(new ImportFileView());
}

void MainWindow::on_actionCategorize_triggered() {
  settings.setValue(CURRENT_VIEW, CATEGORIZE_VIEW);
  setCentralWidget(new CategorizeView());
}

void MainWindow::on_actionBrowse_triggered() {
  settings.setValue(CURRENT_VIEW, BROWSE_DATA_VIEW);
  setCentralWidget(new BrowseDataView());
}

void MainWindow::on_actionSql_Command_triggered() {
  settings.setValue(CURRENT_VIEW, SQL_COMMAND_VIEW);
  setCentralWidget(new SqlCommandView());
}

void MainWindow::on_actionDuplicates_triggered()
{
  settings.setValue(CURRENT_VIEW, DUPLICATES_VIEW);
  setCentralWidget(new DuplicatesView());
}

