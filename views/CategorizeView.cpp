#include <QDate>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>

#include "../lib/Database.h"
#include "CategorizeView.h"
#include "ui_CategorizeView.h"

CategorizeView::CategorizeView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CategorizeView) {
  ui->setupUi(this);
  ui->updateButton->setEnabled(false);
  ui->searchResultsTable->horizontalHeader()->setStretchLastSection(true);

  QSettings settings = QSettings("com.xicra", "wmm");
  QStringList filters = settings.value("filters").toStringList();
  ui->filterEdit->addItems(filters);

  connect(ui->filterEdit->lineEdit(), &QLineEdit::returnPressed, this,
          &CategorizeView::on_filterEdit_returnPressed);
}

CategorizeView::~CategorizeView() { delete ui; }

void CategorizeView::on_categoryComboBox_editTextChanged(const QString &arg1) {
  categoryName = arg1;
  updateUpdateButtonState();
}

void CategorizeView::on_filterEdit_currentIndexChanged(int index) {
  appliedFilter = ui->filterEdit->itemText(index);
}

void CategorizeView::on_filterEdit_editTextChanged(const QString &arg1) {
  appliedFilter = arg1;
}

void CategorizeView::on_filterEdit_returnPressed() {
  on_searchButton_clicked();
}

void CategorizeView::on_searchButton_clicked() {
  searchUncategorizedRows();
  updateUpdateButtonState();
}

void CategorizeView::on_searchResultsTable_itemSelectionChanged() {
  int enable = getSelectedRowsHeaders(ui->searchResultsTable).length() > 0;
  updateUpdateButtonState();
}

void CategorizeView::on_updateButton_clicked() {
  QList<int> selectedRows = getSelectedRowsHeaders(ui->searchResultsTable);

  if (selectedRows.length() == 0) {
    selectedRows = getAllRowsHeaders(ui->searchResultsTable);
  }

  QMessageBox::StandardButton res = QMessageBox::question(
      QApplication::topLevelWidgets().first(), QString(tr("Update")),
      QString(tr("You're about to update %1 records "
                 "with the category: %2\nAre you sure?"))
          .arg(selectedRows.length())
          .arg(categoryName));
  if (res == QMessageBox::StandardButton::No) {
    return;
  }

  updateUncategorizedRows(selectedRows);
}

void CategorizeView::addHeadersToSearchResultsTable(QStringList headers) {
  ui->searchResultsTable->setHorizontalHeaderItem(
      SEARCH_TABLE_BANK_COLUMN,
      new QTableWidgetItem(
          QString(headers.at(DATABASE_TABLE_BANK_FIELD).toUpper())));
  ui->searchResultsTable->setHorizontalHeaderItem(
      SEARCH_TABLE_DATE_COLUMN,
      new QTableWidgetItem(
          QString(headers.at(DATABASE_TABLE_DATE_FIELD).toUpper())));
  ui->searchResultsTable->setHorizontalHeaderItem(
      SEARCH_TABLE_DESCRIPTION_COLUMN,
      new QTableWidgetItem(
          QString(headers.at(DATABASE_TABLE_DESCRIPTION_FIELD).toUpper())));
  ui->searchResultsTable->setHorizontalHeaderItem(
      SEARCH_TABLE_CATEGORY_COLUMN,
      new QTableWidgetItem(
          QString(headers.at(DATABASE_TABLE_CATEGORY_FIELD).toUpper())));
  ui->searchResultsTable->setHorizontalHeaderItem(
      SEARCH_TABLE_AMOUNT_COLUMN,
      new QTableWidgetItem(
          QString(headers.at(DATABASE_TABLE_AMOUNT_FIELD).toUpper())));
}

void CategorizeView::addRowToSearchResultsTable(int row,
                                                QStringList rowFields) {
  QString value;
  QLabel *label;
  QString dateFormat = QLocale().dateFormat(QLocale::ShortFormat);

  ui->searchResultsTable->setVerticalHeaderItem(
      row, new QTableWidgetItem(rowFields.at(DATABASE_TABLE_ID_FIELD)));

  value = rowFields.at(DATABASE_TABLE_BANK_FIELD);
  label = new QLabel(value);
  ui->searchResultsTable->setCellWidget(row, SEARCH_TABLE_BANK_COLUMN, label);

  value = rowFields.at(DATABASE_TABLE_DATE_FIELD);
  label = new QLabel(
      QDate::fromString(value, Qt::DateFormat::ISODate).toString(dateFormat));
  label->setAlignment(Qt::AlignCenter);
  ui->searchResultsTable->setCellWidget(row, SEARCH_TABLE_DATE_COLUMN, label);

  value = rowFields.at(DATABASE_TABLE_DESCRIPTION_FIELD);
  label = new QLabel(value);
  ui->searchResultsTable->setCellWidget(row, SEARCH_TABLE_DESCRIPTION_COLUMN,
                                        label);

  value = rowFields.at(DATABASE_TABLE_CATEGORY_FIELD);
  label = new QLabel(value);
  ui->searchResultsTable->setCellWidget(row, SEARCH_TABLE_CATEGORY_COLUMN,
                                        label);

  value = rowFields.at(DATABASE_TABLE_AMOUNT_FIELD);
  label = new QLabel(QString::number(value.toDouble()));
  label->setAlignment(Qt::AlignRight);
  ui->searchResultsTable->setCellWidget(row, SEARCH_TABLE_AMOUNT_COLUMN, label);
}

QList<int> CategorizeView::getAllRowsHeaders(QTableWidget *tableWidget) {
  QList<int> ids;

  for (int row = 0; row < tableWidget->rowCount(); row++) {
    QTableWidgetItem *item = tableWidget->verticalHeaderItem(row);
    ids.append(item->text().toInt());
  }

  return ids;
}

QList<int> CategorizeView::getSelectedRows(QTableWidget *tableWidget) {
  QList<int> rows;

  foreach (QTableWidgetSelectionRange range, tableWidget->selectedRanges()) {
    for (int row = range.topRow(); row <= range.bottomRow(); row++) {
      rows.append(row);
    }
  }

  return rows;
}

QList<int> CategorizeView::getSelectedRowsHeaders(QTableWidget *tableWidget) {
  QList<int> ids;

  foreach (QTableWidgetSelectionRange range, tableWidget->selectedRanges()) {
    for (int row = range.topRow(); row <= range.bottomRow(); row++) {
      QTableWidgetItem *item = tableWidget->verticalHeaderItem(row);
      ids.append(item->text().toInt());
    }
  }

  return ids;
}

void CategorizeView::searchUncategorizedRows() {
  Database database = Database();

  ui->categoryComboBox->clear();
  ui->categoryComboBox->addItem("");
  ui->categoryComboBox->addItems(database.getCategoryNames());

  ui->searchResultsTable->clear();
  ui->searchResultsTable->setRowCount(0);
  ui->searchResultsTable->setColumnCount(0);

  setFilter(appliedFilter);

  QStringList labels = database.getColumnNames();

  QProgressDialog progress = QProgressDialog(
      QString(tr("Searching for rows with filter %1")).arg(appliedFilter),
      tr("Cancel"), 0, 0);
  progress.setWindowModality(Qt::WindowModal);
  progress.setWindowTitle(tr("Search..."));

  QList<QStringList> uncategorizedRows =
      database.getUncategorizedRows(appliedFilter, &progress);

  if (labels.length() == 0 || uncategorizedRows.length() == 0) {
    progress.cancel();
    QString error = database.getLastErrorText();
    if (!error.isEmpty()) {
      QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                  QString(database.getLastErrorText()))
          .exec();
    }
    return;
  }

  int numberOfRows = uncategorizedRows.count();

  ui->searchResultsTable->setRowCount(numberOfRows);
  ui->searchResultsTable->setColumnCount(SEARCH_TABLE_COLUMN_COUNT);

  addHeadersToSearchResultsTable(labels);

  progress.reset();
  progress.setMaximum(numberOfRows);

  for (int rowCount = 0; rowCount < numberOfRows; rowCount++) {

    progress.setLabelText(QString(tr("Processing %1 of %2 rows found."))
                              .arg(rowCount)
                              .arg(numberOfRows));

    addRowToSearchResultsTable(rowCount, uncategorizedRows.at(rowCount));

    progress.setValue(rowCount);

    if (progress.wasCanceled()) {
      break;
    }
  }
}

void CategorizeView::setFilter(QString filter) {
  QSettings settings = QSettings("com.xicra", "wmm");
  QStringList filters = settings.value("filters").toStringList();
  if (filters.contains(appliedFilter) == false) {
    filters.append(appliedFilter);
    settings.setValue("filters", filters);
    ui->filterEdit->addItem(appliedFilter);
  }
}

void CategorizeView::updateUncategorizedRows(QList<int> rowIds) {
  Database database = Database();

  ulong updatedRows = database.updateRowsCategory(rowIds, categoryName);

  if (updatedRows != rowIds.length()) {
    QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                QString(database.getLastErrorText()))
        .exec();
  } else {
    QMessageBox(QMessageBox::Icon::Information, QString(tr("Database success")),
                QString(tr("A total of %1 rows updated")).arg(updatedRows))
        .exec();
  }

  updateView();
}

void CategorizeView::updateUpdateButtonState() {
  ui->updateButton->setEnabled(ui->searchResultsTable->rowCount() > 0 &&
                               categoryName.length() > 0);
}

void CategorizeView::updateView() {
  ui->categoryComboBox->clearEditText();

  QList<QTableWidgetSelectionRange> ranges =
      ui->searchResultsTable->selectedRanges();

  if (ranges.isEmpty()) {
    ui->filterEdit->clearEditText();
    ui->searchResultsTable->clear();
    ui->searchResultsTable->setColumnCount(0);
    ui->searchResultsTable->setRowCount(0);
  } else {
    while (!ranges.isEmpty()) {
      ui->searchResultsTable->removeRow(ranges.at(0).topRow());
      ranges = ui->searchResultsTable->selectedRanges();
    }
  }
}
