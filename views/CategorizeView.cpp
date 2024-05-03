#include "CategorizeView.h"
#include "../lib/Database.h"
#include "ui_CategorizeView.h"
#include <QDate>
#include <QLabel>
#include <QList>
#include <QMessageBox>

CategorizeView::CategorizeView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CategorizeView) {
  ui->setupUi(this);
  ui->updateButton->setEnabled(false);
  ui->deleteDuplicatesButton->setEnabled(false);
  ui->markNotDuplicatesButton->setEnabled(false);
  ui->searchResultsTable->horizontalHeader()->setStretchLastSection(true);
  ui->duplicateRowsTable->horizontalHeader()->setStretchLastSection(true);

  QSettings settings = QSettings("com.xicra", "wmm");
  QStringList filters = settings.value("filters").toStringList();
  ui->filterEdit->addItems(filters);
}

CategorizeView::~CategorizeView() { delete ui; }

void CategorizeView::addCellToSearchResultsTable(int numberOfColumns,
                                                 int rowCount,
                                                 QStringList rowFields) {
  QString dateFormat = QLocale().dateFormat(QLocale::ShortFormat);

  for (int columnCount = 0; columnCount < numberOfColumns; columnCount++) {
    QString value = rowFields.at(columnCount);

    QLabel *label;
    switch (columnCount) {
    case DATE_COLUMN:
      label = new QLabel(QDate::fromString(value, Qt::DateFormat::ISODate)
                             .toString(dateFormat));
      label->setAlignment(Qt::AlignCenter);
      break;

    case AMOUNT_COLUMN:
      label = new QLabel(QString::number(value.toDouble()));
      label->setAlignment(Qt::AlignRight);
      break;

    default:
      label = new QLabel(value);
    }
    ui->searchResultsTable->setCellWidget(rowCount, columnCount, label);
  }
}

void CategorizeView::on_searchButton_clicked() {
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

  uncategorizedRows = database.getUncategorizedRows(appliedFilter, &progress);

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
  int numberOfColumns = uncategorizedRows.at(0).count();

  ui->searchResultsTable->setRowCount(numberOfRows);
  ui->searchResultsTable->setColumnCount(numberOfColumns);

  for (int n = 0; n < labels.length(); n++) {
    ui->searchResultsTable->setHorizontalHeaderItem(
        n, new QTableWidgetItem(QString(labels.at(n).toUpper())));
  }

  progress.reset();
  progress.setMaximum(numberOfRows);

  for (int rowCount = 0; rowCount < numberOfRows; rowCount++) {

    progress.setLabelText(QString(tr("Processing %1 of %2 rows found."))
                              .arg(rowCount)
                              .arg(numberOfRows));

    addCellToSearchResultsTable(numberOfColumns, rowCount,
                                uncategorizedRows.at(rowCount));

    progress.setValue(rowCount);

    if (progress.wasCanceled()) {
      break;
    }
  }

  updateUpdateButtonState();
}

void CategorizeView::on_updateButton_clicked() {
  QMessageBox::StandardButton res = QMessageBox::question(
      QApplication::topLevelWidgets().first(), QString(tr("Update")),
      QString(tr("You're about to update %1 records "
                 "with the category: %2\nAre you sure?"))
          .arg(uncategorizedRows.length())
          .arg(categoryName));
  if (res == QMessageBox::StandardButton::No) {
    return;
  }

  Database database = Database();

  ulong updatedRows = database.updateRowsCategory(appliedFilter, categoryName);

  if (updatedRows != uncategorizedRows.length()) {
    QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                QString(database.getLastErrorText()))
        .exec();
  } else {
    QMessageBox(QMessageBox::Icon::Information, QString(tr("Database success")),
                QString(tr("A total of %1 rows updated")).arg(updatedRows))
        .exec();
  }

  ui->filterEdit->clearEditText();
  ui->categoryComboBox->clearEditText();
  ui->searchResultsTable->clear();
  ui->searchResultsTable->setColumnCount(0);
  ui->searchResultsTable->setRowCount(0);
  uncategorizedRows.clear();
}

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

void CategorizeView::updateUpdateButtonState() {
  ui->updateButton->setEnabled(uncategorizedRows.length() > 0 &&
                               categoryName.length() > 0);
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

QList<int> CategorizeView::getSelectedRowsHeaders() {
  QList<int> ids;

  foreach (QTableWidgetSelectionRange range,
           ui->duplicateRowsTable->selectedRanges()) {
    for (int row = range.topRow(); row <= range.bottomRow(); row++) {
      QTableWidgetItem *item = ui->duplicateRowsTable->verticalHeaderItem(row);
      ids.append(item->text().toInt());
    }
  }

  return ids;
}

void CategorizeView::on_searchDuplicateButton_clicked() {
  Database database = Database();
  QList<QStringList> duplicates = database.getDuplicateRows();
  if (database.getLastErrorText().isEmpty() == false) {
    QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                QString(database.getLastErrorText()))
        .exec();
    return;
  }
  if (duplicates.isEmpty()) {
    QMessageBox(QMessageBox::Icon::Information, QString(tr("Database")),
                QString(tr("No duplicate rows found.")))
        .exec();
    return;
  }

  int numberOfColumns = 4;
  int numberOfRows = duplicates.length();

  ui->duplicateRowsLabel->setText(
      QString(tr("Duplicate rows: %1")).arg(numberOfRows));
  ui->duplicateRowsTable->clear();
  ui->duplicateRowsTable->setRowCount(numberOfRows);
  ui->duplicateRowsTable->setColumnCount(numberOfColumns);

  ui->duplicateRowsTable->setHorizontalHeaderItem(
      0, new QTableWidgetItem(QString(tr("Bank"))));

  ui->duplicateRowsTable->setHorizontalHeaderItem(
      1, new QTableWidgetItem(QString(tr("Date"))));

  ui->duplicateRowsTable->setHorizontalHeaderItem(
      2, new QTableWidgetItem(QString(tr("Description"))));

  ui->duplicateRowsTable->setHorizontalHeaderItem(
      3, new QTableWidgetItem(QString(tr("Amount"))));

  for (int rowCount = 0; rowCount < numberOfRows; rowCount++) {

    ui->duplicateRowsTable->setVerticalHeaderItem(
        rowCount, new QTableWidgetItem(duplicates.at(rowCount).at(0)));

    ui->duplicateRowsTable->setCellWidget(
        rowCount, 0, new QLabel(duplicates.at(rowCount).at(1)));
    ui->duplicateRowsTable->setCellWidget(
        rowCount, 1, new QLabel(duplicates.at(rowCount).at(2)));
    ui->duplicateRowsTable->setCellWidget(
        rowCount, 2, new QLabel(duplicates.at(rowCount).at(3)));
    ui->duplicateRowsTable->setCellWidget(
        rowCount, 3, new QLabel(duplicates.at(rowCount).at(4)));
  }
}

void CategorizeView::on_deleteDuplicatesButton_clicked() {
  QList<int> selectedIds = getSelectedRowsHeaders();

  QMessageBox::StandardButton res = QMessageBox::question(
      QApplication::topLevelWidgets().first(), QString(tr("DELETE")),
      QString(tr("You're about to DELETE %1 RECORDS "
                 "\nAre you sure?"))
          .arg(selectedIds.length()));
  if (res == QMessageBox::StandardButton::No) {
    return;
  }

  Database database = Database();

  ulong updatedRows = database.deleteRows(selectedIds);

  if (updatedRows != selectedIds.length()) {
    QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                QString(database.getLastErrorText()))
        .exec();
  } else {
    QMessageBox(QMessageBox::Icon::Information, QString(tr("Database success")),
                QString(tr("A total of %1 rows deleted.")).arg(updatedRows))
        .exec();
  }
}

void CategorizeView::on_duplicateRowsTable_itemSelectionChanged() {
  int enable = getSelectedRowsHeaders().length() > 0;
  ui->deleteDuplicatesButton->setEnabled(enable);
  ui->markNotDuplicatesButton->setEnabled(enable);
}

void CategorizeView::on_markNotDuplicatesButton_clicked()
{
  QList<int> selectedIds = getSelectedRowsHeaders();

  QMessageBox::StandardButton res = QMessageBox::question(
      QApplication::topLevelWidgets().first(), QString(tr("MARK NOT DUPLICATE")),
      QString(tr("You're about to mark %1 records as not duplicate."
                 "\nAre you sure?"))
          .arg(selectedIds.length()));
  if (res == QMessageBox::StandardButton::No) {
    return;
  }

  Database database = Database();

  ulong updatedRows = database.markAsNotDuplicateRows(selectedIds);

  if (updatedRows != selectedIds.length()) {
    QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                QString(database.getLastErrorText()))
        .exec();
  } else {
    QMessageBox(QMessageBox::Icon::Information, QString(tr("Database success")),
                QString(tr("A total of %1 rows marked as not duplicate.")).arg(updatedRows))
        .exec();
  }
}

