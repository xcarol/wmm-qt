#include "importfile.h"
#include "csvfile.h"
#include "database.h"
#include "ui_importfile.h"

#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QProgressDialog>
#include <qcontainerfwd.h>
#include <qlogging.h>
#include <qprogressdialog.h>
#include <qtextcursor.h>
#include <qtextformat.h>

ImportFile::ImportFile(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ImportFile) {
  ui->setupUi(this);
  ui->ImportButton->setEnabled(false);
}

ImportFile::~ImportFile() { delete ui; }

void ImportFile::updatePreview() {
  ui->descriptionColumnComboBox->clear();
  ui->amountColumnComboBox->clear();
  ui->dateColumnComboBox->clear();
  ui->filePreviewTable->clear();
  ui->ImportButton->setEnabled(false);

  if (csvFile.isEmpty()) {
    return;
  }

  int rowsCount = 5;
  QList<QStringList> rows = csvFile.getRows(rowsCount);
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

  ui->filePreviewTable->setRowCount(rowsCount - headerRows);
  ui->filePreviewTable->setColumnCount(columnsCount);

  if (headerRows) {
    QStringList headers = rows.at(0);
    for (int n = 0; n < headers.length(); n++) {
      ui->filePreviewTable->setHorizontalHeaderItem(
          n, new QTableWidgetItem(QString(headers.at(n))));
    }
  }

  for (int column = 0; column < columnsCount; column++) {
    ui->ImportButton->setEnabled(false);
    for (int row = 0; row < rowsCount - headerRows; row++) {
      ui->filePreviewTable->setCellWidget(
          row, column, new QLabel(rows.at(row + headerRows).at(column)));
    }
  }
}

void ImportFile::on_openFileButton_clicked() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".",
                                                  tr("Csv Files (*.csv)"));

  ui->fileNameEdit->setText(fileName);

  if (CsvFile::isValidCsvFile(fileName) == true) {
    csvFile.read(fileName);
    updatePreview();
  }
}

void ImportFile::updateImportButtonState() {
  ui->ImportButton->setEnabled(dateColumn > 0 && descriptionColumn > 0 &&
                               amountColumn > 0 && bankName.isEmpty() == false);
}

void ImportFile::on_firstRowCheckBox_stateChanged(int arg1) {
  headerRows = arg1 ? 1 : 0;
  updatePreview();
  ui->ImportButton->setEnabled(false);
}

void ImportFile::on_dateColumnComboBox_currentIndexChanged(int index) {
  dateColumn = index;
  updateImportButtonState();
}

void ImportFile::on_descriptionColumnComboBox_currentIndexChanged(int index) {
  descriptionColumn = index;
  updateImportButtonState();
}

void ImportFile::on_amountColumnComboBox_currentIndexChanged(int index) {
  amountColumn = index;
  updateImportButtonState();
}

void ImportFile::on_ImportButton_clicked() {
  QList<QStringList> rows = csvFile.getRows();
  QList<QStringList> databaseRows;

  for (int n = 0; n < rows.length() - headerRows; n++) {
    QStringList databaseRow;
    databaseRow.append(bankName);
    databaseRow.append(rows.at(n + headerRows).at(dateColumn - indexOffset));
    databaseRow.append(rows.at(n + headerRows).at(descriptionColumn - indexOffset));
    databaseRow.append(rows.at(n + headerRows).at(amountColumn - indexOffset));

    databaseRows.append(databaseRow);
  }

  Database database = Database();

  bool databaseOpen = database.open();

  if (databaseOpen == false) {
    QMessageBox box =
        QMessageBox(QMessageBox::Icon::Critical, QString("Database error"),
                    QString(database.getLastErrorText()));
    box.exec();
  }

  QProgressDialog progress =
      QProgressDialog("Import progress", "Cancel", 0, databaseRows.length());

  ulong storedRows = database.storeRows(databaseRows, &progress);

  progress.close();

  if (storedRows != databaseRows.length()) {
    QMessageBox(QMessageBox::Icon::Critical, QString("Database error"),
                QString(database.getLastErrorText()))
        .exec();
  } else {
    QMessageBox(QMessageBox::Icon::Information, QString("Database success"),
                QString("%1 rows imported").arg(storedRows))
        .exec();
  }

  database.close();
}

void ImportFile::on_banksComboBox_editTextChanged(const QString &arg1) {
  bankName = arg1;
  qDebug() << "on_banksComboBox_editTextChanged - arg1:" << arg1;
  updateImportButtonState();
}

void ImportFile::on_banksComboBox_currentIndexChanged(int index) {
  bankName = ui->banksComboBox->itemText(index);
  qDebug() << "on_banksComboBox_currentIndexChanged - index:" << index;
  updateImportButtonState();
}

void ImportFile::on_banksComboBox_currentTextChanged(const QString &arg1) {
  bankName = arg1;
  qDebug() << "on_banksComboBox_currentTextChanged - arg1:" << arg1;
  updateImportButtonState();
}
