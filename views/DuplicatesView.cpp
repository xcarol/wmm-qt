#include <QDate>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>

#include "../lib/Database.h"
#include "DuplicatesView.h"
#include "ui_DuplicatesView.h"

DuplicatesView::DuplicatesView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::DuplicatesView) {
  ui->setupUi(this);
  ui->deleteDuplicatesButton->setEnabled(false);
  ui->markNotDuplicatesButton->setEnabled(false);
  ui->duplicateRowsTable->horizontalHeader()->setStretchLastSection(true);
}

DuplicatesView::~DuplicatesView() { delete ui; }

void DuplicatesView::on_deleteDuplicatesButton_clicked() {
  QList<int> selectedIds = getSelectedRowsHeaders(ui->duplicateRowsTable);

  QMessageBox::StandardButton res = QMessageBox::question(
      QApplication::topLevelWidgets().first(), QString(tr("DELETE")),
      QString(tr("You're about to DELETE %1 RECORDS "
                 "\nAre you sure?"))
          .arg(selectedIds.length()));
  if (res == QMessageBox::StandardButton::No) {
    return;
  }

  deleteDuplicateRows(selectedIds);
  updateDuplicatesTable();
}

void DuplicatesView::on_duplicateRowsTable_itemSelectionChanged() {
  int enable = getSelectedRowsHeaders(ui->duplicateRowsTable).length() > 0;
  ui->deleteDuplicatesButton->setEnabled(enable);
  ui->markNotDuplicatesButton->setEnabled(enable);
}

void DuplicatesView::on_markNotDuplicatesButton_clicked() {
  QList<int> selectedIds = getSelectedRowsHeaders(ui->duplicateRowsTable);

  QMessageBox::StandardButton res = QMessageBox::question(
      QApplication::topLevelWidgets().first(),
      QString(tr("MARK NOT DUPLICATE")),
      QString(tr("You're about to mark %1 records as not duplicate."
                 "\nAre you sure?"))
          .arg(selectedIds.length()));
  if (res == QMessageBox::StandardButton::No) {
    return;
  }

  markDuplicateRows(selectedIds);
  updateDuplicatesTable();
}

void DuplicatesView::on_searchDuplicateButton_clicked() {
  updateDuplicatesTable();
}

QList<int> DuplicatesView::getSelectedRowsHeaders(QTableWidget *tableWidget) {
  QList<int> ids;

  foreach (QTableWidgetSelectionRange range, tableWidget->selectedRanges()) {
    for (int row = range.topRow(); row <= range.bottomRow(); row++) {
      QTableWidgetItem *item = tableWidget->verticalHeaderItem(row);
      ids.append(item->text().toInt());
    }
  }

  return ids;
}

void DuplicatesView::deleteDuplicateRows(QList<int> ids) {
  Database database = Database();

  ulong updatedRows = database.deleteRows(ids);

  if (updatedRows != ids.length()) {
    QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                QString(database.getLastErrorText()))
        .exec();
  } else {
    QMessageBox(QMessageBox::Icon::Information, QString(tr("Database success")),
                QString(tr("A total of %1 rows deleted.")).arg(updatedRows))
        .exec();
  }
}

void DuplicatesView::markDuplicateRows(QList<int> ids) {
  Database database = Database();

  ulong updatedRows = database.markAsNotDuplicateRows(ids);

  if (updatedRows != ids.length()) {
    QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                QString(database.getLastErrorText()))
        .exec();
  } else {
    QMessageBox(QMessageBox::Icon::Information, QString(tr("Database success")),
                QString(tr("A total of %1 rows marked as not duplicate."))
                    .arg(updatedRows))
        .exec();
  }
}

void DuplicatesView::updateDuplicatesTable() {
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

