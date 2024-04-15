#include "ImportFileView.h"
#include "../lib/CsvFile.h"
#include "../lib/Database.h"
#include "ui_ImportFileView.h"

#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QProgressDialog>
#include <QThread>

ImportFileView::ImportFileView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ImportFileView) {
  ui->setupUi(this);
  ui->ImportButton->setEnabled(false);
  ui->filePreviewTable->horizontalHeader()->setStretchLastSection(true);
}

ImportFileView::~ImportFileView() { delete ui; }

void ImportFileView::resetView() {
  ui->descriptionColumnComboBox->clear();
  ui->amountColumnComboBox->clear();
  ui->dateColumnComboBox->clear();
  ui->filePreviewTable->clear();
  ui->banksComboBox->clear();
  ui->ImportButton->setEnabled(false);
}

void ImportFileView::fillHeaders(QList<QStringList> rows) {
  int columnsCount = rows.at(0).length();

  ui->descriptionColumnComboBox->addItem(QString(" "));
  ui->amountColumnComboBox->addItem(QString(" "));
  ui->dateColumnComboBox->addItem(QString(" "));

  if (headerRows) {
    foreach (QString field, rows.at(0)) {
      ui->descriptionColumnComboBox->addItem(field);
      ui->amountColumnComboBox->addItem(field);
      ui->dateColumnComboBox->addItem(field);
    }
  } else {
    for (int n = 0; n < columnsCount; n++) {
      ui->descriptionColumnComboBox->addItem(QString::number(n));
      ui->amountColumnComboBox->addItem(QString::number(n));
      ui->dateColumnComboBox->addItem(QString::number(n));
    }
  }
}

void ImportFileView::fillRows(QList<QStringList> rows) {
  int rowsCount = rows.length();
  int columnsCount = rows.at(0).length();

  for (int column = 0; column < columnsCount; column++) {
    ui->ImportButton->setEnabled(false);
    for (int row = 0; row < rowsCount - headerRows; row++) {
      ui->filePreviewTable->setCellWidget(
          row, column, new QLabel(rows.at(row + headerRows).at(column)));
    }
  }
}

void ImportFileView::formatPreview(QList<QStringList> rows) {
  int rowsCount = rows.length();
  int columnsCount = rows.at(0).length();

  ui->filePreviewTable->setRowCount(rowsCount - headerRows);
  ui->filePreviewTable->setColumnCount(columnsCount);

  if (headerRows) {
    QStringList headers = rows.at(0);
    for (int n = 0; n < headers.length(); n++) {
      ui->filePreviewTable->setHorizontalHeaderItem(
          n, new QTableWidgetItem(QString(headers.at(n))));
    }
  }
}

void ImportFileView::updatePreview() {
  resetView();

  if (csvFile.isEmpty()) {
    return;
  }

  ui->banksComboBox->addItems(Database().getBankNames());

  QList<QStringList> rows = csvFile.getRows(0, 10);

  formatPreview(rows);
  fillHeaders(rows);
  fillRows(rows);
}

void ImportFileView::selectImportFile() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".",
                                                  tr("Csv Files (*.csv)"));

  if (fileName.isEmpty()) {
    return;
  }

  ui->fileNameEdit->setText(fileName);

  if (CsvFile::isValidCsvFile(fileName) == true) {
    csvFile.open(fileName);
    ui->rowsCountLabel->setText(
        QString("Rows to import: %1").arg(csvFile.rowsCount()));
    if (checkSelectedFile()) {
      updatePreview();
    } else {
      csvFile.close();
    }
  }
}

void ImportFileView::importSelectedFile() {
  Database database = Database();

  if (checkDatabaseConnection() == false) {
    return;
  }

  QProgressDialog progress =
      QProgressDialog("", "Cancel", 0, csvFile.rowsCount());

  progress.setWindowModality(Qt::WindowModal);
  progress.setWindowTitle("Import progress...");

  bool isCancelled = false;
  int storedRows = 0;
  int rowsToStore = csvFile.rowsCount() - headerRows;
  for (; storedRows < rowsToStore; storedRows++) {
    QStringList row = csvFile.getRows(storedRows + headerRows).at(0);

    if (row.length() == 0 || row.length() < csvFile.columnsCount()) {
      break;
    }

    QString date = row.at(dateColumn - INDEX_OFFSET);
    QString description = row.at(descriptionColumn - INDEX_OFFSET);
    double amount = row.at(amountColumn - INDEX_OFFSET).toDouble();

    qDebug() << row.at(amountColumn - INDEX_OFFSET);
    
    if (database.storeRow(bankName, date, description, amount) == false) {
      break;
    }

    progress.setLabelText(
        QString("%1 of %2 rows stored...").arg(storedRows).arg(rowsToStore));
    progress.setValue(storedRows);
    if (progress.wasCanceled()) {
      isCancelled = true;
      break;
    }
  }

  progress.cancel();

  if (isCancelled) {
    QMessageBox(QMessageBox::Icon::Information, QString("Import cancelled"),
                QString("A total of %1 from %2 rows imported")
                    .arg(storedRows)
                    .arg(rowsToStore))
        .exec();
  } else if (database.getLastErrorText().length()) {
    QMessageBox(QMessageBox::Icon::Critical, QString("Database error"),
                QString(database.getLastErrorText()))
        .exec();
  } else {
    QMessageBox(QMessageBox::Icon::Information, QString("Database success"),
                QString("A total of %1 from %2 rows imported")
                    .arg(storedRows)
                    .arg(rowsToStore))
        .exec();
  }
}

bool ImportFileView::checkSelectedFile() {
  QProgressDialog progress =
      QProgressDialog("", "Cancel", 0, csvFile.rowsCount());

  progress.setWindowModality(Qt::WindowModal);
  progress.setWindowTitle("Check progress...");

  bool isCancelled = false;
  int checkedRows = 0;
  int rowsToCheck = csvFile.rowsCount() - headerRows;
  QStringList row;

  for (; checkedRows < rowsToCheck; checkedRows++) {
    row = csvFile.getRows(checkedRows + headerRows).at(0);

    if (row.length() == 0 || row.length() != csvFile.columnsCount()) {
      break;
    }

    // Give time for the dialog to show
    QThread::sleep(std::chrono::milliseconds{1});

    progress.setValue(checkedRows);
    if (progress.wasCanceled()) {
      isCancelled = true;
      break;
    }
  }

  progress.cancel();

  if (isCancelled) {
    QMessageBox(
        QMessageBox::Icon::Information, QString("Check file cancelled"),
        QString("Check process cancelled at line %1.").arg(checkedRows + 1))
        .exec();
    return false;
  } else if (checkedRows != rowsToCheck) {
    QMessageBox(QMessageBox::Icon::Critical, QString("Check file error"),
                QString("Something's wrong at line %1. Row: %2")
                    .arg(checkedRows + 1)
                    .arg(row.length()))
        .exec();
    return false;
  } else {
    QMessageBox(QMessageBox::Icon::Information, QString("Check success"),
                QString("A total of %1 from %2 rows checked successfully")
                    .arg(checkedRows)
                    .arg(rowsToCheck))
        .exec();
    return true;
  }
}

bool ImportFileView::checkDatabaseConnection() {
  Database database = Database();

  if (database.checkConnection() == false) {
    QMessageBox(
        QMessageBox::Icon::Warning, QString("Database connection problem"),
        QString("Error %1 accessing database").arg(database.getLastErrorText()))
        .exec();

    return false;
  }

  return true;
}

void ImportFileView::on_openFileButton_clicked() { selectImportFile(); }

void ImportFileView::updateImportButtonState() {
  ui->ImportButton->setEnabled(dateColumn > 0 && descriptionColumn > 0 &&
                               amountColumn > 0 && bankName.isEmpty() == false);
}

void ImportFileView::on_firstRowCheckBox_stateChanged(int arg1) {
  headerRows = arg1 ? 1 : 0;
  updatePreview();
  ui->ImportButton->setEnabled(false);
}

void ImportFileView::on_dateColumnComboBox_currentIndexChanged(int index) {
  dateColumn = index;
  updateImportButtonState();
}

void ImportFileView::on_descriptionColumnComboBox_currentIndexChanged(
    int index) {
  descriptionColumn = index;
  updateImportButtonState();
}

void ImportFileView::on_amountColumnComboBox_currentIndexChanged(int index) {
  amountColumn = index;
  updateImportButtonState();
}

void ImportFileView::on_ImportButton_clicked() { importSelectedFile(); }

void ImportFileView::on_banksComboBox_editTextChanged(const QString &arg1) {
  bankName = arg1;
  updateImportButtonState();
}

void ImportFileView::on_banksComboBox_currentIndexChanged(int index) {
  bankName = ui->banksComboBox->itemText(index);
  updateImportButtonState();
}

void ImportFileView::on_banksComboBox_currentTextChanged(const QString &arg1) {
  bankName = arg1;
  updateImportButtonState();
}

void ImportFileView::on_databaseStatusButton_clicked() {
  if (checkDatabaseConnection()) {
    QMessageBox(QMessageBox::Icon::Information, QString("Database connection"),
                "Connected successfully to de database.")
        .exec();
  }
}
