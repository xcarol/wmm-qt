#include "SqlCommandView.h"
#include "../lib/Database.h"
#include "ui_SqlCommandView.h"
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
      QProgressDialog("Executing query...", "Cancel", 0, 100, this);

  progress.setWindowModality(Qt::WindowModal);
  progress.setWindowTitle("Sql Command");

  QList<QSqlRecord> records =
      database.execCommand(ui->sqlCommandEdit->toPlainText());

  if (progress.wasCanceled()) {
    return;
  }

  ui->rowsAffectedLabel->setText(
      QString("Rows affected: %1").arg(records.count()));

  QString errorText = database.getLastErrorText();
  if (errorText.isEmpty() == false) {
    QMessageBox(QMessageBox::Icon::Critical, QString("Database error"),
                QString(database.getLastErrorText()))
        .exec();
    return;
  }

  progress.setMaximum(records.count());

  fillResultTable(records,
                  [&progress](int value) { progress.setValue(value); });
}

void SqlCommandView::fillResultTable(QList<QSqlRecord> records,
                                     std::function<void(int)> setProgress) {
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
    setProgress(column);

    for (int row = 0; row < rowsCount; row++) {
      QSqlField field = records.at(row).field(column);

      ui->sqlCommandResultTable->setCellWidget(
          row, column, new QLabel(field.value().toString()));
    }
  }
}