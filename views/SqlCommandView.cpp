#include "SqlCommandView.h"
#include "../lib/Database.h"
#include "ui_SqlCommandView.h"
#include <QThread>
#include <QMessageBox>
#include <QSqlField>

SqlCommandView::SqlCommandView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SqlCommandView) {
  ui->setupUi(this);
}

SqlCommandView::~SqlCommandView() { delete ui; }

void SqlCommandView::on_execButton_clicked() {
  Database database = Database();

  QProgressDialog progress =
      QProgressDialog("Executing query...", "Cancel", 0, 100);

  progress.setWindowModality(Qt::WindowModal);
  progress.setWindowTitle("Search...");

  QList<QSqlRecord> records =
      database.execCommand(ui->sqlCommandEdit->toPlainText());

  if (progress.wasCanceled()) {
    return;
  }

  QString errorText = database.getLastErrorText();
  if (errorText.isEmpty() == false) {
    QMessageBox(QMessageBox::Icon::Critical, QString("Database error"),
                QString(database.getLastErrorText()))
        .exec();
    return;
  }

  progress.setMaximum(records.count());

  fillResultTable(records, &progress);
}

void SqlCommandView::fillResultTable(QList<QSqlRecord> records,
                                     QProgressDialog* dialog) {
  int rowsCount = records.length();

  if (rowsCount <= 0) {
    return;
  }

  int columnsCount = records.at(0).count();

  ui->sqlCommandResultTable->setRowCount(rowsCount);
  ui->sqlCommandResultTable->setColumnCount(columnsCount);

  for (int n = 0; n < columnsCount; n++) {
    ui->sqlCommandResultTable->setHorizontalHeaderItem(
        n, new QTableWidgetItem(QString(records.at(0).fieldName(n))));
  }

  for (int column = 0; column < columnsCount; column++) {
    QThread::sleep(std::chrono::milliseconds{1});
    dialog->setValue(column);

    for (int row = 0; row < rowsCount; row++) {
      QSqlField field = records.at(row).field(column);

      ui->sqlCommandResultTable->setCellWidget(
          row, column, new QLabel(field.value().toString()));
    }
  }
}