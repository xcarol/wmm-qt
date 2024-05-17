#include "SqlCommandView.h"
#include "../lib/Database.h"
#include "ui_SqlCommandView.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlField>
#include <QSqlRecord>

SqlCommandView::SqlCommandView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SqlCommandView) {
  ui->setupUi(this);
  ui->historyComboBox->addItem(QString());
  ui->historyComboBox->addItems(settings.value(settingsName).toStringList());
}

SqlCommandView::~SqlCommandView() { delete ui; }


void SqlCommandView::on_clearHistoryButton_clicked()
{
  settings.setValue(settingsName, QStringList());
  ui->historyComboBox->clear();
}

void SqlCommandView::on_execButton_clicked() {
  Database database = Database();

  QString commandFromHistory = ui->sqlCommandEdit->toPlainText();
  ui->historyComboBox->addItem(commandFromHistory);
  QStringList sqlCommandHistory = settings.value(settingsName).toStringList();
  sqlCommandHistory.append(commandFromHistory);
  settings.setValue(settingsName, sqlCommandHistory);

  QProgressDialog progress =
      QProgressDialog(tr("Executing query..."), tr("Cancel"), 0, 100, this);

  progress.setWindowModality(Qt::WindowModal);
  progress.setWindowTitle(tr("Sql Command"));
  progress.show();

  QList<QSqlRecord> records =
      database.execCommand(ui->sqlCommandEdit->toPlainText());

  if (progress.wasCanceled()) {
    return;
  }

  ui->rowsAffectedLabel->setText(
      QString(tr("Rows affected: %1")).arg(records.count()));

  QString errorText = database.getLastErrorText();
  if (errorText.isEmpty() == false) {
    QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                QString(database.getLastErrorText()))
        .exec();
    return;
  }

  progress.setMaximum(records.count());

  fillResultTable(records,
                  [&progress](int value) { progress.setValue(value); });
}


void SqlCommandView::on_historyComboBox_currentIndexChanged(int index)
{
  QString command = ui->historyComboBox->currentText();
  ui->sqlCommandEdit->setPlainText(command);
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
void SqlCommandView::on_backupButton_clicked() {
  QString filePath =
      QFileDialog::getExistingDirectory(this, tr("Open File"), ".");

  if (filePath.isEmpty()) {
    return;
  }

  QString fileName =
      filePath.append("/")
          .append("wmm-")
          .append(QDate::currentDate().toString(Qt::DateFormat::ISODate))
          .append("-")
          .append(QTime::currentTime().toString())
          .append(".sql");

  Database database = Database();

  if (database.backup(fileName) == false) {
    QString errorText = database.getLastErrorText();
    if (errorText.isEmpty() == false) {
      QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                  QString(database.getLastErrorText()))
          .exec();
    }
    return;
  }

  QMessageBox(QMessageBox::Icon::Information, QString(tr("Database Success")),
              QString(tr("Database backup created at file: %1")).arg(fileName))
      .exec();
}

void SqlCommandView::on_restoreButton_clicked() {
  QMessageBox ready =
      QMessageBox(QMessageBox::Icon::Question, tr("Database restore"),
                  tr("Make sure table `wmm` is already created."),
                  QMessageBox::Yes | QMessageBox::No, this);

  if (ready.exec() == QMessageBox::No) {
    return;
  }

  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".",
                                                  tr("Sql Files (*.sql)"));

  if (fileName.isEmpty()) {
    return;
  }

  Database database = Database();

  if (database.restore(fileName) == false) {
    QString errorText = database.getLastErrorText();
    if (errorText.isEmpty() == false) {
      QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                  QString(database.getLastErrorText()))
          .exec();
      return;
    }
  }

  QMessageBox(QMessageBox::Icon::Information, QString(tr("Database Success")),
              QString(tr("Database restore from file: %1")).arg(fileName))
      .exec();
}

