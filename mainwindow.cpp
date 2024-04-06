#include "mainwindow.h"
#include "./build/wmm_autogen/include/ui_mainwindow.h"
#include "databasesettingsdialog.h"
#include "importfile.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setCentralWidget(new ImportFile());
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionDatabase_triggered() {
  DatabaseSettingsDialog dialog = DatabaseSettingsDialog();

  dialog.init(settings.value("hostname").toString(),
              settings.value("database").toString(),
              settings.value("username").toString(),
              settings.value("userpass").toString());

  bool success = dialog.exec();
  if (success) {
    settings.setValue("hostname", dialog.hostname);
    settings.setValue("database", dialog.database);
    settings.setValue("username", dialog.username);
    settings.setValue("userpass", dialog.userpass);
  }
}
