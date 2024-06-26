#include "ImportFileView.h"
#include "../lib/CsvFile.h"
#include "../lib/Database.h"
#include "../widgets/MessageBox.h"
#include "ui_ImportFileView.h"

#include <QFileDialog>
#include <QList>
#include <QLocale>
#include <QProgressDialog>

ImportFileView::ImportFileView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ImportFileView) {
  ui->setupUi(this);
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

void ImportFileView::applyCategories() {
  Database database = Database();
  bool wasCancelled = false;

  QStringList categories = database.getCategoryNames();
  QProgressDialog progress = QProgressDialog(
      tr("Apply categories progress..."), tr("Cancel"), 0, categories.length());

  foreach (QString category, categories) {
    progress.setLabelText(QString(tr("Applying category %1...")).arg(category));
    database.updateRowsCategory(category);
    if (progress.wasCanceled()) {
      wasCancelled = true;
      break;
    }
  }

  if (wasCancelled) {
    MessageBox::Information(
        QString(tr("Categorize cancelled")),
        QString(tr("Categorize process was cancelled. Use 'Categorize' "
                   "view to check imported transactions.")));
  } else if (database.getLastErrorText().length()) {
    MessageBox::DatabaseError(database.getLastErrorText());
  } else {
    MessageBox::DatabaseSuccess(
        QString(tr("All categories applied. Use 'Categorize' view to check "
                   "imported transactions.")));
  }
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

void ImportFileView::importRows() {
  Database database = Database();

  if (database.checkConnection() == false) {
    return;
  }

  QProgressDialog progress =
      QProgressDialog("", tr("Cancel"), 0, csvFile.rowsCount());

  progress.setWindowModality(Qt::WindowModal);
  progress.setWindowTitle(tr("Import progress..."));

  bool wasCancelled = false;
  int storedRows = 0;
  int rowsToStore = csvFile.rowsCount() - headerRows;
  for (; storedRows < rowsToStore; storedRows++) {
    QStringList row = csvFile.getRows(storedRows + headerRows).at(0);

    if (row.length() == 0 || row.length() < csvFile.columnsCount()) {
      break;
    }

    QString date = row.at(dateColumn - INDEX_OFFSET);
    QString description = row.at(descriptionColumn - INDEX_OFFSET);

    bool ok = false;
    double amount =
        QLocale().toDouble(row.at(amountColumn - INDEX_OFFSET), &ok);

    if (ok == false) {
      QLocale locale = QLocale("C");
      amount = locale.toDouble(row.at(amountColumn - INDEX_OFFSET), &ok);
    }

    if (database.storeRow(bankName, date, description, amount) == false) {
      break;
    }

    progress.setLabelText(QString(tr("%1 of %2 rows stored..."))
                              .arg(storedRows)
                              .arg(rowsToStore));
    progress.setValue(storedRows);
    if (progress.wasCanceled()) {
      wasCancelled = true;
      break;
    }
  }

  progress.cancel();

  if (wasCancelled) {
    MessageBox::Information(QString(tr("Import cancelled")),
                            QString(tr("A total of %1 from %2 rows imported"))
                                .arg(storedRows)
                                .arg(rowsToStore));
  } else if (database.getLastErrorText().length()) {
    MessageBox::DatabaseError(database.getLastErrorText());
  } else {
    MessageBox::DatabaseSuccess(
        QString(tr("A total of %1 from %2 rows imported"))
            .arg(storedRows)
            .arg(rowsToStore));
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
        QString(tr("Rows to import: %1")).arg(csvFile.rowsCount()));
    if (checkSelectedFile()) {
      updatePreview();
    } else {
      csvFile.close();
    }
  }
}

void ImportFileView::importSelectedFile() {
  importRows();
  applyCategories();
}

bool ImportFileView::checkSelectedFile() {
  QProgressDialog progress =
      QProgressDialog("", tr("Cancel"), 0, csvFile.rowsCount());

  progress.setWindowModality(Qt::WindowModal);
  progress.setWindowTitle(tr("Check progress..."));

  bool isCancelled = false;
  int checkedRows = 0;
  int rowsToCheck = csvFile.rowsCount() - headerRows;
  QStringList row;

  for (; checkedRows < rowsToCheck; checkedRows++) {
    row = csvFile.getRows(checkedRows + headerRows).at(0);

    if (row.length() == 0 || row.length() != csvFile.columnsCount()) {
      break;
    }

    progress.setValue(checkedRows);

    if (progress.wasCanceled()) {
      isCancelled = true;
      break;
    }
  }

  progress.cancel();

  if (isCancelled) {
    MessageBox::Information(QString(tr("Check file cancelled")),
                            QString(tr("Check process cancelled at line %1."))
                                .arg(checkedRows + 1));
    return false;
  } else if (checkedRows != rowsToCheck) {
    MessageBox::Critical(QString(tr("Check file error")),
                         QString(tr("Something's wrong at line %1. Row: %2"))
                             .arg(checkedRows + 1)
                             .arg(row.length()));
    return false;
  } else {
    MessageBox::Information(
        QString(tr("Check success")),
        QString(tr("A total of %1 from %2 rows checked successfully"))
            .arg(checkedRows)
            .arg(rowsToCheck));
    return true;
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
