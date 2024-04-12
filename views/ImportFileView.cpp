#include "ImportFileView.h"
#include "../lib/CsvFile.h"
#include "../lib/Database.h"
#include "ui_ImportFileView.h"

#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QProgressDialog>

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

  QList<QStringList> rows = csvFile.getRows(10);

  formatPreview(rows);
  fillHeaders(rows);
  fillRows(rows);
}

void ImportFileView::selectImportFile() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".",
                                                  tr("Csv Files (*.csv)"));

  ui->fileNameEdit->setText(fileName);

  if (CsvFile::isValidCsvFile(fileName) == true) {
    csvFile.open(fileName);
    updatePreview();
  }
}

void ImportFileView::importSelectedFile() {
  Database database = Database();

  QProgressDialog progress =
      QProgressDialog("Import progress", "Cancel", 0, csvFile.rowsCount());

  progress.show();

  int storedRows = 0;
  for (; storedRows < csvFile.rowsCount() - headerRows; storedRows++) {
    QList<QStringList> rows = csvFile.getRows(storedRows);

    QStringList databaseRow;
    databaseRow.append(bankName);
    databaseRow.append(rows.at(storedRows + headerRows).at(dateColumn - indexOffset));
    databaseRow.append(
        rows.at(storedRows + headerRows).at(descriptionColumn - indexOffset));
    databaseRow.append(rows.at(storedRows + headerRows).at(amountColumn - indexOffset));

    if(database.storeRow(databaseRow) == false) {
      break;
    }

    progress.setValue(storedRows);
  }

  progress.cancel();

  if (storedRows != csvFile.rowsCount()) {
    QMessageBox(QMessageBox::Icon::Critical, QString("Database error"),
                QString(database.getLastErrorText()))
        .exec();
  } else {
    QMessageBox(QMessageBox::Icon::Information, QString("Database success"),
                QString("A total of %1 rows imported").arg(storedRows))
        .exec();
  }
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
