#include "mainwindow.h"
#include "./build/wmm_autogen/include/ui_mainwindow.h"
#include "importfile.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setCentralWidget(new ImportFile());
}

MainWindow::~MainWindow() { delete ui; }
