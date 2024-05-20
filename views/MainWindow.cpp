#include "MainWindow.h"
#include "../dialogs/DatabaseSettings.h"
#include "../lib/Database.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->stackedWidget->addWidget(&importFileView);
  ui->stackedWidget->addWidget(&categorizeView);
  ui->stackedWidget->addWidget(&duplicatesView);
  ui->stackedWidget->addWidget(&browseDataView);
  ui->stackedWidget->addWidget(&sqlCommandView);
  ui->stackedWidget->addWidget(&categoriesView);

  if (settings.value(CURRENT_VIEW).toString() == CATEGORIZE_VIEW) {
    ui->stackedWidget->setCurrentIndex(CATEGORIZE_VIEW_INDEX);
  } else if (settings.value(CURRENT_VIEW).toString() == DUPLICATES_VIEW) {
    ui->stackedWidget->setCurrentIndex(DUPLICATES_VIEW_INDEX);
  } else if (settings.value(CURRENT_VIEW).toString() == IMPORT_FILE_VIEW) {
    ui->stackedWidget->setCurrentIndex(IMPORT_FILE_VIEW_INDEX);
  } else if (settings.value(CURRENT_VIEW).toString() == BROWSE_DATA_VIEW) {
    ui->stackedWidget->setCurrentIndex(BROWSE_DATA_VIEW_INDEX);
  } else if (settings.value(CURRENT_VIEW).toString() == SQL_COMMAND_VIEW) {
    ui->stackedWidget->setCurrentIndex(SQL_COMMAND_VIEW_INDEX);
  } else {
    ui->stackedWidget->setCurrentIndex(IMPORT_FILE_VIEW_INDEX);
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
    ui->stackedWidget->setCurrentIndex(IMPORT_FILE_VIEW_INDEX);
}

void MainWindow::on_actionCategorize_triggered() {
  settings.setValue(CURRENT_VIEW, CATEGORIZE_VIEW);
    ui->stackedWidget->setCurrentIndex(CATEGORIZE_VIEW_INDEX);
}

void MainWindow::on_actionBrowse_triggered() {
  settings.setValue(CURRENT_VIEW, BROWSE_DATA_VIEW);
    ui->stackedWidget->setCurrentIndex(BROWSE_DATA_VIEW_INDEX);
}

void MainWindow::on_actionSql_Command_triggered() {
  settings.setValue(CURRENT_VIEW, SQL_COMMAND_VIEW);
    ui->stackedWidget->setCurrentIndex(SQL_COMMAND_VIEW_INDEX);
}

void MainWindow::on_actionDuplicates_triggered() {
  settings.setValue(CURRENT_VIEW, DUPLICATES_VIEW);
    ui->stackedWidget->setCurrentIndex(DUPLICATES_VIEW_INDEX);
}

void MainWindow::on_actionCategories_triggered() {
    ui->stackedWidget->setCurrentIndex(CATEGORIES_VIEW_INDEX);
}
